//Pins
int qr0 = 0, qr1 = 1, qr2 = 2, qr3 = 3; //Analog A0 to A3
int sonarReceive = 4; //Analog A4
int sonarSend = 9; //PWM
int motorLeftOn = 2, motorRightOn = 4; //Digital 2 and 4
int motorLeftForward = 10, motorLeftBack = 11; //PWM
int motorRightForward = 5, motorRightBack = 6; //PWM

final int HI_QR = 50; //Value for White Line. Subject to change
private static final int QR_TOLERANCE = 0.01; //Tolernance for QR_Sensor Subject to change

//Global variables:
int QR_Sensor = 1; //Subject to change
int QR_VALUE;

void setup() {
  //Enable motors
  pinMode(motorLeftOn, OUTPUT);
  pinMode(motorRightOn, OUTPUT);
  digitalWrite(motorLeftOn, HIGH);
  digitalWrite(motorRightOn, HIGH);

  //Enable QR_Sensor
  pinMode(Sensor1,INPUT);
  Serial.begin(9600);
}

void loop() {
  /* So far, move forward until reaching white line. However, IDK how to stop the bot. */
  QR_VALUE = analogRead(QR_Sensor);
  fullSpeedAhead();

  if(QR_VALUE + QR_TOLERANCE > HI_QR){
    //Stop The Car.
    turnLEFT(turnTime); //change num to time needed for turning
  }

  delay(20);
}

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

