#pragma once
#include "pebble.h"
#define LIST_MESSAGE_WINDOW_NUM_ROWS    5
#define LIST_MESSAGE_WINDOW_CELL_HEIGHT 25

#define LIST_MESSAGE_WINDOW_MENU_HEIGHT 150
/*
#define LIST_MESSAGE_WINDOW_MENU_HEIGHT \
    LIST_MESSAGE_WINDOW_NUM_ROWS * LIST_MESSAGE_WINDOW_CELL_HEIGHT
*/
#define LIST_MESSAGE_WINDOW_HINT_TEXT   "Likert scale txt"

void stress_window_push();
void stress_window_destroy();