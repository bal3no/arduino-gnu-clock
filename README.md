arduino-gnu-clock
=================

Simple clock implementation using an Arduino UNO connected on a 20 x 4 HD44780 compatible LCD Display. In this implementation i've used a YM2004A.

The Cuicuit:<br>
LCD (pin 01) Vss to ground<br>
LCD (pin 02) Vcc to +5V<br>
LCD (pin 03) Vee controlled by PWM pin 10<br>
LCD (pin 04) RS pin to Digital 12<br>
LCD (pin 05) R/W tied to ground<br>
LCD (pin 06) Enable pin to Digital 11<br>
LCD (pin 11) DB4 pin to Digital 5<br>
LCD (pin 12) DB5 pin to Digital 4<br>
LCD (pin 13) DB6 pin to Digital 3<br>
LCD (pin 14) DB7 pin to Digital 2<br>
LCD (pin 15) LED+ to +5V<br>
LCD (pin 16) LED- controlled by PWM pin 9<br>

Check on http://andybrown.me.uk/wk/2010/11/28/lcd-backlight-and-contrast-manager/ in case you want to control LCD's backlight and contrast with Arduino's PWM signals.<br>

Sending a signal to pin A1, you can switch display's state to sleep (turned off) or normal (turned on).<br>
Sending a signal to pin A2, you can change the display's auto-sleep timeout.
