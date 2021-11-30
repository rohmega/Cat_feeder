#include "spi_ili9341.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "interface.h"
#include "stm32_ds3231.h"
#include "stdbool.h"
#include "consolidate.h"
#include "w25qxx.h"
#include "tone.h"


extern short feed_time[4][2];
extern short feed_amount[4];
extern char curr_day[2];
extern short selection;
extern long text_colour;

extern short feed_number;
extern short prev_selection;

extern short sub_menu;
extern short prev_sub_menu;extern bool pressed;
extern short menu;
extern short prev_menu;
extern bool button_pressed; 
extern short selection_wheel;
extern uint16_t screen_brightness;
extern short feed_cal;
extern long speed;
extern short screen_off;

extern short curr_time[2];
char str[8];
extern short update;

extern _RTC rtc;

short skip=0;

//bool expressions to save
extern bool feed_status;
extern bool sound_feed;
extern bool food_shake;


extern uint16_t sector_increment;

extern short sound_intensity;
extern short time_out;
extern bool refresh;
extern long timeout_old;
long feed_time_old=0;
long feed_time_new=0;
long feed_time_diff=0;
bool first_time=0;
extern uint8_t curr_sector;
extern long timeout_time;
short feed_once=0;
short feeders=-1;

extern uint8_t buffer[22];
//




void menu_draw(void)
{
	switch(menu)
	{
	case 0: // do not confuse this with the counting in selection 0 is main screen, 1 is the settings screen. 
		draw_main();
		switch(sub_menu)
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		}
		break;

		case 1:
			switch(sub_menu)
			{
			case -1:
				draw_options();
				break;
			case 0:
				feed_set();
				break;
			case 1:
				calibrate_feed();
				break;
			case 2:
				set_time();
				break;
			case 3:
				draw_extra_options();
				break;
			}
			break;
	}

}

