#include <pebble.h>
#include "ask_exit.h"
void destroy_ask_exit(void);
static Window *s_main_window;
static ActionBarLayer *s_action_bar_layer;
static TextLayer *s_down_textlayer;
static GBitmap *tick_icon, *cross_icon;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
destroy_ask_exit();
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	//do nothing
	return;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	window_stack_pop_all(true);
		//save time
		
	}


static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
}

static void window_load(Window *window) {
	 Layer *window_layer = window_get_root_layer(window);
	//txt layer for zakoncz
	s_down_textlayer =text_layer_create(GRect(10, 50, 100, 100));
	text_layer_set_text(s_down_textlayer, "ARE YOU SURE?");
	text_layer_set_text_alignment(s_down_textlayer, GTextAlignmentCenter);
	text_layer_set_font(s_down_textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(window_layer, (Layer *)s_down_textlayer);

	//LOAD BITMAPS
	tick_icon = gbitmap_create_with_resource(RESOURCE_ID_TICK_ICON);
  cross_icon= gbitmap_create_with_resource(RESOURCE_ID_CROSS);
	//acton_bar
	s_action_bar_layer = action_bar_layer_create();
	action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, tick_icon);
	action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, cross_icon);
  
	action_bar_layer_add_to_window(s_action_bar_layer, window);
	action_bar_layer_set_click_config_provider(s_action_bar_layer, click_config_provider);
}

static void window_unload(Window *window) {
 	
	text_layer_destroy(s_down_textlayer);
	
	action_bar_layer_destroy(s_action_bar_layer);
	

	gbitmap_destroy(cross_icon);
	gbitmap_destroy(tick_icon);
		
  window_destroy(window);
  s_main_window = NULL;
}



void show_ask_exit(){
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
  
  
}
void destroy_ask_exit(void) {
	window_stack_remove(s_main_window, true);
}