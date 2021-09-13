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

  double threshold = 20;
  double elevatorSpeed = 50;

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
    // MOBILE GOAL ELEVATOR (4-BAR)

    // Move 4-bar motors depending on button presses
    if(Controller1.ButtonR1.pressing()){
      //Move motors up if R1 is pressed
      LeftElevator .spin(forward, elevatorSpeed, percent);
      RightElevator.spin(forward, elevatorSpeed, percent);
    } else if(Controller1.ButtonR2.pressing()){
      //Move motors down if R2 is pressed
      LeftElevator .spin(reverse, elevatorSpeed, percent);
      RightElevator.spin(reverse, elevatorSpeed, percent);
    } else {
      //Stop motors
      LeftElevator .stop();
      RightElevator.stop();
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
  while(true){
    wait(100, msec);
  }
}
