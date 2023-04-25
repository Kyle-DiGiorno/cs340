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
from flask import Flask, url_for,  Response, request,jsonify
import requests
import atexit
server_adress = "http://127.0.0.1:5000"
port_num = os.environ.get('port_num')

app = Flask(__name__)
tiles = {}
tileSize = 30
tilesAcross = 40

# def cleanup():
#   with open('server_list.json', 'w') as file:
#     file.truncate(0)
#   os.system(f'sudo kill $(lsof -t -i:{port_num})')
# atexit.register(cleanup)

directory = "/mnt/c/Users/kyle0/Downloads/pokediffuse"
fn = None
@app.route('/makeMosaic', methods=["POST"])
def make_mosaic():
    print("here")
    
    tileSize = int(request.form.get('tileSize'))
    tilesAcross = int(request.form.get('tilesAcross'))
    f = request.files['f']
    # f.save(f'a-i.png')
    # with open(f'a-i.png', "rb") as f_i:
    out = mglogic.process_image(f, False, tileSize, tilesAcross, tiles, "",False, roundfn = fn)
        # print("out:")
    # print(out)
    out.save(f'a-i.png')
    with open(f'a-i.png', "rb") as q:
        buffer = q.read()
        b64 = base64.b64encode(buffer)
        response={
            "image": "data:image/png;base64," + b64.decode('utf-8')
        }
    return jsonify(response)
def add_server(server_adress):
    global port_num
    print("yeah?")
    #os.environ['FLASK_APP'] = 'mg.py'
    #os.system(f'flask run --port={port_num} &')
    with app.test_request_context():
        url = url_for('make_mosaic', _external=True)
        # print(f'{url[:-1]}:{port_num}')
        # print(f'{server_adress}/add_server')
        requests.post(f'{server_adress}/add_server',json = {"url":f'{url[:-1*len("/makeMosaic")]}:{port_num}/makeMosaic'})
def setup(server_adress = None):
    global fn
    global tiles
    global port_num
    # if os.path.exists(directory):
    #     print('Directory exists')
    # else:
    #     print('Directory does not exist')
    # for filename in os.listdir(directory):
    #     if os.path.isfile(os.path.join(directory, filename)):
    #         print(filename)
    print("setting up")

    save = False
    filepath = os.path.abspath(__file__)
    dir_path, file_name = os.path.split(filepath)
    image_files = glob.glob(os.path.join(f'{dir_path}/images', '*.png'))
    if(not image_files):
        image_files = glob.glob(os.path.join(directory, '*png'))
        save = True
    for k in image_files:
        
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
