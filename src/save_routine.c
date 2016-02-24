#include <pebble.h>
#include "save_routine.h"

int8_t day_counter=0;//0-13

int8_t likert_counter=0;//0-62
int8_t likert_bank_counter=0;//0-1

int8_t pedo_counter=0;//0-62
int8_t pedo_bank_counter=0;//0-5

zero_time start_time;
zero_time get_time; 

likert_data likert_temp;
likert_data likert_1;
likert_data likert_2;

day_data day_temp;
day_data day_1;

pedo_data pedo_temp;
pedo_data pedo_1;
pedo_data pedo_2;
pedo_data pedo_3;
pedo_data pedo_4;
pedo_data pedo_5;
pedo_data pedo_6;

static void get_time_func(void);
static void zero_time_save(void);
static uint16_t set_timestamp(void);
static void likert_bank_select(int8_t index);
//global functions for save
void likert_save(int8_t type,int8_t ans){
  bool is_all = false;
  likert_temp[likert_counter].timestamp=set_timestamp();
  //likert_temp[likert_counter].type=type;
  switch(type){
    case 0:
   likert_temp[likert_counter].ans_0=ans;
    break;
    case 1:
    likert_temp[likert_counter].ans_1=ans;
    break;
    case 2:
    likert_temp[likert_counter].ans_2=ans;
    break;
    case 3:
    likert_temp[likert_counter].ans_3=ans;
    is_all=true;
    break;
  
     
    
  }
if(is_all){
  
  //saving here, cuz i can
  likert_bank_select(likert_bank_counter); //save is here
  
  if(likert_counter<41){
    likert_counter++;
   }
  else {
    likert_counter=0;
    likert_bank_counter++;
  }
  persist_write_int(PK_LIKERT_COUNTER, likert_counter);
  persist_write_int(PK_LIKERT_BANK_COUNTER, likert_bank_counter);
}
  
}

void day_save(int8_t l_ans, bool *ans){
  day_temp[day_counter].timestamp=set_timestamp();
  day_temp[day_counter].ans=l_ans;
  for(int i=0; i<=7;i++) {
		day_temp[day_counter].answer[i]=ans[i];
	}
persist_write_data(PK_DAY_DATA, &day_temp, sizeof(day_temp));  
    day_counter++;
persist_write_int(PK_DAY_COUNTER, day_counter);
  
}

void pedo_save(int8_t ans){
   pedo_temp[pedo_counter].timestamp=set_timestamp();
    pedo_temp[pedo_counter].steps=ans;
    //pedo bank switch
  //no peedo no save ;_;
}

//END global functions for save

static uint16_t set_timestamp(void){
	get_time_func();
		uint16_t timestamp =0;
		uint16_t year_day=get_time.year_day-start_time.year_day;
		uint16_t hour=get_time.hour-start_time.hour;
		uint16_t minute=get_time.minute-start_time.minute;
	
		timestamp = year_day*1440+hour*60+minute;
	return timestamp;
}

static void get_time_func(void) {
	//func which get actual day, hour, minute to temp struct "get_time"
	//get time struct
  time_t temp =time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char j_buffer[8]; //day of the year 001-366
  static char H_buffer[8]; // hour (24)
  static char M_buffer[8]; // minutes
	
  strftime(j_buffer, sizeof(j_buffer),"%j",tick_time);
  strftime(H_buffer, sizeof(H_buffer),"%H",tick_time);
  strftime(M_buffer, sizeof(M_buffer),"%M",tick_time);

	get_time.year_day=atoi(j_buffer);
	get_time.hour=atoi(H_buffer);
	get_time.minute= atoi(M_buffer);
}
void initial_save(void){
	//used once
	get_time_func();
	zero_time_save();
}

static void zero_time_save(void){
	//used once
	start_time.year_day=get_time.year_day;
	start_time.hour=get_time.hour;
	start_time.minute=get_time.minute;
	persist_write_data(PK_ZERO_TIME, &start_time, sizeof(start_time));
	APP_LOG(APP_LOG_LEVEL_DEBUG, "base time created, SAVE.");
}


