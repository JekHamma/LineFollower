// Pololu #713 motor driver pin assignments
const int PWMA=11; // Pololu drive A
const int AIN2=10;
const int AIN1 =9;
const int STDBY=8;
const int BIN1 =7; // Pololu drive B
const int BIN2 =6;
const int PWMB =5;

const int LR = A0;
const int MR = A1;
const int RR = A2;

float left;
float middle;
float right;

float Kp = 2;
float Ki = .5;
float I_error = 0;
float Kd = .01;
float Der = 0;;
float previous_error = 0;

float speedL = 50;
float speedR = 50;

int tol = 20;

int speed_min = 10;
int max_speed = 50;

void setup(){
  pinMode(PWMA , OUTPUT);
  pinMode(AIN1 , OUTPUT);
  pinMode(AIN2 , OUTPUT);
  pinMode(BIN1 , OUTPUT);
  pinMode(BIN2 , OUTPUT);
  pinMode(PWMB , OUTPUT);
  pinMode(STDBY , OUTPUT);
  digitalWrite(STDBY , HIGH);

  pinMode(LR, INPUT);  
  pinMode(MR, INPUT);
  pinMode(RR, INPUT);

  Serial.begin(9600);
}

void loop(){
  steer();
  drive(speedL, speedR);
}

float compute_PID(float error){
  I_error = I_error + .01*error;
  Der = (error - previous_error)/.01;
  previous_error = error;
  return Kp*error + Ki*I_error + Kd*Der;
}


void motorWrite(int spd, int pin_IN1 , int pin_IN2 , int pin_PWM){
  if (spd < 0){
    digitalWrite(pin_IN1 , HIGH); // go one way
    digitalWrite(pin_IN2 , LOW);
  }
  else{
    digitalWrite(pin_IN1 , LOW); // go the other way
    digitalWrite(pin_IN2 , HIGH);
  }

  analogWrite(pin_PWM , abs(spd));
}

void drive(int speedL, int speedR){
  //Takes both speed inputs and passes them to the motorWrite function with correct parameters assuming drive A is hooked to the left motor.
  motorWrite(speedL, AIN1, AIN2, PWMA);
  motorWrite(speedR, BIN1, BIN2, PWMB);
  
}

float read_data(){
  left = analogRead(LR);
  middle = analogRead(MR);
  right = analogRead(RR);
  // Serial.print(left);
  // Serial.print(", ");
  // Serial.print(middle);
  // Serial.print(", ");
  // Serial.println(right);
 
 }



void steer(){
  read_data();
  float error = abs(left-right);
  float change = compute_PID(error);

  if (abs(left-right) > tol){
    if (left < right){
      speedR = speedR - change;}
    if (left > right){
      speedL = speedL - change;}
  }
  else{
    speedL = max_speed;
    speedR = max_speed;}

  Serial.print(speedL);
  Serial.print("    ");
  Serial.println(speedR);
}

