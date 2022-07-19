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
	// 设置 UCSWRST (BIS.B #UCSWRST,&UCxCTL1)
	UCB0CTL1 = UCSWRST;

	// (2)初始化所有USCI寄存器，UCSWRST=1（包括UCxCTL1）
	P1DIR  |= CS;    //C
	P1OUT  |= CS;
	P1SEL  |= SOMI + SIMO + SCLK;
	P1SEL2 |= SOMI + SIMO + SCLK;

	//配置端口
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 
	UCB0CTL1 |= UCSSEL_2;   // SMCLK

	//通过软件清除 UCSWRST(BIC.B#UCSWRST,&UCxCTL1)
	UCB0CTL1 &= ~UCSWRST; 
	//通过 UCxRXIE 和/或 UCxTXIE 启用中断（可选）
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