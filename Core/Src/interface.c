#include "spi_ili9341.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "consolidate.h"

extern bool pressed;
extern short menu;
extern short prev_menu;
extern bool button_pressed; 


//https://javl.github.io/image2cpp/
//https://github.com/FoxExe/Image2Bitmap/releases/tag/0.6
short prev_feed_time[4][2];
short prev_feed_amount[4];
short prev_feed_number=-1;
short prev_feed_status=3;

extern short feed_time[4][2];
extern short feed_amount[4];
//extern char curr_day[2];
extern short selection;
extern long text_colour;
extern bool feed_status;
extern short feed_number;
extern short prev_selection;

extern short sub_menu;
extern short prev_sub_menu;
extern short selection_wheel;

extern uint8_t curr_day;
extern uint8_t prev_day;

extern _RTC rtc;
extern short skip;
short update=0;
extern uint16_t screen_brightness;

extern bool sound_feed;
extern bool food_shake;
extern short sound_intensity;
extern short time_out;

extern short feed_cal;
short once=0;
short count_var=0;
extern short skip_meal;
int time_diff_meal=1500;
short meal_index_skip=-1;

//uint8_t POWER[]={00000111Y,11111111Y};



uint16_t POWER[]={0x0,0x0,0x1,0xf000,0x0,0x0,0x0,0x0,0x0,0xff80,0x0,0x0,0x0,0x0,0x0,0x3ff8,0x0,0x0,0x0,0x0,0x0,0xfff,0x8000,0x0,0x0,0x0,0x0,0x1ff,0xf000,0x0,0x0,0x0,0x0,0x7f,0xff00,0x0,0x0,0x0,0x0,0xf,0xffe0,0x0,0x0,0x0,0x0,0x1,0xfffc,0x0,0x0,0x0,0x0,0x0,0x3fff,0x8000,0x0,0x0,0x0,0x0,0x7ff,0xf000,0x0,0x0,0x0,0x1c00,0xff,0xfe00,0x70,0x0,0x0,0xfe0,0x1f,0xffc0,0x3f,0x8000,0x0,0x3fe,0x3,0xfff8,0xf,0xf800,0x0,0xff,0xe000,0x7fff,0x3,0xffc0,0x0,0x3f,0xfc00,0xfff,0xe000,0x7ffc,0x0,0xf,0xff80,0x1ff,0xfc00,0xfff,0xc000,0x3,0xfff0,0x3f,0xff80,0x1ff,0xfc00,0x0,0xfffe,0x7,0xfff0,0x3f,0xffc0,0x0,0x3fff,0xc000,0xfffe,0x7,0xfff8,0x0,0xfff,0xf000,0x1fff,0xc000,0x7fff,0x8000,0x3ff,0xfc00,0x3ff,0xf800,0x7ff,0xf800,0xff,0xff00,0x7f,0xff00,0x7f,0xff80,0x1f,0xffc0,0xf,0xffe0,0x7,0xfff0,0x7,0xfff0,0x1,0xfffc,0x0,0x7fff,0x1,0xfffc,0x0,0x3fff,0x8000,0x7ff,0xf000,0x3fff,0x0,0x7ff,0xf000,0x7f,0xfe00,0xfff,0xc000,0xff,0xfe00,0x7,0xffe0,0x1ff,0xf000,0x1f,0xffc0,0x0,0x7ffc,0x7f,0xfe00,0x3,0xfff8,0x0,0xfff,0xc00f,0xff80,0x0,0x7fff,0x0,0xff,0xf803,0xffe0,0x0,0xfff,0xe000,0xf,0xff80,0x7ffc,0x0,0x1ff,0xfc00,0x1,0xfff0,0x1fff,0x0,0x3f,0xff80,0x0,0x1fff,0x3ff,0xe000,0x7,0xfff0,0x0,0x3ff,0xe07f,0xf800,0x0,0xfffe,0x0,0x3f,0xfc1f,0xff00,0x0,0x1fff,0xc000,0x7,0xffc3,0xffc0,0x0,0x3ff,0xf800,0x0,0x7ff8,0x7ff8,0x0,0x7f,0xff00,0x0,0xfff,0xfff,0x0,0xf,0xffe0,0x0,0xff,0xf3ff,0xc000,0x1,0xfffc,0x0,0x1f,0xfe7f,0xf800,0x0,0x3fff,0x8000,0x3,0xffcf,0xff00,0x0,0x7ff,0xf000,0x0,0x7ff9,0xffe0,0x0,0xff,0xfe00,0x0,0x7ff,0x3ff8,0x0,0x1f,0xffc0,0x0,0xff,0xffff,0x0,0x3,0xfff8,0x0,0x1f,0xffff,0xe000,0x0,0x7fff,0x0,0x3,0xffff,0xfc00,0x0,0xfff,0xe000,0x0,0x7fff,0xff80,0x0,0x1ff,0xfc00,0x0,0xfff,0xfff0,0x0,0x3f,0xff80,0x0,0x1ff,0xfffe,0x0,0x3,0xfff0,0x0,0x3f,0xffff,0xc000,0x0,0x7ffc,0x0,0x7,0xffff,0xf800,0x0,0x7ff,0x0,0x0,0xffff,0xff00,0x0,0x7f,0xc000,0x0,0x1fff,0xffe0,0x0,0x7,0xf000,0x0,0x3ff,0xfffc,0x0,0x0,0x0,0x0,0x7f,0xffff,0x8000,0x0,0x0,0x0,0xf,0xffff,0xf000,0x0,0x0,0x0,0x1,0xffef,0xfe00,0x0,0x0,0x0,0x0,0x3ffd,0xffe0,0x0,0x0,0x0,0x0,0x7ff,0x3ffc,0x0,0x0,0x0,0x0,0x1ff,0xe7ff,0x8000,0x0,0x0,0x0,0x3f,0xfcff,0xf000,0x0,0x0,0x0,0x7,0xff8f,0xff00,0x0,0x0,0x0,0x0,0xfff1,0xffe0,0x0,0x0,0x0,0x0,0x3ffc,0x3ffc,0x0,0x0,0x0,0x0,0x7ff,0x87ff,0xc000,0x0,0x0,0x0,0x1ff,0xf07f,0xf800,0x0,0x0,0x0,0x3f,0xfc0f,0xff80,0x0,0x0,0x0,0xf,0xff80,0xfff0,0x0,0x0,0x0,0x1,0xfff0,0x1fff,0x0,0x0,0x0,0x0,0x7ffc,0x3ff,0xe000,0x0,0x0,0x0,0xfff,0x803f,0xfe00,0x0,0x0,0x0,0x3ff,0xe007,0xffe0,0x0,0x0,0x0,0xff,0xfc00,0x7ffe,0x0,0x0,0x0,0x1f,0xff00,0xfff,0xc000,0x0,0x0,0x7,0xffe0,0xff,0xfc00,0x0,0x0,0x1,0xfff8,0xf,0xffc0,0x0,0x0,0x0,0x7fff,0x1,0xfffc,0x0,0x0,0x0,0x1fff,0xc000,0x1fff,0xc000,0x0,0x0,0x7ff,0xf000,0x1ff,0xfc00,0x0,0x0,0x1ff,0xfe00,0x3f,0xffe0,0x0,0x0,0x7f,0xff80,0x3,0xfffe,0x0,0x0,0x3f,0xffe0,0x0,0x3fff,0xe000,0x0,0xf,0xfff8,0x0,0x3ff,0xff00,0x0,0x7,0xfffe,0x0,0x3f,0xfff8,0x0,0x3,0xffff,0x8000,0x3,0xffff,0xc000,0x1,0xffff,0xf000,0x0,0x3fff,0xff00,0x1,0xffff,0xf800,0x0,0x3ff,0xfffc,0x1,0xffff,0xfe00,0x0,0x3f,0xffff,0xffff,0xffff,0xff80,0x0,0x3,0xffff,0xffff,0xffff,0xffe0,0x0,0x0,0x1fff,0xffff,0xffff,0xfff8,0x0,0x0,0x1ff,0xffff,0xffff,0xfffc,0x0,0x0,0x1f,0xffff,0xffff,0xffff,0x0,0x0,0x0,0xffff,0xffff,0xffff,0x8000,0x0,0x0,0x7ff,0xffff,0xffff,0xc000,0x0,0x0,0x3f,0xffff,0xffff,0xe000,0x0,0x0,0x1,0xffff,0xffff,0xf000,0x0,0x0,0x0,0x7ff,0xffff,0xf000,0x0,0x0,0x0,0x1f,0xffff,0xf000,0x0,0x0,0x0,0x0,0x1fff,0xc000,0x0,0x0};
uint16_t LOCK[] ={0x7,0x8000,0x1ff,0xe000,0xfff,0xe000,0xffff,0xc003,0xc00f,0x801e,0x1e,0x70,0x3c,0x3c0,0x70,0xe00,0x1c0,0x3800,0x700,0xe000,0x1c03,0x8000,0x700e,0x1,0xc038,0x7,0xe0,0x1c,0x7ff,0xfff8,0x3fff,0xfff9,0xffff,0xffe7,0xffff,0xffdf,0xffff,0xff7f,0xffff,0xfdff,0xe0ff,0xf7ff,0x1ff,0xdffc,0x7ff,0x7ff0,0x1ffd,0xffc0,0x7ff7,0xff01,0xffdf,0xfe0f,0xff7f,0xfc7f,0xfdff,0xf1ff,0xf7ff,0xc7ff,0xdfff,0x1fff,0x7ffc,0x7ffd,0xffff,0xfff7,0xffff,0xffdf,0xffff,0xff7f,0xffff,0xfdff,0xffff,0xe3ff,0xffff,0x3ff,0xfff0};
uint16_t GEAR[]={0x0,0x7e00,0x0,0xff,0x0,0x0,0xff00,0x0,0xc3,0x0,0x1c1,0xc383,0x8003,0xffc3,0xffc0,0x7ff,0x81ff,0xe00f,0x3c00,0x3cf0,0xe10,0x8,0x700e,0x0,0x70,0x700,0x7e00,0xe007,0x81ff,0x81e0,0x707,0xffe0,0xe007,0xf00,0xf0e0,0x60e,0x70,0x600e,0x1c00,0x3870,0x7e18,0x18,0x7efc,0x3800,0x1c3f,0xe038,0x1c,0x7c0,0x3800,0x1c03,0xc038,0x1c,0x7e0,0x3800,0x1c07,0xfc38,0x1c,0x3f7e,0x1800,0x187e,0xe1c,0x38,0x7006,0xe00,0x7060,0x70f,0xf0,0xe007,0x7ff,0xe0e0,0x781,0xff81,0xe007,0x7e,0xe0,0xe00,0x0,0x700e,0x1000,0x870,0xf3c,0x3c,0xf007,0xff81,0xffe0,0x3ff,0xc3ff,0xc001,0xc1c3,0x8380,0x0,0xc300,0x0,0xff,0x0,0x0,0xff00,0x0,0x7e,0x0};
//uint16_t BACK[]={0x7,0x0,0x0,0x0,0x1f0,0x0,0x0,0x0,0x7e00,0x0,0x0,0x1f,0x8000,0x0,0x0,0x7e0,0x0,0x0,0x1,0xf800,0x0,0x0,0x7e,0x0,0x0,0x0,0x1f80,0x0,0x0,0x7,0xe000,0x0,0x0,0x1f8,0x0,0x0,0x0,0x7e00,0x0,0x0,0x1f,0x8000,0x0,0x0,0x7f0,0x0,0x0,0x1,0xffff,0xffff,0xffff,0x3f,0xffff,0xffff,0xfff8,0x7ff,0xffff,0xffff,0xffc0,0x7fff,0xffff,0xffff,0xfc07,0xe000,0x0,0x1f,0xc07e,0x0,0x0,0xfc,0x7e0,0x0,0x0,0xfc0,0x7e00,0x0,0x0,0xf807,0xe000,0x0,0xf,0x807e,0x0,0x0,0xf0,0x7e0,0x0,0x0,0x1f00,0x7e00,0x0,0x3,0xe007,0xe000,0x0,0x3c,0x7e,0x0,0x0,0x780,0x7c0,0x0,0x0,0xf000,0x7000,0x0,0x1e,0x0,0x0,0x0,0x3c0,0x0,0x0,0x0,0xf800,0x0,0x0,0x1e,0x0,0x0,0x0,0x7c0,0x0,0x0,0x0,0xf800,0x0,0x0,0x3e,0x0,0x0,0x0,0xf80,0x0,0x0,0x7,0xf000,0x0,0x0,0x3fc,0x0,0xf,0xffff,0xff00,0x0,0x3ff,0xffff,0x8000,0x0,0x7fff,0xffe0,0x0,0x7,0xffff,0x3800};
uint16_t BACK[]={0x60,0x0,0x0,0xe000,0x0,0x1e0,0x0,0x3,0xc000,0x0,0x780,0x0,0xf,0x0,0x0,0x1e00,0x0,0x3c,0x0,0x0,0x7800,0x0,0xff,0xffff,0xffc0,0xffff,0xffff,0xf07f,0xffff,0xfff8,0x3c00,0x0,0x7c1e,0x0,0x1c,0xf00,0x0,0xe07,0x8000,0xe,0x3c0,0x0,0x701,0xe000,0x7,0xe0,0x0,0x700,0x6000,0x7,0x0,0x0,0x700,0x0,0x7,0x0,0x0,0xe00,0x0,0x1e,0x0,0x0,0x3c00,0x0,0x7c,0x0,0x3fff,0xf800,0x7f,0xffe0,0x0,0x3fff,0x80};








