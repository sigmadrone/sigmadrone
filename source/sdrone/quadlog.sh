#!/bin/bash

# $1 log file, $2 ang vel, $3 desired ang vel, $4 ang accel, $5 motors

logFile=$1
#if [ "$logfile" = "" ] 
#then
#	echo Assuming flight.log to be the log 
#        logfile="flight.log"
#fi
grep -w "Err   :" $logFile | cut --delimiter=' ' --fields=6-9 > torque.dat
grep -w "ErrPID:" $logFile | cut --delimiter=' ' --fields=3-5 > torquepid.dat
grep -w "Motor" $logFile | cut --delimiter=' ' --fields=4-7 > motor.dat
grep "Q     :" $logFile | cut --delimiter=' ' --fields=8-11 > attq.dat
grep "Accel :" $logFile | cut --delimiter=' ' --fields=4-6 > accel.dat
grep "Gyro  :" $logFile | cut --delimiter=' ' --fields=5-7 > gyro.dat
grep "Mag   :" $logFile | cut --delimiter=' ' --fields=6-8 > mag.dat
