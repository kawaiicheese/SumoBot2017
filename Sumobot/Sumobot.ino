//Pins
int qrFL = 2, qrFR = 3;
int sonicReceive = 3; //Digital 3
int sonicSend = 9; //Digital 9
int motorLeftForward = 10, motorLeftBack = 11; //PWM
int motorRightForward = 6, motorRightBack = 5; //PWM

//Environment parameters:
int bWBarrierValueRight = 400, bWBarrierValueLeft = 400;
int enemyNear = 40; // value when enemy is near
int timeFor120DegLeft = 2000, timeFor120DegRight = 1500;

void setup() {
  //Enable ultrasonic sensor
  pinMode(sonicSend, OUTPUT);
  pinMode(sonicReceive, INPUT);
  
  delay(5000);

  //turnLeftTime(130); //maybe, if you don't wanna charge head on.
  fullSpeedAhead();
}

void loop() {
  //qrPrintCOMDebugSynchronous(5, 100);
  //sonicPrintCOMDebugSynchronous(5, 100);
  
  while (qrAtEdge(qrFL)) {
    //moveBack();
    //delay(350);
    frontLeftAtBorder();
  }
  
  while (qrAtEdge(qrFR)) {
    //moveBack();
    //delay(350);
    frontRightAtBorder();
  }
}

//Main Methods
/**
  @brief will turn 120 degrees unless robot sees enemy.
         we need to test for TIME NEEDED
*/
void frontLeftAtBorder() {
  //moveBack();
  //delay(100);
  long startTime = millis();
  
  while(pollSonic() > enemyNear && millis() - startTime <= timeFor120DegRight) { //runs for time or until enemy near
    turnRight(140);
  }
  
  fullSpeedAhead();
}

void frontRightAtBorder() {
  //moveBack();
  //delay(100);
  long startTime = millis();
  while(pollSonic() > enemyNear && millis() - startTime <= timeFor120DegLeft) { //runs for time or until enemy near
    turnLeft(140);
  }
  
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
  if(analogRead(qrNumber) < bWBarrierValue) {
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

void moveBack() {
  setLeftBack(255);
  setRightBack(255);
}

void stopMoving() {
  setLeftForward(0);
  setRightForward(0);
}

void turnRight(int speed) {
  setLeftForward(speed);
  setRightBack(speed);
}

void turnLeft(int speed) {
  setLeftBack(speed);
  setRightForward(speed);
}

void turnRightTime(int time) {
  turnRight(255);
  delay(time);
  stopMoving();
}

void turnLeftTime(int time) {
  turnLeft(255);
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
