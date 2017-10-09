#include <msp430.h> 


/**
 * main.c
 */

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  PM5CTL0 &= ~LOCKLPM5;           // give power to channel

  P2DIR |= BIT0;                            // P1.6 output
  P2SEL0 |= BIT0;                            // P1.6 for TA0 CCR1 Output Capture
  P2SEL1 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)
  TB1CCR0 = 1000;                             // PWM Freq=1000Hz
  TB1CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
  TB1CCR1 = 500;                               // CCR1 50% PWM duty cycle
  TB1CTL = TBSSEL_2 + MC_1;                  // SMCLK, up mode, 1MhZ

  P1IE |=  BIT1;                            // P1.1 interrupt enabled
  P1IES |= BIT1;                            //falling edge
  P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
  P1OUT |= BIT1;                             //Pull up resistor on P1.1
  P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared



  __bis_SR_register(LPM0_bits + GIE);                // Enter LPM0 for CPUOFF
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    P1IE &= ~BIT1;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P1IFG &=~BIT1;           //clear flag

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P1IN & BIT1))//if button is pressed
    {
        TB1CCR1 += 100;
        if(TB1CCR1==1100){
            TB1CCR1=0;
        }
        //check button status -- whether to change Duty Cycle
    }
    P1IE |= BIT1; //enable button interrupt
}
