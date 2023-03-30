from flask import Flask, render_template, send_file, request
import os

app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  return render_template("index.html")


# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  # ...your code here...
  if(not os.path.exists("temp")):
     os.makedirs("temp")
  # for filepath in request.files['png']:
  #   print(str(filepath))
  #   if(not os.path.exists(str(filepath))):
  #     return str(filepath)+" does not exist on the server", 404
  # tempfiledata = open("TempFileData.png", "w")
  # for line in request.files['png']:
  # tempfiledata.write()
  # tempfiledata.close()
  file_data = request.files['png'].read()

    # Write file data to a new file
  png_sig = b'\x89PNG\r\n\x1a\n'
  if(file_data[:8]!=png_sig):
    print(file_data[:8])
    return "not png",422; 
  filename = "TempFileData.png"
  with open(filename, 'wb') as f:
      f.write(file_data)
      print(f"File {filename} saved successfully.")
      
  for n in range (0,100000):
    #print(n)
    if(not os.path.exists("temp/"+ str(n) +".gif")):
      print("./png-extractGIF " + "TempFileData.png " + "temp/"+str(n)+".gif")
      k = os.system(("./png-extractGIF " + "TempFileData.png " + "temp/"+str(n)+".gif"))
      if(k != 0):
        os.remove("temp/"+ str(n) +".gif")
        if(k == 4*256):
          return "failed extraction, no hidden gif",415
        if(k == 2):
          return "not png",422
        return "What", k
      else:
        return send_file("temp/"+ str(n) +".gif")
        

# Get the nth saved "uiuc" GIF:
@app.route('/extract/<int:image_num>', methods=['GET'])
def extract_image(image_num):
  # ...
  if(not os.path.exists("temp/"+ str(image_num) +".gif")):
    return "no file at desired index"+ str(image_num) ,404
  #temp_file = open("temp/"+ str(image_num) +".gif", 'rb')
  return send_file("temp/"+ str(image_num) +".gif")

# @app.route('/upload', methods=['POST'])
# def upload_file():
#     # Get file data from request body
#     file_data = request.files['png'].read()

#     # Write file data to a new file
#     filename = "new_file.png"
#     with open(filename, 'wb') as f:
#         f.write(file_data)
#         print(f"File {filename} saved successfully.")

#     # Return response to indicate success
#     return "File data received successfully."