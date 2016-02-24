#include <pebble.h>
#include <nastroj.h>
#include <stress.h>
#include <sexual.h>
#include <sleep.h>
#include <run_app.h>
#include "save_routine.h"
#include "first_launch.h"


#define OPEN_WINDOW 1
#define PEDOMETER 2

Window *window;
TextLayer *text_layer;

static void show_question(int index) {
  //TUTEJ BEDZIE FUNKCJA ODCZYTUJĄCA INFO Z BAKGRANDU
  switch(index){
    case 0:
      nastroj_window_push();
    break;
    
    case 3:
      sleep_window_push();
    break;
    default:
  text_layer_set_text(text_layer, "Error ocurred. Contact dev.");  
    break;

  }
vibes_double_pulse();
   psleep(100);
   text_layer_destroy(text_layer);
  window_destroy(window);
}

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
  if (type == OPEN_WINDOW) {
    // Read ticks from worker's packet
    int which = data->data0;
    show_question(which);
      APP_LOG(APP_LOG_LEVEL_INFO, "got question no %d", which);
      }
 
}

static void load_simple_display(){
   window = window_create();
  text_layer = text_layer_create(GRect(5, 20, 144, 40));
  text_layer_set_text(text_layer, "Saving data. do not click.");
  layer_add_child(window_get_root_layer(window), 
                    text_layer_get_layer(text_layer));
  window_stack_push(window, true);
  
}


void handle_init(void) {
  
 
  
  if(persist_read_bool(PK_FIRST_LAUNCH)==true){
  if (launch_reason()==APP_LAUNCH_USER) {
   
    // nastroj_window_push();
    run_main_screen();
  app_worker_message_subscribe(worker_message_handler);
    
  }
  else if (launch_reason()==APP_LAUNCH_WORKER){
    app_worker_message_subscribe(worker_message_handler);
    load_simple_display();
  }
  //read_persist
read_persist_data();
}
  else show_first_launch();
  
}

void handle_deinit(void) {
  
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
