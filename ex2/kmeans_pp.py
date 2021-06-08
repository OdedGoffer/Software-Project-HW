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

	vectors = vectors.drop(['Dist','Prob'], axis=1)
	centroids = centroids.drop(['Dist','Prob'], axis=1)
	vectors = vectors.drop([i for i in centroids.index])
	vectors = centroids.merge(vectors,how='outer')
	num_arr = vectors.to_numpy().flatten()
	
	return num_arr

def closest_Dist(centroids, vector):
	D = sum(np.square(vector[:-2] - centroids.iloc[0][:-2]))
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