import argparse, sys, os
import spkmeans as c_api
import numpy as np
import pandas as pd

np.random.seed(0)

###############################
# Parse command line arguments
###############################

def parse_arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('k', type=int, help="Number of clusters to create")
	parser.add_argument('goal', type=str, help="requested algorithm to preform")
	parser.add_argument('file_name', type=str, help="File name")

	args = parser.parse_args()
	if args.k < 0:
		print("K must be non-negative")
		sys.exit()

	if args.goal not in ['spk', 'wam', 'ddg', 'lnorm', 'jacobi']:
		print("Please enter a valid goal")
		sys.exit()

	return (args.k, args.goal, args.file_name)

########
# Utils
########

def print_vector(vec):
	[print(f"{np.round(val, 4)}, ", end='') for val in vec[:-1]]
	print(np.round(vec[-1],4))


def print_matrix(mat, dim, vec_num):
	matrix = [mat[i:i + dim] for i in range(0, vec_num * dim, dim)]
	for row in matrix:
		print_vector(row)

##########################
# Algo 1.1 from Exercise 2
##########################

# Returns the distance between two vectors.
def dist(vec1, vec2):
	return sum(np.square(vec1 - vec2))


# Returns the distance from vector to closest centroid.
def closest_dist(centroids, vector):
	D = dist(vector[:-2], centroids.iloc[0].to_numpy()[:-2])
	for index, centroid in centroids.iterrows():
		tmp_val = dist(vector[:-2], centroid.to_numpy()[:-2])
		if (tmp_val <= D):
			D = tmp_val
	return D


def print_chosen_centroids(centroids):
	print(str(centroids.index.tolist()).strip("[]").replace(" ", ""))


# Returns a single python list of all vectors, where k centroids are first.
def format_vectors(centroids, vectors):
	vectors = vectors.drop(['Dist', 'Prob'], axis=1)
	centroids = centroids.drop(['Dist', 'Prob'], axis=1)
	vectors = vectors.drop([i for i in centroids.index])
	vectors = pd.concat([centroids, vectors])
	return vectors.to_numpy().flatten().tolist()


# Algo 1.1
def smart_centroids(vectors, K):
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

########
# C API
########

def do_wam(mat, dim, vec_num):
	W = c_api.WAM(mat, dim, vec_num)
	print_matrix(W, vec_num, vec_num)


def do_ddg(mat, dim, vec_num):
	W = c_api.WAM(mat, dim)
	D = c_api.DDG(W, vec_num)
	print_matrix(D, vec_num, vec_num)


def do_Lnorm(mat, dim, vec_num):
	W = c_api.WAM(mat, dim, vec_num)
	D = c_api.DDG(W, vec_num)
	L = c_api.LNORM(W, D, vec_num)
	print_matrix(L, vec_num, vec_num)


def do_jacobi(mat, dim):
	try:
		eigenvectors, eigenvalues = c_api.jacobi(mat, dim)
	except TimeoutError:
		print("Jacobi method reached maximum iterations with no convergence.")
		sys.exit()
	except ValueError as e:
		print(f"Jacobi method returned an error: {e}")
		sys.exit()

	print_vector(eigenvalues)
	print_matrix(eigenvectors, dim, dim)


def do_spk(mat, dim, vec_num, k, MAX_ITER):
	# W = c_api.WAM(mat, dim, vec_num)
	# D = c_api.DDG(W, vec_num)
	# L = c_api.LNORM(W, D, vec_num)
	# eigenvectors, eigenvalues = c_api.jacobi(L, vec_num)
	# T, new_k = eigengap(eigenvectors, eigenvalues, k)
	# kmeans(T, vec_num, new_k, new_k)
	pass

#######
# Main
#######

if __name__ == "__main__":

	k, GOAL, FILENAME = parse_arguments()

	if not os.path.isfile(FILENAME):
		raise FileNotFoundError(f"No such file: {FILENAME}")

	data = pd.read_csv(FILENAME, header=None)
	dim = data.columns.size
	vec_num = data.shape[0]
	mat = data.to_numpy().flatten().tolist()

	if k >= vec_num:
		raise ValueError(f"BAD K: {k}; must be less than vectors count.")

	if GOAL == 'wam':
		do_wam(mat, dim, vec_num)
	elif GOAL == 'ddg':
		do_ddg(mat, dim, vec_num)
	elif GOAL == 'lnorm':
		do_Lnorm(mat, dim, vec_num)
	elif GOAL == 'jacobi':
		do_jacobi(mat, dim)
	elif GOAL == 'spk':
		do_spk(mat, dim, vec_num, k)