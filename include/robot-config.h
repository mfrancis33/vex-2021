using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor FrontLeftMotor;
extern motor FrontRightMotor;
extern motor BackLeftMotor;
extern motor BackRightMotor;
extern motor LeftElevator;
extern motor RightElevator;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );