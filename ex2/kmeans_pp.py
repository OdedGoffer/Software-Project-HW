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

###################
#fit
###################

###################
#Print Results
###################
