
/*****************************************************************************
* Kadai 5  
* 7/2016
******************************************************************************/

#include "io.h"
#include "3048.h"
#include "lcd.h"

/*****************************************************************************
* Private Data 
******************************************************************************/


typedef enum { false = 0, true = 1 } bool;

//State display
int state = 0;
#define mode_display_clock        0
#define mode_display_calendar     1

#define mode_set_clock            2

#define mode_set_calendar         3

//The real clock and calendar value
int sec         = 58;
int min         = 59;
int hour        = 23;

int day         = 28;
int month       = 02;
int year        = 2016;

//Buffer for clock and calendar in set mode
int sec_buf     = 0;
int min_buf     = 0;
int hour_buf    = 0;

int day_buf     = 0;
int month_buf   = 0;
int year_buf    = 0;

//dateofyear is the day number in 1 whole year
int dateofyear  = 59;
bool leapyear   = true;

//Countdown 1 sec used for counting down to wait for 1 second
int countdown_1sec;
int delay_1sec  = 200;

char setting_option = 1;

//value for blinking
char blink = 0;

//Countdown used for scanning the 4 buttons
int countdown_25ms_0 = 0;
int countdown_25ms_1 = 0;
int countdown_25ms_2 = 0;
int countdown_25ms_3 = 0;

int countdown_500ms_0 = 0;
int countdown_500ms_1 = 0;
int countdown_500ms_2 = 0;
int countdown_500ms_3 = 0;


/*****************************************************************************
* Private Functions 
******************************************************************************/


