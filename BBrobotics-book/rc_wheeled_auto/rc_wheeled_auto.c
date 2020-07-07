/******************************************************************************
* rc_wheeled_auto.c
*
* This is a program that uses the compass to turn the wheeled
* platform and then go a certain distance.
******************************************************************************/

#include <rc_usefulincludes.h>
#include <roboticscape.h>

//struct to hold new data
rc_imu_data_t data;
void process_data();
double angle;
int distance;
int turn;

/******************************************************************************
* int main()
*
* This main function contains these critical components
* - call to initialize_cape
* - set up the compass
* - initiate the turn
* - after it comes back - go a certain distance
* - cleanup_roboticscape() at the end
*****************************************************************************/
int main(int argc, char** argv){
  // always initialize cape library first
  rc_initialize();

  printf("\nHello BeagleBone\n");

  angle = atof(argv[1]);
  if (angle > 0)
    turn = 1;
  else
    turn = 0;

  distance = atoi(argv[2]);

  // done initializing so set state to RUNNING
  rc_set_state(RUNNING);

  // bring H-bridges of of standby
  rc_enable_motors();
  rc_set_led(GREEN,ON);
  rc_set_led(RED,ON);

  rc_set_motor_free_spin(1);
  rc_set_motor_free_spin(2);
  printf("Motors are now ready.\n");
  // start with default config and modify based on options
  rc_imu_config_t conf = rc_default_imu_config();
  conf.dmp_sample_rate = 20;
  conf.enable_magnetometer = 1;
  // now set up the imu for dmp interrupt operation
  if(rc_initialize_imu_dmp(&data, conf)){
    printf("rc_initialize_imu_failed\n");
    return -1;
  }
  rc_set_imu_interrupt_func(&process_data);
  // set the unit turning
  if (turn)
    {
      rc_set_motor(1, 0.2);
      rc_set_motor(2, -0.2);
    }
  else
    {
      rc_set_motor(1, -0.2);
      rc_set_motor(2, 0.2);
    }
  //now just wait, print_data() will be called by the interrupt
  while (rc_get_state()!=EXITING) {
    usleep(10000);
  }
  int movement = 0;
  // Now move forward
  while (movement < distance)
    {
      rc_set_motor(1, 0.2);
      rc_set_motor(2, 0.2);
      usleep(1000000);
      movement++;
    }

  rc_set_motor_brake_all();

  // shut things down
  rc_power_off_imu();
  rc_cleanup();
  return 0;
}
/******************************************************************************
 * int process_data()
 *
 * - Called each time the compass interrupts
 * - Compares angles to see if the platform has moved enough
 * - If it has, stop the platform
 *****************************************************************************/
void process_data() // imu interrupt function
{
  printf("\r");
  printf(" ");
  printf("Angle = %6.1f\n",angle);
  printf("Distance = %2d\n",distance);
  printf("   %6.1f   |", data.compass_heading_raw*RAD_TO_DEG);
  printf("   %6.1f   |", data.compass_heading*RAD_TO_DEG);

  if (turn)
    {
    if ((angle - data.compass_heading*RAD_TO_DEG) < 1.0)
      {
        rc_set_motor_brake_all();
        rc_set_state(EXITING);
      }
    }
  else
    if ((-angle + data.compass_heading*RAD_TO_DEG) < 1.0)
      {
        rc_set_motor_brake_all();
        rc_set_state(EXITING);
      }
  fflush(stdout);

  return;
}

