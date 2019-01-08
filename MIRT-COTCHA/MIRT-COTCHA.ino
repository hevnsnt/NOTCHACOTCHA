
//************************************************************
// MIRT CODE
//************************************************************

int counter = 0;
int switchPin = 13;
int LEDARRAY = 0;
int LEDB = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(LEDARRAY, OUTPUT);
  pinMode(LEDB, OUTPUT);
 
}

void loop() {
 //Handle input
 int switchVal = digitalRead(switchPin);
 if(switchVal == HIGH)
 {
   delay(500);  
   counter ++;
   //Reset count if over max mode number
   if(counter == 8)
   {
     counter = 0;
   }
 }

 else
   //Change mode
   switch (counter) {
   case 1:
     analogWrite(LEDR, 255);
     break;
   case 2:
     analogWrite(LEDR, 127);
     break;
   case 3:
     analogWrite(LEDR, 000);
     analogWrite(LEDB, 255);
     break;
   case 4:
     analogWrite(LEDB, 127);
     break;
   case 5:
     analogWrite(LEDR, 255);
     analogWrite(LEDG, 255);
     analogWrite(LEDB, 255);
     break;
   case 6:  
     analogWrite(LEDR, 127);
     analogWrite(LEDG, 127);
     analogWrite(LEDB, 127);
     break;
   case 7: // MIRT MODE
     digitalWrite(LEDARRAY, HIGH);
     delay(35);
     delayMicroseconds(544);
     digitalWrite(LEDARRAY, LOW);
     delay(35);
     delayMicroseconds(552);
     break;
   }
}
