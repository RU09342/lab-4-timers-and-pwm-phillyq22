#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P9IE |= BIT7;                             // P9.7 interrupt enabled
    P9IES |= BIT7;                            // falling edge
    P9REN |= BIT7;                            //Enable resistor on button (P9.7)
    
    P1OUT |= BIT1;                            //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared
    P9OUT |= BIT7;                            //Pull up resistor on P9.7
    P9IFG &= ~BIT7;                           // P9.7 interrupt flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P9DIR |= BIT7;       // P9.7 pin output


    __bis_SR_register(LPM0_bits + GIE);     //timers needed, no CPU, interrupt enable

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    if(P1IE & ~BIT1)
    {
        SFRIE1 &= ~WDTIE;      //disable WDT interrupt
        SFRIFG1 &= ~WDTIFG;    //clear flag
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        if (!(P1IN & BIT1))//if button is pressed, switch LED
        {
            P1OUT ^= BIT0;
        }
        P1IE |= BIT1;   //enable button interrupt
    }
    if(P9IE & ~BIT7)
    {
        SFRIE1 &= ~WDTIE;      //disable WDT interrupt
        SFRIFG1 &= ~WDTIFG;    //clear flag
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        if (!(P1IN & BIT1))//if button is pressed, switch LED
        {
             P1OUT ^= BIT0;
        }
        P1IE |= BIT1;   //enable button interrupt
    }

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    P1IE &= ~BIT1;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P1IFG &=~BIT1;           //clear flag

}

#pragma vector=PORT9_VECTOR
__interrupt void Port_9(void)

{
    P9IE &= ~BIT7;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P9IFG &=~BIT7;           //clear flag

}