import argparse
import numpy as np
import pandas as pd
from kmeans import kmeans as fit
np.random.seed(0)

###################
#Parse CMD
###################

def parse_arguments():
	global k, max_iter, file_name_1, file_name_2
	parser = argparse.ArgumentParser()
	parser.add_argument('k', type=int, help="Number of clusters to create")
	parser.add_argument('max_iter', type=int, nargs='?', default=100, help="Maximum iterations")
	parser.add_argument('file_name_1', type=str, help="File name 1")
	parser.add_argument('file_name_2', type=str, help="File name 2")

	args = parser.parse_args()

	k = args.k
	max_iter = args.max_iter
	file_name_1 = args.file_name_1
	file_name_2 = args.file_name_2 

###################
#Join file_1 + file_2
###################

def join_files(file_name_1, file_name_2):
	data1 = pd.read_csv(file_name_1, header=None)
	data2 = pd.read_csv(file_name_2, header=None)

	data = data1.merge(data2, on=0)
	data.columns = np.arange(len(data.columns))
	data.drop(columns=data.columns[0], axis=1, inplace=True)
	
	return data

###################
#Algo 1.1
###################

def smart_centroids(vectors, K):
	global N,d 
	N,d = vectors.shape
	vectors['Dist'] = vectors['Prob'] = 0.0
	centroids = vectors.sample()
	Z = 1
	
	while (Z < K):
		for i in range(N):
			vectors['Dist'][i] = closest_Dist(centroids, vectors.iloc[i].to_numpy())
		sumd = sum(vectors['Dist'])
		for i in range(N):
			vectors['Prob'][i] = vectors['Dist'][i]/sumd
		Z+=1
		row_num = np.random.choice(N, p=vectors['Prob'])
		centroids = centroids.append(vectors.iloc[row_num])

	vectors = vectors.drop(['Dist','Prob'], axis=1)
	centroids = centroids.drop(['Dist','Prob'], axis=1)
	vectors = vectors.drop([i for i in centroids.index])
	vectors = pd.concat([centroids, vectors])
	num_arr = vectors.to_numpy().flatten().tolist()
	
	return num_arr

def closest_Dist(centroids, vector):
	D = sum(np.square(vector[:-2] - centroids.iloc[0].to_numpy()[:-2]))
	for index, centroid in centroids.iterrows():
		tmp_val = sum(np.square(vector[:N] - centroid.to_numpy()[:N]))
		if (tmp_val <= D):
			D = tmp_val
	return D

###################
#fit
###################

###################
#Print Results
###################

if __name__ == "__main__":
    parse_arguments()
    data = join_files(file_name_1, file_name_2)
    data = smart_centroids(data, k)
    print(data[:10])
    print("")
    print(fit(data, N, d, k, max_iter))
