#include <Wire.h>
#include "String.h"
#include "ds3231.h"

uint8_t ds3231::read8(uint8_t address)
{
	Wire.beginTransmission(devid);
	Wire.write(address);
	Wire.endTransmission();
	Wire.requestFrom(devid,1);
	return Wire.read();
}

uint8_t ds3231::write8(uint8_t address, uint8_t data)
{
	Wire.beginTransmission(devid);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
	return 0;
}

uint8_t ds3231::init()
{
	//Wire.begin();       // ***NOTE: Uncomment this line if the RTC is the only I2C device in use. Else, call Wire.begin() in the setup function***
	// ***NOTE: It is always advisable to intialise the I2C bus (by calling Wire.begin()) in the setup function***
	write8(control,0x04);
	set_year(0x19);
	set_month(0x07);
	set_date(0x02);
	set_day(tuesday);
	set_hours(0,0x20);
	write8(minutes,0x59);
	write8(seconds,0x55);
	return 0;
}

uint8_t ds3231::get_seconds()
{
	uint8_t s=read8(seconds);
	uint8_t hn=(s&0xF0)>>4;
	uint8_t ln=s&0x0F;
	return ((hn*10)+ln);
}

uint8_t ds3231::get_minutes()
{
	uint8_t m=read8(minutes);
	uint8_t hn=(m&0xF0)>>4;
	uint8_t ln=m&0x0F;
	return ((hn*10)+ln);
}

uint8_t ds3231::get_hours()
{
	uint8_t h=read8(hours);
	uint8_t flag=h&0x40;
	if(flag==0)
	{
		uint8_t hn=(h&0x30)>>4;
		uint8_t ln=h&0x0F;
		return ((hn*10)+ln);
	}
	else
	{
		uint8_t hn=(h&0x10)>>4;
		uint8_t ln=h&0x0F;
		return ((hn*10)+ln);
	}
}

uint8_t ds3231::get_day()
{
	return read8(day);
}

uint8_t ds3231::get_date()
{
	uint8_t dt=read8(date);
	uint8_t hn=(dt&0xF0)>>4;
	uint8_t ln=dt&0x0F;
	return ((hn*10)+ln);
}

uint8_t ds3231::get_month()
{
	uint8_t mon=read8(month);
	uint8_t hn=(mon&0x10)>>4;
	uint8_t ln=mon&0x0F;
	return ((hn*10)+ln);
}

uint8_t ds3231::get_year()
{
	uint8_t y=read8(year);
	uint8_t hn=(y&0xF0)>>4;
	uint8_t ln=y&0x0F;
	return ((hn*10)+ln);
}

float ds3231::get_temperature()
{
	uint8_t t=read8(control);
	write8(control,(t|0x20));
	int8_t th=read8(temperature_h);
	uint8_t tl=read8(temperature_l)>>6;
	return th+(0.25*tl);
}

String ds3231::get_day_date_time_str()
{
	String str="";
	str+=String(get_date());
	str+="/";
	str+=String(get_month());
	str+="/";
	str+=String(get_year());
	str+="  ";
	str+=String(get_hours());
	str+=":";
	str+=String(get_minutes());
	str+=":";
	str+=String(get_seconds());
	return str;
}

uint8_t ds3231::set_seconds(uint8_t sec)
{
	write8(seconds,sec);
	return 0;
}

uint8_t ds3231::set_minutes(uint8_t min)
{
	write8(minutes,min);
	return 0;
}

uint8_t ds3231::set_hours(uint8_t mode, uint8_t hrs)
{
	if(mode==0)
	{
		write8(hours,(hrs&0x3F));
	}
	else if(mode==1)
	{
		write8(hours,((hrs&0x1F)|0x40));
	}
	else
	{
		write8(hours,((hrs&0x1F)|0x60));
	}
	return 0;
}

uint8_t ds3231::set_day(enum Day d)
{
	write8(day,(d&0x07));
	return 0;
}

uint8_t ds3231::set_date(uint8_t d)
{
	write8(date,(d&0x3F));
	return 0;
}

uint8_t ds3231::set_month(uint8_t m)
{
	write8(month,(m&0x1F));
	return 0;
}

uint8_t ds3231::set_year(uint8_t y)
{
	write8(year,y);
	return 0;
}

