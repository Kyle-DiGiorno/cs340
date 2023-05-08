import requests
from flask import Flask, url_for,  Response, request,jsonify, make_response
from PIL import Image
import numpy as np
import os
app = Flask(__name__)
server_adress = "http://127.0.0.1:5000"
port_num = os.environ.get('port_num')
@app.route("/reduceMosaic", methods=["POST"])
def reduce():
    print("right")
    baseImage = request.files["baseImage"]
    mosaic1 = request.files["mosaic1"]
    mosaic2 = request.files["mosaic2"]
    print("files correct")
    renderedTileSize = int(request.args.get("renderedTileSize"))
    tilesAcross = int(request.args.get("tilesAcross"))
    f_format = request.args.get("fileFormat")
    print("args correct")
    target_image = Image.open(baseImage)
    tilesDown = round(tilesAcross * (target_image.height/target_image.width))
    output_image = Image.new(mode=target_image.mode, size=(tilesAcross * renderedTileSize, tilesDown*renderedTileSize))
    # Your reduce logic here:
    

    m_i1 = Image.open(mosaic1)
    m_i2 = Image.open(mosaic2)

    for x in range(0,tilesAcross):
        for y in range (0,tilesDown):
            m_a1 = m_i1.crop((x * renderedTileSize, y * renderedTileSize, (x + 1) * renderedTileSize, (y + 1) * renderedTileSize))
            m_a2 = m_i2.crop((x * renderedTileSize, y * renderedTileSize, (x + 1) * renderedTileSize, (y + 1) * renderedTileSize))
            t_axy = np.array(target_image.crop((x * renderedTileSize, y * renderedTileSize, (x + 1) * renderedTileSize, (y + 1) * renderedTileSize)))
            mse1 = np.mean((t_axy - np.array(m_a1)) ** 2)
            mse2 = np.mean((t_axy - np.array(m_a2)) ** 2)
            if(mse1 > mse2):
                output_image.paste(m_a1,(x*renderedTileSize,y*renderedTileSize))
            else:
                output_image.paste(m_a2,(x*renderedTileSize,y*renderedTileSize))
    if(f_format.lower() == 'png'):
        output_image.save(f'a-i.png')
    else:
        output_image.convert('RGB').save(f'a-i.{f_format.lower()}', quality=95)
    with open(f'a-i.{f_format.lower()}', "rb") as q:
        buffer = q.read()
        #b64 = base64.b64encode(buffer)
    if(not buffer):
        return "400 empty buffer"
    return make_response(buffer)
    # Return a reduced mosaic that combines the best of mosaic1 and mosaic2:
    return make_response(output_image)
with app.test_request_context():
        address = url_for('reduce', _external = True)
print(address)
print(f'{address[:-len("/reduceMosaic")]}:{port_num}/reduceMosaic')
requests.put(f'{server_adress}/registerReducer',data = {"name":"reducer"
        ,"url":f'{address[:-len("/reduceMosaic")]}:{port_num}/reduceMosaic', "author":"kylend2"})
print(address)