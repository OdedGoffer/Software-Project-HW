from sklearn.cluster import KMeans
from sklearn.datasets import load_iris
import matplotlib.pyplot as plt
import numpy as np

def inertia(vectors, labels, centers):
	inertia = 0
	for i in range(len(vectors)):
		inertia += sum(np.square(vectors[i]-centers[labels[i]]))
	return inertia

data = load_iris()
vectors = data.data
inertia_lst = []

for k in range(1,11):
	kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0).fit(vectors)
	k_inertia = inertia(vectors, kmeans.labels_, kmeans.cluster_centers_)
	inertia_lst.append(k_inertia)

plt.plot(np.arange(1,11),inertia_lst)
plt.xlabel("K")
plt.ylabel("Average Dispersion")
plt.title('Elbow Meathod for selection of optimal "K" clusters')
plt.savefig('elbow.png')
