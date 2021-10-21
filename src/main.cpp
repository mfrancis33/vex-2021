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
// LeftMotor            motor         1               
// RightMotor           motor         2               
// FrontMotor           motor         3               
// BackMotor            motor         4               
// LeftArm              motor         5               
// RightArm             motor         6               
// LeftLever            motor         7               
// RightLever           motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"

#define AUTON 0

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

///////////////////////////////////////////////////////////////////////////
// AUTON FUNCTIONS

/**
 * Moves the chassis motors at the same time
 */
void robot_drive(double amount){
  LeftMotor .spin(forward, amount, percent);
  RightMotor.spin(forward, amount, percent);
}

/**
 * Stops the chassis motors
 */
void robot_stop(){
  LeftMotor .stop();
  RightMotor.stop();
}

///////////////////////////////////////////////////////////////////////////
// IMPORTANT FUNCTIONS

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

void autonomous(void) {
  switch(AUTON){
    default:
    case 0:
      /**
       * Drives forward for a little bit and stops.
       */
      robot_drive(100);
      wait(2, seconds);
      robot_stop();
      break;
    case 1:
      /**
       * 
       */
      break;
    case 2:
      /**
       * 
       */
      break;
  }
}

void usercontrol(void) {
  // User control code here, inside the loop

  double threshold = 20;
  double armSpeed = 50;
  double leverSpeed = 20;

  while(true){
    ///////////////////////////////////////////////////////////////////////////
    // CHASSIS MOTORS

    // Get percentage from controller axes
    double leftAxisVertPercent   = (double)(Controller1.Axis3.position(percent));
    double rightAxisVertPercent  = (double)(Controller1.Axis2.position(percent));

    double leftAxisHorizPercent  = (double)(Controller1.Axis4.position(percent));
    double rightAxisHorizPercent = (double)(Controller1.Axis1.position(percent));

    // Detect if we need to move the motors that move forwards/backwards
    if(std::abs(leftAxisVertPercent) > threshold || std::abs(rightAxisVertPercent) > threshold){
      // Set velocity to the corresponding axis of the controller
      LeftMotor .spin(forward, leftAxisVertPercent, percent);
      RightMotor.spin(forward, rightAxisVertPercent, percent);
    } else {
      // If no input is detected, stop motors
      LeftMotor .stop();
      RightMotor.stop();
    }

    // Detect if we need to move the motors that move side to side or if we
    // need to go turny-turn in place
    if(std::abs(leftAxisHorizPercent) > threshold){
      // Set velocity to the corresponding axis of the controller
      FrontMotor.spin(forward, leftAxisHorizPercent, percent);
      BackMotor .spin(forward, leftAxisHorizPercent, percent);
    } else if(std::abs(rightAxisHorizPercent) > threshold){
      // Set velocity to the corresponding axis of the controller
      FrontMotor.spin(forward,  rightAxisHorizPercent, percent);
      BackMotor .spin(forward, -rightAxisHorizPercent, percent);
    } else {
      // If no input is detected, stop motors
      FrontMotor.stop();
      BackMotor .stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // ARM MECHANISM

    // Move arm motors depending on button presses
    if(Controller1.ButtonUp.pressing()){
      //Move motors up if up button is pressed
      LeftArm .spin(forward, armSpeed, percent);
      RightArm.spin(forward, armSpeed, percent);
    } else if(Controller1.ButtonDown.pressing()){
      //Move motors down if down button is pressed
      LeftArm .spin(reverse, armSpeed, percent);
      RightArm.spin(reverse, armSpeed, percent);
    } else {
      //Stop motors
      LeftArm .stop();
      RightArm.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // INDIVIDUAL LEVER MECHANISMS

    // Move left lever motor depending on button presses
    if(Controller1.ButtonL1.pressing()){
      //Move motor up if L1 is pressed
      LeftLever.spin(forward, leverSpeed, percent);
    } else if(Controller1.ButtonL2.pressing()){
      //Move motor down if L2 is pressed
      LeftLever.spin(reverse, leverSpeed, percent);
    } else {
      //Stop motors
      LeftLever.stop();
    }

    // Move right lever motor depending on button presses
    if(Controller1.ButtonR1.pressing()){
      //Move motor up if R1 is pressed
      RightLever.spin(forward, leverSpeed, percent);
    } else if(Controller1.ButtonR2.pressing()){
      //Move motor down if R2 is pressed
      RightLever.spin(reverse, leverSpeed, percent);
    } else {
      //Stop motor
      RightLever.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    wait(20, msec);
  }
}

// DO NOT CHANGE THIS FUNCTION IF YOU DO NOT KNOW WHAT YOU ARE DOING!
int main(){
  // Added for debugging purposes:
  Controller1.Screen.print("Current auton: ");
  Controller1.Screen.print(AUTON);

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while(true){
    wait(100, msec);
  }
}
