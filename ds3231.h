#ifndef _DS3231_
#define _DS3231_

// device address
#define devid 0x68

// control registers
#define control 0x0E
#define stat 0x0F
#define ageoffset 0x10

// data registers
#define seconds 0x00
#define minutes 0x01
#define hours 0x02
#define day 0x03
#define date 0x04
#define month 0x05
#define year 0x06
#define temperature_h 0x11
#define temperature_l 0x12

/*
The required data words for setting of the alarms are described on Page No.12 of the datasheet
*/
// alarm 1 registers
#define a1_sec 0x07
#define a1_min 0x08
#define a1_hrs 0x09
#define a1_day 0x0A

// alarm 2 registers
#define a2_min 0x0B
#define a2_hrs 0x0C
#define a2_day 0x0D

enum Day:uint8_t            // Day of the Week
{
	monday=0x01,
	tuesday=0x02,
	wednesday=0x03,
	thursday=0x04,
	friday=0x05,
	saturday=0x06,
	sunday=0x07
};

enum freq:uint8_t          // Square Wave frequency selection
{
	hz1,      // 1 Hz
	hz1024,   // 1024 Hz
	hz4096,   // 4096 Hz
	hz8192    // 8192 Hz
};

enum alarm1_type:uint8_t    // Alarm 1 type
{
	every_second,
	seconds_match,
	min_sec_match,
	hr_min_sec_match,
	date_hr_min_sec_match,
	day_hr_min_sec_match
};

enum alarm2_type:uint8_t    // Alarm 2 type
{
	every_minute,
	minutes_match,
	hr_min_match,
	date_hr_min_match,
	day_hr_min_match
};

class ds3231
{
private:
	/* 
	   Private Low-level functions.
	   Used to pass data from user-level to bare metal level.
	*/
	uint8_t read8(uint8_t address);
	uint8_t write8(uint8_t address, uint8_t data);
public:
	/* 
	   Initialisation Function.
	   Function to initialise the RTC registers to the desired values.
	*/
	uint8_t init();
	/* 
	   getters
	   The data returned by the getters is preconverted to decimal format.
	   Attempts to convert this data may lead to garbage results. Therefore external conversion attempts MUST NOT be made.
	*/
	uint8_t get_seconds();
	uint8_t get_minutes();
	uint8_t get_hours();
	uint8_t get_day();
	uint8_t get_date();
	uint8_t get_month();
	uint8_t get_year();
	float get_temperature();
	String get_day_date_time_str();
	/* 
	   data setters
	   The data for the setters is to be supplied in BCD format.
	   eg. to set the seconds register to read 12 seconds, 0x12 must be supplied as an argument to the corresponding setter.
	*/
	uint8_t set_seconds(uint8_t);
	uint8_t set_minutes(uint8_t);
	uint8_t set_hours(uint8_t, uint8_t);  // mode is 0 for 24 hr format, 1 for AM and 2 for PM in 12 hr format
	uint8_t set_day(enum Day);
	uint8_t set_date(uint8_t);
	uint8_t set_month(uint8_t);
	uint8_t set_year(uint8_t);
	/* 
	   alarm setters
	   Functions to set alarms 1 and 2 respectively. The alarm_type arguement must be chosen from the corresponding enumerations listed above.
	   Alarm 1:
				The parameters to be supplied to the function set_alarm1 are (in the same order):
				alarm type(at) : type of desired alarm. MUST be of type alarm1_type.
				second(sec) : second at which alarm is to be initiated. Defaults to 0.
				minute(min) : minute at which alarm is to be initiated. Defaults to 0.
				hour(hrs) : hour at which alarm is to be initiated. Defaults to 00 in 24 hour format.
				day : day on which alarm is to be initiated. Defaults to 1 (Monday).
				date : date on which alarm is to be initiated. Defaults to 1.
				***NOTE : the day and date parameters are multiplexed into a single variable 'dy_dt'.
				The interpretation of this variable as day or date depends on the value of the 'alarm type' variable.***
				flag : 0 for 24 hour format, 1 for 12 hour format. Defaults to 0.
				am_pm : 0 for AM, 1 for PM. Deaults to 0.
				***NOTE : This variable is considered only in 12 hour format***
	*/
	uint8_t set_alarm1(enum alarm1_type at, uint8_t sec=0x00 , uint8_t min=0x00, uint8_t hrs=0x12, uint8_t dy_dt=0x01, uint8_t flag=0, uint8_t am_pm=0);

	/*
	   Alarm 2:
				The parameters to be supplied to the function set_alarm2 are (in the same order):
				alarm type(at)
				minute : minute at which alarm is to be initiated. Defaults to 0.
				hour : hour at which alarm is to be initiated. Defaults to 00 in 24 hour format.
				date : date on which alarm is to be initiated. Defaults to 1.
				day : day on which alarm is to be initiated. Defaults to 1 (Monday).
				***NOTE : the day and date parameters are multiplexed into a single variable 'dy_dt'.
				The interpretation of this variable as day or date depends on the value of the 'alarm type' variable.***
				flag : 0 for 24 hour format, 1 for 12 hour format. Defaults to 0.
				am_pm : 0 for AM, 1 for PM. Deaults to 0.
				***NOTE : This variable is considered only in 12 hour format***
	*/
	uint8_t set_alarm2(enum alarm2_type at, uint8_t min=0x00, uint8_t hrs=0x12, uint8_t dy_dt=0x01, uint8_t flag=0, uint8_t am_pm=0);
	/*
	   Arming, disarming, and clear functions for the DS3231's alarms.
	   ***NOTE : Not using the respective clear function after an alarm is triggered may lead to malfunctions or spurious interrupts.
	   Care must be taken to prevent such an occurence.***
	*/
	uint8_t arm_alarm1();
	uint8_t disarm_alarm1();
	uint8_t clear_alarm1();
	uint8_t arm_alarm2();
	uint8_t disarm_alarm2();
	uint8_t clear_alarm2();
	/*
		Functions for enabling and disabling fixed frequency square waves.
		***NOTE: enabling these square waves will disable all alarms, since the Alarm Interrupt and Square wave pins are multiplexed***
	*/
	uint8_t enable_sq_wave(enum freq f);
	uint8_t disable_sq_wave();
};

#endif
