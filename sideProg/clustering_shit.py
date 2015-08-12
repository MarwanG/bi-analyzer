#!/usr/bin/python

import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


from sklearn.cluster import KMeans
from sklearn import datasets

from sklearn.preprocessing import normalize

file=sys.argv[1]

matrix = []

with open(file) as f:
	for line in f:
		list = line.split()

		pair = list[0]
		channels = [0] * 12
		for i in range(1,len(list)):
			v = int(list[i])
			channels[((v%30)-3)] = 1
		norm1 = channels / np.linalg.norm(channels)
		matrix.append(norm1)

kaka = KMeans(n_clusters=12)
kaka2 = kaka.fit_predict(matrix)

kaka3 = np.where(kaka2==0)

print kaka3
print matrix[5]
print matrix[11]

plt.hist(kaka2)
plt.show()