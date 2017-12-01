import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from mx.DateTime.ODMG import Interval
import sys
from IPython.core.pylabtools import figsize
import subprocess
from matplotlib.widgets import RadioButtons
import threading

ylim = 3

xdata = []
altitude = []
altitude_baro = []
vert_speed = []
vert_speed_baro = []
vert_speed_acc = []
collect_data = True


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

    xdata = []
    altitude = []
    altitude_baro = []
    vert_speed = []
    vert_speed_baro = []
    vert_speed_acc = []
    line[0].set_data([],[])
    line[1].set_data([],[])
    line[2].set_data([],[])
    line[3].set_data([],[])
    line[4].set_data([],[])
    return line

def reader():
    global collect_data
    try:
        f = open(sys.argv[1], "r")
    except Exception as e:
        print("invalid device file or no device specified")
        print("example: altitudeplot.py /dev/cu.usbserial-A101J62M")
        exit(1)
    subprocess.Popen(["stty", "-f", sys.argv[1], "230400"])
    iter = 0
    while collect_data:
        iter += 1
        accumulate_data((iter, f.readline()))
    
    f.close()

reader_thread = threading.Thread(target=reader)
reader_thread.start()

# animation function.  This is called sequentially
def accumulate_data(indata):
    t, text = indata
    if text.count(',') == 5:
        alt, alt_baro, vs, vs_baro, vs_acc, dt = text.split('\n')[0].split(',')[:6]
        if len(xdata) == 0:
            xdata.append(float(dt))
        else:
            xdata.append(float(dt) + xdata[len(xdata)-1])
        altitude.append(float(alt))
        altitude_baro.append(float(alt_baro))
        vert_speed.append(float(vs))
        vert_speed_baro.append(float(vs_baro))
        vert_speed_acc.append(float(vs_acc) * 10)
    return

def on_click(label):
    global collect_data
    global reader_thread
    if label == 'Collect Data':
        if not collect_data:
            init()
            reader_thread = threading.Thread(target=reader)
            reader_thread.start()
            collect_data = True
    else:
        collect_data = False
        reader_thread.join()
        ax1.set_ylim([min(altitude_baro), max(altitude_baro)])
        ax1.set_xlim([0, xdata[len(xdata)-1]])
        ax2.set_ylim(min([min(vert_speed_baro),min([min(vert_speed), min(vert_speed_acc)])]), 
            max([max(vert_speed_baro),max([max(vert_speed), max(vert_speed_acc)])]))
        ax2.set_xlim([0, xdata[len(xdata)-1]])
        line[0].set_data(xdata, altitude)
        line[1].set_data(xdata, altitude_baro)
        line[2].set_data(xdata, vert_speed)
        line[3].set_data(xdata, vert_speed_baro)
        line[4].set_data(xdata, vert_speed_acc)
        plt.draw()

rax = plt.axes([0.1, 0.85, 0.2, 0.15])
radio = RadioButtons(rax, ('Collect Data', 'Display Data'))
radio.on_clicked(on_click)

plt.show()