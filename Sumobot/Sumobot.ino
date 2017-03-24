//Pins
int qrFL = 0, qrFR = 1, qrBL = 2, qrBR = 3; //Analog A0 to A3, frontleft, frontright, backleft, backright
int sonicReceive = 3; //Digital 3
int sonicSend = 9; //Digital 9
int motorLeftOn = 2, motorRightOn = 4; //Digital 2 and 4
int motorLeftForward = 10, motorLeftBack = 11; //PWM
int motorRightForward = 5, motorRightBack = 6; //PWM

//Global variables:
int bWBarrierValue = 500;
long enemyNear = 50; // value when enemy is near

void setup() {
  //Enable motors
  pinMode(motorLeftOn, OUTPUT);
  pinMode(motorRightOn, OUTPUT);
  digitalWrite(motorLeftOn, HIGH);
  digitalWrite(motorRightOn, HIGH);

  //Enable ultrasonic sensor
  pinMode(sonicSend, OUTPUT);
  pinMode(sonicReceive, INPUT);
  
  //QR sensors are analog and don't need a mode set
}

void loop() {
  Serial.begin(9600);
  //check if bot is on the edge
  fullSpeedAhead();
  while (qrAtEdge(qrFL) || qrAtEdge(qrFL)) {
    stopMoving();
    FrontInCorner();
  }
}

//Main Methods
/**
  @brief will turn 120 degrees unless robot sees enemy.
         we need to test for TIME NEEDED
*/
void FrontInCorner() {
  long startTime = millis();
  long TIMEFOR120DEG = 0;
  do {
    setLeftForward(255);
    setRightBack(255);
  } while(pollSonic() < enemyNear || millis() - startTime <= TIMEFOR120DEG); //runs for time or until enemy near
  stopMoving();
}


//Sonic sensor functions:
long pollSonic() {
  //Send a pulse through the send:
  digitalWrite(sonicSend, LOW);
  delayMicroseconds(100);
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

void EnemyInSonicRange(long enemyDist) {
  if(enemyDist < enemyNear) {
    fullSpeedAhead();
  }
}


//QR sensor functions:
boolean qrAtEdge(int qrNumber) {
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
    Serial.print(analogRead(qrBL));
    Serial.print(" ");
    Serial.println(analogRead(qrBR));
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

void turnRight(int time) {
  setLeftForward(255);
  setRightBack(255);
  delay(time);
  setLeftForward(0);
  setRightForward(0);
}

void turnLeft(int time) {
  setLeftBack(255);
  setRightForward(255);
  delay(time);
  setLeftForward(0);
  setRightForward(0);
}
