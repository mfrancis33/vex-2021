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
// LeftArm              motor         5               
// RightArm             motor         6               
// LeftLever            motor         7               
// RightLever           motor         8               
// CenterMotor          motor_group   3, 4            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"
#include "nerd.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

#define AUTON 0
const auto& sleep = vex::task::sleep; // no clue if this works

const double THRESHOLD = 20;
const double ARMSPEED = 100;
const double LEVERSPEED = 20;
const double LEVERROTATION = 100;

///////////////////////////////////////////////////////////////////////////
// AUTON FUNCTIONS

/**
 * Moves the chassis motors at the same time
 */
void robot_drive(double amount){
  LeftMotor  .spin(forward, amount, percent);
  RightMotor .spin(forward, amount, percent);
  CenterMotor.spin(forward, amount, percent);
}

/**
 * Moves the chassis motors so that the robot turns left or right.
 * Positive values mean right.
 */
void robot_turn(double amount){
  LeftMotor  .spin(reverse, amount, percent);
  RightMotor .spin(forward, amount, percent);
  CenterMotor.stop();
  // CenterMotor.spin(forward, amount, percent);
}

/**
 * Stops the chassis motors
 */
void robot_stop(){
  LeftMotor  .stop();
  RightMotor .stop();
  CenterMotor.stop();
}

/**
 * Moves the arm motors
 */
void arm_move(double amount){
  LeftArm .spin(forward, amount, percent);
  RightArm.spin(forward, amount, percent);
}

/**
 * Stops the arm motors
 */
void arm_stop(){
  LeftArm .stop();
  RightArm.stop();
}

///////////////////////////////////////////////////////////////////////////
// OTHER FUNCTIONS

// https://stackoverflow.com/a/4609795
template <typename T> int sign(T val) {
  return (T(0) < val) - (val < T(0));
}

///////////////////////////////////////////////////////////////////////////
// IMPORTANT FUNCTIONS

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Initialize velocities of motors
  LeftLever.setVelocity(LEVERSPEED, percent);
  RightLever.setVelocity(LEVERSPEED, percent);

  // Put logo on screen
  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(color(128, 0, 0));
  for(int y = 0; y < sizeof(NERD); y++){
    for(int x = 0; x < sizeof(NERD[y]); x++){
      if(NERD[y][x]) Brain.Screen.drawPixel(x, y);
    }
  }
}

