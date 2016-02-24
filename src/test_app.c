#include <pebble.h>
#include "test_app.h"

#include <nastroj.h>
#include <stress.h>
#include <sexual.h>
#include <sleep.h>
#include <sleepy_q.h>
#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 4


static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];


static void menu_select_callback(int index, void *ctx) {
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
     sleepy_window_push(); 
    // sleep_window_push();
    break;

}
}

static void main_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "MOOD",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "STRESS",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "SEXUAL",
    .callback = menu_select_callback,
   
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "SLEEPy",
    .callback = menu_select_callback,
   
  };


  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };
 
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
window_destroy(s_main_window);
}

void show_example() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}


