/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Shoot                motor         1               
// Intake               motor         2               
// Controller1          controller                    
// ChainRelease         motor         3               
// LeftMotor            motor         4               
// RightMotor           motor         5               
// ---- END VEXCODE CONFIGURED DEVICES ----
#include <iostream>
#include "vex.h"
#include <cmath>
#include <sstream>
#include <string>
using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
//Flywheel PID controls
motor_group Flywheel = motor_group(Fly1, Fly2);
bool maintainSpeed = false;
double targetspeed = 600;
double kp = 0.5;
double ki = 0.10;
double kd = 0.05;
double preverror = 0.0;
double error = 0.0;
double totalError = 0.0; // += error // = error-preverror
double flyspeed;
double Power = 0;
bool ReadyShoot = false;
double speed;
double torque1;
double curr;
double volts;
double flykP = 0.5, flykI = 0.10, flykD = 0.05;
double flyError = 0.0, flyPrevError = 0.0, flyTotalError = 0.0; // += error
double flyDerivative = 0.0;
//derivative = error - preverror



bool readyShoot = false;
int recData(){
  while (true){
    speed= (Fly1.velocity(rpm) + Fly2.velocity(rpm))/2; 
    torque1= (Fly1.torque(Nm) + Fly2.torque(Nm))/2; 
    curr = (Fly1.current() + Fly2.current())/2; 
    volts = (Fly1.voltage() + Fly2.voltage())/2;
    std::cout << Brain.Timer.value() << " , " 
  << speed << " , " <<  torque1 << " , " 
  << curr << " , " << volts
  << std::endl;
  }
  return 1;
}
int FlyPID(){
  while(maintainSpeed){
    flyspeed = (Fly1.velocity(rpm) + Fly2.velocity(rpm)) / 2; 
    flyError = targetspeed - flyspeed;
    if (flyError <= 0.1) {
      readyShoot = true;
    }
    else {
      readyShoot = false;
    }
    Power += (flyError*flykP + flyTotalError * flykI + (flyError - flyPrevError) * flykD) / 12;
    Flywheel.spin(vex::directionType::fwd, Power, volt);
    flyPrevError = flyError;
    flyTotalError += flyError;
    recData();
    vex::task::sleep(20);

  }
  Flywheel.stop(vex::brakeType::coast);
  return 1;
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {

  vex::task go(FlyPID);  
  double discPos[20];
  double target[2];
  //move backwards, align with roller
  //turn left until reach target 

  //PLACEHOLDERS
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
//std::ostringstream ss;
void usercontrol(void) {
  vex::task go(FlyPID);
  // User control code here, inside the loop
  while (1) {


    // Shoot and Flywheel control
    
    if (Controller1.ButtonR1.pressing()){
      maintainSpeed = true;
      
     // ss << error;

     //Brain.Screen.print(ss.str());

      //ss << 
      //std::cout << ss.str() << std::endl;
      if (ReadyShoot){
        Controller1.rumble(rumbleShort);
        if (Controller1.ButtonR1.pressing()){
          Indexer.spinFor(vex::directionType::fwd, 1, seconds);
          Indexer.spinFor(reverse, 1, seconds);
        }
      }
    }
    Brain.Screen.print(targetspeed);
    if (Controller1.ButtonX.pressing()){
      Flywheel.spinFor(vex::directionType::fwd, 1, seconds);
      Brain.Screen.print(error);
    }
    /* Just in Case

    */
    // Intake & Roller

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
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