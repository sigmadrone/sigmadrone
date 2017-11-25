import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
import mx
from mx.DateTime.ODMG import Interval
import sys
from IPython.core.pylabtools import figsize
import subprocess

try:
    f = open(sys.argv[1], "r")
except Exception as e:
    print("invalid device file or no device specified")
    print("example: altitudeplot.py /dev/cu.usbserial-A101J62M")
    exit(1)

subprocess.Popen(["stty", "-f", sys.argv[1], "230400"])

ylim = 3
sample_count = 2048

xdata = np.linspace(0,sample_count-1,sample_count)
altitude = np.linspace(0,0,sample_count)
altitude_baro = np.linspace(0,0,sample_count)
vert_speed = np.linspace(0,0,sample_count)
vert_speed_baro = np.linspace(0,0,sample_count)
vert_speed_baro_raw = np.linspace(0,0,sample_count)
vert_speed_acc = np.linspace(0,0,sample_count)

def setup_axes(axes, ax_label, xlim, ylim):
    axes.clear()
    axes.grid()
    axes.set_ylabel(ax_label, fontsize=16)#, color=ax_color)
    axes.set_ylim([-ylim,ylim])
    axes.set_xlim([0,xlim])

# Create a figure with two subplots
fig, (ax1, ax2) = plt.subplots(2,1)
setup_axes(ax1, "Altitude, m",  sample_count-1, ylim)
setup_axes(ax2, "Vert Speed, m/s", sample_count-1, ylim)
line1, = ax1.plot(xdata, altitude, lw=1, label='Altitude', color='blue')
line2, = ax1.plot(xdata, altitude_baro, lw=1, label='Raw Altitude', color='red')
line3, = ax2.plot(xdata, vert_speed, lw=1, label='Vert Speed', color='blue')
line4, = ax2.plot(xdata, vert_speed, lw=1, label='Vert Speed Baro', color='red')
line5, = ax2.plot(xdata, vert_speed, lw=1, label='Vert Speed Accel x 10', color='orange')
#line6, = ax2.plot(xdata, vert_speed, lw=1, label='Vert Speed Baro Raw', color='green')
line = [line1, line2, line3, line4, line5]
ax1.legend(loc='upper left',shadow=True)
ax2.legend(loc='upper left',shadow=True)

# initialization function: plot the background of each frame
def init():
    line[0].set_data(xdata, altitude)
    line[1].set_data(xdata, altitude_baro)
    line[2].set_data(xdata, vert_speed)
    line[3].set_data(xdata, vert_speed_baro)
    line[4].set_data(xdata, vert_speed_acc)
    #line[5].set_data(xdata, vert_speed_baro_raw)
    return line

def update_ydata(ydata, new_sample):
    ydata = np.roll(ydata,-1)
    ydata[sample_count-1] = new_sample
    return ydata

# animation function.  This is called sequentially
def animate(text):
    global altitude
    global vert_speed
    global altitude_baro
    global vert_speed_baro
    global vert_speed_baro_raw
    global vert_speed_acc
    #print(text)
    alt,alt_baro,vs,vs_baro,vs_baro_raw,vs_acc = text.split('\n')[0].split(',')[:6]
    altitude = update_ydata(altitude,alt)
    altitude_baro = update_ydata(altitude_baro,alt_baro)
    vert_speed = update_ydata(vert_speed,vs)
    vert_speed_baro = update_ydata(vert_speed_baro,vs_baro)
    vert_speed_baro_raw = update_ydata(vert_speed_baro_raw,vs_baro_raw)
    vert_speed_acc = update_ydata(vert_speed_acc,float(vs_acc)*10)
    line[0].set_data(xdata, altitude)
    line[1].set_data(xdata, altitude_baro)
    line[2].set_data(xdata, vert_speed)
    line[3].set_data(xdata, vert_speed_baro)
    line[4].set_data(xdata, vert_speed_acc)
    #line[5].set_data(xdata, vert_speed_baro_raw)
    return line

def datagen():
    tmpline = f.readline()
    tmpline = f.readline()
    tmpline = f.readline()
    while True:
        yield f.readline()

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, frames=datagen, init_func=init, interval=1)

plt.show()