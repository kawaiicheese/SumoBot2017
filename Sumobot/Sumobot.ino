//Pins
int qrFL = 2, qrFR = 3; //Analog A0 to A1, frontleft, frontright
int sonicReceive = 3; //Digital 3
int sonicSend = 9; //Digital 9
int motorLeftForward = 10, motorLeftBack = 11; //PWM
int motorRightForward = 6, motorRightBack = 5; //PWM

//Global variables:
int bWBarrierValue = 500;
long enemyNear = 50; // value when enemy is near

void setup() {
  //Enable ultrasonic sensor
  pinMode(sonicSend, OUTPUT);
  pinMode(sonicReceive, INPUT);
  delay(5000);
  //QR sensors are analog and don't need a mode set
}

void loop() {
  Serial.begin(9600);
  //check if bot is on the edge
  fullSpeedAhead();
  delay(1000);
  stopMoving();
  delay(500);

  turnRight(750);

  delay(5000);
  /**
  while (qrAtEdge(qrFL) || qrAtEdge(qrFL)) {
    stopMoving();
    FrontInCorner();
  }
  */
  
  delay(5000);
}

//Main Methods
/**
  @brief will turn 120 degrees unless robot sees enemy.
         we need to test for TIME NEEDED
*/
void FrontInCorner() {
  long startTime = millis();
  long TIMEFOR120DEG = 395;
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
    Serial.println(analogRead(qrFR));
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
