import cv2
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm
from glob import glob

files = glob('*.txt')
for file in files:
	f = open(file)
	lines = f.readlines()
	size = lines[0].split()
	width, height = int(size[0]), int(size[1])
	
	image = []

	row_counter = 0
	row_data = []
	for x in tqdm(range(1, width * height + 1)):
		pixel_str = lines[x].split()
		pixel = [ int(pixel_str[0]) ]
		row_data.append(pixel)
		row_counter += 1

		if row_counter == height:
			row_counter = 0
			image.append(row_data)
			row_data = []

	image = np.array(image).astype(np.uint8)
	cv2.imwrite(f"{file[0:-4]}.jpg", image)
	print(f"{file[0:-4]}.jpg")