void draw_power(short feed_status)
{
	int k=15;
	int m=0;

	for(int i=0;i<100;i++)
	{
		for(int j=0;j<99;j++)
		{
			TFT9341_DrawPixel(20+j,70+i,(((POWER[m]>>k)&0x0001))*((0x07E0)*(feed_status==1)+(0xf800)*(feed_status==0))); //0xf800 red

			if(k==0)
			{
				k=16;
				m++;
			}
			k--;
		}
	}
}


void draw_lock(void)
{
	int k=15;
	int m=0;

	for(int i=0;i<40;i++) // y dimensions
	{
		for(int j=0;j<30;j++)
		{
			TFT9341_DrawPixel(5+j,192+i,(((LOCK[m]>>k)&0x0001))*(0xffff)); //0xf800 red

			if(k==0)
			{
				k=16;
				m++;
			}
			k--;
		}
	}
}

void draw_gear(void)
{
	int k=15;
	int m=0;

	for(int i=0;i<40;i++) // y dimensions
	{
		for(int j=0;j<40;j++)
		{
			TFT9341_DrawPixel(5+j,5+i,(((GEAR[m]>>k)&0x0001))*(0xffff)); //0xf800 red

			if(k==0)
			{
				k=16;
				m++;
			}
			k--;
		}
	}
}

