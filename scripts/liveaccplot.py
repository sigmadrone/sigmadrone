#!/usr/bin/python

#from mx.DateTime.ODMG import Interval
import sys
#from matplotlib.pyplot import autoscale
#from IPython.core.pylabtools import figsize

if (len(sys.argv) > 1):
    try:
        f = open(sys.argv[1])
    except Exception as e:
        print("invalid device file or no device specified")
        print("example: livedataplot.py /dev/ttyUSB0")
        exit(1)
else:
    f = sys.stdin

import threading
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import select

samplecount = 25

data1 = [[0,0,0]]
data2 = [[0,0,0] for i in range(samplecount)]
data3 = [[0,0,0] for i in range(750)]
lock = threading.Lock()
runreader = True
alpha = 0.0


def read_line_nb(f):
    line = ""
    rlist = [f]
    wlist = []
    xlist = []
    rd,wl,xl = select.select(rlist, wlist, xlist, 5.0)
    if rd:
        line = rd[0].readline()
    return line

def reader():
    """ Thread data reader function """
    global alpha
    line = read_line_nb(f)
    line = read_line_nb(f)
    line = read_line_nb(f)
    while (runreader):
        line = read_line_nb(f)
        if line == "":
            break
        data = line.split('\n')[0].split(' ')
        lock.acquire(True)
        alpha = 0.0
        a=np.reshape(np.array(data[:9]), (3,3))
        data1.append(a[0].tolist())
        del data1[0]
        data2.append(a[1].tolist())
        del data2[0]
        data3.append(a[2].tolist())
        del data3[0]        
        lock.release()

def animate(i):
    ylimits = [-0.5, 0.5]
    xlimits = [-0.5, 0.5]
    ax1.clear()
    ax1.grid()
    ax1.set_xlabel(r"X", fontsize=16, color="darkgreen")    
    ax1.set_ylabel(r"Y", fontsize=16, color="darkgreen")
    ax1.set_ylim(ylimits)
    ax1.set_xlim(xlimits)
    ax2.clear()
    ax2.grid()
    ax2.set_ylabel(r"Y (Rad/Sec)", fontsize=16, color="darkgreen")
    ax2.set_ylim(ylimits)
    time_text = ax1.text(-0.3, 0.3, '', style='italic')
    time_text.set_text('Alpha = %.4f' % alpha)
    if (lock.acquire(False)):
        acc1 = np.array(data1)
        acc2 = np.array(data2)
        w = np.array(data3)
        ax1.plot(acc2[:,0], acc2[:,1], "yo")
        ax1.plot(acc1[:,0], acc1[:,1], "ro")
        ax2.plot(w[:,0], "green")
        lock.release()

# reader()
# exit()

t = threading.Thread(target=reader)
t.start()

fig = plt.figure()
ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)
ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()
runreader = False
t.join(0.5)
        
