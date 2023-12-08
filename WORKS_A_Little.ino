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
float Ki = 1.5;
float integral = 0;
float Kd = .1;
float derivative = 0;;
float previous_error = 0;
float control_value = 0;

float speedL = 50;
float speedR = 50;

int tol = 65;

int speed_min = 10;
int max_speed = 150;

int threshold = 575;
int LED = 2;

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
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop(){
  steer();
  drive(speedL, speedR);
  check_gap();
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
  float error = left-right;
  // computing PID
  integral = integral + .01*error;
  derivative = error-previous_error ;
  integral = constrain(integral,-50,50);
  Serial.println(integral);

  control_value = Kp*error + Ki*integral + Kd*derivative;
  control_value = constrain(control_value, -max_speed, max_speed);


  if (control_value < 0){
    //turn right
    speedL = max_speed;
    speedR = max_speed + control_value;
  }
  if (control_value > 0){
    //turn left
    speedL = max_speed - control_value;
    speedR = max_speed;
  }

  speedL = constrain(speedL,-max_speed,max_speed);
  speedR = constrain(speedR,-max_speed,max_speed);

  
}


void check_gap(){
  if ((left < threshold) && (middle < threshold) && (right < threshold)){
    digitalWrite(LED, HIGH);
  }
  else
    digitalWrite(LED, LOW);
}