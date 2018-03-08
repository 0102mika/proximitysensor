/*Stepper motor examples using the Adafruit motorshiel
 * 
 * uses a modified AccelStepper library: https://github.com/adafruit/AccelStepper
 * Libray guide:  http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a4f0989d0ae264e7eadfe1fa720769fb6
 * 
 * step sequence
 * 
 * 
 * 
 */

#include <AccelStepper.h>
#include <AFMotor.h>
#include <NewPing.h>


AF_Stepper motor2(20, 2);  //inputs: number of steps on the motor,  motornumber
float m2MaxSpeed = 200;  //this changes per motor + means clockwis - means counterclockwise
float m2accel = 100;

int stepTargets[] = {300,-500, 400, -200}; //where to step to/from
int holdTimes[] =   {1000,  1000,  0, 2000};  //how long to hold once reaching the target
int targetCount = 0;


unsigned long beginHold;  //used to create the holding timer
boolean isHolding = false;





#define TRIGGER_PIN  14  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     15  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int dis;


//set up the Accel library to work with the motor shield
void forwardstep2() 
{  
  motor2.onestep(FORWARD, SINGLE); ///Options:  SINGLE, DOUBLE, INTERLEAVE, MICROSTEP
}
void backwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE); ///Options:  SINGLE, DOUBLE, INTERLEAVE, MICROSTEP
}

AccelStepper stepper2(forwardstep2, backwardstep2);
///

void setup() 
{
stepper2.setMaxSpeed(m2MaxSpeed);
stepper2.setAcceleration(m2accel);
stepper2.moveTo(stepTargets[targetCount]);
 Serial.begin(115200);
}

void loop() 
{
 delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
 dis=sonar.ping_cm();

 if(dis >= 50 || dis <= 0){

  stepper2.setMaxSpeed(0);
  stepper2.setAcceleration(0);
  Serial.println("Out of range");
  
 } else {

  if (stepper2.distanceToGo() == 0) //check how far it is from the target, if 0 changes
  {
    if(!isHolding)          //check to see if it just reached the destination
    {
      beginHold = millis();  //grab the time so we can calculate the hold
      isHolding = true;  //set the flag to true so it won't keep recalculating
    }
    
    
        if(millis()-beginHold>=holdTimes[targetCount])  //timer used to compare the time when it reached the step to the holding time
        {
        
            if(targetCount + 1 <(sizeof(stepTargets)/sizeof(int)))   //checks the size of the array to see if there is another value in the list
            {
              targetCount++;
            }
            else        //if not go back to the beginning
            {
              targetCount = 0;
            }
    
        isHolding = false;  //set the flag back to false
        stepper2.moveTo(stepTargets[targetCount]);  //flip the direction
        }

 
  }
  Serial.println(dis);
  stepper2.run();
 }
  
}
