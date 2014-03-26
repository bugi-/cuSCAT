import numpy as np
import matplotlib.pyplot as plt

filename = "map.out"

data = np.loadtxt(filename)

print "Max value:", np.max(data)

plt.pcolor(data, vmin=0, vmax=np.max(data))
plt.show()

