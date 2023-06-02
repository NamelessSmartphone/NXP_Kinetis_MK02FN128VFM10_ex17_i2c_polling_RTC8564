#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK02F12810.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_uart.h"
#include "MK02FN128VFM10_uart.h"
#include "xprintf.h"

/* TODO: insert other definitions and declarations here. */
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
UART_BUFF_T TxBuff;
UART_BUFF_T RxBuff;

/*******************************************************************************
 * Code
 ******************************************************************************/
void UART0_PutByte (uint8_t d)
{
	int32_t i;

	/* Wait for Tx buffer ready */
	while (TxBuff.ct >= BUFF_SIZE) ;

	__disable_irq();
	if(TxBuff.act){
		i = TxBuff.wi;		/* Put a byte into Tx byffer */
	  	TxBuff.buff[i++] = d;
	  	TxBuff.wi = i % BUFF_SIZE;
	  	TxBuff.ct++;
	} else {
	  	TxBuff.act = 1;		/* Trigger Tx sequense */
	  	UART_WriteByte(UART0, d);
	}
	__enable_irq();
	UART_EnableInterrupts(UART0, kUART_TransmissionCompleteInterruptEnable);
}

uint8_t UART0_GetByte (void)
{
	uint8_t d;
	int i;

	/* Wait while Rx buffer is empty */
	while (!RxBuff.ct) ;

	i = RxBuff.ri;			/* Get a byte from Rx buffer */
	d = RxBuff.buff[i++];
	RxBuff.ri = i % BUFF_SIZE;
	__disable_irq();
	RxBuff.ct--;
	__enable_irq();

	return d;
}

void UART0_RX_TX_IRQHandler(void)
{
	uint32_t uart_flag;
	uint8_t d;
	int i, cnt;

	uart_flag = UART_GetStatusFlags(UART0);
    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & uart_flag) {
        i = RxBuff.wi;
        cnt = RxBuff.ct;
//        while (Chip_UART_ReadLineStatus(LPC_USART) & UART_LSR_RDR) {	/* Get all data in the Rx FIFO */
        	d = UART_ReadByte(UART0);
        	if (cnt < BUFF_SIZE) {	/* Store data if Rx buffer is not full */
        		RxBuff.buff[i++] = d;
        		i %= BUFF_SIZE;
        		cnt++;
        	}
//        }
       RxBuff.wi = i;
       RxBuff.ct = cnt;
       UART_ClearStatusFlags(UART0, kUART_RxDataRegFullFlag | kUART_RxOverrunFlag);
    }

//    if ((kUART_TxDataRegEmptyFlag | kUART_TransmissionCompleteFlag) & uart_flag) {
   if ((kUART_TransmissionCompleteFlag) & uart_flag) {
		cnt = TxBuff.ct;
		if(cnt){/* There is one or more byte to send */
			i = TxBuff.ri;
			for (d = 16; d && cnt; d--, cnt--){	/* Fill Tx FIFO */
				UART_WriteByte(UART0, TxBuff.buff[i++]);
				i %= BUFF_SIZE;
			}
			TxBuff.ri = i;
			TxBuff.ct = cnt;
		}else{
			TxBuff.act = 0; /* When no data to send, next putc() must trigger Tx sequense */
			UART_DisableInterrupts(UART0, kUART_TransmissionCompleteInterruptEnable);
		}
	   UART_ClearStatusFlags(UART0, kUART_TransmissionCompleteFlag);
    }

    SDK_ISR_EXIT_BARRIER;
}
