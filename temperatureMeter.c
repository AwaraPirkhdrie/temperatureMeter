/*
 * lab5.c
 * This program reads from an LM35 sensor and displays it on a LCD.
 * Created: 2019-01-08 07:53:23
 *  Author: Awara
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "hmi/hmi.h"
#include "numkey/numkey.h"
#include "temp/temp.h"

enum state
{
	SHOW_TEMP_C,
	SHOW_TEMP_F,
	SHOW_TEMP_CF
};

void show_temp_c();
void show_temp_f();
void show_temp_cf();

typedef enum state state_t;
state_t current_state;
state_t next_state;

char temp_str[17];

int main(void){
	/* Initialize */
	hmi_init();
	temp_init();
	char key = '1'; 
	current_state = SHOW_TEMP_C; // Default state is SHOW_TEMP_C
	
	/* Main loop */
    while(1){
		key = numkey_read();
	    
		switch(current_state){
		    case SHOW_TEMP_C:
		    if(key == NO_KEY || key == '1'){
			    next_state = SHOW_TEMP_C;
		    } else if (key == '2'){
				next_state =SHOW_TEMP_F;
			}
		    show_temp_c();
		    break;
		    
		    case SHOW_TEMP_F:
		    if(key == NO_KEY || key == '2'){
			   next_state = SHOW_TEMP_F;
			} else if (key == '1'){
				next_state = SHOW_TEMP_C;
			} else if (key == '3'){
				next_state = SHOW_TEMP_CF;
			}
			show_temp_f();
		    break;
		    
		    case SHOW_TEMP_CF:
		    if(key == NO_KEY || key == '3'){
			    next_state = SHOW_TEMP_CF;
		    } else if(key == '1'){
				next_state = SHOW_TEMP_C;
			} else if (key == '2'){
				next_state = SHOW_TEMP_F;
			}
		    show_temp_cf();
		    break;
	    }
			current_state = next_state;
    }
	
	return 0;
}

/* This gets called everytime the state machine is SHOW_TEMP_C */
/* Shows the temperature in celsius. */
void show_temp_c(void){
	sprintf(temp_str, "%u%cC", temp_read_celsius(), 0xDF);
	output_msg("TEMPERATURE:",temp_str,0);
}

/* This gets called everytime the state machine is SHOW_TEMP_F */
/* Shows the temperature in fahrenheit. */
void show_temp_f(void){
	sprintf(temp_str, "%u%cF", temp_read_fahrenheit(), 0xDF);
	output_msg("TEMPERATURE:",temp_str,0);
}

/* This gets called everytime the state machine is SHOW_TEMP_CF */
/* Shows the temperature in celsius and fahrenheit. */
void show_temp_cf(void){
	sprintf(temp_str, "%u%cC %u%cC", temp_read_celsius(), 0xDF,temp_read_fahrenheit(),0xDF);
	output_msg("TEMPERATURE:",temp_str,0);
}