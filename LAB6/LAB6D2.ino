//Autor: Brayan Castillo
//Laboratorio 6 Digital 2
#define LEDV PF_3   //Definicion de los LEDS y Botones
#define LEDA PF_2
#define LEDR PF_1
#define Boton1 PF_4
#define Boton2 PF_0
  int c1=0;         //Definicion de los contadores
  int c2=0;
  int inicio = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(LEDV, OUTPUT); //Declarar LEDS y Botones como entradas y salidas
pinMode(LEDA, OUTPUT); 
pinMode(LEDR, OUTPUT);
pinMode(Boton1, INPUT_PULLUP); 
pinMode(Boton2, INPUT_PULLUP);
for(int i=2;i<=9;i++){ // Ciclo para los puertos del contador 1
pinMode(i, OUTPUT);
}
for(int i=23;i<=30;i++){ // Ciclo para los puertos del contador 2
pinMode(i, OUTPUT);
}
digitalWrite(LEDV, HIGH); //Semaforo
delay(1000);
digitalWrite(LEDV, LOW);
digitalWrite(LEDA, HIGH);
delay(1000);
digitalWrite(LEDA, LOW);
digitalWrite(LEDR, HIGH);
delay(1000);
}
void loop() {

  // put your main code here, to run repeatedly: 
    
  if(c1==8 || c2==8){                         //Comprobar si alguno de los jugadores ya gano
    inicio = 1;
    }  
  if(digitalRead(Boton1)==LOW && inicio == 0){// Si se presiona el boton 1 aumentar el contador 1
     c1=c1+1;
     while (digitalRead(Boton1)==LOW){} 
     }
  else if(digitalRead(Boton2)==LOW && inicio == 0){ // Si se presiona el boton 2 aumentar el contador 2
     c2=c2+1;
     while (digitalRead(Boton2)==LOW){}
     }
    switch(c1){   //En cada caso encender un LED conforme el contador vaya aumentando
      case 0:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(2, HIGH);
    break;
    
    case 1: 
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(3, HIGH);
    break;
    case 2:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(4, HIGH);
    break;
    case 3:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(5, HIGH);
    break;
    case 4:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(6, HIGH);
    break;
    case 5:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(7, HIGH);
    break;
    case 6:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(8, HIGH);
    break;
    case 7:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(9, HIGH);
    break;    
    case 8:
      for(int i=2;i<=9;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(LEDV, HIGH);      //Si gano el jugador 1 encender LED verde
    break;

    default:    
    break;
      
      }
     
     switch(c2){//En cada caso encender un LED conforme el contador vaya aumentando
      case 0:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(23, HIGH);
    break;
    
    case 1: 
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(24, HIGH);
    break;
    case 2:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(25, HIGH);
    break;
    case 3:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(26, HIGH);
    break;
    case 4:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(27, HIGH);
    break;
    case 5:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(28, HIGH);
    break;
    case 6:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(29, HIGH);
    break;
    case 7:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(30, HIGH);
    break;    
    case 8:
      for(int i=23;i<=30;i++){ 
      digitalWrite(i, LOW);
      }
      digitalWrite(LEDR, HIGH);    //Si gano el jugador 2 encender LED rojo  
    break;
    
    default:    
    break;
      
      
     }
     }
       
