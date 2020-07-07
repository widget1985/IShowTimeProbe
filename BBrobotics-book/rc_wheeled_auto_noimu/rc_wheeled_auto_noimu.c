/**
 * @file rc_test_motors.c
 * @example    rc_test_motors
 *
 * Demonstrates use of H-bridges to drive motors with the Robotics Cape and
 * BeagleBone Blue. Instructions are printed to the screen when called.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h> // for atoi
#include <getopt.h>
#include <rc/motor.h>
#include <rc/time.h>
#include <rc/led.h>

int freq_hz = RC_MOTOR_DEFAULT_PWM_FREQ;

double angle;
int distance;
int turn;

#define MOTOR_FR 3
#define MOTOR_FL 2
#define MOTOR_BR 4
#define MOTOR_BL 1

//-1 to 1
void motors_set_forward(float str){
    rc_motor_set(MOTOR_FR, str);
    rc_motor_set(MOTOR_FL, -str);
    rc_motor_set(MOTOR_BR, str);
    rc_motor_set(MOTOR_BL, -str);
}

// -1 to 1
void motors_set_rotate(float str){
    rc_motor_set(MOTOR_FR, str);
    rc_motor_set(MOTOR_FL, str);
    rc_motor_set(MOTOR_BR, str);
    rc_motor_set(MOTOR_BL, str);
}

int main(int argc, char** argv){

    angle = atof(argv[1]);
    if (angle > 0)
        turn = 1;
    else
        turn = 0;

    distance = atoi(argv[2]);

    printf("\r");
    printf("Angle = %6.1f\n",angle);
    printf("Distance = %2d\n",distance);
    fflush(stdout);

    // initialize hardware first
    if(rc_motor_init_freq(freq_hz)) return -1;
    
    rc_led_set(RC_LED_GREEN,1);
    rc_led_set(RC_LED_RED,1);
    
    //free spin all motors?
    rc_motor_free_spin(0);
    printf("Motors are now ready.\n");

    float movement = 0;
    while (movement < abs(angle))
    {
      if(turn){
        motors_set_rotate(0.4);
      }else{
        motors_set_rotate(-0.4);
      }
      rc_usleep	(25000);
      movement+=1;
      printf("\r");
      printf("Angle = %6.1f\n",movement);
      fflush(stdout);
    }
    
    motors_set_forward(0);
    rc_usleep (10000);
    
    movement = 0;
    while (movement < distance)
    {
      motors_set_forward(0.4);
      rc_usleep	(100000);
      movement++;
      printf("\r");
      printf("Distance = %6.1f\n",movement);
      fflush(stdout);
    }
    
    motors_set_forward(0);    
        
    rc_motor_cleanup();
    rc_led_cleanup();
}
