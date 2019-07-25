import pandas as pd
import numpy as np
import glob
import matplotlib.pyplot as plt
n=10
num_rows=10000
filenames=[]
f = "population_size_00001"
filenames.append(f)
num_lines = sum(1 for l in open(f))
skip_idx = [x for x in range(1,num_lines) if ((x%n!=0) or (x>n*num_rows))]
data_temp = pd.read_csv(f, skiprows=skip_idx, sep='\t')
num_cols=data_temp.shape[1]
data=np.asarray(data_temp).reshape(num_rows,num_cols,1)
for f in glob.glob('*'):
  if f!="population_size_00001" and f!="parameters.txt":
    print(f)
    filenames.append(f)
    num_lines = sum(1 for l in open(f))
    skip_idx = [x for x in range(1,num_lines) if ((x%n!=0) or (x>n*num_rows))]
    data_temp = pd.read_csv(f, skiprows=skip_idx, sep='\t')
    print(np.asarray(data_temp).shape)
    data = np.append(data,np.asarray(data_temp).reshape(num_rows,num_cols,1),axis=2)


count_non_trivial=0
for i in range(data.shape[2]):
  if np.sum(data[-1,1:num_cols-1,i]>0)>1:
    count_non_trivial += 1


count_surviving_species=np.zeros(np.range(1,num_cols-1))
for i in range(data.shape[2]):
  for j in range(1,num_cols-1):
    count_surviving_species[j-1] += data[-1,j,i]>0



count=1
for i in range(9):
  if np.sum(data[-1,1:num_cols-1,i]>0)>1:
    plt.subplot(3,3,count)
    for j in range(1,num_cols-1):
      plt.plot(data[:,0,i],data[:,j,i])
    plt.yscale('log')
    count += 1

plt.show()


