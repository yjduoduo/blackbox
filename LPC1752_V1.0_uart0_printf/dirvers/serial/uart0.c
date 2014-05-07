#include "uart0.h"


void uart0_pconp_conf(void)
{
    PCONP |= (1<<3);
}
void uart0_pin_conf(void)
{
    PINSEL0 &= ~(0x3<<4);//clear P0.2
    PINSEL0 |= (0x1<<4);//P0.2,Func[1]
//    FIO0CLR &= ~(0x1<<2);//level P0.2,low

    PINSEL0 &= ~(0x3<<6);//clear P0.3
    PINSEL0 |= (0x1<<6);//P0.3,Func[1]
//    FIO0CLR &= ~(0x1<<3);//level P0.3,low



//    PINSEL0 &= ~0x000000F0; //p0.2 p0.3 as 0000(clear first,then change)
//    PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */ //5(01 01 as second fun);
}

void uart0_init(uint32 baudrate)
{
    uint32 Fdiv;
    uint32 pclkdiv, pclk;

    uart0_pconp_conf();
    uart0_pin_conf();
/* By default, the PCLKSELx value is zero, thus, the PCLK for
    all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 6~7 is for UART0 */
    pclkdiv = (PCLKSEL0 >> 6) & 0x03;
    switch ( pclkdiv )
    {
    case 0x00:
    default:
        pclk = SystemFrequency/4;//12M
        break;
    case 0x01:
        pclk = SystemFrequency;
        break;
    case 0x02:
        pclk = SystemFrequency/2;
        break;
    case 0x03:
        pclk = SystemFrequency/8;
        break;
    }

    U0LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit ,DLAB = 1*/
    Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    U0DLM = Fdiv / 256;
    U0DLL = Fdiv % 256;
    U0LCR = 0x03;		/* DLAB = 0 ��ֹ�����Ĵ������¸�ֵ*/
    U0FCR = 0x07;		/* Enable and reset TX and RX FIFO. */
    // 	U0FCR = 0xa7;		/* Enable and reset TX and RX FIFO 10���ֽڴ���. */


    U0IER = IER_RBR;// | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */


    //    U0IER = IER_RBR | IER_THRE | IER_RLS;

    uart0_enable();//�ж����ȼ�
}

void uart0_enable(void)
{
    U0IER = IER_RBR;
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn, PRIO_FOUR); //�ж����ȼ�
}

void uart0_disable(void)
{
    NVIC_DisableIRQ(UART0_IRQn);
}

int uart0_sendbyte(char buf)
{
    //����⵽UARTn THR�ѿ�ʱ�� THRE�ͻ����������� дUnTHR������THRE
    //0  -  UnTHR������Ч�ַ�
    //1  -  UnTHRΪ��
    while(0 == (U0LSR & (0x20)));       //�ȴ��ж�LSR[5](��THRE)�Ƿ���1,1ʱ��ʾTHR��Ϊ��
    U0THR = buf;                    //��������

    return 0;
}

uint32 uart0_getbyte(void)
{
    uint32 ucRcvData;

    while ((U0LSR & 0x01) == 0);                                        /* �ȴ����ձ�־��λ             */
    ucRcvData = U0RBR;                                                  /* ��ȡ����                     */
    return (ucRcvData);
}

void uart0_getstring (uint32 *pucStr, uint32 ulNum)
{
    for (; ulNum > 0; ulNum--){
        *pucStr++ =  uart0_getbyte();
    }
}

void uart0_sendstring (char *s)
{
    while ( *s != 0 )
    {
        uart0_sendbyte(*s++);
    }
}


