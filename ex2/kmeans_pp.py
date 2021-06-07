import numpy as np
import pandas as pd

###################
#Parse CMD
###################

###################
#Join file_1 + file_2
###################

###################
#Algo 1.1
###################

def smart_centroids(vectors, K):
	d = vectors.shape()[1]
	N = vectors.shape()[0]
	vectors['Dist'] = vectors['Prob'] = 0
	centroids = vectors.sample()
	Z = 1

	while (Z < K):
		for i in range(N):
			vectors['Dist'][i] = closest_Dist(centroids, vectors.iloc[i])
		sum = sum(vectors['Dist'])
		for i in range(N):
			vectors['Prob'][i] = vectors['Dist'][i]/sum
		Z++
		row_num = np.random.choice(N, vectors['Prob'])
		centroids = centroids.append(vectors.iloc[row_num])

	iter = centroids.iterrows()
	num_arr = []
	for i in range(K):
		curr = next(iter)[1]
		for j in range(d):
			num_arr.append(curr[j])

	iter = vectors.iterrows()
	curr = next(iter, None)
	while (curr != None & curr[1].name not in centroids.index):
		for i in range(d):
			num_arr.append(curr[j])

	return num_arr

def closest_Dist(centroids, vector, d):
	D = sum(np.square(vector[:d] - centroids.iloc[0][:d]))
	for (centroid in centroids):
		tmp_val = sum(np.square(vector[:N] - centroid[:N]))
		if (tmp_val <= D):
			D = tmp_val
	return D

###################
#fit
###################

###################
#Print Results
###################