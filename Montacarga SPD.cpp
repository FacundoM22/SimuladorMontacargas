#define LED_RED 12
#define LED_GREEN 13
#define LED_YELLOW 7
#define BUTTON_INCREASE 4
#define BUTTON_STOP 2
#define BUTTON_DECREASE 3



// Visualizador 7 segmentos 


#define DISPLAY_SEG_A 9
#define DISPLAY_SEG_B 8
#define DISPLAY_SEG_F 10
#define DISPLAY_SEG_G 11
#define DISPLAY_SEG_E A0
#define DISPLAY_SEG_D A1
#define DISPLAY_SEG_C A2


// Variables

int floorCounter = 0;
unsigned long timerElevator = 0;
unsigned long timerElevatorStopped = 0;


unsigned long timeMovement = 3000;
unsigned long initialTime = 0;
unsigned long timeLeft = 0;

int segments[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

int pines[7] = {
	
	DISPLAY_SEG_A, 
	DISPLAY_SEG_B,
	DISPLAY_SEG_C,
	DISPLAY_SEG_D,
	DISPLAY_SEG_E,
	DISPLAY_SEG_F,
	DISPLAY_SEG_G
};

// Estado del montacarga


int ElevatorState;
const int STOPPED = 0;
const int MOVING_UP = 1;
const int MOVING_DOWN = 2;
int bufferState = 0;
bool flagState = 0;



void setup()
{
  initialTime = millis();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON_INCREASE, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  pinMode(BUTTON_DECREASE, INPUT_PULLUP);
  
  // Display
  pinMode(DISPLAY_SEG_B, OUTPUT);
  pinMode(DISPLAY_SEG_A, OUTPUT);
  pinMode(DISPLAY_SEG_C, OUTPUT);
  pinMode(DISPLAY_SEG_D, OUTPUT);
  pinMode(DISPLAY_SEG_E, OUTPUT);
  pinMode(DISPLAY_SEG_F, OUTPUT);
  
  
  Serial.begin(9600);
}



void display(int num)
{
  for(int i = 0; i < 7; i++)
  {
	digitalWrite(pines[i],segments[num][i]);
  }
	
}

void loop()
{
	checkButton(BUTTON_DECREASE);
	checkButton(BUTTON_INCREASE);
	checkButton(BUTTON_STOP);
	display(floorCounter);
	
}

void checkButton(int pinButton)
{
	int buttonState = digitalRead(pinButton);
	
if (buttonState == LOW)
	{
		switch (pinButton)
		{
			case BUTTON_INCREASE:
			 setElevator(1);
			 
			break;
			case BUTTON_DECREASE:
			 setElevator(0);
			 
			break;
			
		}
	}
		
}


void setElevator(bool direction)
{
	if(direction == 1 && ElevatorState == STOPPED)
	{
		timeHandler(direction);
	}
	else if (direction == 0 && ElevatorState == STOPPED)
	{
		timeHandler(direction);
	}
}

void stopElevator(unsigned long timeleft)
{
	int buttonState = digitalRead(BUTTON_STOP);
	
  if(buttonState == LOW)
  {
	!flagState;
	switch(ElevatorState) 
	{
		case MOVING_UP:
		bufferState = MOVING_UP;
		ElevatorState = STOPPED;
		digitalWrite(LED_YELLOW,HIGH);
		digitalWrite(LED_GREEN, LOW);
		break;
		case MOVING_DOWN:
		bufferState = MOVING_DOWN;
		ElevatorState = STOPPED;
		digitalWrite(LED_YELLOW,HIGH);
		digitalWrite(LED_RED, LOW);
		break;
		default:
		 if (flagState % 2 == 0)
		 {
			timerElevatorStopped = millis();
			ElevatorState = bufferState;
			digitalWrite(LED_YELLOW,LOW);
			while(timeleft + (timeleft - timerElevatorStopped) != timeMovement)
			{
				moveElevator(ElevatorState);
			}
			anchorElevator();
		 }
			
		break;
	}
 }
}

void moveElevator(int elevatorState)
{
	if( elevatorState == MOVING_DOWN)
	{
		digitalWrite(LED_YELLOW,LOW);
		digitalWrite(LED_RED, HIGH);
	}
	else if( elevatorState == MOVING_UP)
	{
		
		digitalWrite(LED_GREEN,HIGH);
	}
}

void anchorElevator()
{
	if (ElevatorState == MOVING_UP)
			{
			digitalWrite(LED_GREEN, LOW); // Apagamos la luz verde.
			ElevatorState = STOPPED;
			floorCounter++;
			Serial.println(floorCounter);
			
			}
	else if (ElevatorState == MOVING_DOWN)
	{
			digitalWrite(LED_RED, LOW); // Apagamos la luz roja.
			ElevatorState = STOPPED;
			floorCounter--;
			Serial.println(floorCounter);
	}
}

void timeHandler(bool direction)
{	
	

	 if(direction == 1)
		 {
			unsigned long actualTime;
			timerElevator = millis();
			ElevatorState = MOVING_UP;
			while (((actualTime = millis()) - timerElevator) != timeMovement) // Mientras desde que tomamos el tiempo no hayan pasado 3 segundos:
			{	
				
				stopElevator(timeLeft = 3000 -(actualTime - timerElevator));
				if (ElevatorState == MOVING_UP)
				{
					digitalWrite(LED_GREEN, HIGH); // Mantenmos encendida la luz verde.
				}
			}
			anchorElevator();
			
		 }
		else
		 {
			unsigned long actualTime;
			timerElevator = millis();
			ElevatorState = MOVING_DOWN;
			while (((actualTime = millis()) - timerElevator) != timeMovement) // Mientras desde que tomamos el tiempo no hayan pasado 3 segundos:
			{	
				
				digitalWrite(LED_RED, HIGH); // Mantenmos encendida la luz verde.
			}
			anchorElevator();
		 }
 
}



