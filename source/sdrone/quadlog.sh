#!/bin/bash

# $1 log file, $2 ang vel, $3 desired ang vel, $4 ang accel, $5 motors

logFile=$1
#if [ "$logfile" = "" ] 
#then
#	echo Assuming flight.log to be the log 
#        logfile="flight.log"
#fi

grep -w AngV $logFile | cut --delimiter=' ' --fields=5-7 > angv.dat
grep -w AngVD $logFile | cut --delimiter=' ' --fields=4-6 > angvdes.dat
grep -w AngAcc $logFile | cut --delimiter=' ' --fields=3-5 > angacc.dat
grep -w Torque $logFile | cut --delimiter=' ' --fields=3-5,7-9,11 > torque.dat
grep -w "Motor" $logFile | cut --delimiter=' ' --fields=4-7 > motor.dat
grep "Q     :" $logFile | cut --delimiter=' ' --fields=8-11 > attq.dat
