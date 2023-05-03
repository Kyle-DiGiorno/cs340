from flask import Flask, jsonify, render_template, url_for, request
import requests
import json
import base64
import os
import importlib.util
from PIL import Image
import atexit


app = Flask(__name__)
port_num = 5000
tileSize = 30
tilesAcross = 40
modules = []
def cleanup():
  with open('server_list.json', 'w') as file:
    file.truncate(0)
  os.system(f'sudo kill $(lsof -t -i:{port_num})')
atexit.register(cleanup)
@app.route('/', methods=["GET"])
def GET_index():
  global modules
  '''Route for "/" (frontend)'''
  with app.test_request_context():
      url = url_for('GET_index', _external = True)
      url = f'{url[:-1]}:{port_num}'
      # for dirpath, dirnames, filenames in os.walk("MGs"):
      #       for filename in filenames:
                # if filename.endswith(".py"):
                #     fullpath = os.path.join(dirpath, filename)
                #     module_name = filename[:-3]  # remove the .py extension
                #     spec = importlib.util.spec_from_file_location(module_name, fullpath)
                #     module = importlib.util.module_from_spec(spec)
                #     spec.loader.exec_module(module)
                    # if hasattr(module, "setup") and callable(module.setup):
                    #     module.setup(url)
                    #     modules.append(module)
  return render_template("index.html")
@app.route('/addMMG', methods = ["PUT"])
def add_server():
    print("Adding server")
    name = request.form["name"]
    addr = request.form["url"]
    author = request.form["author"]
    print(addr)
    with open('server_list.json', 'r') as json_file:
      try: 
        servers = json.load(json_file)
      except json.decoder.JSONDecodeError:
        servers = {}
        servers['servers'] = []
      #print("in")
      # if 'servers' not in servers:
      if (not addr in servers['servers']):
            #print("in 2")
            servers['servers'].append(addr)
    with open("server_list.json", "w") as jsonFile:
        json.dump(servers, jsonFile)
    return '200 OK'
@app.route('/makeMosaic', methods=["POST"])
def POST_makeMosaic():
  global modules
  f = request.files["image"]
  response = []
  i = 0
  with open('server_list.json') as json_file:
        s_list = json.load(json_file)
  for s in s_list["servers"]:
      print(s)
      # with open(f'a-{i}.png', 'wb') as out_file:
      #   print(i)
      f.seek(0)
        # f.save(out_file)
      # t = requests.get("http://127.0.0.1:5001/rim")
      # with open(f'a-{i}.png', "rb") as t:
      #   print("f")
      out = requests.post(f'{s}?tilesAcross={tilesAcross}&renderedTileSize={tileSize}',
                files={"image": open(base_img_name, "rb")})
      #print(out.json())
      i+=1
      print(i)
      response+=out.json()
      #f = request.files["image"]

  return jsonify(response)
