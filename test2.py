import pandas as pd
import numpy as np

data = {'rank': [1, 2, 3, 4],
        'speed': [5, 6, 7, 8],
        'acceleration': [3, 4, 5, 6]}
df1 = pd.DataFrame(data)
nparr1 = np.array(df1)
x1=[1,2,3]
x2=(1,2,3,4,5)
x3=6.71
x4={"Bob":39}
x5=4
x6='fdjsklfa'
df = pd.read_csv('D:\downloads\df_erweitert_Test.txt', sep=';',header=0)
df2 = df.head()
row_10 = df.iloc[9]
arr1 = np.array(df.head())
rand_array = np.random.rand(3,4)*100
pdarr = pd.DataFrame(rand_array)