void draw_back()
{
	int k=15;
	int m=0;

	for(int i=0;i<29;i++)
	{
		for(int j=0;j<40;j++)
		{
			TFT9341_DrawPixel(15+j,190+i,(((BACK[m]>>k)&0x0001))*0xffff); //0xf800 red

			if(k==0)
			{
				k=16;
				m++;
			}
			k--;
		}
	}
}


void draw_main()
{
	char str[8];
	//char str[4];



	if(prev_day!=curr_day||prev_menu!=menu || update)
	{
		time_diff_meal=1500;

		count_var++;
		update=0;
		prev_day=curr_day;
		prev_menu=menu;
		selection=0;
		TFT9341_FillScreen(0x0000);
		TFT9341_SetFont(&Font20);
		//TFT9341_DrawRect(0xffff,8,8,34,34);
		TFT9341_SetTextColor(0xffff);
		TFT9341_String(60,15,"Voermachine!");
		// Draw feed times and amounts:
		TFT9341_SetTextColor(0x0000);
		TFT9341_SetFont(&Font24);


		if(skip_meal==1)
		{
			for(int i=0;i<(feed_number);i++)
			{
				int Alarm_diff=(feed_time[i][0]-rtc.Hour)*60+feed_time[i][1]-rtc.Min;
				if(Alarm_diff<0)
				{
					Alarm_diff=Alarm_diff+24*60;
				}

				if(Alarm_diff<time_diff_meal)
				{
					time_diff_meal=Alarm_diff;
					meal_index_skip=i;
				}
			}
			//TFT9341_SetTextColor(0xf0f0);
			//sprintf(str,"%d",time_diff_meal);
			//TFT9341_String(30,180,str);
		}

		// Erase previous
		/*
		for(int i=0;i<(prev_feed_number);i++)
		{
			TFT9341_SetTextColor(0xffff);

			sprintf(str,"%d:%d",prev_feed_time[i][0],prev_feed_time[i][1]);
			TFT9341_String(140,90+i*40+((2-prev_feed_number)*20),str);
			sprintf(str,"%d",prev_feed_amount[i]);
			TFT9341_String(250,90+i*40+((2-prev_feed_number)*20),strcat(str,"g"));
		}
		*/
		TFT9341_SetTextColor(0xFFFF);
		// draw new
		for(int i=0;i<(feed_number);i++)
		{
			if(meal_index_skip==i && skip_meal==1)
			{
				TFT9341_SetTextColor(0xF000);
			}
			else
			{
				TFT9341_SetTextColor(0xFFFF);
			}
			sprintf(str,"%d:%d",feed_time[i][0],feed_time[i][1]);

			TFT9341_String(140,90+i*40+((2-feed_number)*20),time_text(feed_time[i][0],feed_time[i][1],1));
			sprintf(str,"%d",feed_amount[i]);
			TFT9341_String(250,90+i*40+((2-feed_number)*20),strcat(str,"g"));
			TFT9341_SetTextColor(0xFFFF);
		}

		draw_lock();
		draw_gear();
		draw_power(feed_status);

		for(int i=0;i<4;i++)
		{
			for(int j=0;j<2;j++)
			{
				prev_feed_time[i][j]=feed_time[i][j];

			}
			prev_feed_amount[i]=feed_amount[i];
		}

		prev_feed_number=feed_number;

		//sprintf(str,"%d:%d",feed_time[0][0],feed_time[0][1]);
		//TFT9341_String(140,70,str);

		//TFT9341_String(51,2,"Voermachine!");
		//TFT9341_DrawRect(0xFFFF,5,5,300,10);
		//TFT9341_DrawLine(0xFFFF,5,26,315,26);
		//TFT9341_String(5,34,"13g = 5.4s");
		/*
		TFT9341_SetFont(&Font24);
		TFT9341_String(140,70,"08:00 10g");
		TFT9341_String(140,110,"13:00 10g");
		TFT9341_String(140,150,"18:00 10g");

		 */
		TFT9341_String(50,210,"Ma");
		TFT9341_String(88,210,"Di");
		TFT9341_String(126,210,"Wo");
		TFT9341_String(164,210,"Do");
		TFT9341_String(202,210,"Vr");
		TFT9341_String(240,210,"Za");
		TFT9341_String(278,210,"Zo");



		//TFT9341_DrawLine(0x0000,50+38*(0),232,115+38*(6),232);
		int m;
		for(int i=1;i<8;i++)
		{
			/*
				m=i-1;
				if(m==0)
				{
					m=7;
				}
			 */
			//m=i;
			m=i-1;
			if(m==0)
			{
				m=7;
			}
			if(curr_day==i)
			{
				//TFT9341_DrawLine(0xffff,50+38*(m-1),232,80+38*(m-1),232); //115
				TFT9341_FillRect(50+38*(m-1), 232, 80+38*(m-1),234, 0xffff);
			}
			else
			{
				//TFT9341_DrawLine(0x0000,50+38*(m-1),232,80+38*(m-1),232);

				TFT9341_FillRect(50+38*(m-1), 232, 80+38*(m-1),234, 0x0000);
			}

		}

		//sprintf(str,"%d",skip_meal);
		//TFT9341_String(30,180,str);

	}




	if(prev_feed_status!=feed_status)
	{
		draw_power(feed_status);
		prev_feed_status=feed_status;
	}




	if(selection!=prev_selection && selection_wheel==-1)
	{
		limit_selection(0, 2);
		prev_selection=selection;
		switch(selection)
		{
		case 0:
		{
			TFT9341_FillRect(5,50,45,53,0xFD60);
			TFT9341_FillRect(20,175,120,178,0x0000);
			TFT9341_FillRect(5,235,35,238,0x0000);
			break;
		}
		case 1:
		{
			TFT9341_FillRect(5,50,45,53,0x0000);
			TFT9341_FillRect(20,175,120,178,0xFD60);
			TFT9341_FillRect(5,235,35,238,0x0000);
			/*
			if(button_pressed==true)
			{
				feed_status=1-feed_status;
				//draw_main();
				button_pressed=false;
			}
			 */
			break;
		}
		case 2:
		{
			TFT9341_FillRect(5,50,45,53,0x0000);
			TFT9341_FillRect(20,175,120,178,0x0000);
			TFT9341_FillRect(5,235,35,238,0xFD60);
			break;
		}
		}
	}

	if(selection_wheel==0)
	{
		short offset=48;
		short middle_move=5;
		short outer_move=10;
		limit_selection(0, 2);
		if(once==0)
		{
			once=1;
			//TFT9341_FillRect(55, 35, 320-55,40 , 0x00f0);
			TFT9341_FillRect(5, 30, 320-5,160 , 0x0000);

			TFT9341_SetTextColor(0xffff);
			TFT9341_SetFont(&Font16);

			if(feed_status==1)
			{
				TFT9341_String(12, 40, "Uitzetten? Of volgende");
			}
			else
			{
				TFT9341_String(12, 40, "Aanzetten? Of volgende");
			}

			TFT9341_String(12, 60, "voermoment skippen?");

			TFT9341_FillRect(65-offset-outer_move, 85, 150-offset-outer_move, 125, 0x0E3F);
			TFT9341_FillRect(170-offset-10, 85, 255-offset+10, 125, 0x0E3F);
			TFT9341_FillRect(275-offset+outer_move, 85, 360-offset, 125, 0x0E3F);
			TFT9341_SetFont(&Font20);
			TFT9341_SetTextColor(0xf000);
			TFT9341_SetBackColor(0x0E3F);
			TFT9341_String(85-offset-12-outer_move,98,"Terug");
			if (feed_status==1)
			{

				TFT9341_String(195-offset-30+20,98,"Uit");
			}
			else
			{

				TFT9341_String(195-offset-30+20,98,"Aan");
			}

			TFT9341_String(305-offset-14,98,"Skip");
			TFT9341_SetBackColor(0x0000);

		}

		if(selection!=prev_selection)
		{

			if(selection==0)
			{
				TFT9341_FillRect(275-offset+outer_move, 135, 360-offset, 145, 0x0000);
				TFT9341_FillRect(170-offset-middle_move, 135, 255-offset+middle_move, 145, 0x0000);
				TFT9341_FillRect(65-offset-outer_move, 135, 150-offset-outer_move, 145, 0xf000);
			}
			if(selection==1)
			{
				TFT9341_FillRect(275-offset+outer_move, 135, 360-offset, 145, 0x0000);
				TFT9341_FillRect(170-offset-middle_move, 135, 255-offset+middle_move, 145, 0xf000);
				TFT9341_FillRect(65-offset-outer_move, 135, 150-offset-outer_move, 145, 0x0000);
			}
			if(selection==2)
			{
				TFT9341_FillRect(275-offset+outer_move, 135, 360-offset, 145, 0xf000);
				TFT9341_FillRect(170-offset-middle_move, 135, 255-offset+middle_move, 145, 0x0000);
				TFT9341_FillRect(65-offset-outer_move, 135, 150-offset-outer_move, 145, 0x0000);

			}
			prev_selection=selection;
			//sprintf(str,"%d",selection);
			//TFT9341_String(140,70,str);
		}
	}
	else
	{
		once=0;
	}
	/*
	switch(selection)
	{
	case 0:
		if(button_pressed==true)
		{
			menu=1;
			button_pressed=false;
		}
		break;
		//TFT9341_FillRect(5,5,300,10,0xff);
	}
	 */


	/*
	for (i=0;i<60;i++){
			for (j=0;j<60;j++){
					TFT9341_DrawPixel(100+j,60+i,image[k]);
					k++;
			}
		}
	 */



}

