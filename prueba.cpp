/* Protocolo: "Accion nropin ON/OFF\r\n" haciendo que el arduino escriba
los pines y conteste "OK\r\n" . r nropin , lee pin y contesta ON/OFF
(estaria bueno que sea el pin leido y el valor*/

//An EEPROM write takes 3.3 ms to complete. The EEPROM memory has a specified life of 100,000 write/erase cycles, 
//so you may need to be careful about how often you write to it. 
#include <EEPROM.h>

String line="";
const int pinesSalida[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int cantPines = sizeof(pinesSalida)/sizeof(int);
const String prender= "Accion ";
const int bufferLimit=200000;
boolean flag=false;
//vector del 2 al 13 con los inputs y outputs para configurar al hacer setup. o=output, i=input

void setup() {
  Serial.begin(9600);
  for (int i=0; i<cantPines; i++){
    pinMode(pinesSalida[i], OUTPUT);  
    setPinHighOrLow(pinesSalida[i],EEPROM.read(pinesSalida[i]));
  }
}
void loop() {
  flag=false;
  
  while(Serial.available()){
    //milis arduino para el timeout
    char caracter= Serial.read();
    line = line + caracter;
    if(line.length()>bufferLimit){
      line="";
    }  
    if((line.indexOf("\r\n")>-1) && (line.indexOf(prender)>-1)){
      if((line.lastIndexOf("\r\n")) > (line.indexOf(prender))){
        int posicion=line.lastIndexOf(prender);
        int posicionPin=posicion+prender.length();//cambiar lo de que es mas de un numero el pin
        int pin=line.charAt(posicionPin)-'0';
        if(verificar(1,9,pin)){
          if(pin!=1){
            int accion=line.charAt(posicionPin+2)-'0';
            if(verificar(0,1,accion)){
              setPinHighOrLow(pin,accion);
              EEPROM.write(pin,accion);
              flag=true;
              Serial.print("Ok\r\n");//mando confirmacion a PI
            }else{
              Serial.print("La accion no es valida\r\n");
              break;
            }
          }else{
            int pinUnidad=line.charAt(posicionPin+1)-'0';
            int pinDecimal=pin;
            pin= pinDecimal*10+pinUnidad;
            int accion=line.charAt(posicionPin+3)-'0';
            if(verificar(0,1,accion) && verificar(10,13,pin)){
              setPinHighOrLow(pin,accion);
              EEPROM.write(pin,accion);
              flag=true;
              Serial.print("Ok\r\n");//mando confirmacion a PI
            }else{
              Serial.print("El pin no es valido o accion no son validos\r\n");
              break;
            }
          }
        }else{
          Serial.print("El pin no es valido\r\n");
        }
        
      }else{
        // el \r\n es basura 
      }
    }   
  }
  if(flag){
    line="";
  }
  delay(10);
}
int setPinHighOrLow(int pin, int accion){
   //digitalWrite(pin,accion);
   if(accion==1){
     digitalWrite(pin, HIGH);
   }else if(accion==0){
     digitalWrite(pin, LOW);
   }
   return 0;
}
int verificar(int mayor,int menor, int numero){ 
  int retorno=0;
  if(numero <= mayor && menor <= numero){
    retorno = 1;
  }
  return retorno;
}