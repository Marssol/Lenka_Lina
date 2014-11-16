#!/usr/bin/python
#-*- coding:utf8 -*-

import numpy as np
import matplotlib.pyplot as plt
import sys
import io

#x, y = np.genfromtxt('stats.csv', delimiter=',').T
input = io.open(sys.stdin.fileno(), "rb")
x, y = np.genfromtxt(input, delimiter=' ').T
plt.plot(x, '-', label='Sequential time')
plt.plot(y, '-', label='Parallel time')
plt.xlabel("Size of the array")
plt.ylabel("Time (micro secondes)")
plt.title('Search time in function of the size of the array')
plt.legend(loc='best')
plt.show()

