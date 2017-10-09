#include <msp430.h> 


/**
 * main.c
 */

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT6;                            // P1.6 output
  P1SEL |= BIT6;                            // P1.6 for TA0 CCR1 Output Capture
  P1SEL2 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)
  CCR0 = 1000;                             // PWM Freq=1000Hz
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
  CCR1 = 500;                               // CCR1 50% PWM duty cycle
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode, 1MhZ

  P1IE |=  BIT3;                            // P1.3 interrupt enabled
  P1IES |= BIT3;                            //falling edge
  P1REN |= BIT3;                            // Enable resistor on SW2 (P1.3)
  P1OUT |= BIT3;                             //Pull up resistor on P1.3
  P1IFG &= ~BIT3;                           // P1.3 Interrupt Flag cleared



  __bis_SR_register(LPM0_bits + GIE);                // Enter LPM0 for CPUOFF
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    P1IE &= ~BIT3;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    IE1 |= WDTIE;           //enable WDT interrupt
    P1IFG &=~BIT3;           //clear flag

}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    IE1 &= ~WDTIE;      //disable WDT interrupt
    IFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P1IN & BIT3))//if button is pressed
    {
        CCR1 += 100;
        if(CCR1==1100){
            CCR1=0;
        }
        //check button status -- whether to change Duty Cycle
    }
    P1IE |= BIT3; //enable button interrupt
}
