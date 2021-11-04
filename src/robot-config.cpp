#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor FrontLeftMotor = motor(PORT11, ratio18_1, false);
motor FrontRightMotor = motor(PORT12, ratio18_1, true);
motor BackLeftMotor = motor(PORT13, ratio18_1, false);
motor BackRightMotor = motor(PORT14, ratio18_1, true);
motor LeftArm = motor(PORT15, ratio18_1, false);
motor RightArm = motor(PORT16, ratio18_1, true);
motor Forklift = motor(PORT17, ratio18_1, false);
motor Claw = motor(PORT18, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}