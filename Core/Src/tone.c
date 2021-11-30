#include "spi_ili9341.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "interface.h"
#include "main.h"
#include "consolidate.h"

//uint16_t ladder[48]={4287,4542,4812,5097,5401,5722,6062,6422,6804,7209,7637,8092,8573,9082,9623,10195,10801,11443,12124,12845,13608,14418,15275,16183,17146,18165,19245,20390,21602,22887,24247,25690,27217,28835,30550,32366,34291,36331,38491,40779,43204,45774,48495,51379,54434,57671,61100,64734};
uint16_t ladder[48]={47776,45094,42563,40177,37920,35792,33784,31888,30097,28409,26815,25310,23890,22549,21283,20089,18961,17897,16892,15944,15049,14205,13407,12655,11944,11274,10642,10044,9480,8948,8446,7972,7525,7102,6704,6327,5972,5637,5321,5022,4740,4474,4223,3986,3762,3551,3352,3164};
					//47776
 //c2-b5
char ladder_text[48][3]={"c2","c#2","d2","d#2","e2","f2","f#2","g2","g#2","a2","a#2","b2","c3","c#3","d3","d#3","e3","f3","f#3","g3","g#3","a3","a#3","b3","c4","c#4","d4","d#4","e4","f4","f#4","g4","g#4","a4","a#4","b4"};
bool tone_played=false;
bool timer_reset=false;
uint16_t curr_tone;
short match=-1;
short count=0;
bool reset_tone=false;
//extern long tone_old,tone_diff,tone_now;
int tone_time_old;
int tone_time_new;
int tone_time_diff;
void play_tone(char note[3],int length)
{
	motor_driver(1);
	//plays a single instance of the right frequency
	if(timer_reset==false)
	{
		TIM3->CNT = 0;
		for(int i=0;i<48;i++)
		{
			if(strcmp(note,ladder_text[i])==0)
			{
				curr_tone=ladder[i];
				match=i;
			}
		}
		timer_reset=true;
		tone_time_old=TIM2->CNT;
	}
	//if(timer_reset==true)
	tone_time_new=TIM2->CNT;
	tone_time_diff=0;
	while(tone_time_diff<length || count!=0)
	{
			tone_time_diff=(tone_time_new-tone_time_old);
			tone_time_new=TIM2->CNT;
			if(TIM3->CNT>curr_tone) //ladder[match]
			{

				HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
				TIM3->CNT = 0;
				count++;
				//HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);

			if(count==2)
			{
				HAL_GPIO_TogglePin(DIR_GPIO_Port,DIR_Pin);
				//count=0;
			}
			if(count==4)
			{
				count=0;
				HAL_GPIO_TogglePin(DIR_GPIO_Port,DIR_Pin);

			}
			}

		}
	if(tone_time_new-tone_time_old>length)
	{

		//match=-1;
	}
	timer_reset=false;
	motor_driver(0);
	return;


}

void play_tune(void)
{

	/* encounters of the third kind`
	play_tone("d4",800);
	play_tone("e4",800);
	play_tone("c4",800);
	play_tone("c3",800);
	play_tone("g3",1600);
	*/

	play_tone("d4",200);
	play_tone("e4",200);
	play_tone("c4",200);
	play_tone("c3",200);
	play_tone("g3",400);

	/*
	play_tone("g3",200);
	play_tone("b3",200);
	play_tone("d4",200);
	play_tone("f4",200);
	//play_tone("g3",1600);
	*/

	/*
	play_tone("g4",200);
	play_tone("d4",200);
	play_tone("c4",200);
	play_tone("g3",200);
	*/

	/*
	play_tone("c4",900);
	play_tone("d4",900);
	play_tone("e4",900);
	play_tone("f4",900);
	*/
}