void button(uint16_t x,uint16_t y,uint16_t r)
{
	TFT9341_DrawCircle(x,y,r,0xffff);
	TFT9341_DrawRect(0xFFFF,5,120,50,140);
	HAL_Delay(1000);
	TFT9341_DrawRect(0xFFFF,4,119,51,141);
	HAL_Delay(500);
	TFT9341_DrawRect(0x000f,4,119,51,141);
}

void test(uint16_t x_cor, uint16_t y_cor)
{
	for(int x=x_cor;x<x_cor+60;x++){
		for(int y=y_cor;y<y_cor+20;y++){
			if((x==x_cor)&&(y==y_cor))
			{
				TFT9341_DrawPixel(x,y,0xffff);
			}
		}
	}
}	

void draw_extra_options(void) //overige instellingen
{
	char str[50];
	short x_pos=15;
	short y_pos[7];
	y_pos[0]=15;

	if(sub_menu!=prev_sub_menu||update)
	{
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}
		TFT9341_FillScreen(0x0000);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);



		//sprintf(str,"Overige instellingen");
		//TFT9341_String(x_pos,y_pos[0],str);
		sprintf(str,"Time-out scherm : %s",timeout_vis());
		TFT9341_String(x_pos,y_pos[0],str);
		short percentage=screen_brightness/655;
		sprintf(str,"Schermhelderheid:%d %%",percentage);
		TFT9341_String(x_pos,y_pos[1],str);
		if(sound_feed==1)
		{
			sprintf(str,"Geluid voeren       : AAN");
			TFT9341_String(x_pos,y_pos[2],str);
		}
		else
		{
			sprintf(str,"Geluid voeren       : UIT");
			TFT9341_String(x_pos,y_pos[2],str);
		}

		if(food_shake==1)
		{
			sprintf(str,"Trillen voer        : AAN");
			TFT9341_String(x_pos,y_pos[3],str);
		}
		else
		{
			sprintf(str,"Trillen voer        : UIT");
			TFT9341_String(x_pos,y_pos[3],str);
		}

		if(sound_intensity==0)
		{
			sprintf(str,"Geluid knoppen      : UIT  ");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==1)
		{
			sprintf(str,"Geluid knoppen      : ZACHT");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==2)
		{
			sprintf(str,"Geluid knoppen      : NORM ");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==3)
		{
			sprintf(str,"Geluid knoppen      : HARD ");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		//sprintf(str,"Geluid intensiteit  : HOOG");
		//TFT9341_String(x_pos,y_pos[4],str);
		TFT9341_FillRect(x_pos,y_pos[0]+20,x_pos+80,y_pos[0]+22,0xFD60); // draw selection on #0
		draw_back();
		prev_sub_menu=sub_menu;
	}
	/*
	if(selection>6)
	{
		selection=0;
	}
	if(selection<0)
	{
	selection=6;
	}
	 */
	if(selection_wheel==-1)
	{
		limit_selection(0, 5);
	}

	if(selection!=prev_selection ||update)
	{
		update=0;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		if(selection!=5)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0x0000);
		}
		else if(selection_wheel==-1)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0xFD60);
		}
		for(int i=0;i<5;i++)
		{
			if(selection!=i)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0x0000);
			}
			else if(selection_wheel==-1)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0xFD60);
			}
		}
		if(selection_wheel==-1)
		{
			prev_selection=selection;
		}
	}


	//---------- selection_wheel -------------------

	if(selection!=prev_selection && selection_wheel==0) //time_out
	{
		prev_selection=selection;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		limit_selection(0,4);
		time_out=selection;
		TFT9341_FillRect(220,y_pos[0]+20,250,y_pos[0]+22,0xf000);
		sprintf(str,"Time-out scherm : %s",timeout_vis());
		TFT9341_String(x_pos,y_pos[0],str);

	}
	if(selection!=prev_selection && selection_wheel==1)
	{
		prev_selection=selection;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		limit_selection(1, 20);
		TIM1->CCR1=(65500/20)*selection;
		screen_brightness=(65500/20)*selection;
		//sprintf(str,"%d",screen_brightness);
		//TFT9341_String(250,200,str);
		int percentage=screen_brightness/655;

		sprintf(str,"Schermhelderheid:%d %%   ",percentage);
		TFT9341_String(x_pos,y_pos[1],str);

	}
	if(selection!=prev_selection && selection_wheel==2) // geluid bij voeren
	{
		prev_selection=selection;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		limit_selection(0,1);
		sound_feed=selection;
		TFT9341_FillRect(258,y_pos[2]+20,288,y_pos[2]+22,0xf000);

		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);

		if(sound_feed==1)
		{
			sprintf(str,"Geluid voeren       : AAN");
			TFT9341_String(x_pos,y_pos[2],str);
		}
		else
		{
			sprintf(str,"Geluid voeren       : UIT");
			TFT9341_String(x_pos,y_pos[2],str);
		}

	}
	if(selection!=prev_selection && selection_wheel==3) // geluid bij knoppen
	{
		prev_selection=selection;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		limit_selection(0,1);
		food_shake=selection;
		TFT9341_FillRect(258,y_pos[3]+20,288,y_pos[3]+22,0xf000);

		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);

		if(food_shake==1)
		{
			sprintf(str,"Trillen voer        : AAN");
			TFT9341_String(x_pos,y_pos[3],str);
		}
		else
		{
			sprintf(str,"Trillen voer        : UIT");
			TFT9341_String(x_pos,y_pos[3],str);
		}

	}
	if(selection!=prev_selection && selection_wheel==4) // geluidintensiteit
	{
		prev_selection=selection;
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		limit_selection(0,3);
		sound_intensity=selection;
		TFT9341_FillRect(258,y_pos[4 ]+20,288,y_pos[4]+22,0xf000);

		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);

		if(sound_intensity==0)
		{
			sprintf(str,"Geluid knoppen      : UIT  ");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==1)
		{
			sprintf(str,"Geluid knoppen      : ZACHT");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==2)
		{
			sprintf(str,"Geluid knoppen      : NORM ");
			TFT9341_String(x_pos,y_pos[4],str);
		}
		if(sound_intensity==3)
		{
			sprintf(str,"Geluid knoppen      : HARD ");
			TFT9341_String(x_pos,y_pos[4],str);
		}

	}


	//TFT9341_DrawRect(0xffff,5,235,5+60,235-42);



}

