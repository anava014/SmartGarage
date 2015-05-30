# SmartGarage
Hacking a garage and making it smart so that it knows when you're approaching with your vehicle.

This was a DIY project proposed to IEEE @ UCR and it was my first time learning lots of new material such as
using a GPS module, bluetooth module, raspberry Pi funcitonality and even creating my own android app.

It starts with the GPS module. The GPS module sends coordinates of its current location to the Arduino Uno.
The Uno then uses an algorithm to calculate the distance to your home and converts that to feet. Once your
car is within 200 feet within home, the arduino sends a signal via bluetooth serial to an android phone.
The android app on the android wait for this signal and once it receives it, it uses an html GET command to
trigger the server on the raspberry pi. The raspberry pi then switches off the GPIO pins in order to switch the 5V relay on.
the relay then triggers the garages input and the garage is open in time for you to come in.

Youtube: https://youtu.be/XH645ALYQ3I

![alt tag](http://i.imgur.com/fbGySta.png)
