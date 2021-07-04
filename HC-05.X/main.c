
#include "mcc_generated_files/mcc.h"

void send(unsigned char data);

void main(void)
{
    SYSTEM_Initialize();

    ANSELA = 0x0;
    ANSELB = 0x0;
    
    TRISA = 0x0;
    //RX pin set input mode
    TRISB = 0x02;
    
    //TX on
    TXSTA = 0x02;
    //RC on
    RCSTA = 0x90;
    //BAUDCON 16bit
    BAUDCON = 0x08;
    //BRGH = 0, BRG16 = 1 9600bit mode
    SPBRG = 51;
    
    //interrupt flg clear
    PIR1bits.RCIF = 0;
    
    //USART recive interrupt enable
    PIE1bits.RCIE = 1;
    //peripheral enable
    PEIE = 1;
    //global enable
    GIE = 1;
    
    
    while (1)
    {
        
    }
}

/**
 * send data
 * @param data
 */
void send(unsigned char data){
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

/**
 * interrupt func
 */
void __interrupt() isr(void){
    if(PIR1bits.RCIF){
        //flg clear
        PIR1bits.RCIF = 0;
        //error
        if((RCSTAbits.OERR) || (RCSTAbits.FERR)){
            RCSTA = 0;
            RCSTA = 0x90;
        }else{
        //not error
            if(RCREG == 0){
                //LED OFF
                LATA = 0x0;
            }
            if(RCREG == 1){
                //LED ON
                LATA = 0x02;
            }
            send(RCREG);
        }
    }
}