void draw_options()
{

	char str[50];
	short x_pos=15;
	short y_pos[6];
	y_pos[0]=20;
	if(menu!=prev_menu || sub_menu!=prev_sub_menu || update)
	{
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
				{
					y_pos[i]=y_pos[i-1]+30;
				}
		update=0;
		selection=0;
		TFT9341_FillScreen(0x0000);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);



		sprintf(str,"Voertijden/hoeveelheden");
		TFT9341_String(x_pos,y_pos[0],str);
		sprintf(str,"Calibreer voerhoeveelheid");
		TFT9341_String(x_pos,y_pos[1],str);
		sprintf(str,"Tijd instellen");
		TFT9341_String(x_pos,y_pos[2],str);
		sprintf(str,"Overige instellingen");
		TFT9341_String(x_pos,y_pos[3],str);
		sprintf(str,"Instellingen opslaan");
		TFT9341_String(x_pos,y_pos[4],str);

		TFT9341_FillRect(x_pos,y_pos[0]+20,x_pos+80,y_pos[0]+22,0xFD60);
		draw_back();

		//draw only once
		prev_menu=menu;
		prev_sub_menu=sub_menu;
	}




	if(selection!=prev_selection)
	{
		prev_selection=selection;
		limit_selection(0, 5);
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
				{
					y_pos[i]=y_pos[i-1]+30;
				}
		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		if(selection==5)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0xFD60);
		}
		if(selection!=5)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0x0000);
		}
		for(int i=0;i<((sizeof (y_pos))/(sizeof (short)))-1;i++)
		{
			if(selection!=i)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0x0000);
			}
			else
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0xFD60);
			}


		}

	}




	//TFT9341_DrawRect(0xffff,5,235,5+60,235-42);



}

