#include <pebble.h>
#include "first_launch.h"
#include "save_window.h"
#include "save_routine.h"
#include "test_app.h"
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_condensed_21;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
Layer *immergo_layer;
static GBitmap *immergo_bitmap;

static int s_uptime=0;

static void icon_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GRect bitmap_bounds = gbitmap_get_bounds(immergo_bitmap);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, immergo_bitmap, (GRect){.origin = bounds.origin, .size = bitmap_bounds.size});
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	s_uptime++;
  if(s_uptime==2) {
    //text_layer_set_text(s_textlayer_1, "CHUJ badanie");
    layer_destroy(immergo_layer);
    tick_timer_service_unsubscribe();
  }
  /*
 if(s_uptime==3) {
	 
	 text_layer_set_text(s_textlayer_1, "OK. Restart.");
	   text_layer_set_text(s_textlayer_2, ".");
	  text_layer_set_text(s_textlayer_3, ".");

 }
 */
	//	hide_stopwatch();
	if (s_uptime==3){
		//window_stack_pop_all(true);
		//disp main menu
		//show_first_menu();
		//window_stack_pop_all( true);
		// tick_timer_service_unsubscribe();
    end_window();
	}
	
}

static void start_data_collect(void){
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	   APP_LOG(APP_LOG_LEVEL_INFO, "START");
  
  persist_write_bool(PK_FIRST_LAUNCH, true);
		//window_stack_pop_all(true);
  persist_delete(PK_DAY_COUNTER);
 persist_delete(PK_DAY_DATA);
 persist_delete(PK_LIKERT_COUNTER);
 persist_delete(PK_LIKERT_BANK_COUNTER);
 persist_delete(PK_LICKERT_1);
 persist_delete(PK_LICKERT_2);
    psleep(500);
    
  initial_save();
  
  AppWorkerResult result;

 result = app_worker_launch();

    if (result == APP_WORKER_RESULT_SUCCESS) {
      vibes_long_pulse();
    
    } 
  psleep(200);
	start_data_collect();

	//never ever show this window, start saving data
	
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "EXIT");
	//quit app
	window_stack_pop_all(true);
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "APP TEST");
	//show UI without data save
	hide_first_launch();
	show_example();

}


static void click_config_provider(void *context) {
	
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void initialise_ui(void) {
  s_window = window_create();
 
  
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 62, 137, 51));
  text_layer_set_text(s_textlayer_1, "START");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_1, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(2, 128, 136, 29));
  text_layer_set_text(s_textlayer_2, "APP TEST");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_2, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(1, 25, 39, 25));
  text_layer_set_text(s_textlayer_3, "EXIT");
  text_layer_set_font(s_textlayer_3, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
  //BITMAPLAYER
 	immergo_bitmap=gbitmap_create_with_resource(RESOURCE_ID_IMMERGO);
  GRect bitmap_bounds = gbitmap_get_bounds(immergo_bitmap);
	immergo_layer = layer_create(bitmap_bounds);
	layer_set_update_proc(immergo_layer, icon_update_proc);
  layer_add_child(window_get_root_layer(s_window), (immergo_layer));
  
  	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
	
}

void show_first_launch(void) {
  initialise_ui();
	persist_write_bool(PK_FIRST_LAUNCH, false);

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  window_set_click_config_provider(s_window, (ClickConfigProvider)click_config_provider); //buttons useful now

}
void hide_first_launch(){
	
	window_stack_remove(s_window, true);
}

