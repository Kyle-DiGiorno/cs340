import requests
from flask import Flask, jsonify, redirect, render_template, request
from dotenv import load_dotenv
import json
import random
import os
import torch
import torch.nn as nn
#from PIL import Image
import numpy as np

load_dotenv()


app = Flask(__name__)
row = 0
col = 0
old_row = 0
old_col = 0
# fmt: off
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
vals_pre_dict = {(row,col):vals_pre.copy()}
# vals_pre = ["1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000",  
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1000"+"1000"+"1000"+"1000"+"1000"+"1000"+"0000", 
#             "1110111111011111011111011111"]


url_to_put = "http://sp23-cs340-adm.cs.illinois.edu:34000/addMG"
#url_to_put = "http://127.0.0.1:5000/addMG"
url_to_get = "http://sp23-cs340-200.cs.illinois.edu:3000/"
#url_to_get = "http://127.0.0.1:3000"
out_file = open("outm.json", "w")

requests.put(url=url_to_put, json={"name": "Diffusion monad sheaf",
                                                                            "url": url_to_get,
                                                                            "author": "Kyle DiGaetano",
                                                                            "weight": 1})




class SinDDM(nn.Module):
    def __init__(self, x, depth=3, in_channels = 1, out_channels = 1):
        super(SinDDM, self).__init__()

        self.depth = depth
        
        self.decoder = nn.Sequential(
            nn.Conv2d(in_channels=1, out_channels=7*28, kernel_size=3, padding=1),
            #nn.BatchNorm2d(7*28),
            nn.ReLU(),
            nn.Conv2d(in_channels=7*28, out_channels=7*28, kernel_size=3, padding=1),
            #nn.BatchNorm2d(7*28),
            nn.ReLU(),
            nn.Conv2d(in_channels=7*28, out_channels=1, kernel_size=3, padding=1),
            # nn.ReLU(inplace=True),
            # nn.Conv2d(64, 64, 3, padding=1),
            # nn.ReLU(inplace=True),
            # nn.Conv2d(64, in_channels, 3, padding=1),
        )

        self.x = x
        self.train_depth = depth
    def forward(self, x, training, depth):
        layers_encode = []
        layers_decode = []
        if(training):
            #noise = x
            for i in range(self.train_depth):
              #t = torch.tensor(float(i) / self.train_depth, device=y.device)
              noise = x+torch.randn_like(x) * torch.tensor(i/10)
              #x_enc = self.encoder(noise)
              noise = noise.view(-1, 1, 7, 28)
              #noise = torch.round(noise/torch.max(noise))
              x_dec = self.decoder(noise)
              layers_decode.append(x_dec)
              #layers_encode.append(x_enc)
        else:
          x_dec = x
          for i in reversed(range(depth)):
              #t = #torch.tensor(float(i) / depth, device=y.device)
              #noise = torch.randn_like(x) * torch.sqrt(t)
              #x_enc = self.encoder(noise)
              x_dec = x_dec.view(-1, 1, 7, 28)
              x_dec = self.decoder(x_dec)
              layers_decode.append(x_dec)
              #layers_encode.append(x_enc)
        # noisy_x = x
        # for i in range(self.train_depth):
        #   noisy_x = x + torch.randn_like(x) * 0.1
        #   x = self.layer(noisy_x)
        #   x = self.activation(x)
        #layers_decode[-1]/=torch.max(layers_decode[-1])
        #layers_decode[-1] = torch.round(layers_decode[-1])
        return x, layers_encode, layers_decode
# def binary(x,bits):
#     mask = 2**torch.arange(bits).to(x.device,x.dtype)
#     return x.unsqueeze(-1).bitwise_and(mask).ne(0).byte()
model = 0
def load_diffusion():
    global model
    x = torch.stack([torch.tensor([int(k,2) for k in n],dtype = torch.float32) for n in vals_pre])#torch.tensor(np.array(float(vals_pre,2)), dtype=torch.float32).unsqueeze(0)
    #torch.tensor(np.array(img2).transpose(2, 0, 1), dtype=torch.float32).unsqueeze(0) / 255.0

    pre_noise_img = (x.squeeze().detach().cpu().numpy() * 255.0).astype(np.uint8)

    # Create the SinDDM model and set it to evaluation mode
    model = SinDDM(x,depth=10)
    # model.eval()
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    criterion = nn.HuberLoss()
    num_epochs = 5

    # optimizer.zero_grad()
    # output = model(x,True,7)
    # loss = criterion(output, x)
    # loss.backward()
    # optimizer.step()
    pre_output_imgs = []
    for epoch in range(num_epochs):
        print(epoch)
        optimizer.zero_grad()
        #temp = output.copy()
        output = model(x,True,epoch)
        pre_output_imgs.append(output[2])
        #vals_pre_f = run_diffusion(torch.stack([torch.tensor([int(k,2) for k in n],dtype = torch.float32) for n in vals_pre])).squeeze().detach().cpu().numpy()

        loss = criterion(torch.abs(output[2][len(output[2])-1]/torch.max(torch.abs(output[2][len(output[2])-1]))),torch.abs(x.view(-1, 1, 7, 28)/torch.max(torch.abs(x))))
        loss.backward()
        optimizer.step()