void read_persist_data() {
  //read all counters
  if(persist_exists(PK_DAY_COUNTER)){
    day_counter=persist_read_int(PK_DAY_COUNTER);
    APP_LOG(APP_LOG_LEVEL_INFO, "Day counter: %d", day_counter);
  }
  if(persist_exists(PK_LIKERT_COUNTER)){
    likert_counter=persist_read_int(PK_LIKERT_COUNTER);
    APP_LOG(APP_LOG_LEVEL_INFO, "likert counter: %d", likert_counter);
  }  
  if(persist_exists(PK_LIKERT_BANK_COUNTER)){
  likert_bank_counter=persist_read_int(PK_LIKERT_BANK_COUNTER);
    APP_LOG(APP_LOG_LEVEL_INFO, "likert bank counter: %d", likert_bank_counter);
  }
  if(persist_exists(PK_PEDO_COUNTER)){
    pedo_counter=persist_read_int(PK_PEDO_COUNTER);
    APP_LOG(APP_LOG_LEVEL_INFO, "Day counter: %d", pedo_counter);
  }
  if(persist_exists(PK_PEDO_BANK_COUNTER)){
  pedo_bank_counter=persist_read_int(PK_PEDO_BANK_COUNTER);
    APP_LOG(APP_LOG_LEVEL_INFO, "Day counter: %d", pedo_bank_counter);
  }
  
  if(persist_exists(PK_ZERO_TIME)) {
		persist_read_data(PK_ZERO_TIME, &start_time, sizeof(start_time));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "base time already exists");
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Base time:day_of_the_year: %d,hour: %d,minute: %d", start_time.year_day, start_time.hour, start_time.minute);
		
	}
  
  if(persist_exists(PK_DAY_DATA)) {
    persist_read_data(PK_DAY_DATA, &day_1, sizeof(day_1));
    APP_LOG(APP_LOG_LEVEL_INFO, "Timestamp,L_ans,checkbox");
    for(int i =0; i<14;i++){
      APP_LOG(APP_LOG_LEVEL_INFO, "%u,%d,%s,%s,%s,%s,%s,%s,%s,%s", day_1[i].timestamp, day_1[i].ans, day_1[i].answer[0]?"T":"F",day_1[i].answer[1]?"T":"F",day_1[i].answer[2]?"T":"F",day_1[i].answer[3]?"T":"F",
             day_1[i].answer[4]?"T":"F",day_1[i].answer[5]?"T":"F",day_1[i].answer[6]?"T":"F",day_1[i].answer[7]?"T":"F");
      
    }
  }
  
  if(persist_exists(PK_LICKERT_1)){
    persist_read_data(PK_LICKERT_1, &likert_1, sizeof(likert_1));
    APP_LOG(APP_LOG_LEVEL_INFO, "Timestamp,type,ans");
    for(int i =0; i<64;i++){
      APP_LOG(APP_LOG_LEVEL_INFO, "%u,%d,%d,%d,%d", likert_1[i].timestamp, likert_1[i].ans_0 , likert_1[i].ans_1 , likert_1[i].ans_2 , likert_1[i].ans_3 );
    }
    
  }
  if(persist_exists(PK_LICKERT_2)){
   persist_read_data(PK_LICKERT_2, &likert_2, sizeof(likert_2));
   APP_LOG(APP_LOG_LEVEL_INFO, "Timestamp,type,ans");
   for(int i =0; i<64;i++){
     APP_LOG(APP_LOG_LEVEL_INFO, "%u,%d,%d,%d,%d", likert_2[i].timestamp, likert_2[i].ans_0 , likert_2[i].ans_2 , likert_2[i].ans_2 , likert_2[i].ans_3  );
   }
  }
  if(persist_exists(PK_PEDO_1)){
   persist_read_data(PK_PEDO_1, &pedo_1, sizeof(pedo_1));
   APP_LOG(APP_LOG_LEVEL_INFO, "Timestamp,type,ans");
   for(int i =0; i<64;i++){
     APP_LOG(APP_LOG_LEVEL_INFO, "%u,%d", pedo_1[i].timestamp, pedo_1[i].steps );
   }
 }

//which array save to temp:
  switch(likert_bank_counter){
    
    case 0:
    if(persist_exists(PK_LICKERT_1)){
    persist_read_data(PK_LICKERT_1, &likert_temp, sizeof(likert_temp));
    };
    return;
    case 1:
    if(persist_exists(PK_LICKERT_2)){
    persist_read_data(PK_LICKERT_2, &likert_temp, sizeof(likert_temp));
    };
    return;
    
  }
//switch for pedo

return;
}

static void likert_bank_select(int8_t index){
  //where throw temp
  switch(index){
     case 0:
     persist_write_data(PK_LICKERT_1, &likert_temp, sizeof(likert_temp));
    return;
     case 1:
     persist_write_data(PK_LICKERT_2, &likert_temp, sizeof(likert_temp));
    return;
    
  }
  
}

void save_persist_data() {
 /* //read bool
  likert_bank_select(likert_bank_counter);
  
  if(likert_counter<62){
    likert_counter++;
   }
  else {
    likert_counter=0;
    likert_bank_counter++;
  }
  if(pedo_counter<62){
    pedo_counter++;
   }
  else {
    pedo_counter=0;
    pedo_bank_counter++;
  }
  day_counter++;
  
  persist_write_int(PK_LIKERT_COUNTER, likert_counter);
  persist_write_int(PK_LIKERT_BANK_COUNTER, likert_bank_counter);
  */
}