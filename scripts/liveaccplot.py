#!/usr/bin/python

#from mx.DateTime.ODMG import Interval
import sys
from cmath import sqrt
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

samplecount = 1

data1 = [[0,0,0] for i in range(samplecount)]
data2 = [[0,0,0]]
data3 = [[0,0,0] for i in range(750)]
data4 = [[0,0,0]]
data5 = [[0,0,0]]
data6 = [[0,0,0]]
data7 = [[0,0,0]]
data8 = [[0,0,0]]
data9 = [[0,0,0]]


lock = threading.Lock()
runreader = True
alpha = 0.0
scale_factor = 2.5
arrow_width = 0.0001
arrow_head_length=0.01
arrow_head_width = arrow_head_length/2

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
        a=np.reshape(np.array(data[:27]), (9,3))
        data1.append([float(i) * -1 for i in a[0].tolist()])
        del data1[0]
        data2.append([float(i) * -1 for i in a[1].tolist()])
        del data2[0]
        data3.append(a[2].tolist())
        del data3[0]
        del data4[:]
        data4.append([float(i) * -1.0 for i in a[3].tolist()])
        del data5[:]
        data5.append([float(i) * -1.0 for i in a[4].tolist()])
        del data6[:]
        data6.append([float(i) * -1.0 for i in a[5].tolist()])        
        del data7[:]
        data7.append([float(i) * -1.0 for i in a[6].tolist()])        
        del data8[:]
        data8.append([float(i) * -1.0 for i in a[7].tolist()])     
        del data9[:]
        data9.append([float(i) * -1.0 for i in a[8].tolist()])     
        lock.release()

def animate(i):
    ylimits = [-0.25, 0.25]
    xlimits = [-0.25, 0.25]
    ax1.clear()
    ax1.grid()
    ax1.set_xlabel(r"Y", fontsize=16, color="darkgreen")    
    ax1.set_ylabel(r"X", fontsize=16, color="darkgreen")
    ax1.set_ylim(ylimits)
    ax1.set_xlim(xlimits)
    ax2.clear()
    ax2.grid()
    ax2.set_ylabel(r"Y (Rad/Sec)", fontsize=16, color="darkgreen")
    ax2.set_ylim(ylimits)
    torq_p_text = ax1.text(-0.2, 0.2, '', style='italic', color='r')
    torq_d_text = ax1.text(-0.2, 0.15, '', style='italic', color='b')
    torq_i_text = ax1.text(-0.2, 0.10, '', style='italic', color='g')
    if (lock.acquire(False)):
        acc1 = np.array(data1)
        acc2 = np.array(data2)
        second_acc = np.array(data9)
        w = np.array(data3)
        torq_p = np.array(data4)
        torq_d = np.array(data5)
        torq_i = np.array(data6)
        earth_g = np.array(data7)
        target =  np.array(data8)
        torq_p_text.set_text('Torque_P = %.4f' % ((float(torq_p[0][0])**2 + (float(torq_p[0][1])**2)))**0.5)
        torq_d_text.set_text('Torque_D = %.4f' % ((float(torq_d[0][0])**2 + (float(torq_d[0][1])**2)))**0.5)
        torq_i_text.set_text('Torque_I = %.4f' % ((float(torq_i[0][0])**2 + (float(torq_i[0][1])**2)))**0.5)
        ax1.plot(acc1[:,1], acc1[:,0], "ro")
        ax1.plot(second_acc[:,1], second_acc[:,0], "mo", alpha=0.5)
        ax1.plot(acc2[:,1], acc2[:,0], "yo")
        ax1.plot(earth_g[:,1], earth_g[:,0], "go", markersize=20, alpha=0.5)
        ax1.plot(target[:,1], target[:,0], "bo", markersize=3, alpha=0.75)        
        ax1.arrow(acc2[0][1], acc2[0][0], torq_p[0][1] * scale_factor, torq_p[0][0] * scale_factor, fc='r', ec='r', width=arrow_width, head_width=arrow_head_width, head_length=arrow_head_length);
        ax1.arrow(acc2[0][1], acc2[0][0], torq_d[0][1] * scale_factor, torq_d[0][0] * scale_factor, fc='b', ec='b', width=arrow_width, head_width=arrow_head_width, head_length=arrow_head_length);
        ax1.arrow(acc2[0][1], acc2[0][0], torq_i[0][1] * scale_factor, torq_i[0][0] * scale_factor, fc='g', ec='g', width=arrow_width, head_width=arrow_head_width, head_length=arrow_head_length);
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
        
