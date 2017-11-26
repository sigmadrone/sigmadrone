import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from mx.DateTime.ODMG import Interval
import sys
from IPython.core.pylabtools import figsize
import subprocess
from matplotlib.widgets import RadioButtons

try:
    f = open(sys.argv[1], "r")
except Exception as e:
    print("invalid device file or no device specified")
    print("example: altitudeplot.py /dev/cu.usbserial-A101J62M")
    exit(1)

subprocess.Popen(["stty", "-f", sys.argv[1], "230400"])

ylim = 3

xdata = [0.0]
altitude = [0.0]
altitude_baro = [0.0]
vert_speed = [0.0]
vert_speed_baro = [0.0]
vert_speed_acc = [0.0]
pause = False


def setup_axes(axes, ax_label, xlim, ylim):
    axes.clear()
    axes.grid()
    axes.set_ylabel(ax_label, fontsize=16)  #, color=ax_color)
    axes.set_ylim([-ylim, ylim])
    axes.set_xlim([0, xlim])


# Create a figure with two subplots
fig, (ax1, ax2) = plt.subplots(2, 1)
setup_axes(ax1, "Altitude, m", 64, ylim)
setup_axes(ax2, "Vert Speed, m/s", 64, ylim)
line1, = ax1.plot(xdata, altitude, lw=1, label='Altitude', color='blue')
line2, = ax1.plot(
    xdata, altitude_baro, lw=1, label='Raw Altitude', color='red')
line3, = ax2.plot(xdata, vert_speed, lw=1, label='Vert Speed', color='blue')
line4, = ax2.plot(
    xdata, vert_speed, lw=1, label='Vert Speed Baro', color='red')
line5, = ax2.plot(
    xdata, vert_speed, lw=1, label='Vert Speed Accel x 10', color='orange')
line = [line1, line2, line3, line4, line5]
ax1.legend(loc='upper left', shadow=True)
ax2.legend(loc='upper left', shadow=True)

plt.subplots_adjust(top=0.8)

# initialization function: plot the background of each frame
def init():
    global xdata
    global altitude
    global altitude_baro
    global vert_speed
    global vert_speed_baro
    global vert_speed_acc

    xdata = [0.0]
    altitude = [0.0]
    altitude_baro = [0.0]
    vert_speed = [0.0]
    vert_speed_baro = [0.0]
    vert_speed_acc = [0.0]
    line[0].set_data(xdata, altitude)
    line[1].set_data(xdata, altitude_baro)
    line[2].set_data(xdata, vert_speed)
    line[3].set_data(xdata, vert_speed_baro)
    line[4].set_data(xdata, vert_speed_acc)
    return line

# animation function.  This is called sequentially
def animate(indata):
    t, text = indata

    if pause or (text.count(',') is not 5):
        return line

    alt, alt_baro, vs, vs_baro, vs_acc, dt = text.split('\n')[0].split(',')[:6]
    
    if float(alt) < ax1.get_ylim()[0] or float(alt) > ax1.get_ylim()[1]:
        ax1.set_ylim([float(alt)-ylim, float(alt)+ylim]) 

    xmin, xmax = ax1.get_xlim()
    if t >= xmax:
        ax1.set_xlim(xmin, 2*xmax)
        ax2.set_xlim(xmin, 2*xmax)
        ax1.figure.canvas.draw()
        ax2.figure.canvas.draw()
    
    xdata.append(t)
    altitude.append(alt)
    altitude_baro.append(alt_baro)
    vert_speed.append(vs)
    vert_speed_baro.append(vs_baro)
    vert_speed_acc.append(float(vs_acc) * 10)

    line[0].set_data(xdata, altitude)
    line[1].set_data(xdata, altitude_baro)
    line[2].set_data(xdata, vert_speed)
    line[3].set_data(xdata, vert_speed_baro)
    line[4].set_data(xdata, vert_speed_acc)
    return line

iter = 0
def on_click(label):
    global pause
    global iter
    pause = label == 'Off'
    if not pause:
        init()
        iter = 0
        ax1.set_xlim(0, 64)
        ax2.set_xlim(0, 64)
        ax1.figure.canvas.draw()
        ax2.figure.canvas.draw()

def datagen():
    global iter
    iter = 0
    lastline = f.readline()
    lastline = f.readline()
    lastline = f.readline()
    while True:
        iter += 1
        lastline = f.readline()
        yield iter, lastline

rax = plt.axes([0.1, 0.85, 0.11, 0.11])
radio = RadioButtons(rax, ('On', 'Off'))
radio.on_clicked(on_click)

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(
    fig, animate, frames=datagen, init_func=init, interval=1, blit=True)

plt.show()
