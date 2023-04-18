from flask import Flask, jsonify, render_template, request
import base64
import os
import importlib.util
from PIL import Image

app = Flask(__name__)
modules = []
@app.route('/', methods=["GET"])
def GET_index():
  global modules
  '''Route for "/" (frontend)'''
  
  for dirpath, dirnames, filenames in os.walk("MGs"):
        for filename in filenames:
            if filename.endswith(".py"):
                fullpath = os.path.join(dirpath, filename)
                module_name = filename[:-3]  # remove the .py extension
                spec = importlib.util.spec_from_file_location(module_name, fullpath)
                module = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(module)
                if hasattr(module, "setup") and callable(module.setup):
                    module.setup()
                    modules.append(module)
  return render_template("index.html")
@app.route('/makeMosaic', methods=["POST"])
def POST_makeMosaic():
  global modules
  f = request.files["image"]
  response = []
  i = 0
  for module in modules:
    out = module.make_mosaic(f)
    out.save(f'a-{i}.png')
    with open(f'a-{i}.png', "rb") as q:
      buffer = q.read()
    b64 = base64.b64encode(buffer)
    response.append({
      "image": "data:image/png;base64," + b64.decode('utf-8')
    })
    i+=1

  return jsonify(response)
