
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h> // for atoi
#include <signal.h>
#include <rc/time.h>
#include <rc/led.h>

#define WAIT_US 500000 // time to light each LED in microseconds

static int running = 1;

// interrupt handler to catch ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy)
{
     //this doesnt seem to work, accept that we won't exit cleanly and LED might stay on :/
        printf("got crtl-c\n");
        running=0;
        return;
}

int main(int argc, char *argv[])
{
    while(running){
        rc_led_set(RC_LED_RED,1);
        rc_usleep(WAIT_US);
        rc_led_set(RC_LED_RED,0);
        rc_usleep(WAIT_US);
    }
    
    printf("cleaning\n");
    //turn off before exiting
    rc_led_set(RC_LED_RED,0);
    rc_led_cleanup();
    return 0;
}