#include <Servo.h> 
 
Servo myservo;  

// save the servo current position 
int pos = 0;

// save the disatance of the clostest obstacle
int dist = 100;

// tells the robot where to go when finding an obstacle
boolean goRight = true;

// the minimum cm which we allow to be near an obstacle
const int minimumObstacleDistance = 10;

// the pin number of the sensor's output:
const int sonicPin = 7;

// the pin number of the servo outpur
const int servoPin = 10;

void setup() 
{ 
  // attaches the servo on pin 9 to the servo object 
  myservo.attach(servoPin);
  
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin
} 
 
 
void loop() 
{ 
  // start moving forward
  moveForward();
  
  // keep moving until we recognize an obstacle
  do {
    // goes from 60 degrees to 100 degrees 
    for(pos = 60 ; pos < 100 ; pos += 1)  
    {
      myservo.write(pos);
      
      // waits 15ms for the servo to reach the position
      delay(15);
      
      dist = getDistanceToObjectAhead();
      
      if(dist <= minimumObstacleDistance)
      {
        break;
      }
    }
    
    // goes from 100 degrees to 60 degrees 
    for(pos = 100 ; pos >= 60 ; pos -= 1)
    {                                
      myservo.write(pos);
      
      // waits 15ms for the servo to reach the position
      delay(15);
      
      dist = getDistanceToObjectAhead();
      
      if(dist <= minimumObstacleDistance)
      {
        break;
      }
    }
  } while(dist > minimumObstacleDistance);
  
  // stop the robot
  stopMoving();
  
  delay(1000);
  
  // try to turn right or left
  if(goRight)
  {
    turnRight();
  }
  else {
   turnLeft();
  }
  
  // stop the robot
  stopMoving();
  
  // next time go to opposite direction
  goRight = !goRight;
}

// stop the motors
void stopMoving()
{
  //Engage the Brake for Channel A
  digitalWrite(9, HIGH);
  //Engage the Brake for Channel B
  digitalWrite(8, HIGH);
  // set motor A speed
  analogWrite(3, 0);
  // set motor B speed
  analogWrite(11, 0);
}

// turn the robot to the right
void turnRight() {
  //Motor A forward @ half speed
  //Establishes forward direction of Channel A
  digitalWrite(12, HIGH);
  //Disengage the Brake for Channel A
  digitalWrite(9, LOW);
  //Spins the motor on Channel A at half speed
  analogWrite(3, 128);
  
  // motor B stops
  //Engage the Brake for Channel B
  digitalWrite(8, HIGH);
  // set motor B speed
  analogWrite(11, 0);
  
  delay(1000);
}

// turn the robot to the right
void turnLeft() {
  //Motor B forward @ half speed
  //Establishes forward direction of Channel B
  digitalWrite(13, LOW);
  //Disengage the Brake for Channel B
  digitalWrite(8, LOW);
  //Spins the motor on Channel B at half speed
  analogWrite(11, 128);
  
  // motor A stops
  //Engage the Brake for Channel A
  digitalWrite(9, HIGH);
  // set motor B speed
  analogWrite(3, 0);
  
  delay(1000);
}

// start the motors to spin forward
void moveForward()
{
  //Motor A forward @ full speed
  //Establishes forward direction of Channel A
  digitalWrite(12, HIGH);
  //Disengage the Brake for Channel A
  digitalWrite(9, LOW);
  //Spins the motor on Channel A at full speed
  analogWrite(3, 255);
  
  //Motor B forward @ full speed
  //Establishes forward direction of Channel B
  digitalWrite(13, LOW);
  //Disengage the Brake for Channel B
  digitalWrite(8, LOW);
  //Spins the motor on Channel B at full speed
  analogWrite(11, 255);
}



// get the distance in cm from the closest obstacle ahead of us
long getDistanceToObjectAhead()
{
   // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(sonicPin, OUTPUT);
  digitalWrite(sonicPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sonicPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonicPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(sonicPin, INPUT);
  duration = pulseIn(sonicPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  return cm;
}

// convert microsecond to inches
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}

// convert microseconds to centimeters
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
