import numpy as np
import matplotlib.pyplot as plt

filename = "map.out"

data = np.loadtxt(filename)

print "Max value:", np.max(data)

plt.pcolormesh(data, vmin=0, vmax=np.max(data), cmap="binary_r") # Reversed grayscale colours are used
plt.colorbar()
plt.show()

