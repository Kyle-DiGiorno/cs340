import requests
from flask import Flask, jsonify, redirect, render_template, request
from dotenv import load_dotenv
import json
import os
# load_dotenv()


app = Flask(__name__)

walls = ['1001100010000000100010001100',
            '1000000000000000000000100',
            '1000000000000000000000100',
            '0', 
            '1000000000000000000000100',
            '1000000000000000000000100',
            '11001000100000001000100110']
vals_pre = ['0000'+'0000'+'0000'+'0110'+'0100'+'0000'+'0000',
            '0000'+'0000'+'0000'+'0011'+'0110'+'0000'+'0000',
            '0000'+'0000'+'1001'+'0100'+'1000'+'1100'+'0000',
            '0000'+'0000'+'0000'+'0100'+'0000'+'0000'+'0000',
            '0000'+'0000'+'0000'+'0110'+'0010'+'0000'+'0000',
            '0000'+'0000'+'0100'+'0000'+'0000'+'0001'+'0000',
            '0000'+'0000'+'0000'+'0000'+'0000'+'0000'+'0000',]

# vals_pre = ["1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000",  
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1110111111011111011111011111"]


url_to_put = "http://sp23-cs340-adm.cs.illinois.edu:34000/addMG"
#url_to_put = "http://127.0.0.1:5000/addMG"
#url_to_get = "http://sp23-cs340-200.cs.illinois.edu:4000/"
url_to_get = "http://127.0.0.1:4000"
out_file = open("outm.json", "w")

requests.put(url=url_to_put, json={"name": "Diffusion monad sheaf",
                                                                            "url": url_to_get,
                                                                            "author": "Kyle DiGaetano",
                                                                            "weight": 1})
@ app.route('/generate', methods=["GET"])
def generate_dyamic():
    global vals_pre
    global walls
    vals = vals_pre
    #val_hex = [str(hex(int(vals[0], 2)))[2:], str(hex(int(vals[1], 2)))[2:], str(hex(int(vals[2], 2)))[2:], str(hex(
        #int(vals[3], 2)))[2:], str(hex(int(vals[4], 2)))[2:], str(hex(int(vals[5], 2)))[2:], str(hex(int(vals[6], 2)))[2:]]
    val_hex = ["0"*(7-len(hex(int(vals[n],2)|int(walls[n],2))[2:]))+ hex(int(vals[n],2)|int(walls[n],2))[2:]  for n in range(len(vals))]#["988088c", "1000004", "1000004", "0", "1000004", "1000004", "3220226"]
    # walls = ["0"*(7-len(hex(int(n,2))[2:]))+ hex(int(n,2))[2:]  for n in walls]
                #[hex(int(n,2))[2:] for n in vals]
    print(val_hex)
    return jsonify({"geom": val_hex}), 200