uint8_t ds3231::set_alarm1(enum alarm1_type at, uint8_t sec, uint8_t min, uint8_t hrs, uint8_t dy_dt, uint8_t flag, uint8_t am_pm)
{
	switch(at)
	{
		case every_second: sec|=0x80;
		min|=0x80;
		hrs|=0x80;
		dy_dt|=0x80;
		break;
		case seconds_match: sec&=0x7F;
		min|=0x80;
		hrs|=0x80;
		dy_dt|=0x80;
		break;
		case min_sec_match: sec&=0x7F;
		min&=0x7F;
		hrs|=0x80;
		dy_dt|=0x80;
		break;
		case hr_min_sec_match: sec&=0x7F;
		min&=0x7F;
		hrs&=0x7F;
		dy_dt|=0x80;
		break;
		case date_hr_min_sec_match: sec&=0x7F;
		min&=0x7F;
		hrs&=0x7F;
		dy_dt&=0x3F;
		break;
		case day_hr_min_sec_match: sec&=0x7F;
		min&=0x7F;
		hrs&=0x7F;
		dy_dt&=0x7F;
		break;
	}
	write8(a1_sec,sec);
	write8(a1_min,min);
	if(flag==1)
	{
	  if(am_pm==0)
	  {
		  write8(a1_hrs,(hrs|0x40));
	  }
	  else
	  {
		  write8(a1_hrs,(hrs|0x60));
	  }
	}
	else
	{
	  write8(a1_hrs,hrs);
	}
	write8(a1_day,dy_dt);
	return 0;
}

uint8_t ds3231::set_alarm2(enum alarm2_type at, uint8_t min, uint8_t hrs, uint8_t dy_dt, uint8_t flag, uint8_t am_pm)
{
	switch(at)
	{
		case every_minute: min|0x80;
		hrs|=0x80;
		dy_dt|=0x80;
		break;
		case minutes_match: min&0x7F;
		hrs|=0x80;
		dy_dt|=0x80;
		break;
		case hr_min_match: min&0x7F;
		hrs&=0x7F;
		dy_dt|=0x80;
		break;
		case date_hr_min_match: min&0x7F;
		hrs&=0x7F;
		dy_dt&=0x3F;
		break;
		case day_hr_min_match: min&0x7F;
		hrs&=0x7F;
		dy_dt&=0x7F;
		break;
	}
	write8(a2_min,min);
	if (flag==1)
	{
		if (am_pm==0)
		{
			write8(a2_hrs,(hrs|0x40));
		}
		else
		{
			write8(a2_hrs,(hrs|0x60));
		}
	}
	else
	{
		write8(a2_hrs,hrs);
	}
	write8(a2_day,dy_dt);
	return 0;
}

uint8_t ds3231::arm_alarm1()
{
	uint8_t t=read8(control);
	write8(control,(t|0x05));
	return 0;
}

uint8_t ds3231::disarm_alarm1()
{
	uint8_t t=read8(control);
	write8(control,(t&0xFE));
	return 0;
}

uint8_t ds3231::clear_alarm1()
{
	uint8_t t=read8(stat);
	write8(stat,(t&0xFE));
	return 0;
}

uint8_t ds3231::arm_alarm2()
{
	uint8_t t=read8(control);
	write8(control,(t|0x06));
	return 0;
}

uint8_t ds3231::disarm_alarm2()
{
	uint8_t t=read8(control);
	write8(control,(t&0xFD));
	return 0;
}

uint8_t ds3231::clear_alarm2()
{
	uint8_t t=read8(stat);
	write8(stat,(t&0xFD));
	return 0;
}

uint8_t ds3231::enable_sq_wave(enum freq f)
{
	uint8_t t=read8(control);
	t&=0xFB;
	switch(f)
	{
		case hz1: write8(control,(t&0xE7));
		break;
		case hz1024: write8(control,((t&0xEF)|0x08));
		break;
		case hz4096: write8(control,((t&0xFE)|0x10));
		break;
		case hz8192: write8(control,(t|0x18));
		break;
	}
	return 0;
}

uint8_t ds3231::disable_sq_wave()
{
	uint8_t t=read8(control);
	write8(control,(t|0x40));
	return 0;
}
