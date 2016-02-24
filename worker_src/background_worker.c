#include <pebble_worker.h>
#include "background_worker.h"

#define OPEN_WINDOW 1
#define PEDOMETER 2
static int8_t actual_hour=0;
static int8_t actual_minute=0;

static int8_t day_counter=0;

static int8_t sleep_hour[]=
{
  8,
  8,
  7,
  7,
  8,
  7,
  7,
  7,
  8,
  8,
  8,
  7,
  8,
  8
};
static int8_t sleep_minute[]=
{
  28,
  44,
  24,
  51,
  42,
  16,
  31,
  23,
  56,
  11,
  58,
  52,
  20,
  24
};
static int8_t hour_q_one[]=
{
  12	,
  10	,
  10	,
  12	,
  10	,
  10	,
  11	,
  10	,
  10	,
  11	,
  10	,
  11	,
  11	,
  12	

};
static int8_t minute_q_one[]=
{
  47	,
  0	  ,
  49	,
  56	,
  48	,
  22	,
  52	,
  12	,
  44	,
  59	,
  25	,
  32	,
  25	,
  16	

};
static int8_t hour_q_two[]=
{
  16	,
  15	,
  16	,
  15	,
  15	,
  14	,
  16	,
  15	,
  15	,
  15	,
  15	,
  15	,
  16	,
  16	

};
static int8_t minute_q_two[]=
{
  10	,
  18	,
  31	,
  32	,
  25	,
  37	,
  5	,
  41	,
  56	,
  33	,
  12	,
  5	,
  9	,
  34	 
  
};
static int8_t hour_q_three[]=
{
  18	,
  19	,
  19	,
  19	,
  20	,
  20	,
  18	,
  20	,
  20	,
  20	,
  19	,
  20	,
  19	,
  19	 

};
static int8_t minute_q_three[]=
{
  0	,
  36	,
  11	,
  28	,
  59	,
  49	,
  14	,
  20	,
  12	,
  16	,
  11	,
  40	,
  46	,
  30	 

};


static void send_pedometer(int8_t hour, uint32_t steps){
  worker_launch_app();
  psleep(300);
  AppWorkerMessage pedo_data = {
    .data0 = hour,
    .data1 = steps
  };
  app_worker_send_message(PEDOMETER, &pedo_data);

}

static void send_data(int8_t data){
  //launch app
  worker_launch_app();
  //wait till app is Ready
  psleep(300);
  //from 0 to 3, depending on question type
  AppWorkerMessage open_data = {
    .data0 = data
  };
  app_worker_send_message(OPEN_WINDOW, &open_data);
//data:  0 nastroj, 1 stress, 2 sexual, 3 sleep,
}

static void day_check(void){
  if(actual_hour==0&&actual_minute==0){
    day_counter++;
   
  }

}
static void get_time(void) {
	//func which get actual day, hour, minute to temp struct "get_time"
	//get time struct
  time_t temp =time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char H_buffer[8]; // hour (24)
  static char M_buffer[8]; // minutes

  strftime(H_buffer, sizeof(H_buffer),"%H",tick_time);
  strftime(M_buffer, sizeof(M_buffer),"%M",tick_time);

	actual_hour=atoi(H_buffer);
	actual_minute= atoi(M_buffer);
  APP_LOG(APP_LOG_LEVEL_INFO, "%d:%d", actual_hour, actual_minute);
 // APP_LOG(APP_LOG_LEVEL_INFO, "morning question counter %d",day_counter);
}
/*
static void tick_handler(struct tm *tick_timer, TimeUnits units_changed) {
  //for testing, remove after
   get_time();
  day_check();
  if (actual_hour==7){
    
    if(actual_minute==25||actual_minute==29){
      send_data(0);
    }
    if(actual_minute==26||actual_minute==30){
      send_data(1);
    }
    if(actual_minute==27||actual_minute==31){
      send_data(2);
    }
    if(actual_minute==28||actual_minute==32){
      send_data(3);
    }
    
  }
  
}
*/       


static void tick_handler(struct tm *tick_timer, TimeUnits units_changed) {
  get_time();
  day_check();
//morning question is here
  APP_LOG(APP_LOG_LEVEL_INFO, "DAY: %d,", day_counter);

  if(actual_hour==sleep_hour[day_counter] && actual_minute==sleep_minute[day_counter]){
    send_data(3);
  }

//first lickert -morning
  else if(actual_hour==hour_q_one[day_counter] && actual_minute==minute_q_one[day_counter]){
      send_data(0);
  }
//second lickert -lunch time
  else if(actual_hour==hour_q_two[day_counter] && actual_minute==minute_q_two[day_counter]){
      send_data(0);
  }
//third lickert  - evening
  else if(actual_hour==hour_q_three[day_counter] && actual_minute==minute_q_three[day_counter]){
      send_data(0);
  }


}

static void worker_init() {
  // Use the TickTimer Service as a data source
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void worker_deinit() {
  // Stop using the TickTimerService
  tick_timer_service_unsubscribe();
}

int main(void) {
  worker_init();
  worker_event_loop();
  worker_deinit();
}
