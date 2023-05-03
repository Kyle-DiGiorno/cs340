
from PIL import Image
import numpy as np
from sklearn.neighbors import KDTree


# def scale(image, tileSize):
#     print(image)
#     image.resize((tileSize, tileSize), resample=Image.BILINEAR)
#     print(image)
#     print("_______________")
#     return image

def get_average_color(image):
    img = image.convert('RGB')
    pixels = img.load()
    width, height = img.size
    rgb_tot = [0,0,0]
    for x in range(width):
        for y in range(height):
            r,g,b = pixels[x,y]
            rgb_tot[0]+=r
            rgb_tot[1]+=g
            rgb_tot[2]+=b
    num_pixels = width * height
    rgb_avg = [t / num_pixels for t in rgb_tot]
    return tuple(rgb_avg)

def set_up_round(y):
    #print([len(t) for t in y])
    # sorted_y = np.array(sorted(y))
    # def f(x):
    #     diff = np.linalg.norm(sorted_y - x, axis=1)
    #     f_out = np.argmin(diff)
    #     return f_out
    # def g(i):
    #     return sorted_y[i]
    # def round_to_closest(x):
    #     f_val = f(x)
    #     return g(f_val)
    sorted_y = np.array(sorted(y, key=lambda x: tuple(x)))#.reshape(-1, 1)
    tree = KDTree(sorted_y)

    def round_to_closest(x):
        dist, ind = tree.query(np.array(x).reshape(1, -1))
        return sorted_y[tuple(ind[0])]
    return round_to_closest

def process_image(input_image_path, to_upload, tileSize, tilesAcross, tiles, pre_string, save, roundfn = None):
    
    with Image.open(input_image_path) as input_image:
        input_image = input_image.convert('RGB')
        if to_upload:
            scaled_image = input_image.copy()
            scaled_image = scaled_image.resize((tileSize, tileSize))
            average_color = get_average_color(scaled_image)
            tiles[average_color] = scaled_image
            if(save): 
                input_image.save(f'{pre_string}')
            return tiles
        else:
            tilesDown = round(tilesAcross * (input_image.height/input_image.width))
            output_image = Image.new(mode=input_image.mode, size=(tilesAcross * tileSize, tilesDown*tileSize))
            input_tileSize = input_image.height/tilesAcross
            for x in range(tilesAcross):
                for y in range(tilesDown):
                    subimage = input_image.crop((x * input_tileSize, y * input_tileSize, (x + 1) * input_tileSize, (y + 1) * input_tileSize))
                    average_color = get_average_color(subimage)
                    tile_index = roundfn(average_color)
                    #print(tile_index)
                    #print(tiles[tuple(tile_index)])
                    output_image.paste(tiles[tuple(tile_index)].resize((tileSize,tileSize)),(x*tileSize,y*tileSize))
            #print(output_image)
            #output_image.save("Oiii.png")
    return output_image