/*
 * bsp_SPI.c
 *
 *  Created on: 2022年7月16日
 *      Author: alex
 */

#include <msp430.h>
#include <bsp_SPI.h>

void spi_init(void)
{
	/**
	 * From TIs users manual
	 *
     * The recommended USCI initialization/re-configuration process is:
 	 * 1. Set UCSWRST (BIS.B #UCSWRST,&UCxCTL1)
 	 * 2. 
 	 * 3. 
 	 * 4. 
 	 * 5. 
 	 */

	// Set UCSWRST (BIS.B #UCSWRST,&UCxCTL1)
	UCB0CTL1 = UCSWRST;

	// (2)Initialize all USCI registers with UCSWRST=1 (including UCxCTL1)
    P1DIR  |= CS; 
    P1OUT  |= CS;
  	P1SEL  |= SOMI + SIMO + SCLK;
  	P1SEL2 |= SOMI + SIMO + SCLK;

    // Configure ports
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 
	UCB0CTL1 |= UCSSEL_2;   // SMCLK

	// Clear UCSWRST via software (BIC.B #UCSWRST,&UCxCTL1)
	UCB0CTL1 &= ~UCSWRST; 
	//Enable interrupts (optional) via UCxRXIE and/or UCxTXIE
	spi_csh();
}

void spi_csh(void)
{
    P1OUT |= CS;
}

void spi_csl(void)
{
    P1OUT &= ~CS;
}

unsigned char spi_xfer_byte(unsigned char data)
{
  	UCB0TXBUF = data; 

	// wait for TX 
	while (!(IFG2 & UCB0TXIFG)); 	

	return UCB0RXBUF;
}