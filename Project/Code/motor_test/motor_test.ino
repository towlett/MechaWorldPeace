#define enable_pin_right    5
#define enable_pin_left     6
#define dir_pin_right       7
#define dir_pin_left        8

void drive_forward(void);
void drive_backward(void);
void drive_stop(void);
void spin_left(void);
void spin_right(void);
void swerve_left(void);
void swerve_right(void);
void drive_brake(void);

void setup() {  // setup() function required for Arduino 
  // MOTOR SETUP 
  pinMode(dir_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
  
  Serial.begin(9600);
  Serial.println("Starting Panda");
}

void loop() {
  drive_forward();
  delay(3000);
  drive_backward();
  delay(3000);
  spin_left();
  delay(3000);
  spin_right();
  delay(3000);
  drive_forward();
  delay(3000);
  swerve_left();
  delay(3000);
  swerve_right();
  delay(3000);
  drive_stop();
  delay(3000);
}

void drive_forward(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 250);
  analogWrite(enable_pin_left, 250);
}

void drive_backward(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 200);
  analogWrite(enable_pin_left, 200);
}

void drive_stop(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
}

void spin_right(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 175);
  analogWrite(enable_pin_left, 175);
}

void spin_left(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 175);
  analogWrite(enable_pin_left, 175);
}

void swerve_right(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 200);
}

void swerve_left(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 200);
  analogWrite(enable_pin_left, 0);
}

void drive_brake(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 250);
  analogWrite(enable_pin_left, 250);
  delay(10);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
}
