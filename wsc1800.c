

#define FACTOR (66.0/1000)
#define QOV (0.5*5.0)
#define RESOL (5.0/1023.0)
#define OFFSET_MAG (25/1000)
#define TRUE 1

/*PROTOTIPOS DE FUNCIONES*/
void configuracion(void);
int ADC_lectura(const char  bit0,const char bit1,const char bit2,const char  bit3);
float lectura_WSC1800(void);
float prom_corrinte( const unsigned int max);

void main() {

      float corriente_promedio;
      unsigned char str[16]={0};


/*CONFIGURACION DE LOS PUERTOS GPIO*/
       ANSELH=0X00;
     PORTB=0X00;
      TRISB=0X00;//
     PORTD=0X00;
      TRISD=0x00;//
     PORTE=0x00;
      TRISE=0x03;//
       ANSEL=0x63;
     PORTA=0x00;
      TRISA=0x03;//
     PORTC=0x00;
      TRISC=0xFF;//

   /*CONFIGURACION DEL ADC*/
     ADCON0=0x81;     //10 0000 01
     ADCON1=0x80;    // 10000000
     ADRESH=0x00;
     ADRESL=0x00;
  //INTERRUPCION POR TIMER 0

       UART1_Init(9600);
  
     while(TRUE){

             corriente_promedio=prom_corrinte(500);
             FloatToStr(corriente_promedio,str);
             UART1_Write_Text(str);
             UART1_Write_Text("\r\n") ;
             Delay_ms(2000);

          }

}

     
     

int ADC_lectura(const char  bit0,const char bit1,const char bit2,const char  bit3){

     unsigned  int pasos=0;
     CHS0_bit=bit3;
     CHS1_bit=bit2;
     CHS2_bit=bit1;
     CHS3_bit=bit0;
     GO_DONE_bit=1;
    while(GO_DONE_bit);
     pasos= (ADRESH<<8) | (ADRESL);

       return(pasos);
    }
    

float lectura_WSC1800(void){

      int pasos=ADC_lectura(0,1,0,1);
      float voltaje=0.0;
      float corriente=0.0;
      

          float voltaje_puro= pasos*RESOL;
          voltaje=(float)voltaje_puro-QOV;
          corriente=(float)(voltaje/FACTOR)-(OFFSET_MAG);
        return(corriente);

      

     }


float prom_corrinte( const unsigned int max){
           unsigned int cont=0;
           float sum=0.0;
           float aux=0.0;

      for(cont=0;cont<=max;cont++){

             aux=lectura_WSC1800(void);
             sum+=aux;

          
         }

      return (sum/cont);
     }