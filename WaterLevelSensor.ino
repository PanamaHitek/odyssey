#include <AltSoftSerial.h> //Librería AltSoftSerial
AltSoftSerial altSerial; //Instancia de la clase AltSoftSerial
//Flags para el control de la entrada de datos
boolean startSequence = true;
boolean receiveData = false;
void setup() {
  //Se inicia el puerto serie con la computadora
  Serial.begin(9600);
  //Se inicia el puerto serie con el sensor
  altSerial.begin(4800);
}

void loop() {
  /*
     Bloque de programación para recibir datos
     de la computadora
  */
  if (Serial.available() > 0) {
    String input = Serial.readString();
    //Si el dato recibido es start, se empieza a leer datos
    if (input == "start") {
      Serial.println("starting...");
      receiveData = true;
      Serial.flush();
      altSerial.flush();
      startTraceMode();
    }
  }
  //Si el flag esta activo
  if (receiveData) {
    //Si hay datos disponibles en el puerto serie del sensor...
    if (altSerial.available() > 0) {
      while (altSerial.available() > 0) {
        delay(25);
        //Se bloquea la secuencia de inicio
        if (startSequence) {
          altSerial.read();
        }
        else
        {
          //Se transforman los datos leidos a capacitancia
          int capacitance = 0;
          capacitance = altSerial.read();
          capacitance = capacitance + (altSerial.read() * 256);
          //Se imprime el resultado
          Serial.println(capacitance);
        }
      }
      if (startSequence) {
        startSequence = false;
      }
    }
  }
}
//Secuencia de inicio del sensor
void startTraceMode() {
  startSequence = true;
  altSerial.write(1);
  delay(1000);
  altSerial.write(0x54);
  altSerial.write(0x0A);
}

