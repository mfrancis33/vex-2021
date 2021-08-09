/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       M. Francis                                                */
/*    Created:      Mon Jul 12 2021                                           */
/*    Description:  Code for team 2337B's robot for the 2021 season of VEX    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// FrontLeftMotor       motor         1               
// FrontRightMotor      motor         2               
// BackLeftMotor        motor         3               
// BackRightMotor       motor         4               
// LeftElevator         motor         5               
// RightElevator        motor         6               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

///////////////////////////////////////////////////////////////////////////
// CUSTOM METHODS

void setChassisMotors(int leftPer, int rightPer){
  //Set motor velocities
  FrontLeftMotor .setVelocity(leftPer,  percent);
  BackLeftMotor  .setVelocity(leftPer,  percent);
  FrontRightMotor.setVelocity(rightPer, percent);
  BackRightMotor .setVelocity(rightPer, percent);
}

void setChassisMotors(int percent){
  //Set motor velocities
  setChassisMotors(percent, percent);
}

void stopChassisMotors(){
  //Stop motors
  setChassisMotors(0);
}

///////////////////////////////////////////////////////////////////////////
// IMPORTANT FUNCTIONS

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

void autonomous(void) {
  // 
}

void usercontrol(void) {
  // User control code here, inside the loop

  int threshold = 20;
  int elevatorSpeed = 50;

  while(1){
    ///////////////////////////////////////////////////////////////////////////
    // CHASSIS MOTORS

    // Get percentage from controller axes
    int leftAxisPercent  = Controller1.Axis3.position(percent);
    int rightAxisPercent = Controller1.Axis2.position(percent);

    // Detect if we need to move the motors
    if(abs(leftAxisPercent) > threshold || abs(rightAxisPercent) > threshold){
      // Set velocity to the corresponding axis of the controller
      setChassisMotors(leftAxisPercent, rightAxisPercent);
    } else {
      // If no input is detected, stop motors
      stopChassisMotors();
    }

    ///////////////////////////////////////////////////////////////////////////
    // MOBILE GOAL ELEVATOR (4-BAR)

    // Move 4-bar motors depending on button presses
    if(Controller1.ButtonR1.pressing()){
      //Move motors up if R1 is pressed
      LeftElevator .setVelocity(elevatorSpeed, percent);
      RightElevator.setVelocity(elevatorSpeed, percent);
    } else if(Controller1.ButtonR2.pressing()){
      //Move motors down if R2 is pressed
      LeftElevator .setVelocity(-elevatorSpeed, percent);
      RightElevator.setVelocity(-elevatorSpeed, percent);
    } else {
      //Stop motors
      LeftElevator .setVelocity(0, percent);
      RightElevator.setVelocity(0, percent);
    }

    ///////////////////////////////////////////////////////////////////////////
    wait(20, msec);
  }
}

// DO NOT CHANGE THIS FUNCTION!
int main(){
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while(1){
    wait(100, msec);
  }
}
