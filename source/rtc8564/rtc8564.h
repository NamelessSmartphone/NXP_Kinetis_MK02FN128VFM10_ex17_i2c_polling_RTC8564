#ifndef __RTC8564_H__
#define __RTC8564_H__

#define I2C2_DEV_S_OWN_ADDR  0x51

#define TZD (+9)

#define RTC_CONTROL1 0x00
#define RTC_CONTROL2 0x01
#define RTC_SECONDS  0x02
#define RTC_MINUTES  0x03
#define RTC_HOURS    0x04
#define RTC_DAYS     0x05
#define RTC_WEEKDAYS 0x06
#define RTC_C_MONTHS 0x07
#define RTC_YEARS    0x08
#define RTC_MINUTE_ALARM  0x09
#define RTC_HOUR_ALARM    0x0a
#define RTC_DAY_ALARM     0x0b
#define RTC_WEEKDAY_ALARM 0x0c
#define RTC_CLKOUT_FREQ   0x0d
#define RTC_TIMER_CONTROL 0x0e
#define RTC_TIMER         0x0f

typedef struct
{
  uint8_t  year; // RTC year
  uint8_t  mon;  // RTC month
  uint8_t  day;  // RTC day
  uint8_t  week; // RTC week
  uint8_t  hour; // RTC hour
  uint8_t  min;  // RTC minute
  uint8_t  sec;  // RTC second
} RTC_INFO;

enum WEEK {
	SUN = 0,
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT
};

extern uint8_t BCD_INT(uint8_t num);
extern uint8_t INT_BCD(uint8_t bcd);
extern uint8_t *Get_Week_String(uint32_t week);
extern void RTC8564_Write_Reg(uint32_t addr, uint32_t data);
extern uint32_t RTC8564_Read_Reg(uint32_t addr);
extern void RTC8564_Config_Request ( void );
extern void RTC8564_Initialize_Request(uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
              uint8_t week,    uint8_t hour, uint8_t minute, uint8_t second);
extern void RTC8564_Get_Current_Time_Request(RTC_INFO *pTS_RTC_INFO);

#endif // __RTC8564_H__
