#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK02F12810.h"
/* TODO: insert other include files here. */
#include "my_delay.h"
#include "rtc8564.h"
#include "my_i2c.h"

uint8_t BCD_INT(uint8_t num)
{
  return ((num / 10) << 4) + (num % 10);
}

uint8_t INT_BCD(uint8_t bcd)
{
  return (((bcd >> 4) * 10) + (bcd & 0x0f));
}

uint8_t *Get_Week_String(uint32_t week)
{
  static const char *WEEK[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

  return (uint8_t*) WEEK[week];
}

//=====================
// RTC Write Register
//=====================

void RTC8564_Write_Reg(uint32_t addr, uint32_t data)
{
  int write_count;
  uint8_t snd_data[2];

  snd_data[0] = (uint8_t)addr;
  snd_data[1] = (uint8_t)data;

  write_count = i2c_Send_Data_Request(I2C0, I2C2_DEV_S_OWN_ADDR, snd_data, 2);
  if (write_count != 2) while(1);
}
//====================
// RTC Read Register
//====================

uint32_t RTC8564_Read_Reg(uint32_t addr)
{
  uint8_t snd_data, rcv_data;

  snd_data = (uint8_t)addr;
  i2c_Send_Receive_Data_Request(I2C0, I2C2_DEV_S_OWN_ADDR, &snd_data, 1, &rcv_data, 1);

  return (uint32_t)rcv_data;
}

void RTC8564_Config_Request ( void )
{
  i2c_Config_Request();
}

//==================
// Initialize RTC
//==================
void RTC8564_Initialize_Request (uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
                                 uint8_t  week,   uint8_t hour, uint8_t minute, uint8_t second)
{
  // Initialize RTC
  if (do_adj) {
    	  //
    RTC8564_Write_Reg(RTC_CONTROL1, 0x20); // STOP
    RTC8564_Write_Reg(RTC_CONTROL2, 0x00);
        //
    RTC8564_Write_Reg(RTC_HOURS,   BCD_INT(hour));
    RTC8564_Write_Reg(RTC_MINUTES, BCD_INT(minute));
    RTC8564_Write_Reg(RTC_SECONDS, BCD_INT(second));
        //
    RTC8564_Write_Reg(RTC_YEARS,    BCD_INT(year));
    RTC8564_Write_Reg(RTC_C_MONTHS, BCD_INT(month));
    RTC8564_Write_Reg(RTC_DAYS,     BCD_INT(day));
    RTC8564_Write_Reg(RTC_WEEKDAYS, BCD_INT(week));
        //
    RTC8564_Write_Reg(RTC_MINUTE_ALARM,  0x00);
    RTC8564_Write_Reg(RTC_HOUR_ALARM,    0x00);
    RTC8564_Write_Reg(RTC_DAY_ALARM,     0x00);
    RTC8564_Write_Reg(RTC_WEEKDAY_ALARM, 0x00);
        //
    RTC8564_Write_Reg(RTC_CLKOUT_FREQ,  0x00);
    RTC8564_Write_Reg(RTC_TIMER_CONTROL,0x00);
    RTC8564_Write_Reg(RTC_TIMER,        0x00);
        //
    RTC8564_Write_Reg(RTC_CONTROL1, 0x00); // START
  }
}

//====================
// Get RTC Data
//====================
void RTC8564_Get_Current_Time_Request(RTC_INFO *pTS_RTC_INFO)
{
  pTS_RTC_INFO->year = INT_BCD(RTC8564_Read_Reg(RTC_YEARS));
  pTS_RTC_INFO->mon  = INT_BCD(RTC8564_Read_Reg(RTC_C_MONTHS) & 0x1f);
  pTS_RTC_INFO->day  = INT_BCD(RTC8564_Read_Reg(RTC_DAYS) & 0x3f);
  pTS_RTC_INFO->week = RTC8564_Read_Reg(RTC_WEEKDAYS) & 0x07;
  pTS_RTC_INFO->hour = INT_BCD(RTC8564_Read_Reg(RTC_HOURS) & 0x3f);
  pTS_RTC_INFO->min  = INT_BCD(RTC8564_Read_Reg(RTC_MINUTES) & 0x7f);
  pTS_RTC_INFO->sec  = INT_BCD(RTC8564_Read_Reg(RTC_SECONDS) & 0x7f);
}
