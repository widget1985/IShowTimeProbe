
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> // for atoi
#include <getopt.h>
#include <rc/motor.h>
#include <rc/time.h>
#include <rc/led.h>

int freq_hz = RC_MOTOR_DEFAULT_PWM_FREQ;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

double rot;
double x_str;
double y_str;
long move_time;

double angle;
int distance;
int turn;

#define MOTOR_FR 3
#define MOTOR_FL 2
#define MOTOR_BR 4
#define MOTOR_BL 1

//-1 to 1
void motors_set_forward(float str){
    rc_motor_set(MOTOR_FR, -str);
    rc_motor_set(MOTOR_FL, str);
    rc_motor_set(MOTOR_BR, -str);
    rc_motor_set(MOTOR_BL, str);
}

// -1 to 1
void motors_set_rotate(float str){
    rc_motor_set(MOTOR_FR, str);
    rc_motor_set(MOTOR_FL, str);
    rc_motor_set(MOTOR_BR, str);
    rc_motor_set(MOTOR_BL, str);
}

// -1 to 1
void motors_set_strafe(float str){
    rc_motor_set(MOTOR_FR, -str);
    rc_motor_set(MOTOR_FL, -str);
    rc_motor_set(MOTOR_BR, str);
    rc_motor_set(MOTOR_BL, str);
}

void motors_set_motion(double x, double y, double rot){
    double FR = 0, FL = 0, BR = 0, BL = 0;
    
    FR += x;
    FL += x;
    BR += x;
    BL += x;
    
    FR -= y;
    FL -= y;
    BR += y;
    BL += y;
    
    FR += rot;
    FL -= rot;
    BR += rot;
    BL -= rot;
    
    double max = MAX(MAX(abs(FL), abs(FR)), MAX(abs(BL), abs(BR)));
    if (max > 1) {
        FR /= max;
        FL /= max;
        BR /= max;
        BL /= max;
    }
    
    rc_motor_set(MOTOR_FR, -FR);
    rc_motor_set(MOTOR_FL, FL);
    rc_motor_set(MOTOR_BR, -BR);
    rc_motor_set(MOTOR_BL, BL);
}

int main(int argc, char** argv){

//    if (argc != 4){
//        return -1;
//    }
    
    x_str = atof(argv[1]);
    y_str = atof(argv[2]);
    rot = atof(argv[3]);
    move_time = atof(argv[4]);
    
    printf("\r");
    printf("forward = %6.1f\n",x_str);
    printf("side = %6.1f\n",y_str);
    printf("turn = %6.1f\n",rot);
    fflush(stdout);
 
 
    // initialize hardware first
    if(rc_motor_init_freq(freq_hz)) return -1;
    
    rc_led_set(RC_LED_GREEN,1);
    rc_led_set(RC_LED_RED,1);
    
    //free spin all motors?
    rc_motor_free_spin(0);
    printf("Motors are now ready.\n");
    
    motors_set_motion(x_str, y_str, rot);
    rc_usleep(move_time*100000);
  
    motors_set_forward(0);    
    
    rc_motor_brake(0);
    
    rc_motor_cleanup();
    rc_led_cleanup();
}
