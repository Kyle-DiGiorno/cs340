import os
import glob
import mglogic

tiles = {}
tileSize = 30
tilesAcross = 40
directory = "/mnt/c/Users/kyle0/Downloads/kerorogunso"
fn = None
def setup():
    global fn
    global tiles
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
    print("set up complete")
    #print(tiles)

def make_mosaic(f):
    return mglogic.process_image(f, False, tileSize, tilesAcross, tiles, "",False, roundfn = fn)