int day_count_tab[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

//Check if year_input is leap year
bool is_leap_year(int year_input)
{
  bool ret;

  if(((year_input% 4 ==0) && (year_input%100 !=0)) || (year_input%400 ==0))
    ret = true;
  else
    ret = false;

  return ret;
}

//Calculate day and month based on dateofyear and year
void calc_date(int *day_input, int *month_input, int year_input,int dateofyear_input)
{
  int i=0;
  int temp = dateofyear_input;
  int month_days;

  for(i=0;i<12;i++)
  {
    month_days = day_count_tab[i];
    if(i==1)
      month_days += is_leap_year(year_input);
    if(temp > month_days)
      temp -= month_days;
    else
    {
      *day_input = temp;
      *month_input = i+1;
      break;
    }
  }
}

//Calculate dateofyear based on day, month and year
int calc_dateofyear(int day_input,int month_input,int year_input)
{
  int i = 0;
  int ret = 0;
  for(i = 0;i<(month_input-1);i++)
  {
    ret += day_count_tab[i];
    if((is_leap_year(year_input)==true)&&(i==1))
      ret ++;
  }
  ret += day_input;
  return ret;
}

//New day handling function
void newday(void){
  leapyear = is_leap_year(year);

  if(leapyear == true){
    if(dateofyear==366){
      year++;
      dateofyear=1;
    }
    else{
      dateofyear++;
    }
  }
  else{
    if(dateofyear==365){
      year++;
      dateofyear=1;
    }
    else{
      dateofyear++;
    }
  }
  calc_date(&day,&month,year,dateofyear);
}

//Clock update function every second
void runtime_clock(void)
{
  sec++;
  if(sec>59){
    sec=0;
    min++;
  }
  if(min>59){
    min=0;
    hour++;
  }
  if(hour>23){
    hour=0;
    newday();
  }
}


/*****************************************************************************
* adjust_button
* Adjust time and calendar based on state and option
*
* int option
* Option to increment or decrement
* 1 : increment
* 2 : decrement
*
******************************************************************************/

void adjust_button(int option)
{
  switch(state)
  {
    case mode_set_clock:
    {
      switch(setting_option)
      {
        case 1:
        {
          if(option==1)
            hour_buf++;
          else
            hour_buf--;

          if(hour_buf > 23)
            hour_buf = 0;
          else if(hour_buf <0)
            hour_buf = 23;
        }
        break;

        case 2:
        {
          if(option==1)
            min_buf++;
          else
            min_buf--;

          if(min_buf > 59)
            min_buf = 0;          
          else if(min_buf < 0)
            min_buf = 59;
        }
        break;

        case 3:
        {
          if(option==1)
            sec_buf++;
          else
            sec_buf--;

          if(sec_buf > 59)
            sec_buf = 0;          
          else if(sec_buf < 0)
            sec_buf = 59;
        }
        break;
      }
    }
    break;

    case mode_set_calendar:
    {
      switch(setting_option)
      {
        case 1:
        {
          if(option==1)
            year_buf++;
          else
            year_buf--;

          if(year_buf<0)
            year_buf = 9999;
          else if(year_buf>9999)
            year_buf = 0;
        }
        break;

        case 2:
        {
          if(option==1)
            month_buf++;
          else
            month_buf--;

          if(month_buf > 12)
            month_buf = 1;
          else if(month_buf < 1)
            month_buf = 12;
        }
        break;

        case 3:
        {
          if(option==1)
            day_buf++;
          else
            day_buf--;

          //If month_buf is not February
          if(month_buf != 2)
          {
            if(day_buf > day_count_tab[month_buf-1])
              day_buf = 1; 
            else if(day_buf < 1)
              day_buf = day_count_tab[month_buf-1];
          }
          //If month_buf is February and year_buf is leap year
          else if((month_buf == 2)&&is_leap_year(year_buf))
          {
            if(day_buf > 29)
              day_buf = 1;
            else if(day_buf < 1)
              day_buf = 29;
          }
        }
        break;
      }
    }
    break;
  }
}


/*****************************************************************************
* Button Handlers
******************************************************************************/


void display_button_handler(void)
{
  if( (countdown_500ms_0 > 0) && (countdown_500ms_0 < 20) )
  {
    switch(state)
    {
      case mode_display_clock:
        state = mode_display_calendar;
      break;

      case mode_set_calendar:
        state = mode_display_calendar;
      break;

      default:
      state = mode_display_clock;
    }
    countdown_500ms_0 = 0;
  }
}

void set_button_handler(void)
{
  //Condition <2s
  if((countdown_500ms_1 > 0) && (countdown_500ms_1 < 80)){
    switch(state)
    {
      case mode_display_clock:
      {
        setting_option  = 1;

        hour_buf        = hour;
        min_buf         = min;
        sec_buf         = sec;

        state           = mode_set_clock;
      }
      break;

      case mode_display_calendar:
      {
        setting_option  = 1;

        year_buf        = year;
        month_buf       = month;
        day_buf         = day;

        state           = mode_set_calendar;
      }
      break;

      case mode_set_clock:
      {
        if(setting_option==3){
          setting_option=1;

          hour  = hour_buf;
          min   = min_buf;
          sec   = sec_buf;

          state = mode_display_clock;
        }
        else
          setting_option++;
      }
      break;

      case mode_set_calendar:
      {
        if(setting_option==3){
          setting_option=1;

          year  = year_buf;
          month = month_buf;
          day   = day_buf;
          dateofyear = calc_dateofyear(day_buf,month_buf,year_buf);

          state = mode_display_calendar;
        }
        else
          setting_option++;
      }
      break;

    }
  }
}

void up_button_handler(void)
{
  //Condition <500ms
  if( (countdown_500ms_2 > 0) && (countdown_500ms_2 < 20) )
  {
    adjust_button(1);
    countdown_500ms_2 = 0;
  }
  // > 500ms case
  if( (countdown_500ms_2 > 20) && (countdown_500ms_2 % 4 == 0) )
  {
    adjust_button(1);
  }
}

void down_button_handler(void)
{
  //Condition <500ms
  if( (countdown_500ms_3 > 0) && (countdown_500ms_3 < 20) )
  {
    adjust_button(2);
    countdown_500ms_3 = 0;
  }

  // > 500ms case
  if( (countdown_500ms_3 > 20) && (countdown_500ms_3 %4 == 0) )
  {
    adjust_button(2);
  }
}


/*****************************************************************************
* Timer Interrupt Handlers 
******************************************************************************/


//Interrupt every 5ms
void int_imia1(void)
{
  countdown_1sec-=1;

  
  /*****************************************************************************
  * Blink LCD every 500ms when in set mode 
  ******************************************************************************/


  if((state == mode_set_calendar)||(state == mode_set_clock))
  {
    switch(countdown_1sec)
    {
      //end of 1 second
      case 0:
        blink = 0;
      break;

      //1/2th of 1 second
      case 100:
        blink = 1;
      break;
    }
  }
  
  /*****************************************************************************
  * Execute runtime_clock() every 1 sec 
  ******************************************************************************/
  

  if(countdown_1sec == 0)
  {
    P5.DR.BYTE = ~P5.DR.BYTE; //LED blink
    countdown_1sec = delay_1sec;
    runtime_clock();
  }

  
  /*****************************************************************************
  * Button Scanning every 5 ms
  ******************************************************************************/


  //Display button scan
  if(!P4.DR.BIT.B4)
  {
    countdown_25ms_0++;
  }
  else
  {
    display_button_handler();
    countdown_500ms_0 = 0;
  }

  if(countdown_25ms_0>4)
  {
    countdown_25ms_0 = 0;
    countdown_500ms_0++;
  }

  //Set button scan
  if(!P4.DR.BIT.B5)
  {
    countdown_25ms_1++;
  }
  else
  {
    set_button_handler();
    countdown_500ms_1 = 0;
  }

  if(countdown_25ms_1>4)
  {
    countdown_25ms_1 = 0;
    countdown_500ms_1++;
  }

  //Up button scan
  if(!P4.DR.BIT.B7)
  {
    countdown_25ms_2++;
  }
  else
  {
    up_button_handler();
    countdown_500ms_2 = 0;
  }

  if(countdown_25ms_2>4)
  {
    countdown_25ms_2 = 0;
    countdown_500ms_2++;
  }

  if(!P4.DR.BIT.B7&&(countdown_500ms_2>20))
  {
    up_button_handler();
  }

  //Down button scan
  if(!P4.DR.BIT.B6)
  {
    countdown_25ms_3++;
  }
  else
  {
    down_button_handler();
    countdown_500ms_3 = 0;
  }

  if(countdown_25ms_3>4)
  {
    countdown_25ms_3 = 0;
    countdown_500ms_3++;
  }

  if(!P4.DR.BIT.B6&&(countdown_500ms_3>20))
  {
    down_button_handler();
  }
  
  //Clear interrupt flag
  ITU1.TSR.BIT.IMFA = 0;
}


/*****************************************************************************
*   Initialization Functions
******************************************************************************/


//Initialize Timer1
void init_tim1(void)
{
  countdown_1sec = delay_1sec;       //countdown_1sec = 200 times * 5ms => 1s

  ITU1.TCR.BIT.CCLR    = 1;     //
  ITU1.TCR.BIT.TPSC    = 3;     //16MHz/8=2MHz
  ITU1.GRA        =9999;        //2MHz/10000=200Hz => 5ms
  ITU1.TIER.BIT.IMIEA = 1;      //IMFA
  ITU.TSTR.BIT.STR1 = 1;        //Start timer1
}

//Setup before entering loop
void setup(void)
{
  P3DDR = 0xff;       //LCD init port
  lcdini();           //LCD init function
  P4DDR = 0;          //Input port 4
  P4PCR = 0xff;       //Pull-up MOS ON
  wait();
  lcd_string1(0," Team LSIVN");

  P5.DDR = 0xff;      //LED as output
  P5.DR.BYTE = 0xff;  //LED value set

  DI;
  init_tim1();
  EI;

  lcd_time(hour,min,sec);
}


/*****************************************************************************
* Entry Point 
******************************************************************************/


int main(void)
{
  setup();
  while(1)
  {
    switch(state)
    {
      //Update LCD to display clock time
      case mode_display_clock:
      {
        lcd_time(hour,min,sec);
      }
      break;

      //Update LCD to display calendar time
      case mode_display_calendar:
      {
        calc_date(&day,&month,year,dateofyear);
        lcd_date(year,month,day);
      }
      break;

      //Update LCD to display clock set mode while blinking
      case mode_set_clock:
      {
        if(blink)
          lcd_time(hour_buf,min_buf,sec_buf);
        else
        {
          switch(setting_option)
          {
            case 1:
            {
              lcd_string2(0,"  ");
            }
            break;

            case 2:
            {
              lcd_string2(3,"  ");
            }
            break;

            case 3:
            {
              lcd_string2(6,"    ");
            }
            break;
          }
        }
      }
      break;

      //Update LCD to display calendar set mode while blinking
      case mode_set_calendar:
      {
        if(blink)
          lcd_date(year_buf,month_buf,day_buf);
        else
        {
          switch(setting_option)
          {
            case 1:
            {
              lcd_string2(0,"    ");
            }
            break;

            case 2:
            {
              lcd_string2(5,"  ");
            }
            break;

            case 3:
            {
              lcd_string2(8,"  ");
            }
            break;
          }
        }
      }
      break;
    }
  }
}