void selection_action(void)
{
	if(button_pressed==true)
	{
		//play_tune();
		switch(menu)
		{
		case 0: //main menu
			switch(selection_wheel) // no submenu in main menu
			{
			case -1: //normal navigation in main menu

				switch(selection)
				{
				case 0:
					menu=1; // options menu (it makes more sense from this point onward)
					break;
				case 1:
					selection_wheel=0;
					// add some verification --> are you sure?
					selection=0;
					break;
				case 2:
					//lock the device
					timeout_old=TIM2->CNT-timeout_time-10;
					screen_off=1;

					HAL_Delay(100);
					break;
				}
				break;

				case 0: //selection_wheel=0 --> verification
					if(selection==1)
					{
						feed_status=1-feed_status;
						write_vars(buffer);
					}
					update=1;
					selection_wheel=-1;
					break;
			}
			break;


			case 1: //options menu
				switch(sub_menu)
				{
				case -1: //no sub_menu selected
					switch(selection)
					{
					case 0:
						//voertijden/hoeveelheden
						sub_menu=0;
						break;

					case 1:
						sub_menu=1;
						selection=0;
						break;

					case 2:
						sub_menu=2;
						selection=0;
						break;

					case 3:
						sub_menu=3;
						selection=0;
						break;

					case 4:
						TFT9341_FillScreen(0x0000);
						TFT9341_SetTextColor(0xffff);
						TFT9341_SetFont(&Font20);
						TFT9341_String(74,95, "Opgeslagen!");
						write_vars(buffer);
						HAL_Delay(1000);
						menu=1;
						sub_menu=-1;
						selection=4;
						update=1;
						break;

					case 5:


						menu=0;
						sub_menu=-1;
						selection=0;
						break;
					}
					break;

					case 0: //voertijden/hoeveelheden
						switch(selection_wheel)
						{
						case -1: // regular interface selection
							if(selection<=feed_number)
							{
								switch(selection)
								{
								case 0:
									selection_wheel=0;
									selection=feed_number;
									break;
								case 1:
									selection_wheel=1;
									selection=feed_time[0][0];
									break;
								case 2:
									selection_wheel=2;
									selection=feed_time[1][0];
									break;
								case 3:
									selection_wheel=3;
									selection=feed_time[2][0];
									break;
								case 4:
									selection_wheel=4;
									selection=feed_time[3][0];
									break;
								}
							}
							if(selection==feed_number+1) // variable position of return button
							{
								sub_menu=-1;
								selection_wheel=-1;
							}
							break;

						case 0:
							feed_number=selection;
							selection_wheel=-1;
							update=1;
							break;

						case 1: // adjusting first feed time
							if(skip==0)
							{
								skip++;
								selection=feed_time[0][1];
							}
							else if(skip==1)
							{
								skip++;
								selection=feed_amount[0];
							}
							else if(skip==2)
							{
								selection_wheel=-1;
								selection=1;
								skip=0;
								update=1;
							}
							break;
						case 2: // adjusting second feed time
							if(skip==0)
							{
								skip++;
								selection=feed_time[1][1];
							}
							else if(skip==1)
							{
								skip++;
								selection=feed_amount[1];
							}
							else if(skip==2)
							{
								selection_wheel=-1;
								selection=2;
								skip=0;
								update=1;
							}
							break;
						case 3: // adjusting second feed time
							if(skip==0)
							{
								skip++;
								selection=feed_time[2][1];
							}
							else if(skip==1)
							{
								skip++;
								selection=feed_amount[2];
							}
							else if(skip==2)
							{
								selection_wheel=-1;
								selection=3;
								skip=0;
								update=1;
							}
							break;
						case 4: // adjusting second feed time
							if(skip==0)
							{
								skip++;
								selection=feed_time[3][1];
							}
							else if(skip==1)
							{
								skip++;
								selection=feed_amount[3];
							}
							else if(skip==2)
							{
								selection_wheel=-1;
								selection=4;
								skip=0;
								update=1;
							}
							break;
						}
						break;

						case 1: //voer calibreren
							if(skip==4)
							{
								skip++;
								update=1;
							}
							switch(selection)
							{
							case 0:
								if(skip==0)
								{
									skip++;
									update=1;
								}
								else if(skip==1)
								{
									skip++;
									update=1;
								}
								else if(skip==2)
								{
									skip++;
									update=1;
								}
								else if(skip==3)
								{
									skip++;
									update=1;
									selection=feed_cal;
									feed_cat(20000);
								}
								else if(skip==5)
								{
									update=1;
									skip=0;
									sub_menu=-1;
								}
								break;

							case 1:
								sub_menu=-1;
								skip=0;
								break;
							}

							break;


							case 2: //tijd instellen
								switch(selection_wheel)
								{
								case-1: // no selection made
									switch(selection)
									{
									case 0:
										selection_wheel=0;
										selection=rtc.Hour;
										break;
									case 1:
										selection_wheel=1;
										selection=rtc.DaysOfWeek;
										break;
									case 2:
										sub_menu=-1;
										break;
									}
									break;
									case 0: // tijd
										if(skip==0)
										{
											skip++;
											selection=rtc.Min;
										}
										else if(skip==1)
										{
											skip=0;
											selection=0;
											selection_wheel=-1;
											update=1;
											DS3231_SetTime(&rtc);
										}
										break;

									case 1: // dag
										selection=1;
										selection_wheel=-1;
										update=1;
										DS3231_SetTime(&rtc);
										break;

								}
								break; // tijd instellen break

								case 3: //submenu overige instellingen
									switch(selection_wheel)
									{
									case-1: //no selection made
										switch(selection)
										{
										case 0:
											selection_wheel=0;
											selection=time_out;
											//refresh=1;
											break;
										case 1:
											selection_wheel=1;
											selection=(screen_brightness/65500)*20;
											//refresh=1;
											break;
										case 2:
											selection_wheel=2;
											selection=sound_feed;
											//refresh=1;
											break;
										case 3:
											selection_wheel=3;
											selection=food_shake;
											//refresh=1;
											break;
										case 4:
											selection_wheel=4;
											selection=sound_intensity;
											//refresh=1;
											break;
										case 5:
											sub_menu=-1;
											break;
										}
										break;
										case 0:
											//0=10s, 1=20s, 2=30s, 3=1min, 4=5min, 5=inf
											switch(time_out)
											{
											case 0:
												timeout_time=10000; // in ms
												break;
											case 1:
												timeout_time=20000; // in ms
												break;
											case 2:
												timeout_time=30000; // in ms
												break;
											case 3:
												timeout_time=60000; // in ms
												break;
											case 4:
												timeout_time=300000; // in ms
												break;
											}

											selection_wheel=-1;
											selection=0;
											update=1;
											break;
											case 1: //after setting brightness
												selection_wheel=-1;
												selection=1;
												update=1;
												break;
											case 2:
												selection_wheel=-1;
												selection=2;
												update=1;
												break;
											case 3:
												selection_wheel=-1;
												selection=3;
												update=1;
												break;
											case 4:
												selection=4;
												selection_wheel=-1;
												update=1;
												break;
									}
									break;

				}
				break;

				case 2: // lock the device

					break;


		}
		button_pressed=false;
	}

}

