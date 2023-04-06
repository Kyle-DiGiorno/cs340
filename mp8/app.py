import json
from flask import Flask, jsonify, send_file, render_template, request
import requests
import os
import io
import boto3
import base64
import dotenv
import subprocess
import botocore.exceptions

app = Flask(__name__)
sub_route = "twilight_shifted/0.3603:-0.1205:0.002:512:256"
s3 = boto3.resource('s3',
                    aws_access_key_id='ROOTNAME',
                    endpoint_url="http://127.0.0.1:9000",
                    aws_secret_access_key='CHANGEME123',
                    region_name='us-east-1')
is_init = 0
def set_route(route):
    global sub_route
    sub_route = route

@app.route('/moveUp', methods=["POST"])
def moveup():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[1] = str(float(sr_1[1])+.25*float(sr_1[2]))
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    print(sub_route)
    return get_image(sub_route)


@app.route('/moveDown', methods=["POST"])
def movedown():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[1] = str(float(sr_1[1])-.25*float(sr_1[2]))
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    return get_image(sub_route)


@app.route('/moveLeft', methods=["POST"])
def moveleft():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[0] = str(float(sr_1[0])-.25*float(sr_1[2]))
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    return get_image(sub_route)


@app.route('/moveRight', methods=["POST"])
def moveright():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[0] = str(float(sr_1[0])+.25*float(sr_1[2]))
    sr_array[1] = ":".join(sr_1)
    sub_route ="/".join(sr_array)
    return get_image(sub_route)


@app.route('/zoomIn', methods=["POST"])
def zoomin():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[2] = str(float(float(sr_1[2])/1.4))
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    return get_image(sub_route)


@app.route('/zoomOut', methods=["POST"])
def zoomout():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[2] = str(float(sr_1[2])*1.4)
    sr_array[1] = ":".join(sr_1)
    sub_route= "/".join(sr_array)
    return get_image(sub_route)


@app.route('/smallerImage', methods=["POST"])
def smallerimage():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[3] = str(float(sr_1[3])/1.25)
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    return get_image("/".join(sr_array))


@app.route('/largerImage', methods=["POST"])
def largerimage():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[3] = str(float(1.25*float(sr_1[3])))
    sr_array[1] = (":".join(sr_1))
    sub_route = "/".join(sr_array)
    print(sub_route)
    return get_image(sub_route)


@app.route('/moreIterations', methods=["POST"])
def moreiterations():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[4] = str(2*float(sr_1[4]))
    sr_array[1] = (":".join(sr_1))
    sub_route ="/".join(sr_array)
    print(sub_route)
    return get_image(sub_route)


@app.route('/lessIterations', methods=["POST"])
def lessiterations():
    global sub_route
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_1[4] = str(float(float(sr_1[4])/2))
    sr_array[1] = (":".join(sr_1))
    sub_route = "/".join(sr_array)
    return get_image(sub_route)
    
@app.route('/changeColorMap', methods=["POST"])
def color_map():
    global sub_route
    req = request.json
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_array[0] = req["colormap"]
    sr_array[1] = ":".join(sr_1)
    sub_route = "/".join(sr_array)
    return get_image(sub_route)

def get_image(addr):
    subprocess.call(['bash', "docker_script.sh"])
    global s3
    print(f'{"http://127.0.0.1:34000/mandelbrot"}/{addr}')
    if not s3.Bucket('bucket') in s3.buckets.all():
        print("bucket make")
        s3.create_bucket(Bucket="bucket")
    files_in_s3 = s3.Bucket('bucket').objects.all()
    for file_addr in files_in_s3:
        print("in s3")
        if f'{"data:image/png;base64,"}{addr}' == file_addr.key:
            curr_addr = addr
            with open('temp.png', 'wb') as f_d:
                s3.Bucket('bucket').download_fileobj(
                    file_addr.key, f_d)
            with open("temp.png", "rb") as f_u:
                sr_1= send_file(f_u, mimetype="image/png")
            return send_file("temp.png", mimetype="image/png")
    print("not in s3")
    #print("ping result google")
    hostname = "google.com"
    # response = os.system("ping -c 1 " + hostname)
    # #and then check the response...
    # if response == 0:
    #     print("google up")
    # else:
    #     print("google down")
    # # hostname = f'{"http://127.0.0.1:34000/mandelbrot"}/{addr}'
    # response = os.system("ping -c 1 " + hostname)

    # #and then check the response...
    # if response == 0:
    #     print("local up")
    # else:
    #     print("local down")
    tl = requests.get(
        f'{"http://127.0.0.1:34000/mandelbrot"}/{addr}')
    print("could request")
    img_data = requests.get(
        f'{"http://127.0.0.1:34000/mandelbrot"}/{addr}').content
    print("found image sucessfully")
    print(img_data[:40])
    data_out = s3.Bucket("bucket").upload_fileobj(
        io.BytesIO(img_data), f'{"data:image/png;base64,"}{addr}')
    print("final_line")
    return get_image(addr)
