import argparse
import numpy as np
import pandas as pd

###################
#Parse CMD
###################

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

print(k, max_iter, file_name_1, file_name_2)
###################
#Join file_1 + file_2
###################

def join_files(file_name_1, file_name_2):
    data1 = pd.read_csv(file_name_1, header=None)
    data2 = pd.read_csv(file_name_2, header=None)

    print(data1.columns)

    data = data1.join(data2, on=0, how="inner", lsuffix="__")
    data.columns = np.arange(len(data.columns))
    
    return data

data = join_files(file_name_1, file_name_2)
print(data)

###################
#Algo 1.1
###################

def smart_centroids(vectors, K):
	d = vectors.shape()[1]
	N = vectors.shape()[0]
	vectors['Dist'] = vectors['Prob'] = 0
	centroids = vectors.sample()
	np.random.seed(0)
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
