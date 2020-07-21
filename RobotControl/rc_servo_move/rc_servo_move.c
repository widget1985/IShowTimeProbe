
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h> // for atoi
#include <signal.h>
#include <rc/time.h>
#include <rc/adc.h>
#include <rc/dsm.h>
#include <rc/servo.h>

static int running = 1;
static long running_counter = 0;
// interrupt handler to catch ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy)
{
        running=0;
        return;
}

int main(int argc, char *argv[])
{
    double servo_pos=0;
    int frequency_hz = 50; // default 50hz frequency to send pulses
    
    servo_pos = atof(argv[1]);

    if(servo_pos>1.5 || servo_pos<-1.5){
            fprintf(stderr,"Servo position must be from -1.5 to 1.5\n");
            return -1;
    }
    // read adc to make sure battery is connected
    if(rc_adc_init()){
            fprintf(stderr,"ERROR: failed to run rc_adc_init()\n");
            return -1;
    }
    if(rc_adc_batt()<6.0){
            fprintf(stderr,"ERROR: battery disconnected or insufficiently charged to drive servos\n");
            return -1;
    }
    rc_adc_cleanup();
    // initialize PRU
    if(rc_servo_init()) return -1;

    // turn on power
    //printf("Turning On 6V Servo Power Rail\n");
    rc_servo_power_rail_en(1);
    while(running){
        if(rc_servo_send_pulse_normalized(1, servo_pos)==-1) return -1;
        // sleep roughly enough to maintain frequency_hz
        rc_usleep(1000000/frequency_hz);
        running_counter++;
        if(running_counter > 30){
            running = 0;
        }
    }

    printf("servo shoulda moved\n");
    rc_usleep(50000);
     // turn off power rail and cleanup
    rc_servo_power_rail_en(0);
    rc_servo_cleanup();
    return 0;
}