#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    TA0CTL |= TAIE;             //enable TA0 interrupt
    TA0CCTL0 |= CCIE;              //enable CCR0 interrupt
    TA0CCTL0 &= ~TAIFG;            //clear flag
    TA0CCTL1 |= CCIE;              //enable CCR1 interrupt
    TA0CCTL1 &= ~TAIFG;            //clear flag

    TA0CTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK, upmode, clk/1
    TA0CCR1 = 490;                               //50% duty to start
    TA0CCR0 = 1000;                             //1 kHz signal

    P5IE |=  BIT5;                            // P5.5 interrupt enabled
    P5IES |= BIT5;                            //falling edge
    P5REN |= BIT5;                            // Enable resistor on SW2 (P5.5)
    P5OUT |= BIT5;                             //Pull up resistor on P5.5
    P5IFG &= ~BIT5;                           // P5.5 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
}


//Timer A interrupt routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1OUT |= BIT0;      //turn LED on
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A0 (void)
{
    switch(TA0IV){
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

    if (!(P5IN & BIT5))//if button is pressed
    {
        //check button status -- whether to change Duty Cycle
        switch(TA0CCR1){    //Linearly increment PWM due to log vision
        case 0:
        {
            TA0CCR1 = 8;
        }break;
        case 8:
        {
            TA0CCR1 = 16;
        }break;
        case 16:
        {
            TA0CCR1 = 27;
        }
        case 27:
        {
            TA0CCR1 = 43;
        }break;
        case 43:
        {
            TA0CCR1 = 70;
        }break;
        case 70:
        {
            TA0CCR1 = 118;
        }break;
        case 118:
        {
            TA0CCR1 = 157;
        }break;
        case 157:
        {
            TA0CCR1 = 196;
        }break;
        case 196:
        {
            TA0CCR1 = 255;
        }break;
        case 255:
        {
            TA0CCR1 = 314;
        }break;
        case 314:
        {
            TA0CCR1 = 392;
        }
        case 392:
        {
            TA0CCR1 = 490;
        }break;
        case 490:
        {
            TA0CCR1 = 627;
        }break;
        case 627:
        {
            TA0CCR1 = 784;
        }break;
        case 784:
        {
            TA0CCR1 = 1000;
        }break;
        case 1000:
        {
            TA0CCR1 = 0;
        }break;
        }
    }
    P5IE |= BIT5;       //enable button interrupt

}

#pragma vector=PORT5_VECTOR
__interrupt void Port_1(void)

{
    P5IE &= ~BIT5;          //disable interrupt
    WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
    SFRIE1 |= WDTIE;           //enable WDT interrupt
    P5IFG &=~BIT5;           //clear flag

}
