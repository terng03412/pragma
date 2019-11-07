import cv2
import numpy as np
from glob import glob
from tqdm import tqdm

files = glob('*.jpg')
for file in files:
	img = cv2.imread(file)
	img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
	img = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
	
	f = open(f"{file[0:-4]}.txt","w+")
	f.write(f"{img.shape[0]} {img.shape[1]}\n")
	
	for row in tqdm(img):
		for col in row:
			f.write(str(col)+"\n")
			
	f.close()
	print(file)