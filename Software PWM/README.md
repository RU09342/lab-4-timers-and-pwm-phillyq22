# Software PWM
Philip Quinn: Software PWM required that the timer be put into upmode so that CCR0 could be used as a limit to the timers count. In this way, a duty cycle could be set to whatever the user pleased. (CCR1/CCR0)*100 produced the duty cycle, so with an increase in CCR1 with each button press, the duty cycle could be visibly changed.

## Extra Work
### Linear Brightness
All boards were given the ability to perform linear brightness increase by using a case statement that looked for specific log-based CCR1 values. This made it easy for someone to see the increase in brightness after each click of the button.