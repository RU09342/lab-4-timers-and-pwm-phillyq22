# Hardware PWM
## Explanation
Philip Quinn: Note: The signal that chooses CCR1 is different for the G, F, and FR series MSP430s. In our case, the G2553 uses PxSEL and PxSEL2 while F5529 uses P1SEL and FR series use PxSEL0 and PxSEL1.

Within this lab, LEDs are driven to directly by timers through the utilization of the CCRx Capture Mode. OUTMOD_7 indicates that CCR0 is high on capture and CCR1 is low on capture. Implementation works the same as the Software PWM for all boards.