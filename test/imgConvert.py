from PIL import Image
import argparse
import os

def convert_img(old_name, new_name, h, w):
	img = Image.open("/home/user/cgra/computer_graphics/scratchwork/" + old_name)
	new_img = img.resize( (h, w) )
	new_img.save( "/home/user/cgra/computer_graphics/scratchwork/" +new_name + '.bmp' )
	#os.remove("/home/user/cgra/computer_graphics/HW6/" + old_name)

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Process some integers.')
	parser.add_argument( '--f', dest='old_name',
                    help='file name to convert')
	parser.add_argument( '--n', dest='new_name',
                    help='new file name')
	parser.add_argument( '--h', dest='height', type=int,
                    help='height of reformated image')
	parser.add_argument( '--w', dest='width', type=int,
                    help='width of reformated image')                

	args = parser.parse_args()
	convert_img(args.old_name, args.new_name, args.height, args.width)
