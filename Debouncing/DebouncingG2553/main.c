#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1IE |=  BIT3;                            // P1.3 interrupt enabled
    P1IES |= BIT3;                            //falling edge
    P1REN |= BIT3;                            // Enable resistor on SW2 (P1.3)
    P1OUT |= BIT3;                             //Pull up resistor on P1.3
    P1IFG &= ~BIT3;                           // P1.3 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output



    __bis_SR_register(LPM0_bits + GIE);     //timers needed, no CPU, interrupt enable

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    IE1 &= ~WDTIE;      //disable WDT interrupt
    IFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P1IN & BIT3))//if button is pressed, switch LED
    {
        P1OUT ^= BIT0;
    }
    P1IE |= BIT3;   //enable button interrupt

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    P1IE &= ~BIT3;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    IE1 |= WDTIE;           //enable WDT interrupt
    P1IFG &=~BIT3;           //clear flag

}
