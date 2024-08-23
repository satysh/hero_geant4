import pandas as pd
from math import sin, cos, pi
import random as rnd
from mpl_toolkits.mplot3d import Axes3D 
import matplotlib.pyplot as plt 
import numpy as np 
from pylab import *


min_theta = 0.
max_theta = pi
min_phi = 0.
max_phi = 2. * pi

thetas = []
phis = []
cos_thetas_rnd = []


for _ in range(10000):
    thetas.append(rnd.uniform(min_theta, max_theta))
    phis.append(rnd.uniform(min_phi, max_phi))
    cos_thetas_rnd.append(rnd.uniform(-1., 1.))


thetas = pd.Series(thetas)
phis= pd.Series(phis)

sin_thetas = thetas.apply(sin)
cos_thetas = thetas.apply(cos)
sin_phis = phis.apply(sin)
cos_phis = phis.apply(cos)

px = sin_thetas * cos_phis
py = sin_thetas * sin_phis
pz = cos_thetas.copy()

cos_thetas_rnd = pd.Series(cos_thetas_rnd)
sin_thetas_rnd = (pd.Series([1. for _ in range(len(cos_thetas_rnd))]) - cos_thetas_rnd**2)**0.5

px_alter = sin_thetas_rnd * cos_phis
py_alter = sin_thetas_rnd * sin_phis
pz_alter = cos_thetas_rnd

fig = plt.figure()
ax = fig.add_subplot(projection='3d')
#colo = [px_alter + py_alter + pz_alter]
colo = [px + py + pz]

color_map = cm.ScalarMappable(cmap=cm.Greens_r) 
color_map.set_array(colo) 

#ax.scatter(px_alter, py_alter, pz_alter, marker='o', s=10, color='green')
ax.scatter(px, py, pz, marker='o', s=10, color='green');

plt.colorbar(color_map) 
plt.show()