#pragma once
#include <pebble.h>

#define PK_FIRST_LAUNCH 0
#define PK_ZERO_TIME 1

#define PK_DAY_COUNTER 2
#define PK_DAY_DATA 3

#define PK_LIKERT_COUNTER 4
#define PK_LIKERT_BANK_COUNTER 5 
#define PK_LICKERT_1 6
#define PK_LICKERT_2 7

#define PK_PEDO_COUNTER 8
#define PK_PEDO_BANK_COUNTER 9
#define PK_PEDO_1 10
#define PK_PEDO_2 11
#define PK_PEDO_3 12
#define PK_PEDO_4 13
#define PK_PEDO_5 14
#define PK_PEDO_6 15


void likert_save(int8_t type,int8_t ans);
void day_save(int8_t l_ans, bool *ans);
void pedo_save(int8_t ans);
void read_persist_data(void);
void initial_save(void);

typedef struct zero_time {
	uint16_t year_day;
	int8_t hour;
	int8_t minute;
	
}__attribute__((__packed__)) zero_time;	

typedef struct likert_data {
  //TWO ARRAYS
  uint16_t timestamp;
  
  int8_t ans_0;
  int8_t ans_1;
  int8_t ans_2;
  int8_t ans_3;

  
}__attribute__((__packed__)) likert_data[42];
typedef struct day_data {
  //JUST ONE
  uint16_t timestamp;
  int8_t ans; //from likert
  bool answer[8];
  
}__attribute__((__packed__)) day_data[14];

typedef struct pedo_data{
  //W CHUJ CZYLI SZEŚĆ
  uint16_t timestamp;
  uint16_t steps;
}__attribute__((__packed__)) pedo_data[63];