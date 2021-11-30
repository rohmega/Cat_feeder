#include "spi_ili9341.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "interface.h"
#include "stm32_ds3231.h"


void menu_draw(void);

void selection_action(void);

char* time_text(short Hour,short Min,bool dots);

char* enum_to_day(uint8_t day_num);

void limit_selection(short L,short N);
	
void set_time(void);

char* timeout_vis(void);

void feed_cat(short time);

void write_vars(uint8_t buffer[21]);

void read_vars(void);

void feed_cat_time(void);

void motor_driver(short status);

void motor_intensity(short sound_intensity);
