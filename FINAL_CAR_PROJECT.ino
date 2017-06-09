// Define Wheel
#define IN_1 5
#define IN_2 4
#define IN_3 7
#define IN_4 6 
#define ENA 9
#define ENB 15

// Define Sensor Waves
#define TRIG 12
#define ECHO 13
#define TIMEOUT 5000
#define LOOP_TIMEOUT 200

// Infrared Sensor
#define IS_A0_L 9
#define IS_A0_R 8
int a, b, delta;

#define MIN_SPEED 0 
#define MAX_SPEED 180
#define OBSTACLE_LENGTH 15
// Setup
void setup() {

  // Setup Wheel
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Setup Sound Sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  Serial.begin(9600);
  
  analogWrite(ENA, 50);
  analogWrite(ENB, 50);

  pinMode(IS_A0_L,INPUT);
  pinMode(IS_A0_R,INPUT);
}

// Get distance
float getDistance() {
  long duration, distanceCM;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH, TIMEOUT);

  //convert to distance
  distanceCM = duration / 29.1 / 2;

  return distanceCM;
}

void motor1Tien(int speed) {
  // speed -= 55;
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN_1, HIGH);
  analogWrite(IN_2, MAX_SPEED - speed);
}

void motor2Tien(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN_3, HIGH);
  analogWrite(IN_4, MAX_SPEED - speed);
}

void motor1Lui(int speed) {
  // speed -= 55;
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN_1, LOW);
  analogWrite(IN_2, speed);
}

void motor2Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN_3, LOW);
  analogWrite(IN_4, speed);
}

void motor1Dung() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
}

void motor2Dung() {
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
}

void tien(int speed) {
  motor1Tien(speed);
  motor2Tien(speed);
}

void lui(int speed) {
  motor1Lui(speed);
  motor2Lui(speed);
}

void dung() {
  motor1Dung();
  motor2Dung();
}

void trai(int speed) {
  motor1Lui(speed);
  motor2Dung();
}

void phai(int speed) {
  motor1Dung();
  motor2Lui(speed);
}

///////////////////////
void stopCar(int length) {
  dung();
  delay(length);
}

void turnLeft(int speed, int length) {
  trai(speed);
  delay(length);
}

void turnRight(int speed, int length) {
  phai(speed);
  delay(length);
}

void goStraight(int speed, int length = -1) {
  tien(speed);
  if (length > -1) {
    delay(length);  
  }
  
}

void goBehind(int speed, int length = -1) {
  lui(speed);
  if (length > -1) {
    delay(length);
  }
}

void aroundLeft(int speed, int length = -1) {
  motor1Lui(speed);
  motor2Tien(speed);

  if (length > -1) {
    delay(length);
  }
}

void aroundRight(int speed, int length = -1) {
  motor1Tien(speed);
  motor2Lui(speed);

  if (length > -1) {
    delay(length);
  }
}

void changeDirection() {
  a = digitalRead(IS_A0_L); // gia tri cam bien trai
  b = digitalRead(IS_A0_R); // gia tri cam bien phải
  Serial.println(a);
  Serial.println(b);

  if (a == 0 && b == 0) goStraight(MAX_SPEED);
  else {
    delta = b - a;
    if (delta < 0) {
      Serial.println("TRAI");
      aroundLeft(MAX_SPEED, 100);
      stopCar(500);
    } else if (delta > 0) {
      Serial.println("PHAI");
      aroundRight(MAX_SPEED, 100);
      stopCar(500);
    } else {
      goStraight(MAX_SPEED, 200);
    }
  }

  

}
void runCar() {

  long distance = getDistance();

  Serial.print(">> DISTANCE: ");
  Serial.println(distance);

  // Call when the car faces the obstacle
  if (distance > 0 && distance <= OBSTACLE_LENGTH) {

    // Stop a little while
    stopCar(500);

    // Stay behind a little bit
    goBehind(MAX_SPEED, 200);

    // Determine go left or right here
    aroundLeft(MAX_SPEED, 250);

  }

  // Adjust the direction to follow the line on the road
  changeDirection();

  goStraight(MAX_SPEED);  
  
}

void loop() {
  runCar();
}
