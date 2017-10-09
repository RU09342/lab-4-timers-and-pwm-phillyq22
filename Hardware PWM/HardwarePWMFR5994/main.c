#include <msp430.h> 


/**
 * main.c
 */

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  PM5CTL0 &= ~LOCKLPM5;           // give power to channel

  P1DIR |= BIT0;                            // P1.6 output
  P1SEL0 |= BIT0;                            // P1.6 for TA0 CCR1 Output Capture
  P1SEL1 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)
  TA0CCR0 = 1000;                             // PWM Freq=1000Hz
  TA0CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
  TA0CCR1 = 500;                               // CCR1 50% PWM duty cycle
  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode, 1MhZ

  P5IE |=  BIT5;                            // P5.5 interrupt enabled
  P5IES |= BIT5;                            //falling edge
  P5REN |= BIT5;                            // Enable resistor on SW2 (P5.5)
  P5OUT |= BIT5;                             //Pull up resistor on P5.5
  P5IFG &= ~BIT5;                           // P5.5 Interrupt Flag cleared



  __bis_SR_register(LPM0_bits + GIE);                // Enter LPM0 for CPUOFF
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)

{
    P5IE &= ~BIT5;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P5IFG &=~BIT5;           //clear flag

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P5IN & BIT5))//if button is pressed
    {
        TA0CCR1 += 100;
        if(TA0CCR1==1100){
            TA0CCR1=0;
        }
        //check button status -- whether to change Duty Cycle
    }
    P5IE |= BIT5; //enable button interrupt
}