void feed_set(void) //activates on button_press since it is in that function --> perhaps not the way to go since selection should also change the screen
{
	/*
	if(selection_wheel==-1)
	{
		if(selection>feed_number+1)
		{
			selection=0;
		}
		if(selection<0)
		{
		selection=5;
		}
	}
	 */


	char str[20];	
	short x_pos=15;
	short y_pos[5];
	y_pos[0]=20;



	//sprintf(str,"selection=%d",selection);
	//TFT9341_String(180,160,str);

	if(menu!=prev_menu || sub_menu!=prev_sub_menu || feed_number!=prev_feed_number ||update) //change upon sub_menu will draw this once
	{
		prev_selection=-1;
		selection=0;
		TFT9341_FillScreen(0x0000);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);

		sprintf(str,"Voermomenten:%d",feed_number);
		TFT9341_String(15,20,str);

		//TFT9341_String(15,20,"Aantal Voermomenten:");
		for(int i=0;i<feed_number;i++)
		{
			//sprintf(str,time_text(feed_time[i][1],feed_time[i][2]));
			TFT9341_String(15,20+(i+1)*30,time_text(feed_time[i][0],feed_time[i][1],1));
			sprintf(str,"%d g   ",feed_amount[i]);


			TFT9341_SetTextColor(0xffff);
			TFT9341_String(110,20+(i+1)*30,str);

		}


		draw_back();
		prev_menu=menu;
		prev_sub_menu=sub_menu;
		prev_feed_number=feed_number;
	}

	if(selection_wheel==-1)
	{
		limit_selection(0,feed_number+1);
	}

	if(selection!=prev_selection || update)
	{

		for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
		{
			y_pos[i]=y_pos[i-1]+30;
			//sprintf(str,"%d",y_pos[i]);
			//TFT9341_String(100+30*i,90+10*i,str);
		}

		if(selection==feed_number+1  && selection_wheel==-1)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0xFD60);
		}
		if(selection!=feed_number+1  && selection_wheel==-1)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0x0000);
		}
		for(int i=0;i<feed_number+1;i++)
		{
			if(selection!=i) // && selection_wheel==-1
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0x0000);
			}
			else if(selection_wheel==-1)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0xFD60);
			}

			if(selection_wheel!=-1)
			{
				if(i!=selection_wheel)
				{
					TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0x0000);
				}
			}

			//sprintf(str,"%d",y_pos[i]);
			//TFT9341_String(100+30*i,50+10*i,str);
		}
		update=0;
	}

	if(selection_wheel==0 && selection!=prev_selection)
	{
		limit_selection(1,4);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		sprintf(str,"Voermomenten:%d",selection);
		TFT9341_String(15,20,str);
		TFT9341_FillRect(x_pos+4,y_pos[0]+20,x_pos+90,y_pos[0]+18,0xf000);
	}

	if(selection!=prev_selection && selection_wheel!=-1 && selection_wheel>0)
	{
		if(skip==0)
		{
			limit_selection(0,23);
			feed_time[selection_wheel-1][0]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[selection_wheel-1]+50,x_pos+24,y_pos[selection_wheel-1]+48,0xf000);
		}
		else if(skip==1)
		{
			limit_selection(0,59);
			feed_time[selection_wheel-1][1]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[selection_wheel-1]+50,x_pos+24,y_pos[selection_wheel-1]+48,0x0000);
			TFT9341_FillRect(x_pos+34,y_pos[selection_wheel-1]+50,x_pos+54,y_pos[selection_wheel-1]+48,0xf000);
		}
		else if(skip==2)
		{
			limit_selection(0,100);
			feed_amount[selection_wheel-1]=selection;
			TFT9341_FillRect(x_pos+34,y_pos[selection_wheel-1]+50,x_pos+54,y_pos[selection_wheel-1]+48,0x0000);
			TFT9341_FillRect(x_pos+94,y_pos[selection_wheel-1]+50,x_pos+130,y_pos[selection_wheel-1]+48,0xf000);
			sprintf(str,"%d g   ",feed_amount[selection_wheel-1]);
			TFT9341_String(110,20+(selection_wheel)*30,str);
		}
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		TFT9341_String(15,20+(selection_wheel)*30,time_text(feed_time[selection_wheel-1][0],feed_time[selection_wheel-1][1],1));
	}


	/*

	if(selection_wheel==1 && selection!=prev_selection)
	{
		if(skip==0)
		{
			limit_selection(0,24);
			feed_time[0][0]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[0]+50,x_pos+24,y_pos[0]+42,0xf000);
		}
		else if(skip==1)
		{
			limit_selection(0,59);
			feed_time[0][1]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[0]+50,x_pos+24,y_pos[0]+42,0x0000);
			TFT9341_FillRect(x_pos+34,y_pos[0]+50,x_pos+54,y_pos[0]+42,0xf000);
		}
		else if(skip==2)
		{
			TFT9341_FillRect(x_pos+34,y_pos[0]+50,x_pos+54,y_pos[0]+42,0x0000);
			TFT9341_FillRect(x_pos+90,y_pos[0]+50,x_pos+110,y_pos[0]+48,0xf000);
		}
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		TFT9341_String(15,20+(0+1)*30,time_text(feed_time[0][0],feed_time[0][1]));
	}

	if(selection_wheel==2 && selection!=prev_selection)
	{
		if(skip==0)
		{
			limit_selection(0,24);
			feed_time[1][0]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[1]+50,x_pos+24,y_pos[1]+42,0xf000);
		}
		else if(skip==1)
		{
			limit_selection(0,59);
			feed_time[1][1]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[1]+50,x_pos+24,y_pos[1]+42,0x0000);
			TFT9341_FillRect(x_pos+34,y_pos[1]+50,x_pos+54,y_pos[1]+42,0xf000);
		}
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		TFT9341_String(15,20+(0+2)*30,time_text(feed_time[1][0],feed_time[1][1]));
	}
	if(selection_wheel==3 && selection!=prev_selection)
	{
		if(skip==0)
		{
			limit_selection(0,24);
			feed_time[2][0]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[2]+50,x_pos+24,y_pos[2]+42,0xf000);
		}
		else if(skip==1)
		{
			limit_selection(0,59);
			feed_time[2][1]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[2]+50,x_pos+24,y_pos[2]+42,0x0000);
			TFT9341_FillRect(x_pos+34,y_pos[2]+50,x_pos+54,y_pos[2]+42,0xf000);
		}
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		TFT9341_String(15,20+(0+3)*30,time_text(feed_time[2][0],feed_time[2][1]));
	}
	if(selection_wheel==4 && selection!=prev_selection)
	{
		if(skip==0)
		{
			limit_selection(0,24);
			feed_time[3][0]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[3]+50,x_pos+24,y_pos[3]+42,0xf000);
		}
		else if(skip==1)
		{
			limit_selection(0,59);
			feed_time[3][1]=selection;
			TFT9341_FillRect(x_pos+4,y_pos[3]+50,x_pos+24,y_pos[3]+42,0x0000);
			TFT9341_FillRect(x_pos+34,y_pos[3]+50,x_pos+54,y_pos[3]+42,0xf000);
		}
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		TFT9341_String(15,20+(0+4)*30,time_text(feed_time[3][0],feed_time[3][1]));
	}
	 */


	prev_selection=selection;

	/*
	if(button_pressed==true && selection!=4)
	{
		bool back=false;
		while(back==false)
		{

		}

	}
	 */

}

