import argparse
import sys
import numpy as np
import pandas as pd

np.random.seed(0)

#############
# Parse command line arguments:

def parse_arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('k', type=int, help="Number of clusters to create")
	parser.add_argument('goal', type=str, help="requested algorithm to preform")
	parser.add_argument('file_name', type=str, help="File name")
	max_iter = 300

	args = parser.parse_args()
	if args.k < 0:
		print("K must be non-negative")
		sys.exit()

	if args.goal not in ['spk', 'wam', 'ddg', 'lnorm', 'jacobi']:
		print("Please enter a valid goal")
		sys.exit()

	return (args.k, max_iter, args.goal, args.file_name)

#############
# Utils
def print_matrix(mat):
	for row in mat:
		print(row)

###################
# Algo 1.1:

def dist(vec1, vec2):  # returns distance between two vectors
	return sum(np.square(vec1 - vec2))


def closest_dist(centroids, vector):  # returns distance from vector to closest centroid
	D = dist(vector[:-2], centroids.iloc[0].to_numpy()[:-2])
	for index, centroid in centroids.iterrows():
		tmp_val = dist(vector[:-2], centroid.to_numpy()[:-2])
		if (tmp_val <= D):
			D = tmp_val
	return D


def print_chosen_centroids(centroids):
	print(str(centroids.index.tolist()).strip("[]").replace(" ", ""))


def format_vectors(centroids, vectors):  # return a single python list of all vectors, where k centroids are first
	vectors = vectors.drop(['Dist', 'Prob'], axis=1)
	centroids = centroids.drop(['Dist', 'Prob'], axis=1)
	vectors = vectors.drop([i for i in centroids.index])
	vectors = pd.concat([centroids, vectors])
	return vectors.to_numpy().flatten().tolist()


def smart_centroids(vectors, K):  # Algo 1.1

	N, d = vectors.shape
	if N <= K:
		return (None, None, None)

	vectors['Dist'] = vectors['Prob'] = 0.0
	centroids = vectors.iloc[[np.random.choice(N)]]

	Z = 1
	while (Z < K):
		for i in range(N):
			vectors['Dist'][i] = closest_dist(centroids, vectors.iloc[i].to_numpy())
		sumd = sum(vectors['Dist'])
		for i in range(N):
			vectors['Prob'][i] = vectors['Dist'][i] / sumd
		Z += 1
		row_num = np.random.choice(N, p=vectors['Prob'])
		centroids = centroids.append(vectors.iloc[row_num])

	print_chosen_centroids(centroids)
	num_arr = format_vectors(centroids, vectors)
	return (num_arr, N, d)


###################
# fit and print

def fit_and_print_centroids(num_arr, N, d, k, MAX_ITER):
	centroids = np.split(np.array(fit(num_arr, N, d, k, MAX_ITER)), k)
	for centroid in centroids:
		centroid = np.round(centroid, decimals=4)
		print(str(centroid.tolist()).strip("[]").replace(" ", ""))

def do_wam(mat):
	W = fit_wam(mat)
	print_matrix(W)

def do_ddg(mat):
	D = fit_ddg(mat)
	print_matrix(D)

def do_Lnorm(mat):
	L = fit_Lnorm(mat)
	print_matrix(L)

def do_jacobi(mat):
	U = fit_jacobi(mat)
	print_matrix(U)

def do_spk(mat, k, MAX_ITER):
	W = fit_wam(mat)
	Lnorm = fit_lnorm(W)
	if k == 0:
		k = fit_eigen(Lnorm)
	mat = Lnorm[:k]
	data = pd.DataFrame(mat)
	U = data.transpose()
	for i in range(len(U)):
		norm = (U.iloc[i] ** 2).sum()
		for j in range(U.columns.size):
			U[j][i] = U[j][i] / norm

	num_arr = U.to_numpy().flatten()
	N = len(U)
	d = U.columns.size
	fit_and_print_centroids(num_arr, N, d, k, MAX_ITER)

###################
# Main:

if __name__ == "__main__":

	k, MAX_ITER, GOAL, FILENAME = parse_arguments()

	data = pd.read_csv(FILENAME)
	mat = data.values.tolist()
	if GOAL == 'wam':
		do_wam(mat)
	if GOAL == 'ddg':
		do_ddg(mat)
	if GOAL == 'lnorm':
		do_Lnorm(mat)
	if GOAL == 'jacobi':
		do_jacobi(mat)
	if GOAL == 'spk':
		do_spk(mat, k, MAX_ITER)
