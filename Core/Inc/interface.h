#include "stm32f4xx_hal.h"
#include "stdbool.h"

void draw_main(void);

void button(uint16_t x,uint16_t y,uint16_t r);

void test(uint16_t x_cor, uint16_t y_cor);
	
void draw_power(short feed_status);

void draw_lock(void);
	
void draw_gear(void);

void draw_options(void);

void draw_extra_options(void);

void feed_set(void);

void calibrate_feed(void);

void set_time(void);
