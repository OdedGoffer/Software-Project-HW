from sklearn.cluster import KMeans
from sklearn.datasets import load_iris
import matplotlib.pyplot as plt
import numpy as np

data = load_iris()
vectors = data.data
inertia_lst = []

k in range(1,11):
	kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0).fit(vectors)
	k_inertia = inertia(vectors, kmeans.labels_, kmeans.cluster_centers_)
	inertia_lst.append(k_inertia)

plt.plot(np.arrange(1,11),inertia_lst)
plt.show
