#include <pebble.h>
#include "run_app.h"
#include <nastroj.h>
#include <stress.h>
#include <sexual.h>
#include <sleep.h>

#define OPEN_WINDOW 1
#define PEDOMETER 2

static Window *s_main_window;

static TextLayer *s_down_textlayer;

int click_check=0;

static void show_question(int index) {
  //TUTEJ BEDZIE FUNKCJA ODCZYTUJĄCA INFO Z BAKGRANDU
  switch(index){
    case 0:
      nastroj_window_push();
    break;
    case 1:
     stress_window_push();
    break;
    case 2:
      sexual_window_push();
    break;
    case 3:
      sleep_window_push();
    break;

  }
vibes_double_pulse();
 
}

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
  if (type == OPEN_WINDOW) {
    // Read ticks from worker's packet
    int which = data->data0;
show_question(which);
      APP_LOG(APP_LOG_LEVEL_INFO, "got question no %d", which);
      }
 
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Check to see if the worker is currently active
  bool running = app_worker_is_running();
  // Toggle running state
  AppWorkerResult result;
    
  if (running) {
    text_layer_set_text(s_down_textlayer, "RUNNING");
    if(click_check==1 ){ 
      result = app_worker_kill();

    if (result == APP_WORKER_RESULT_SUCCESS) {
      text_layer_set_text(s_down_textlayer, "Worker stopped!");
    } else {
      text_layer_set_text(s_down_textlayer, "Error killing worker!");
    }
    click_check=0;
    }
  } 
  else {
    result = app_worker_launch();

    if (result == APP_WORKER_RESULT_SUCCESS) {
      text_layer_set_text(s_down_textlayer, "Worker launched!");
      
    } else {
      text_layer_set_text(s_down_textlayer, "Error launching worker!");
    }
  }

//  APP_LOG(APP_LOG_LEVEL_INFO, "Result: %d", result);
}

static void long_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //SHOW MENU
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_long_click_subscribe(BUTTON_ID_UP, 2000, long_down_click_handler, NULL);
}


static void window_load(Window *window) {
	 Layer *window_layer = window_get_root_layer(window);
	//txt layer for zakoncz
	s_down_textlayer =text_layer_create(GRect(10, 50, 100, 100));
	text_layer_set_text(s_down_textlayer, "select to run");
	text_layer_set_text_alignment(s_down_textlayer, GTextAlignmentCenter);
	text_layer_set_font(s_down_textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(window_layer, (Layer *)s_down_textlayer);

}

static void window_unload(Window *window) {
 	
	text_layer_destroy(s_down_textlayer);
		
  window_destroy(window);
  s_main_window = NULL;
}

void run_main_screen(){
   if(!s_main_window) {
		s_main_window = window_create();
    window_set_background_color(s_main_window, GColorWhite);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
       .unload = window_unload,
    });
  }
	window_set_click_config_provider(s_main_window, (ClickConfigProvider)click_config_provider);
  window_stack_push(s_main_window, true);
    app_worker_message_subscribe(worker_message_handler);
    
  
  //simple_menu_load();
  
  
}