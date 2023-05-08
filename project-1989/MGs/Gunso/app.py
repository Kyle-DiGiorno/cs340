import os
import subprocess
import glob
import sys
import base64
# result = subprocess.run(['./port_echo.sh'], stdout=subprocess.PIPE)#environ['port_num']
# output = result.stdout.decode('utf-8').strip()
# port_num= output.split()[-1]
sys.path.append('../../')

import MGs.mglogic as mglogic
from flask import Flask, url_for,  Response, request,jsonify, make_response
import requests
import atexit
server_adress = "http://sp23-cs340-adm.cs.illinois.edu:1989"
port_num = os.environ.get('port_num')


app = Flask(__name__)
tiles = {}
tileSize = 30
tilesAcross = 40
dir_path = ''
image_count = 0
# def cleanup():
#   with open('server_list.json', 'w') as file:
#     file.truncate(0)
#   os.system(f'sudo kill $(lsof -t -i:{port_num})')
# atexit.register(cleanup)

directory = "/mnt/c/Users/kyle0/Downloads/kerorogunso"
fn = None
@app.route('/makeMosaic', methods=["POST"])
def make_mosaic():
    print("here")
    global tileSize
    global tilesAcross
    global tiles
    global dir_path
    global fn
    tileSize_t = int(request.args.get('renderedTileSize'))
    tilesAcross_t = int(request.args.get('tilesAcross'))
    f_format = request.args.get('fileFormat')
    if(not(tileSize_t == tileSize and tilesAcross_t == tilesAcross)):
        print("DIFFER")
        tileSize = tileSize_t
        tilesAcross = tilesAcross_t
        image_files = glob.glob(os.path.join(f'{dir_path}/images', '*.png'))
        for k in image_files:
            print("YEAH")
            tiles = mglogic.process_image(k, True, tileSize, tilesAcross, tiles, f'{dir_path}/images/{k.split("/")[-1]}',save)
        fn = mglogic.set_up_round(tiles.keys())
    print(tileSize)
    f = request.files['image']
    # f.save(f'a-i.png')
    # with open(f'a-i.png', "rb") as f_i:
    out = mglogic.process_image(f, False, tileSize, tilesAcross, tiles, "",False, roundfn = fn)
        # print("out:")
    # print(out)
    print("HERE")
    if(f_format.lower() == 'png'):
        out.save(f'a-i.png')
    else:
        out.convert('RGB').save(f'a-i.{f_format.lower()}', quality=95)
    with open(f'a-i.{f_format.lower()}', "rb") as q:
        buffer = q.read()
        #b64 = base64.b64encode(buffer)
    if(not buffer):
        return "400 empty buffer"
    return make_response(buffer)
def add_server(server_adress):
    global image_count
    global port_num
    print("yeah?")
    #os.environ['FLASK_APP'] = 'mg.py'
    #os.system(f'flask run --port={port_num} &')
    with app.test_request_context():
        url = url_for('make_mosaic', _external=True)

        # print(f'{url[:-1]}:{port_num}')
        # print(f'{server_adress}/add_server')
        current_directory = os.getcwd()
        name = os.path.basename(current_directory)
        print(name)
        requests.put(f'{server_adress}/addMMG',data = {"name":name
        , "url":f' 	http://172.22.150.202:5020:{port_num}/makeMosaic', "author":"kylend2", "tileImageCount":image_count})
def setup(server_adress = None):
    global fn
    global tiles
    global port_num
    global image_files
    global image_count
    # if os.path.exists(directory):
    #     print('Directory exists')
    # else:
    #     print('Directory does not exist')
    # for filename in os.listdir(directory):
    #     if os.path.isfile(os.path.join(directory, filename)):
    #         print(filename)
    print("setting up")
    port_num = os.environ.get('port_num')
    print("PRINTING")
    print(port_num)
    save = False
    filepath = os.path.abspath(__file__)
    dir_path, file_name = os.path.split(filepath)
    image_files = glob.glob(os.path.join(f'{dir_path}/images', '*.png'))
    if(not image_files):
        image_files = glob.glob(os.path.join(directory, '*png'))
        save = True
    for k in image_files:
        image_count+=1
        tiles = mglogic.process_image(k, True, tileSize, tilesAcross, tiles, f'{dir_path}/images/{k.split("/")[-1]}',save)
    fn = mglogic.set_up_round(tiles.keys())
    if server_adress:
        add_server(server_adress)
    #print("set up complete")
setup(server_adress = server_adress)

@app.route('/rim', methods = ["GET"])
def getter():
    print("got")
    return "OK 200"
