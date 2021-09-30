//Autor: Brayan Castillo
//Laboratorio 7 Digital 2

/*----------------------------Librerias-------------------------------*/
#include <SPI.h>
#include <SD.h>
/*----------------------------Variables ------------------------------*/
Sd2Card card;                //Variables de las librerias SD obtenido de los ejemplos
SdVolume volume;
SdFile root;
File ARCH;                   //Variable para los archivos

int Boton1 = PUSH1;             //Botón a utilizar
const int chipSelect = PA_3;    //PIN CS

/*----------------------------Setup-----------------------------------*/
void setup() {
  Serial.begin(9600);                    //Inicialización Serial a 9600 baudrate
  SPI.setModule(0);                      //Inicialización SPI
  pinMode(PA_3, OUTPUT);                 //CS como OUTPUT
  pinMode(Boton1, INPUT_PULLUP);         //Botón para el menu
 
  if (!card.init(SPI_HALF_SPEED, chipSelect)) { //Inicialización de tarjeta 
    Serial.println("initialization failed!");
    return;
  }
 if (!SD.begin(PA_3)) {                         
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
   
  volume.init(card);                            //Inicialización de informacion de la tarjeta
}
/*------------------------------Loop----------------------------------*/

void loop() {
  int menu = digitalRead(Boton1);             //Lectura del Boton

  if(menu == LOW){                          //Si esta en LOW desplegar el menu
  Serial.println("\nMenú de Archivos: ");   //Título del menú
  Serial.println("");
  root.openRoot(volume);                    //Función para obtener los datos de los archivos de la SD
  root.ls(LS_R | LS_DATE | LS_SIZE);        // Crea una lista de los archivos de la SD
  }
   int OPC = (int)Serial.read();           //Envío de datos al monitor serial
    if(OPC == 49){                               //ASCII "1" envía el contenido de H.txt
        ARCH = SD.open("H.txt");                 //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
          while (ARCH.available()) {             //Hasta que se termine el contenido
            Serial.write(ARCH.read());           //Leer y escribirlo en monitor serial
          }
          ARCH.close();                          //Cerrar el archivo
        } 
        
        else {                                 

          Serial.println("error opening H.txt"); //Si no enviar mensaje de error
        }  
    }
    if(OPC == 50){                               //ASCII "2" envía el contenido de F.txt
        ARCH = SD.open("F.txt");                 //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
          while (ARCH.available()) {             //Hasta que se termine el contenido
            Serial.write(ARCH.read());           //Leer y escribirlo en monitor serial
          }
          ARCH.close();                          //Cerrar el archivo
        } 
        
        else {                                 
          Serial.println("error opening 3.txt"); //Si no enviar mensaje de error
        }
    }
    if(OPC == 51){                               //ASCII "3" envía el contenido de jhin.txt
        ARCH = SD.open("jhin.txt");              //Abrir archivo
        if (ARCH) {                              //Sí abre correctamente
          while (ARCH.available()) {             //Hasta que se termine el contenido
            Serial.write(ARCH.read());           //Leer y escribirlo en monitor serial
          }
          ARCH.close();                          //Cerrar el archivo
        } 
        
        else {                                 
          Serial.println("error opening jhin.txt"); //Si no enviar mensaje de error
        }  
    }
  }
