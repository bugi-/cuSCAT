import numpy as np
import matplotlib.pyplot as plt

filename = "map.out"

def plot_file(filename):
	data = np.loadtxt(filename)
	print plot_file.i, "Max value:", np.max(data)
	
	plt.subplot(1, 2, plot_file.i, aspect="equal")
	plt.pcolormesh(data, vmin=0, vmax=np.max(data), cmap="binary_r") # Reversed grayscale colours are used so that white means the most photons
	plt.colorbar()
	plt.title(filename)
	plot_file.i += 1

plot_file.i = 1

fig = plt.figure()
fig.set_size_inches(20,10)
plot_file(filename)
plot_file("map0.0.out")

plt.show()