#ifndef Init_functions__
#define Init_functions__



#define PWM_MAXIMUM 5000
#define TIME_TO_TURN 125
// -----------------------------------------------------------------------------
// Public functions:
// -----------------------------------------------------------------------------

void turnOnBlueLED(void);
void turnOffBlueLED(void);
void init_ports(void);
void get_analog_val(int id);
void init_analog(void);
void set_timer4(void);
void set_timer1(void);


#endif