load_diffusion()
def run_diffusion(x):
    global row
    global col
    global old_col
    global old_row
    global model
    depth = 1
    #print(x)
    y = x
    if(max(abs(col),abs(row)) >= max(abs(old_col),abs(old_row))):
        y = y + torch.randn_like(x) * torch.tensor(0.3)
        #y/=torch.max(y)
    #post_noise_img = pre_noise_img = (y.squeeze().detach().cpu().numpy() * 255.0).astype(np.uint8)
    if(max(abs(col),abs(row)) <= max(abs(old_col),abs(old_row))):
        output = model(y+ torch.randn_like(x) * torch.tensor(0.1),False,depth)
        return output[2][-1]
    return y
start = 1
@ app.route('/generate', methods=["GET"])
def generate_dyamic():
    global vals_pre_dict
    global vals_pre
    global walls
    global start
    global row
    global col
    global old_col
    global old_row
    if(abs(row+col) > 0):
        # print("&=============&")
        # print((old_row,old_col))
        # print(vals_pre_dict[(old_row,old_col)])
        vals_pre_f = run_diffusion(torch.stack([torch.tensor([int(k,2) for k in n],dtype = torch.float32) for n in vals_pre_dict[(0,0)]])).squeeze().detach().cpu().numpy()
        # print(vals_pre_dict[(old_row,old_col)])
        mx=np.max(np.abs(vals_pre_f))
        vals_pre_f/=mx
        
        #print(vals_pre_f)
        #vals_pre_f%=1
        # print(vals_pre_dict[(old_row,old_col)])
        for k in range(len(vals_pre_f)):
            #print(vals_pre_f[k])
            vals_pre_f[k] = np.round(vals_pre_f[k])
            t = ""
            for l in vals_pre_f[k]:
                t += str(int(abs(l)))
            vals_pre[k] = t
        
        # print(vals_pre_dict.keys())
        # print("********")
        # print((row,col))

        #print(vals_pre_dict[(row,col)])
        # print("vpd________")
        # print(vals_pre_dict)
        vals_pre_dict[(row,col)]=vals_pre.copy()
        # print(vals_pre_dict[(old_row,old_col)])
        # print(vals_pre_dict)
    #val_hex = [str(hex(int(vals[0], 2)))[2:], str(hex(int(vals[1], 2)))[2:], str(hex(int(vals[2], 2)))[2:], str(hex(
        #int(vals[3], 2)))[2:], str(hex(int(vals[4], 2)))[2:], str(hex(int(vals[5], 2)))[2:], str(hex(int(vals[6], 2)))[2:]]
    val_hex = ["0"*(7-len(hex(int(vals_pre[n],2)|int(walls[n],2))[2:]))+ hex(int(vals_pre[n],2)|int(walls[n],2))[2:]  for n in range(len(vals_pre))]#["988088c", "1000004", "1000004", "0", "1000004", "1000004", "3220226"]
    #val_hex = ["0"*(7-len(hex(int(walls[n],2))[2:]))+ hex(int(walls[n],2))[2:]  for n in range(len(walls))]#["988088c", "1000004", "1000004", "0", "1000004", "1000004", "3220226"]
    
    # walls = ["0"*(7-len(hex(int(n,2))[2:]))+ hex(int(n,2))[2:]  for n in walls]
                #[hex(int(n,2))[2:] for n in vals]
    #print(vals_pre)
    return jsonify({"geom": val_hex}), 200
@ app.route('/putRowCol', methods=["PUT"])
def put_row_col():
    global row
    global col
    global old_col
    global old_row
    data = request.get_json()
    old_col = col
    old_row = row
    row = data['row']
    col = data['col']
    print(row)
    print(col)
    return jsonify({"msg": "hi"}), 200