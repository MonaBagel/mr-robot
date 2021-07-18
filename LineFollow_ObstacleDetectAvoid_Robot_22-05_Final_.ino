//Motors
#include <DRV8835MotorShield.h>
#define LED_PIN 13
DRV8835MotorShield motors;
const int speedM1 = 110; //speed for left motor
const int speedM2 = 120; //speed for right motor
int speedStop = 0; //stop motors 

//Line Sensors
int RS = A4; //Line Sensors pin allocation  
int LS = A0;                              
int CS = A2;
int blockCount = 0; //used to count black blocks on course

//Ultrasonic Sensors
// defining the pins
const int trigPin = 12;
const int echoPin = 13;
//defining variables
long duration;
int distance;
int wall = 0;

void setup() 
{
  //Motors
    pinMode(LED_PIN, OUTPUT);
    //motors' directions need to be flipped to move forward in the right direction
    motors.flipM1(true);
    motors.flipM2(true);

   //Line sensors
    pinMode(LS, INPUT); //LS = 5 -> BLACK
    pinMode(RS, INPUT); // RS = 2 -> BLACK
    pinMode(CS, INPUT); // CS = 4 -> BLACK 
  
  //Ultrasonic Sensors
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // open serial monitor @ 9600 baud

   delay(4000);//4 second start delay
}

void loop() 
{
  //Ultrasonic Sensors
    digitalWrite(trigPin, LOW); // Clears the trigPin
    delay(10);
    //Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);   
    //Distance Calculation
    duration = pulseIn(echoPin, HIGH); // Reads the echoPin-> returns sound wave travel time 
    distance= duration*0.034/2; // Calculating the distance

    
  //Get values from light Sensors
    int rspin = analogRead(RS);
    int lspin = analogRead(LS);
    int cspin= analogRead(CS);


  //LINE DETECTING -> FOR LINE FOLLOWING
    if((cspin <= 4) && (rspin > 10) && (lspin > 10)) moveForward();    // Move Forward

    if((cspin > 10) && (rspin <= 3) && (lspin > 10)) turnRight();// Turn right  

    if((cspin > 10) && (rspin > 10) && (lspin <= 5)) turnLeft();//Turn left
          
    if((cspin <= 4) && (rspin > 10) && (lspin <= 5)) turnLeft();//Turn left but when cs and ls detect black

    if((cspin <= 4) && (rspin <= 3) && (lspin > 10)) turnRight();// Turn right but when cs and rs detect black

    if((cspin <= 4) && (rspin <= 3) && (lspin <= 5)) // All sensors detect black
    {
      if(blockCount < 2)
      {
        stopRobot();
        delay(2000);
        moveForward();
        delay(500);
      }

      if(blockCount >= 2)
      {
        stopRobot();
        delay(1000);        
      }  
        blockCount++;
    }
    if(wall == 0)//used once with avoiding wall
     {
        if(distance >= 10 && distance < 15) 
        {
          Serial.print("Distance: "); //Prints the distance on the Serial Monitor (FOR TESTING)
          Serial.println(distance);
          Serial.println("RIGHT");
          turnLeft(); //turns right some reason???
          delay(800);
          moveForward();
          delay(500);
          turnRight(); //turns left some reason???
        }
        wall = 1; 
     }
    //Used with any other obstacle detection and avoidance     
    if(distance >= 1 && distance < 9) 
    {
      Serial.print("Distance: "); //Prints the distance on the Serial Monitor (FOR TESTING)
      Serial.println(distance);
      Serial.println("RIGHT");
      turnLeft(); //turns right for some reason???
      delay(600);
      moveForward();
      delay(300);
      turnRight(); //turns left for some reason???    
    }
}      

void moveForward() // Move Forward
{
  digitalWrite(LED_PIN, HIGH);
  motors.setM1Speed(speedM1);
  motors.setM2Speed(speedM2);
}

void turnRight() // Turn right
{
  digitalWrite(LED_PIN, HIGH);
  motors.setM1Speed(speedM1);
  motors.setM2Speed(speedStop);
}

void turnLeft() // Turn left
{
  digitalWrite(LED_PIN, HIGH);
  motors.setM1Speed(speedStop);
  motors.setM2Speed(speedM2);
}

void stopRobot() // Stop
{
  digitalWrite(LED_PIN, LOW);
  motors.setM1Speed(speedStop);
  motors.setM2Speed(speedStop);
}

boolean hasObstacle(double distance) // detects obstacle 
{
  if(distance >= 1 && distance < 9)
    return true;
  else
    return false;
}
