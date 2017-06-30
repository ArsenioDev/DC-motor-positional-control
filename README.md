# DC-motor-positional-control
Working towards a positional control for DC motors with encoders
Requires any DC motor with a quadrature capable encoder, 2 HW interrupts and PWM channels, [PID library](https://github.com/br3ttb/Arduino-PID-Library), potentiometer, and a H bridge.

_"THE BEER-WARE LICENSE" (Revision 42):
Arsenio Dev wrote this file.  As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a beer in return._

- [x] Basic Bang-Bang control plant
- [X] Simple untuned PID control plant
- [X] Multiple kinds of H bridge support
- [ ] PWM input(RC servo control)
- [ ] Remove dependency on other libraries
- [ ] Turn this into a simple library

Updated to drive standard PWM instead of direction and PWM, now works with any standard H bridge

To use TB6612FNG in this manner, connect PWMA/B to VCC and drive INA/B with PWM. 

For Bang-Bang control, see the two bang bang files. 

![Driver](https://cdn.sparkfun.com//assets/parts/3/1/5/7/09457-01b.jpg "TB6612FNG motor driver")
![Example motor](http://www.cnclablb.com/storage/Products/Product10988/Photo_4613/Orig/_W4A0628-900x600.jpg "Example motor with encoder")
