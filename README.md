# SimuladorMontacargas
Este proyecto tiene como objetivo simular un montacargas utilizando una placa de Arduino en complemento con una variedad de componentes. El sistema del montacargas es básico, cuenta con 3 botones(UP, STOP, DOWN) con las funcionalidades de subir o bajar un piso como así también frenarlo. También tiene incorporado un visualizador de 7 segmentos.


## Definición de pines

```cpp
const int LED_RED = 2;
const int LED_GREEN = 3;
const int LED_YELLOW = 4;
const BUTTON_INCREASE = 5const int BUTTON_STOP = 6;
const int BUTTON_DECREASE = 7;
const int DISPLAY_SEG_A = 8;
const int DISPLAY_SEG_B = 9;
const int DISPLAY_SEG_C = 10;
const int DISPLAY_SEG_D = 11;
const int DISPLAY_SEG_E = 12;
const int DISPLAY_SEG_F = 13;
const int DISPLAY_SEG_G = A0;
```

## Variables

```cpp
int floorCounter = 0;
unsigned long timerElevator = 0;
unsigned long timerElevatorStopped = 0;
unsigned long timeMovement = 3000;
unsigned long initialTime = 0;
unsigned long timeLeft = 0;
int segments[10][7] = {{1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,10,1,1,0,1}, {1,1,1,1,00,1}, {0,1,1,0,0,11}, {1,0,1,1,0,1,1}, {1,0,,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}};
int pines[7] = {DISPLAY_SEG_A, DISPLAY_SEG_B, DISPLAY_SEG_C, DISPLAY_SEG_D, DISPLAY_SEG_E, DISPLAY_SEG_F, DISPLAY_SEG_G};
int ElevatorState = 0;
const int STOPPED = 0;
const int MOVING_UP = 1;
const int MOVING_DOWN = 2;
int bufferState = 0;
bool flagState = false;
```

## Función de configuración

##### Función que se ejecuta una sola vez al inicio del programa y se utiliza para inicializar los pines de entrada y salida, así como para configurar la comunicación serial.

```cpp
void setup() {
  // Configuración de pines
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_INCREASE, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  pinMode(BUTTON_DECREASE, INPUT_PULLUP);
  for (int i = 0; i < 7; i++) {
    pinMode(pines[i], OUTPUT);
  }
  // Inicialización de variables
  initialTime = millis();
  display(floorCounter);
}
```

## Función principal


```cpp
void loop() {
  #### Lectura de botones
  checkButton(BUTTON_INCREASE);
  checkButton(BUTTON_STOP);
  checkButton(BUTTON_DECREASE);
 display(floorCounter);
}
```
## Función para leer el estado de un botón
##### Función que se encarga de leer el estado de un botón específico y, si se detecta una pulsación, llama a la función setElevator() con el parámetro correspondiente.

```cpp
void checkButton(int pinButton) {
  if (digitalRead(pinButton) == LOW) {
    if (pinButton == BUTTON_INCREASE) {
      setElevator(true);
    } else if (pinButton == BUTTON_STOP) {
      stopElevator(timeLeft);
    } else if (pinButton == BUTTON_DECREASE) {
      setElevator(false);
    }
  }
}
```

## Función para iniciar el movimiento del montacargas
##### Función que se encarga de iniciar el movimiento del montacargas en la dirección especificada por el parámetro direction. Si el montacargas está detenido, llama a la función

```cpp

void setElevator(bool direction) {
  if (ElevatorState == STOPPED) {
    ElevatorState = direction ? MOVING_UP : MOVING_DOWN;
    moveElevator(ElevatorState);
    timeHandler(direction);
  }
}
```

## Función para detener el movimiento del montacargas

```cpp
void stopElevator(unsigned long timeleft) {
  if (ElevatorState != STOPPED) {
    if (!flagState) {
      bufferState = ElevatorState;
      flagState = true;
    } else {
      ElevatorState = bufferState;
      flagState = false;
    }
    anchorElevator();
    if (ElevatorState == MOVING_UP) {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      floorCounter++;
    } else if (ElevatorState == MOVING_DOWN) {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      floorCounter--;
    }
    timeLeft = timeleft;
  }
}
```

## Función para encender el LED correspondiente

```cpp
void moveElevator(int elevatorState) {
  if (elevatorState == MOVING_UP) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else if (elevatorState == MOVING_DOWN) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}
```

## Función para detener completamente el movimiento del montacargas
##### Función que se encarga de detener completamente el movimiento del montacargas y actualizar el contador de pisos.


```cpp
void anchorElevator() {
  ElevatorState = STOPPED;
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  timerElevatorStopped = millis();
}
```

## Función para iniciar el temporizador de movimiento del montacargas
##### Función que se encarga de iniciar el temporizador de movimiento del montacargas. Si direction es igual a 1, el montacargas se mueve hacia arriba. Si es igual a 0, el montacargas se mueve hacia abajo. La función utiliza un bucle while para esperar a que pasen 3 segundos antes de detener el movimiento del montacargas y llamar a la función anchorElevator(). Durante el movimiento, se llama a la función stopElevator() para detectar si se ha pulsado el botón de parada.

```cpp
void timeHandler(bool direction) {
  timeLeft = timeMovement;
  while (timeLeft > 0) {
    timeLeft = timeMovement - (millis() - timerElevator);
    if (timeLeft <= 0) {
      anchorElevator();
    } else {
      stopElevator(timeLeft);
    }
  }
  timerElevator = millis();
  ElevatorState = direction ? MOVING_UP : MOVING_DOWN;
}
```

## Función para mostrar un número en el visualizador de 7 segmentos
#####  Función que se encarga de mostrar un número en el visualizador de 7 segmentos. Recibe como parámetro un número entero del 0 al 9 y utiliza una matriz de 2 dimensiones para encender los segmentos correspondientes.

```cpp
void display(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(pines[i], segments[num][i]);
  }
}
```