@app.route('/')
def index():
    global is_init
    if(is_init < 1):
        # subprocess.call(['bash', "docker_run.sh"])
        subprocess.call(['bash', "docker_script.sh"])
        is_init+=1
    print("attapboy")
    if not s3.Bucket('bucket') in s3.buckets.all():
        s3.create_bucket(Bucket="bucket")
        init_data = requests.get(
        "http://127.0.0.1:34000/mandelbrot/twilight_shifted/0.3603:-0.1205:0.002:512:256").content
        init_data_out = s3.Bucket("bucket").upload_fileobj(
        io.BytesIO(init_data), "data:image/png;base64,twilight_shifted/0.3603:-0.1205:0.002:512:256")
    return render_template('index.html')

@app.route('/all')
def all():
    return render_template('all.html')
@app.route('/mandelbrot', methods=["GET"])
def get_curr_image():
    global sub_route
    print("This one")
    return get_image(sub_route)
@app.route('/mandelbrot/<path:addr>', methods=["GET"])
def set_curr_image(addr):
    global sub_route
    sub_route = addr
    print("g")
    print(sub_route)
    return get_image(sub_route)
@app.route('/storage', methods=["GET"])
def get_stored_imgs():
    data = []
    files_in_s3 = s3.Bucket('bucket').objects.all()
    for file_upload in files_in_s3:
        with open('temp.png', 'wb') as f_w:
            s3.Bucket('bucket').download_fileobj(
                file_upload.key, f_w)
        with open('temp.png', 'rb') as f_r:
            data_instance = base64.b64encode(f_r.read()).decode('utf-8')
            data.append(
                {"key": file_upload.key, "image": "data:image/png;base64,"+data_instance})
    return jsonify(data), 200
@app.route('/clearCache', methods=["GET"])
def clearCache():
    global s3
    try:
        s3.create_bucket(Bucket="bucket")
    except botocore.exceptions.EndpointConnectionError as nce:
        print(nce)
        os.system("docker run -it --rm -p 9000:9000 -p 9090:9090 --name minio -e \"MINIO_ROOT_USER=ROOTNAME\" -e \"MINIO_ROOT_PASSWORD=CHANGEME123\" quay.io/minio/minio server /data --console-address :9090")
        s3 = boto3.resource('s3',
                                aws_access_key_id='ROOTNAME',
                                endpoint_url="http://127.0.0.1:9000",
                                aws_secret_access_key='CHANGEME123',
                                region_name='us-east-1')
    except BaseException as e:
      print(type(e))
    bucket = s3.Bucket('bucket')
    files_in_s3 = s3.Bucket('bucket').objects.all()
    for file_addr in files_in_s3:
        file_addr.delete()
    return "200 OK"

@app.route('/resetTo', methods=["POST"])
def reset():
    global is_init
    global sub_route
    if( is_init < 1):
        subprocess.call(['bash', "docker_script.sh"])
        is_init+=1
    sub_route = "twilight_shifted/0.3603:-0.1205:0.002:512:256"
    print("Debug 1")
    req = request.json
    print("Debug 2")
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    sr_array[0] = req["colormap"]
    sr_1[0] = str(req["real"])
    sr_1[1] = str(req["imag"])
    sr_1[2] = str(req["height"])
    sr_1[3] = str(req["dim"])
    sr_1[4] = str(req["iter"])
    sr_array[1] = (":".join(sr_1))
    sub_route = "/".join(sr_array)
    print("Debug 3")
    print(sub_route)
    return get_image(sub_route)

@app.route('/getState', methods=["GET"])
def get_state():
    global sub_route
    req = {"type_init": "dict"}
    sr_array= sub_route.split("/")
    sr_1= sr_array[1].split(":")
    req["colormap"] = sr_array[0]
    req["real"] = float(sr_1[0])
    req["imag"] = float(sr_1[1])
    req["height"] = float(sr_1[2])
    req["dim"] = float(sr_1[3])
    req["iter"] = float(sr_1[4])
    return jsonify(req), 200