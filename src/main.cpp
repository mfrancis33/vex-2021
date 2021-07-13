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
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

void autonomous(void) {
  // 
}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // Set motor velocities
    int leftAxisPercent  = Controller1.Axis3.position(percent);
    int rightAxisPercent = Controller1.Axis2.position(percent);
    // Detect if we need to move the motors
    // The value of 20 is an arbitrary low value to prevent motor waste from
    // controller drifting. Absolute value is used to ensure that this works
    // in both directions.
    if(abs(leftAxisPercent) > 20 || abs(rightAxisPercent) > 20){
      // Set velocity to the amount the corresponding axis of the controller
      // is set to.
      FrontLeftMotor .setVelocity(leftAxisPercent,  percent);
      BackLeftMotor  .setVelocity(leftAxisPercent,  percent);
      FrontRightMotor.setVelocity(rightAxisPercent, percent);
      BackRightMotor .setVelocity(rightAxisPercent, percent);
    } else {
      // If no input is detected, set velocities to 0
      FrontLeftMotor .setVelocity(0, percent);
      BackLeftMotor  .setVelocity(0, percent);
      FrontRightMotor.setVelocity(0, percent);
      BackRightMotor .setVelocity(0, percent);
    }

    // DO NOT REMOVE THIS!
    wait(20, msec);
  }
}

// DO NOT CHANGE THIS FUNCTION!
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