void set_time(void)
{
	char str[10];
	short x_pos=15;
	short y_pos[6];
	y_pos[0]=20;
	y_pos[1]=50;
	//
	if(selection!=prev_selection &&selection_wheel==-1)
	{
		limit_selection(0, 2);
	}
	if(menu!=prev_menu || sub_menu!=prev_sub_menu ||update)
	{
		prev_selection=-1;
		selection=0;
		TFT9341_FillScreen(0x0000);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		sprintf(str,"Tijd: %s",time_text(rtc.Hour,rtc.Min,1));
		TFT9341_String(x_pos,y_pos[0],str);
		sprintf(str,"Dag: %s",enum_to_day(rtc.DaysOfWeek));
		TFT9341_String(x_pos,y_pos[1],str);

		prev_menu=menu;
		prev_sub_menu=sub_menu;
		update=0;
		draw_back();

	}

	if(selection!=prev_selection)
	{
		if(selection_wheel==-1)
		{
			prev_selection=selection;
		}
		x_pos=15;
		y_pos[0]=20;
		for(int i=1;i<2;i++)
		{
			y_pos[i]=y_pos[i-1]+30;
		}

		if(selection==2 && selection_wheel==-1)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0xFD60);
		}
		if(selection!=2)
		{
			TFT9341_FillRect(15,190+29+4,x_pos+40,190+29+4+2,0x0000);
		}
		for(int i=0;i<2;i++)
		{
			if(selection!=i)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0x0000);
			}
			else if(selection_wheel==-1)
			{
				TFT9341_FillRect(x_pos,y_pos[i]+20,x_pos+80,y_pos[i]+22,0xFD60);
			}


		}
	}


	if(selection_wheel==0 && selection!=prev_selection) //time adjust selected
	{
		prev_selection=selection;
		if(skip==0)
		{
			limit_selection(0, 23);
			TFT9341_FillRect(x_pos+67,y_pos[0]+20,x_pos+87,y_pos[0]+18,0xf000);

			TFT9341_SetTextColor(0xffff);
			TFT9341_SetFont(&Font16);
			rtc.Hour=selection;
			sprintf(str,"Tijd: %s",time_text(rtc.Hour,rtc.Min,1));
			TFT9341_String(x_pos,y_pos[0],str);
			sprintf(str,"Dag: %s",enum_to_day(rtc.DaysOfWeek));
			TFT9341_String(x_pos,y_pos[1],str);
		}
		if(skip==1)
		{

			limit_selection(0, 59);
			TFT9341_FillRect(x_pos+67,y_pos[0]+20,x_pos+87,y_pos[0]+18,0x0000);
			TFT9341_FillRect(x_pos+100,y_pos[0]+20,x_pos+120,y_pos[0]+18,0xf000);
			TFT9341_SetTextColor(0xffff);
			TFT9341_SetFont(&Font16);
			rtc.Min=selection;
			sprintf(str,"Tijd: %s",time_text(rtc.Hour,rtc.Min,1));
			TFT9341_String(x_pos,y_pos[0],str);
			sprintf(str,"Dag: %s",enum_to_day(rtc.DaysOfWeek));
			TFT9341_String(x_pos,y_pos[1],str);
		}
	}
	if(selection_wheel==1 && selection!=prev_selection)
	{
		prev_selection=selection;
		limit_selection(1,7);
		rtc.DaysOfWeek=selection;
		TFT9341_FillRect(x_pos+55,y_pos[0]+50,x_pos+75,y_pos[0]+48,0xf000);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);
		sprintf(str,"Dag: %s",enum_to_day(rtc.DaysOfWeek));
		TFT9341_String(x_pos,y_pos[1],str);
	}
	/*
	for(int i=1;i<((sizeof (y_pos))/(sizeof (short)));i++)
	{
		y_pos[i]=y_pos[i-1]+30;
	}
	 */

}



