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
// LeftArm              motor         5               
// RightArm             motor         6               
// Forklift             motor         7               
// Claw                 motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"
#include "nerd.h"
#include "sstream"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

#define AUTON 0
const auto& sleep = vex::task::sleep; // no clue if this works

const double THRESHOLD = 20;
const double ARMSPEED = 100;

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
// https://stackoverflow.com/a/13636164
template <typename T> std::string to_string ( T Number ){
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

///////////////////////////////////////////////////////////////////////////
// IMPORTANT FUNCTIONS

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

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
  // Run difdferent auton
  switch(AUTON){
    default:
    case 0:
      /**
       * Drives forward for a little bit and stops.
       * Hopefully you were aiming at a goal because it tries to pick it up.
       * 
       */
      robot_drive(100);
      sleep(1500);
      robot_stop();
      arm_move(ARMSPEED);
      sleep(500);
      arm_stop();
      robot_drive(-60);
      sleep(1000);
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
    // CONTROLLER TESTS/EXTRAS
    if(!buttonIsPressed){
      buttonIsPressed = true;
      if(Controller1.ButtonA.pressing()){
        // Test auton
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print("Testing auton: ");
        Controller1.Screen.print(AUTON);
        autonomous();
      } else if(Controller1.ButtonX.pressing()){
        // Displays if the levers are up or down
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print("Left lever: ");
        Controller1.Screen.print(leftLeverIsUp ? "up" : "down");
        Controller1.Screen.newLine();
        Controller1.Screen.print("Right lever: ");
        Controller1.Screen.print(rightLeverIsUp ? "up" : "down");
      } else if(Controller1.ButtonY.pressing()){
        // System diagnostics
        // Robot battery
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print("Battery: " + to_string(Brain.Battery.capacity(percent)) + "%");
        Controller1.Screen.newLine();
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
      Controller1.Screen.newLine();
      Controller1.Screen.print("X - Lever status");
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
