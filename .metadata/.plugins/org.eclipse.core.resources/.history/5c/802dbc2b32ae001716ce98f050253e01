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
    P1IE |= BIT2;                             // P1.2 interrupt enabled
    P1IES |= BIT2;                            // falling edge
    P1REN |= BIT2;                            //Enable resistor on button (P1.2)

    P1OUT |= BIT1;                            //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared
    P1OUT |= BIT2;                            //Pull up resistor on P1.2
    P1IFG &= ~BIT2;                           // P1.2 interrupt flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P9DIR |= BIT7;       // P9.7 pin output


    __bis_SR_register(LPM0_bits + GIE);     //timers needed, no CPU, interrupt enable

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    if(P1IE & ~BIT1)//If interrupt from Button P1.1
    {
        SFRIE1 &= ~WDTIE;      //disable WDT interrupt
        SFRIFG1 &= ~WDTIFG;    //clear flag
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        if (!(P1IN & BIT1))//if button is pressed, switch LED
        {
            P1OUT ^= BIT0;//Flip LED
        }
        P1IE |= BIT1;   //enable button interrupt
    }
    else if(P1IE & ~BIT2)//If interrupt from Button P1.2
    {
        SFRIE1 &= ~WDTIE;      //disable WDT interrupt
        SFRIFG1 &= ~WDTIFG;    //clear flag
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

        if (!(P1IN & BIT2))//if button is pressed, switch LED
        {
             P9OUT ^= BIT7;//Flip LED
        }
        P1IE |= BIT2;   //enable button interrupt
    }
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    if(P1IFG & BIT1)//If button P1.1 set off interrupt flag
    {
        P1IE &= ~BIT1;          //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
        SFRIE1 |= WDTIE;           //enable WDT interrupt
        P1IFG &=~BIT1;           //clear flag
    }

    else if(P1IFG & BIT2)//If button P1.2 set off interrupt flag
    {
        P1IE &= ~BIT2;          //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
        SFRIE1 |= WDTIE;           //enable WDT interrupt
        P1IFG &=~BIT2;           //clear flag
    }


}
