#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    P5IE |=  BIT5;                            // P5.5 interrupt enabled
    P5IES |= BIT5;                            //falling edge
    P5REN |= BIT5;                            // Enable resistor on SW2 (P5.5)
    P5OUT |= BIT5;                             //Pull up resistor on P5.5
    P5IFG &= ~BIT5;                           // P5.5 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output



    __bis_SR_register(LPM0_bits + GIE);     //timers needed, no CPU, interrupt enable

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P5IN & BIT5))//if button is pressed, switch LED
    {
        P1OUT ^= BIT0;
    }
    P5IE |= BIT5;   //enable button interrupt

}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)

{
    P5IE &= ~BIT5;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P5IFG &=~BIT5;           //clear flag

}
