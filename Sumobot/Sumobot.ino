//Pins
int qrFL = 2, qrFR = 3;
int sonicReceive = 3; //Digital 3
int sonicSend = 9; //Digital 9
int motorLeftForward = 10, motorLeftBack = 11; //PWM
int motorRightForward = 6, motorRightBack = 5; //PWM

//Environment parameters:
int bWBarrierValueRight = 1000, bWBarrierValueLeft = 1000;
int enemyNear = 50; // value when enemy is near
int timeFor120DegLeft = 750, timeFor120DegRight = 395;

void setup() {
  //Enable ultrasonic sensor
  pinMode(sonicSend, OUTPUT);
  pinMode(sonicReceive, INPUT);
  
  delay(5000);

  turnLeftTime(230); //maybe, if you don't wanna charge head on.
  /********************************************************fullSpeedAhead();***********************************/
}

void loop() {
  qrPrintCOMDebugSynchronous(5, 100);
  sonicPrintCOMDebugSynchronous(5, 100);
  
  /*****************************************************************************************while (qrAtEdge(qrFL)) {
    frontLeftAtBorder();
  }
  
  while (qrAtEdge(qrFR)) {
    frontRightAtBorder();
  }**************************************************************************************************************/
}

//Main Methods
/**
  @brief will turn 120 degrees unless robot sees enemy.
         we need to test for TIME NEEDED
*/
void frontLeftAtBorder() {
  long startTime = millis();;
  do {
    turnRight();
  } while(pollSonic() > enemyNear || millis() - startTime <= timeFor120DegRight); //runs for time or until enemy near
  fullSpeedAhead();
}

void frontRightAtBorder() {
  long startTime = millis();
  do {
    turnLeft();
  } while(pollSonic() > enemyNear || millis() - startTime <= timeFor120DegLeft); //runs for time or until enemy near
  fullSpeedAhead();
}





//Sonic sensor functions:
long pollSonic() {
  //Send a pulse through the send:
  digitalWrite(sonicSend, LOW);
  delayMicroseconds(20);
  digitalWrite(sonicSend, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonicSend, LOW);

  //Receive a pulse:
  long duration = pulseIn(sonicReceive, HIGH);
  long distanceInCm = duration / 58;

  return distanceInCm;
}

void sonicPrintCOMDebugSynchronous(int numPrints, int delayTime) { //WARNING: Takes full ocntrol of CPU. Synchronous!!!!
  Serial.begin(9600);
  for(int i = 0; i < numPrints; i++) {
    Serial.print(pollSonic());
    Serial.println("cm");
    delay(delayTime);
  }
  Serial.end();
}



//QR sensor functions:
boolean qrAtEdge(int qrNumber) {
  int bWBarrierValue = qrNumber == qrFR ? bWBarrierValueRight : bWBarrierValueLeft;
  if(analogRead(qrNumber) > bWBarrierValue) {
    return true;
  } else {
    return false;
  }
}

void qrPrintCOMDebugSynchronous(int numPrints, int delayTime) { //WARNING: Takes full ocntrol of CPU. Synchronous!!!!
  Serial.begin(9600);
  for(int i = 0; i < numPrints; i++) {
    Serial.print(analogRead(qrFL));
    Serial.print(" ");
    Serial.print(analogRead(qrFR));
    Serial.print(" ");
    delay(delayTime);
  }
  Serial.end();
  
}




//Motor Functions:
void setLeftForward(int speed) {
  analogWrite(motorLeftBack, 0);
  analogWrite(motorLeftForward, speed);
}

void setLeftBack(int speed) {
  analogWrite(motorLeftForward, 0);
  analogWrite(motorLeftBack, speed);
}

void setRightForward(int speed) {
  analogWrite(motorRightBack, 0);
  analogWrite(motorRightForward, speed);
}

void setRightBack(int speed) {
  analogWrite(motorRightForward, 0);
  analogWrite(motorRightBack, speed);
}

void fullSpeedAhead() {
  setLeftForward(255);
  setRightForward(255);
}

void stopMoving() {
  setLeftForward(0);
  setRightForward(0);
}

void turnRight() {
  setLeftForward(255);
  setRightBack(255);
}

void turnLeft() {
  setLeftBack(255);
  setRightForward(255);
}

void turnRightTime(int time) {
  turnRight();
  delay(time);
  stopMoving();
}

void turnLeftTime(int time) {
  turnLeft();
  delay(time);
  stopMoving();
}

/*void turnRightDegrees(int degrees) {
  turnRightTime(millisPer10Degrees*degrees/10);
}

void turnLeftDegrees(int degrees) {
  turnLeftTime(millisPer10Second*degrees/10);
}
*/
