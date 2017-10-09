#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    TB0CTL |= TBIE;             //enable TA0 interrupt
    TB0CCTL0 |= CCIE;              //enable CCR0 interrupt
    TB0CCTL0 &= ~TBIFG;            //clear flag
    TB0CCTL1 |= CCIE;              //enable CCR1 interrupt
    TB0CCTL1 &= ~TBIFG;            //clear flag

    TB0CTL = TBSSEL_2 + MC_1 + ID_0;           // SMCLK, upmode, clk/1
    TB0CCR1 = 490;                               //50% duty to start
    TB0CCR0 = 1000;                             //1 kHz signal

    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
}


//Timer A interrupt routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{
    P1OUT |= BIT0;      //turn LED on
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer_B0 (void)
{
    switch(TB0IV){
    case 2:
    {
        P1OUT &= ~BIT0;     //turn LED off for CCR1 interrupt
    }
    }
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P1IN & BIT1))//if button is pressed
    {
        //check button status -- whether to change Duty Cycle
        switch(TB0CCR1){    //Linearly increment PWM due to log vision
        case 0:
        {
            TB0CCR1 = 8;
        }break;
        case 8:
        {
            TB0CCR1 = 16;
        }break;
        case 16:
        {
            TB0CCR1 = 27;
        }
        case 27:
        {
            TB0CCR1 = 43;
        }break;
        case 43:
        {
            TB0CCR1 = 70;
        }break;
        case 70:
        {
            TB0CCR1 = 118;
        }break;
        case 118:
        {
            TB0CCR1 = 157;
        }break;
        case 157:
        {
            TB0CCR1 = 196;
        }break;
        case 196:
        {
            TB0CCR1 = 255;
        }break;
        case 255:
        {
            TB0CCR1 = 314;
        }break;
        case 314:
        {
            TB0CCR1 = 392;
        }
        case 392:
        {
            TB0CCR1 = 490;
        }break;
        case 490:
        {
            TB0CCR1 = 627;
        }break;
        case 627:
        {
            TB0CCR1 = 784;
        }break;
        case 784:
        {
            TB0CCR1 = 1000;
        }break;
        case 1000:
        {
            TB0CCR1 = 0;
        }break;
        }
    }
    P1IE |= BIT1;       //enable button interrupt

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{
    P1IE &= ~BIT1;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P1IFG &=~BIT1;           //clear flag

}
