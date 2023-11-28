import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('res.csv', header=None)
DIMS = [10, 100, 1000, 10000]
LEN = len(DIMS)
data = [[[] for _ in range(2)] for _ in range(2)]
for idx, row in df.iterrows():
    data[idx // LEN][0].append(np.log10(row.values[0]))
    data[idx // LEN][1].append(row.values[1])
for i in range(2):
    plt.semilogy(data[i][0], data[i][1], label=['Integer', 'Complex'][i])
    
plt.legend()
plt.xlabel('log10(Dimension)')
plt.ylabel('Time (s)')
plt.show()