void calibrate_feed(void)
{
	char str[100];
	if(menu!=prev_menu || sub_menu!=prev_sub_menu ||update)
	{
		prev_selection=-1;
		if(skip!=4)
		{
			selection=0;
		}
		TFT9341_FillScreen(0x0000);
		TFT9341_SetTextColor(0xffff);

		prev_menu=menu;
		prev_sub_menu=sub_menu;
		update=0;
		draw_back();



		if(skip==0)
		{
			sprintf(str,"Er wordt 20 seconden");
			TFT9341_String(15, 20, str);
			sprintf(str,"lang voer uitgedraaid.");
			TFT9341_String(15, 50, str);
			sprintf(str,"Druk op de knop om");
			TFT9341_String(15, 80, str);
			sprintf(str,"verder te gaan.");
			TFT9341_String(15, 110, str);

		}
		else if(skip==1)
		{
			sprintf(str,"Pak een weegschaal");
			TFT9341_String(15, 20, str);
			sprintf(str,"en een lege kom om de");
			TFT9341_String(15, 50, str);
			sprintf(str,"hoeveelheid voer te meten.");
			TFT9341_String(15, 80, str);
		}
		else if(skip==2)
		{
			sprintf(str,"Houd eerst de knop");
			TFT9341_String(15, 20, str);
			sprintf(str,"lang ingedrukt totdat er");
			TFT9341_String(15, 50, str);
			sprintf(str,"voer uitkomt. Haal dit wel");
			TFT9341_String(15, 80, str);
			sprintf(str,"uit de bak voordat je naar");
			TFT9341_String(15, 110, str);
			sprintf(str,"de volgende stap gaat.");
			TFT9341_String(15, 140, str);
		}
		else if(skip==3)
		{
			sprintf(str,"In de volgende stap");
			TFT9341_String(15, 20, str);
			sprintf(str,"wordt de 20 seconden lang");
			TFT9341_String(15, 50, str);
			sprintf(str,"voer uitgedraaid.");
			TFT9341_String(15, 80, str);
		}
		else if(skip==4)
		{
			sprintf(str,"Gemeten hoeveelheid");
			TFT9341_String(15, 20, str);
			sprintf(str,"voer: ");
			TFT9341_String(15, 50, str);

		}
		else if(skip==5)
		{
			sprintf(str,"De voerhoeveelheid is");
			TFT9341_String(15, 20, str);
			sprintf(str,"gecalibreerd!");
			TFT9341_String(15, 50, str);
			sprintf(str,"Calibratiewaarde:%d  ",feed_cal);
			TFT9341_String(15, 80, str);
		}

	}

	if(selection!=prev_selection)
	{
		prev_selection=selection;

		if(skip!=4)
		{
			limit_selection(0, 1);
			if(selection==0)
			{
				TFT9341_FillRect(15,190+29+4,15+40,190+29+4+2,0x0000); //streep onder terugknop verwijderen.
			}
			else
			{
				TFT9341_FillRect(15,190+29+4,15+40,190+29+4+2,0xFD60);
			}
		}
		else //skip==4
		{
			limit_selection(1,99);
			feed_cal=selection;
			sprintf(str,"%d g   ",feed_cal);
			TFT9341_String(80, 50, str);
		}
	}




}




