#define BUFF_SIZE	8	/* Length of Receive/Transmission FIFO */

typedef struct {
  uint16_t	ri, wi, ct, act;
  uint8_t		buff[BUFF_SIZE];
} UART_BUFF_T;

extern UART_BUFF_T TxBuff;
extern UART_BUFF_T RxBuff;

void UART0_PutByte (uint8_t);
uint8_t UART0_GetByte (void);