char* time_text(short Hour,short Min,bool dots)
{
	if(Hour<10 || Min<10)
	{
		if(Hour<10 && Min<10)
		{
			curr_time[0]=Hour;
			if(dots==1)
			{
				sprintf(str,"%d%d:%d%d",0,Hour,0,Min);
			}
			else
			{
				sprintf(str,"%d%d %d%d",0,Hour,0,Min);
			}
			return str;

		}
		if(Hour>=10)
		{
			if(dots==1)
			{
				sprintf(str,"%d:%d%d",Hour,0,Min);
			}
			else
			{
				sprintf(str,"%d %d%d",Hour,0,Min);
			}
			return str;
		}
		if(Min>=10)
		{
			if(dots==1)
			{
				sprintf(str,"%d%d:%d",0,Hour,Min);
			}
			else
			{
				sprintf(str,"%d%d %d",0,Hour,Min);
			}
			return str;
		}

	}
	else 
	{
		if(dots==1)
		{
			sprintf(str,"%d:%d",Hour,Min);
		}
		else
		{
			sprintf(str,"%d %d",Hour,Min);
		}
		return str;
	}
	return "00:00";
}

char* enum_to_day(uint8_t day_num)
{
	switch(day_num)
	{
	case 1:
		return "Zo";
	case 2:
		return "Ma";
	case 3:
		return "Di";
	case 4:
		return "Wo";
	case 5:
		return "Do";
	case 6:
		return "Vr";
	case 7:
		return "Za";
	}
	return 0;
}

void limit_selection(short L,short N)
{
	if(selection>N)
	{
		selection=L;
	}
	if(selection<L)
	{
		selection=N;
	}
}

char* timeout_vis(void)
{
	switch(time_out)
	{
	case 0:
		return "10s ";
		break;
	case 1:
		return "20s ";
		break;
	case 2:
		return "30s ";
		break;
	case 3:
		return "1min";
		break;
	case 4:
		return "5min";
		break;
	}
	return 0;
}

void feed_cat(short time)
{
	//char str[10];
	motor_intensity(0);
	if(first_time==0)
	{
		HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
		first_time=1;
		feed_time_old=TIM2->CNT;
		feed_time_diff=(TIM2->CNT-feed_time_old);
		TIM3->CNT=0;
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
		//TFT9341_FillScreen(0xFD90);
	}
	while(feed_time_diff<time) //
	{
		feed_time_diff=(TIM2->CNT-feed_time_old);
		if(TIM3->CNT>speed)
		{
			TIM3->CNT=0;
			HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
		}
		first_time=0;

	}
	if(feed_time_diff>=time)
	{
		if(food_shake==1)
		{
			motor_intensity(2);
			play_tone("e2",800);
			HAL_Delay(1000);
		}
		if(sound_feed==1)
		{
			motor_intensity(0);
			play_tone("e4",100);
			play_tone("b4",200);
		}
		HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
		TIM3->CNT=0;
		first_time=0;
		return;
	}




}


