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
// FrontLeftMotor       motor         11              
// FrontRightMotor      motor         12              
// BackLeftMotor        motor         13              
// BackRightMotor       motor         14              
// LeftArm              motor         15              
// RightArm             motor         16              
// Forklift             motor         17              
// Claw                 motor         18              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"
#include "nerd.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

///////////////////////////////////////////////////////////////////////////
#define AUTON 1 // set auton here
///////////////////////////////////////////////////////////////////////////

// sleep alias
const auto& sleep = vex::task::sleep;

// important constants
const double THRESHOLD = 20;
const double ARMSPEED = 100;
const double CLAWSPEED = 75;
const double FORKLIFTSPEED = 100;
const double FORKLIFTRATIO = 11;
const double FORKLIFTDOWN = 90 * FORKLIFTRATIO;
const double FORKLIFTUP = 45 * FORKLIFTRATIO;

///////////////////////////////////////////////////////////////////////////
// AUTON FUNCTIONS

/**
 * Moves the chassis motors at the same time
 */
void robot_drive(double amount){
  FrontLeftMotor .spin(forward, amount, percent);
  FrontRightMotor.spin(forward, amount, percent);
  BackLeftMotor .spin(forward, amount, percent);
  BackRightMotor.spin(forward, amount, percent);
}

/**
 * Moves the chassis motors so that the robot turns left or right.
 * Positive values mean right.
 */
void robot_turn(double amount){
  FrontLeftMotor .spin(reverse, amount, percent);
  FrontRightMotor.spin(forward, amount, percent);
  BackLeftMotor .spin(reverse, amount, percent);
  BackRightMotor.spin(forward, amount, percent);
}

/**
 * Stops the chassis motors
 */
void robot_stop(){
  FrontLeftMotor .stop();
  FrontRightMotor.stop();
  BackLeftMotor .stop();
  BackRightMotor.stop();
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

  // Put logo on screen
  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(color(255, 32, 32));
  for(int y = 0; y < sizeof(NERD); y++){
    for(int x = 0; x < sizeof(NERD[y]); x++){
      if(NERD[y][x]) Brain.Screen.drawPixel(x, y);
    }
  }

  // Set motor speed
  Forklift.setVelocity(FORKLIFTSPEED, percent);
  Forklift.resetPosition();
  Forklift.resetRotation();
  Claw.setVelocity(CLAWSPEED, percent);
}

void autonomous(void) {
  // Open claw and move it down
  Claw.spin(forward, CLAWSPEED, percent);
  Forklift.rotateTo(FORKLIFTDOWN, degrees, false);
  sleep(250);
  Claw.stop();
  sleep(750);

  // Run different auton
  switch(AUTON){
    default:
    case 0:
      /**
       * Drives forward for a little bit and stops.
       * Hopefully you were aiming at a goal because it tries to pick it up.
       */
      robot_drive(100);
      sleep(1500);
      robot_stop();
      Claw.spin(reverse);
      sleep(500);
      Claw.stop();
      arm_move(ARMSPEED);
      sleep(500);
      arm_stop();
      robot_drive(-60);
      sleep(1000);
      robot_stop();
      break;
    case 1:
      /**
       * Drives backwards to get the mobile goal, picks it up, and drives back
       */
      robot_drive(-100);
      sleep(1500);
      robot_stop();
      Forklift.rotateTo(FORKLIFTUP, degrees, false);
      sleep(500);
      robot_drive(100);
      sleep(1250);
      robot_stop();
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

  bool forkliftDown = Forklift.rotation(degrees) > 67.5 * FORKLIFTRATIO;//false
  bool buttonIsPressed = true;
  Controller1.Screen.clearScreen();

  while(true){
    ///////////////////////////////////////////////////////////////////////////
    // CHASSIS MOTORS

    // Get percentage from controller axes
    double leftAxisVertPercent  = (double)(Controller1.Axis3.position(percent));
    double rightAxisVertPercent = (double)(Controller1.Axis2.position(percent));

    // double leftAxisHorizPercent  = (double)(Controller1.Axis4.position(percent));
    // double rightAxisHorizPercent = (double)(Controller1.Axis1.position(percent));

    // Chassis motors
    if(std::abs(leftAxisVertPercent) > THRESHOLD || std::abs(rightAxisVertPercent) > THRESHOLD){
      // Move left and right motors to the corresponding controller axis
      FrontLeftMotor .spin(forward, leftAxisVertPercent, percent);
      FrontRightMotor.spin(forward, rightAxisVertPercent, percent);
      BackLeftMotor .spin(forward, leftAxisVertPercent, percent);
      BackRightMotor.spin(forward, rightAxisVertPercent, percent);
    } else {
      // If no input is detected, stop motors
      FrontLeftMotor .stop();
      FrontRightMotor.stop();
      BackLeftMotor .stop();
      BackRightMotor.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // ARM MECHANISM

    // Move arm motors depending on button presses
    if(Controller1.ButtonR1.pressing()){
      //Move motors up if up button is pressed
      LeftArm .spin(forward, ARMSPEED, percent);
      RightArm.spin(forward, ARMSPEED, percent);
    } else if(Controller1.ButtonR2.pressing()){
      //Move motors down if down button is pressed
      LeftArm .spin(reverse, ARMSPEED, percent);
      RightArm.spin(reverse, ARMSPEED, percent);
    } else {
      //Stop motors
      LeftArm .stop();
      RightArm.stop();
    }

    ///////////////////////////////////////////////////////////////////////////
    // "END EFFECTOR" MECHANISMS
    // These are simple so I'm combining them

    if(Controller1.ButtonL2.pressing()){
      Claw.spin(forward);
    } else if(Controller1.ButtonL1.pressing()){
      Claw.spin(reverse);
    }

    if(Controller1.ButtonUp.pressing() && forkliftDown){
      Forklift.rotateTo(FORKLIFTUP, degrees, false);
      forkliftDown = false;
    } else if(Controller1.ButtonDown.pressing() && !forkliftDown){
      Forklift.rotateTo(FORKLIFTDOWN, degrees, false);
      forkliftDown = true;
    }

    ///////////////////////////////////////////////////////////////////////////
    // CONTROLLER TESTS/EXTRAS
    if(!buttonIsPressed){
      buttonIsPressed = true;
      if(Controller1.ButtonA.pressing()){
        // Test auton
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print("Testing auton: ");
        Controller1.Screen.print(AUTON);
        pre_auton();
        autonomous();
      // } else if(Controller1.ButtonX.pressing()){
        // 
      } else if(Controller1.ButtonY.pressing()){
        // System diagnostics
        // Robot battery
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        if(Brain.Battery.capacity(percent) < 25)
          Controller1.Screen.print("Battery: < 25%");
        else if(Brain.Battery.capacity(percent) < 50)
          Controller1.Screen.print("Battery: < 50%");
        else if(Brain.Battery.capacity(percent) < 75)
          Controller1.Screen.print("Battery: < 75%");
        else
          Controller1.Screen.print("Battery: good");
        // Controller1.Screen.newLine();
      } else {
        buttonIsPressed = false;
      }
      
    } else if(
      buttonIsPressed &&
      !Controller1.ButtonA.pressing() &&
      !Controller1.ButtonX.pressing() &&
      !Controller1.ButtonY.pressing()
    ){
      buttonIsPressed = false;
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("A - Test auton");
      // Controller1.Screen.newLine();
      // Controller1.Screen.print("X - Lever status");
      Controller1.Screen.newLine();
      Controller1.Screen.print("Y - Diagnostics");
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
