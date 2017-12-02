#!/usr/bin/python

import sys
#from matplotlib.pyplot import autoscale
import threading
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import select

if (len(sys.argv) > 1):
    try:
        f = open(sys.argv[1])
    except Exception as e:
        print("invalid device file or no device specified")
        print("example: livedataplot.py /dev/ttyUSB0")
        exit(1)
else:
    f = sys.stdin


samplecount = 750

gyro = [[0,0,0] for i in range(samplecount)]
acc = [[0,0,0] for i in range(samplecount)]
pid = [[0,0,0] for i in range(samplecount)]
lock = threading.Lock()
runreader = True

"""
Non blocking read
"""
def read_line_nb(f):
    line = ""
    rlist = [f]
    wlist = []
    xlist = []
    rd,wl,xl = select.select(rlist, wlist, xlist, 5.0)
    if rd:
        line = rd[0].readline()
    return line


""" Thread data reader function """
def reader():
    for i in range(10):
        line = read_line_nb(f)
    while (runreader):
        line = read_line_nb(f)
        data = line.split('\n')[0].split(' ')
        a = np.reshape(np.array(data[:9]), (3,3))

        lock.acquire(True)
        gyro.append(a[0].tolist())
        del(gyro[0])
        acc.append(a[1].tolist())
        del(acc[0])
        pid.append(a[2].tolist())
        del(pid[0])
        lock.release()

def animate(i):
    ylimits = [-100, 100]
    ax1.clear()
    ax1.grid()
    ax1.set_ylabel(r"X (Deg/Sec)", fontsize=16, color="darkgreen")
    ax1.set_ylim(ylimits)
    ax2.clear()
    ax2.grid()
    ax2.set_ylabel(r"Y (Deg/Sec)", fontsize=16, color="darkgreen")
    ax2.set_ylim(ylimits)
    if (lock.acquire(False)):
        array_gyro = np.array(gyro);
        array_acc = np.array(acc);
        array_pid = np.array(pid);
        scaled_acc_x = [(float(i) * ylimits[1] * 1.5) for i in array_acc[:,0]]
        scaled_acc_y = [-(float(i) * ylimits[1] * 1.5) for i in array_acc[:,1]]
        scaled_pid_x = [(float(i) * ylimits[1] * 5.0) for i in array_pid[:,0]]
        scaled_pid_y = [(float(i) * ylimits[1] * 5.0) for i in array_pid[:,1]]                
        ax1.plot(array_gyro[:,0], "red", scaled_acc_x, "green", scaled_pid_x, "blue")
        ax2.plot(array_gyro[:,1], "red", scaled_acc_y, "green", scaled_pid_y, "blue")        
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
        
