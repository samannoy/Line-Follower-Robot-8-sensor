# Line-Follower-Robot-8-sensor
Line follower robot
Hardware: 8 IR sensor array,
Atmega 328(Arduino Uno or pro Mini),
HC-05 Bluetooth Module to update PID values,
2 DC Motors(500rpm geared motors),
Motor Driver-Tb6612fng

## If you use powerful motors that draw a lot of current, DO NOT USE L293D/L298N; it will fry your Arduino due to power surge.
The code has been tested on a race track with complementary black and white lines and works perfectly well.
Depending on the type of turns present on the race track, modify the possible error conditions and flags.

The PCB files were designed using ExpressPCB and fabricated in-house and hence are not similar to professional design files like the ones generated in EAGLE or Altium.

** Feel free to ping or raise requests if you face any troubles or if you want to add something as well
## Don't forget to credit me if the code comes to your help. Cheers!!
