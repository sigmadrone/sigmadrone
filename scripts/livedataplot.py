#!/usr/bin/python

from mx.DateTime.ODMG import Interval
import sys
from matplotlib.pyplot import autoscale
from IPython.core.pylabtools import figsize

try:
    f = open(sys.argv[1])
except Exception as e:
    print("invalid device file or no device specified")
    print("example: livedataplot.py /dev/ttyUSB0")
    exit(1)

import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

samplecount = 3000

gyro_x = [0 for i in range(samplecount)]
gyro_y = [0 for i in range(samplecount)]
gyro_z = [0 for i in range(samplecount)]

lock = threading.Lock()
runreader = True

def reader():
    """ Thread data reader function """
    line = f.readline()
    line = f.readline()
    line = f.readline()
    while (runreader):
        line = f.readline()
        x,y,z = line.split('\n')[0].split(',')[:3]
        lock.acquire(True)
        gyro_x.append(x)
        del gyro_x[0]
        gyro_y.append(y)
        del gyro_y[0]
        gyro_z.append(z)
        del gyro_z[0]        
        lock.release()

def animate(i):
    ylimits = [-90, 90]
    ax1.clear()
    ax1.grid()
    ax1.set_ylabel(r"X (Deg/Sec)", fontsize=16, color="darkgreen")
    ax1.set_ylim(ylimits)
    ax2.clear()
    ax2.grid()
    ax2.set_ylabel(r"Y (Deg/Sec)", fontsize=16, color="darkgreen")
    ax2.set_ylim(ylimits)
    if (lock.acquire(False)):
        ax1.plot(gyro_x, "red")
        ax2.plot(gyro_y, "green")        
        lock.release()

t = threading.Thread(target=reader)
t.start()

fig = plt.figure()
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)
ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()
runreader = False
t.join(0.5)
        