void write_vars(uint8_t buffer[21])
{
	/*
	uint8_t buffer1[24];
	W25qxx_ReadSector(buffer1, 1, 0, 23);
	sector_increment=buffer1[21]|buffer1[22]<<8;
	sector_increment++;
	curr_sector=buffer1[23];
	 */
	buffer[0]=feed_status<<2|sound_feed<<1|food_shake; // first three bits contain these in order 011=3
	buffer[1]=feed_time[0][0];
	buffer[2]=feed_time[0][1];
	buffer[3]=feed_time[1][0];
	buffer[4]=feed_time[1][1];
	buffer[5]=feed_time[2][0];
	buffer[6]=feed_time[2][1];
	buffer[7]=feed_time[3][0];
	buffer[8]=feed_time[3][1];
	buffer[9]=feed_time[4][0];
	buffer[10]=feed_time[4][1];
	buffer[11]=feed_amount[0];
	buffer[12]=feed_amount[1];
	buffer[13]=feed_amount[2];
	buffer[14]=feed_amount[3];
	buffer[15]=feed_number;
	buffer[16]=screen_brightness & 0x00ff; //low byte
	buffer[17]=screen_brightness >> 8; // high byte
	buffer[18]=sound_intensity;
	buffer[19]=time_out;
	buffer[20]=feed_cal;
	/*
	sector_increment=65534;
	if(sector_increment>=65533)
	{

		curr_sector++;
		buffer[23]=curr_sector;
		sector_increment=65534;
		buffer[21]=sector_increment & 0x00ff ;//sector_increment low byte
		buffer[22]=sector_increment >>8; // sector_increment high byte
		W25qxx_EraseSector(1);
		W25qxx_WriteSector(buffer, 1, 0, 23);
		sector_increment=0;
	}

	buffer[21]=sector_increment & 0x00ff ;//sector_increment low byte
	buffer[22]=sector_increment >>8; // sector_increment high byte
	//curr_sector=1;
	buffer[23]=curr_sector;
	 */
	W25qxx_EraseSector(1);
	W25qxx_WriteSector(buffer, 1, 0, 21); // write to sector 1 and write bytes 0-23

	return;

}

void read_vars(void)
{
	uint8_t buffer[24]={0};
	/*

	W25qxx_ReadSector(buffer, 1, 0, 23);
	curr_sector=buffer[23];
	//curr_sector=1;
	W25qxx_ReadSector(buffer, curr_sector, 0, 23);
	 */
	W25qxx_ReadSector(buffer, 1, 0, 21);
	feed_status=(buffer[0] & 4)>>2; // select third bit
	sound_feed=(buffer[0]&2)>>1; // select second bit
	food_shake=buffer[0]&1; // select first bit
	feed_time[0][0]=buffer[1];
	feed_time[0][1]=buffer[2];
	feed_time[1][0]=buffer[3];
	feed_time[1][1]=buffer[4];
	feed_time[2][0]=buffer[5];
	feed_time[2][1]=buffer[6];
	feed_time[3][0]=buffer[7];
	feed_time[3][1]=buffer[8];
	feed_time[4][0]=buffer[9];
	feed_time[4][1]=buffer[10];
	feed_amount[0]=buffer[11];
	feed_amount[1]=buffer[12];
	feed_amount[2]=buffer[13];
	feed_amount[3]=buffer[14];
	feed_number=buffer[15];
	screen_brightness=buffer[16]|buffer[17]<<8;
	sound_intensity=buffer[18];
	time_out=buffer[19];
	feed_cal=buffer[20];
}

void feed_cat_time(void)
{

	for(int i=0;i<feed_number;i++)
	{
		if(rtc.Hour==feed_time[i][0] && rtc.Min==feed_time[i][1] && feed_once==0)
		{
			feeders=i;
			feed_once=1;
			if(feed_status==1)
			{
				feed_cat(20000/feed_cal*feed_amount[i]);
			}
		}
		else if(rtc.Hour!=feed_time[i][0] || rtc.Min!=feed_time[i][1])
		{
			if(feeders==i)
			{
				feed_once=0;
				feeders=-1;
			}
		}


	}
}

void motor_driver(short status)
{
	if(status==1)
	{
		HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
	}
	else if(status==0)
	{
		HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
	}
	return;
}

void motor_intensity(short sound_intensity)
{
	//0 zacht,1 middel, 2 hard
	if(sound_intensity==2)
	{
		HAL_GPIO_WritePin(MS1_GPIO_Port,MS1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port,MS2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS3_GPIO_Port,MS3_Pin,GPIO_PIN_RESET);
	}
	else if(sound_intensity==1)
	{
		HAL_GPIO_WritePin(MS1_GPIO_Port,MS1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MS2_GPIO_Port,MS2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS3_GPIO_Port,MS3_Pin,GPIO_PIN_RESET);
	}
	else if(sound_intensity==0)
	{
		HAL_GPIO_WritePin(MS1_GPIO_Port,MS1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS2_GPIO_Port,MS2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(MS3_GPIO_Port,MS3_Pin,GPIO_PIN_SET);
	}
	return;
}