void autonomous(void) {
  // Drop holder thingy
  arm_move(ARMSPEED);
  sleep(250);
  arm_move(-ARMSPEED);
  sleep(250);
  arm_stop();

  // Run difdferent auton
  switch(AUTON){
    default:
    case 0:
      /**
       * Drives forward for a little bit and stops.
       */
      robot_drive(100);
      sleep(1500);
      robot_stop();
      // arm_move(ARMSPEED);
      // sleep(500);
      // arm_stop();
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

  bool buttonIsPressed = true;
  Controller1.Screen.clearScreen();

  bool leftLeverIsUp = false;
  bool rightLeverIsUp = false;

  while(true){
    ///////////////////////////////////////////////////////////////////////////
    // CHASSIS MOTORS

    // Get percentage from controller axes
    double leftAxisVertPercent  = (double)(Controller1.Axis3.position(percent));
    double rightAxisVertPercent = (double)(Controller1.Axis2.position(percent));

    // double leftAxisHorizPercent  = (double)(Controller1.Axis4.position(percent));
    // double rightAxisHorizPercent = (double)(Controller1.Axis1.position(percent));

    // Left and right motors
    if(std::abs(leftAxisVertPercent) > THRESHOLD || std::abs(rightAxisVertPercent) > THRESHOLD){
      // Move left and right motors to the corresponding controller axis
      LeftMotor .spin(forward, leftAxisVertPercent, percent);
      RightMotor.spin(forward, rightAxisVertPercent, percent);
    } else {
      // If no input is detected, stop motors
      LeftMotor .stop();
      RightMotor.stop();
    }

    /**
     * Detect if we want to spin the center motor
     * (only do if we are going straight-ish)
     * 
     * Steps:
     * 1. Make sure we don't divide by 0 by making sure the motors are above the
     *    THRESHOLD to save checking later
     * 2. Check the sign of controller axes division to see if it is positive.
     *    They should give a positive value with division if they are the same sign
     */
    if(
      std::abs(leftAxisVertPercent) > THRESHOLD && std::abs(rightAxisVertPercent) > THRESHOLD && 
      sign(leftAxisVertPercent / rightAxisVertPercent) == 1
    ){
      // Set velocity to the corresponding axis of the controller
      CenterMotor.spin(forward, (leftAxisVertPercent + rightAxisVertPercent) / 2, percent);
    } else {
      // If no input is detected, stop motors
      CenterMotor.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // ARM MECHANISM

    // Move arm motors depending on button presses
    if(Controller1.ButtonR1.pressing()){
      //Move motors up if up button is pressed
      LeftArm .spin(forward, ARMSPEED, percent);
      RightArm.spin(forward, ARMSPEED, percent);
    } else if(Controller1.ButtonL1.pressing()){
      //Move motors down if down button is pressed
      LeftArm .spin(reverse, ARMSPEED, percent);
      RightArm.spin(reverse, ARMSPEED, percent);
    } else {
      //Stop motors
      LeftArm .stop();
      RightArm.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // INDIVIDUAL LEVER MECHANISMS

    // Move left lever motor depending on button presses
    if(Controller1.ButtonL2.pressing() && !leftLeverIsUp){
      leftLeverIsUp = true;
      LeftLever.spinFor(forward, LEVERROTATION, degrees);
    } else if(!Controller1.ButtonL2.pressing() && leftLeverIsUp){
      leftLeverIsUp = false;
      LeftLever.spinFor(reverse, LEVERROTATION, degrees);
    }

    // Move right lever motor depending on button presses
    if(Controller1.ButtonR2.pressing() && !rightLeverIsUp){
      rightLeverIsUp = true;
      RightLever.spinFor(forward, LEVERROTATION, degrees);
    } else if(!Controller1.ButtonR2.pressing() && rightLeverIsUp){
      rightLeverIsUp = false;
      RightLever.spinFor(reverse, LEVERROTATION, degrees);
    }

    ///////////////////////////////////////////////////////////////////////////
    // CONTROLLER TESTS/EXTRAS
    if(!buttonIsPressed){
      buttonIsPressed = true;
      if(Controller1.ButtonA.pressing()){
        // Test auton
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(0, 0);
        Controller1.Screen.print("Testing auton: ");
        Controller1.Screen.print(AUTON);
        autonomous();
      } else if(Controller1.ButtonX.pressing()){
        // Displays if the levers are up or down
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(0, 0);
        Controller1.Screen.print("Left lever: ");
        Controller1.Screen.print(leftLeverIsUp ? "up" : "down");
        Controller1.Screen.setCursor(1, 0);
        Controller1.Screen.print("Right lever: ");
        Controller1.Screen.print(rightLeverIsUp ? "up" : "down");
      // } else if(Controller1.ButtonY.pressing()){
        // 
      // } else if(Controller1.ButtonB.pressing()){
        // 
      } else {
        buttonIsPressed = false;
      }
      
    } else if(
      buttonIsPressed &&
      !Controller1.ButtonA.pressing() &&
      !Controller1.ButtonX.pressing()// &&
      // !Controller1.ButtonY.pressing() &&
      // !Controller1.ButtonB.pressing()
    ){
      buttonIsPressed = false;
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(0, 0);
      Controller1.Screen.print("A - Test auton \n");
      Controller1.Screen.setCursor(1, 0);
      Controller1.Screen.print("X - Display lever status \n");
      // Controller1.Screen.print("Y -  \n");
      // Controller1.Screen.print("B - ");
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
