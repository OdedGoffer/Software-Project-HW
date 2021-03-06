import argparse
import sys
import numpy as np
import pandas as pd
from kmeanssp import fit
np.random.seed(0)

#############
#Parse command line arguments:

def parse_arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('k', type=int, help="Number of clusters to create")
	parser.add_argument('max_iter', type=int, nargs='?', default=100, help="Maximum iterations")
	parser.add_argument('file_name_1', type=str, help="File name 1")
	parser.add_argument('file_name_2', type=str, help="File name 2")

	args = parser.parse_args()
	if args.max_iter<=0:
		print("Maximum interations must be a poitive whole number.")
		sys.exit()

	return (args.k, args.max_iter, args.file_name_1, args.file_name_2)

###################
#Join file_1 + file_2:

def join_files(file_name_1, file_name_2):
	try:
	  data1 = pd.read_csv(file_name_1, header=None, dtype={0:int})
	except FileNotFoundError:
		print('Error opening file:', file_name_1)
		sys.exit()
	try:
	  data2 = pd.read_csv(file_name_2, header=None, dtype={0:int})
	except FileNotFoundError:
		print('Error opening file:', file_name_2)
		sys.exit()

	data = data1.merge(data2, on=0).sort_values(0)
	data.set_index(0, inplace=True)
	return data

###################
#Algo 1.1:

def dist(vec1, vec2): #returns distance between two vectors
	return sum(np.square(vec1 - vec2))

def closest_dist(centroids, vector): #returns distance from vector to closest centroid
	D = dist(vector[:-2], centroids.iloc[0].to_numpy()[:-2])
	for index, centroid in centroids.iterrows():
		tmp_val = dist(vector[:-2], centroid.to_numpy()[:-2])
		if (tmp_val <= D):
			D = tmp_val
	return D

def print_chosen_centroids(centroids):
	print(str(centroids.index.tolist()).strip("[]").replace(" ",""))

def format_vectors(centroids, vectors): #return a single python list of all vectors, where k centroids are first
	vectors = vectors.drop(['Dist','Prob'], axis=1)
	centroids = centroids.drop(['Dist','Prob'], axis=1)
	vectors = vectors.drop([i for i in centroids.index])
	vectors = pd.concat([centroids, vectors])
	return vectors.to_numpy().flatten().tolist()

def smart_centroids(vectors, K): #Algo 1.1
	
	N,d = vectors.shape
	if N<=K:
		return (None,None,None)

	vectors['Dist'] = vectors['Prob'] = 0.0
	centroids = vectors.iloc[[np.random.choice(N)]]

	Z = 1
	while (Z < K):
		for i in range(N):
			vectors['Dist'][i] = closest_dist(centroids, vectors.iloc[i].to_numpy())
		sumd = sum(vectors['Dist'])
		for i in range(N):
			vectors['Prob'][i] = vectors['Dist'][i]/sumd
		Z+=1
		row_num = np.random.choice(N, p=vectors['Prob'])
		centroids = centroids.append(vectors.iloc[row_num])

	print_chosen_centroids(centroids)
	num_arr = format_vectors(centroids, vectors)
	return (num_arr, N, d)

###################
#fit and print 

def fit_and_print_centroids(num_arr, N, d, k, MAX_ITER):
	centroids = np.split(np.array(fit(num_arr, N, d, k, MAX_ITER)),k)
	for centroid in centroids:
		centroid = np.round(centroid, decimals=4)
		print(str(centroid.tolist()).strip("[]").replace(" ",""))



if __name__ == "__main__":

	k, MAX_ITER, FILENAME_1, FILENAME_2 = parse_arguments()

	data = join_files(FILENAME_1, FILENAME_2)
	num_arr, N, d = smart_centroids(data, k)
	if num_arr == None:
		print("K value must be less than the number of vectors")
		sys.exit()
	fit_and_print_centroids(num_arr, N, d, k, MAX_ITER)
