arduino-gnu-clock
=================

Simple clock implementation using an Arduino UNO connected on a 20 x 4 HD47780 compatible LCD Display. In this implementation i've used a YM2004A.

The Cuicuit:
  LCD (pin 04) RS pin to Digital 12
  LCD (pin 05) Enable pin to Digital 11
  LCD (pin 11) DB4 pin to Digital 5
  LCD (pin 12) DB5 pin to Digital 4
  LCD (pin 13) DB6 pin to Digital 3
  LCD (pin 14) DB7 pin to Digital 2
  LCD (pin 16) V0 controlled by PWM pin 9
  LCD (pin 03) Vss controlled by PWM pin 10
  LCD (pin 05) R/W tied to ground
  LCD (pin 02) Vcc to +5V

Check on http://andybrown.me.uk/wk/2010/11/28/lcd-backlight-and-contrast-manager/ for controlling backlight and contrast with Arduino's PWM signals.
