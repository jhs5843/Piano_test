/*
 * piano_test.cpp
 *
 * Created: 2018-09-27 오전 9:45:07
 * Author : USER
 */ 
#include "myfunc.h"

ISR(TIMER2_OVF_vect){
	TCNT2 = 6;
	count++; // 0.001초마다 1회증가
	timeCount++;
	if(count >2000){
		menuSelectCount++;
		ExamSelectCount++;
		count =0;
		if(menuSelectCount>3) menuSelectCount=0;
		if(ExamSelectCount>3) ExamSelectCount=0;
	}
	if(timeCount >1000){ // 1초
		sec++;
		timeCount=0;
	}
}
int main(void)
{
	DDRA= 0xff; // LED 출력 설정
	DDRB = 0xff; // 스피커 출력 설정
	DDRD = 0x0f; // key Matrix 입출력 설정
	
	PORTA = 0xff; // LED OFF
	PORTD = 0xff; // Key Matrix 초기값
	
	char str[16];
	
	
	sei();
	port_init();
	lcd_init();
	/* Replace with your application code */
	
	
	while (1)
	{
		key = getKey();
		
		if (mode == POWER_ON_MODE){
			powerOn();
		}
		else if(mode ==POWER_OFF_MODE){
			powerOff();
			if (key == K_POWER){
				mode = POWER_ON_MODE;
				PowerOnDisplayState = POWER_ON_DISPLAY;
				key =0;
				_delay_ms(100);
			}
		}
		else if(mode == NOMAL_MODE){
			key_init();
			if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
				mode = POWER_OFF_MODE;
				PowerOffDisplayState = POWER_OFF_DISPLAY;
				key =0;
				_delay_ms(100);
			}
			if (key == K_MENU){
				mode = MENU_SELECT_MODE;
				menuSelectCount =0;
				key=0;
				_delay_ms(100);
			}
		}
		else if(mode == MENU_SELECT_MODE){
			lcd_putsf(0,0,(unsigned char *)"    M E N U     ");
			if (menuSelectCount == MENU_RECORD_MODE){
				lcd_putsf(0,1,(unsigned char *)"1. Recording    ");
				if (key == K_MENU){
					mode = RECORD_MODE;
					key=0;
					_delay_ms(100);
				}
			}
			else if(menuSelectCount == MENU_LISTEN_MODE){
				lcd_putsf(0,1,(unsigned char *)"2. Listening    ");
				if (key == K_MENU){
					mode = LISTEN_MODE;
					key=0;
					_delay_ms(100);
				}
			}
			else if(menuSelectCount == MENU_EXAM_MODE){
				lcd_putsf(0,1,(unsigned char *)"3. Example      ");
				if (key == K_MENU){
					mode = EXAM_SELECT_MODE;
					ExamSelectCount =0;
					key=0;
					_delay_ms(100);
				}
			}
			else if(menuSelectCount == MENU_RETURN_MODE){
				lcd_putsf(0,1,(unsigned char *)"4. Return       ");
				if (key == K_MENU){
					mode = NOMAL_MODE;
					key=0;
					_delay_ms(100);
				}
			}
			if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
				mode = POWER_OFF_MODE;
				PowerOffDisplayState = POWER_OFF_DISPLAY;
				key =0;
				_delay_ms(100);
			}
		}
		else if(mode == RECORD_MODE){
			recordDisplay();
			if (key == K_DO && n_flag==0){ note_time[n_index] = msec; note[n_index + 1] = DO;  msec =0; n_flag=1; n_index++; freq(DO); }
			else if(key == K_RE && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = RE; msec = 0; n_flag = 1; n_index++; freq(RE); }
			else if(key == K_MI && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = MI; msec = 0; n_flag = 1; n_index++; freq(MI); }
			else if(key == K_FA && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = FA; msec = 0; n_flag = 1; n_index++; freq(FA); }
			else if(key == K_SOL && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = SOL; msec = 0; n_flag = 1; n_index++; freq(SOL); }
			else if(key == K_RA && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = RA; msec = 0; n_flag = 1; n_index++; freq(RA); }
			else if(key == K_SI && n_flag == 0) { note_time[n_index] = msec; note[n_index + 1] = SI; msec = 0; n_flag = 1; n_index++; freq(SI); }
			else if (key == 0 && n_flag == 1){ note_time[n_index] = msec; note[n_index + 1] = NO; msec = 0; n_flag = 0; n_index++; freq(0); }
			if (key == K_MENU){
				mode = MENU_SELECT_MODE;
				key=0;
				_delay_ms(100);
			}
			
			if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
				mode = POWER_OFF_MODE;
				PowerOffDisplayState = POWER_OFF_DISPLAY;
				key =0;
				_delay_ms(100);
			}
		}
		else if(mode == LISTEN_MODE){
			ListenDisplay();
			listen();
			if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
				mode = POWER_OFF_MODE;
				PowerOffDisplayState = POWER_OFF_DISPLAY;
				key =0;
				_delay_ms(100);
			}
		}
		else if(mode == EXAM_SELECT_MODE){
			lcd_putsf(0,0,(unsigned char *)"   -Example-    ");
			if(ExamSelectCount == EXAM1){
				lcd_putsf(0,1,(unsigned char *)"1. Example 1    ");
			}
			else if(ExamSelectCount == EXAM2){
				lcd_putsf(0,1,(unsigned char *)"2. Example 2    ");
			}
			else if(ExamSelectCount == EXAM3){
				lcd_putsf(0,1,(unsigned char *)"3. Example 3    ");
			}
			else if(ExamSelectCount == EXAMRETURN){
				lcd_putsf(0,1,(unsigned char *)"4. Return       ");
			}
			if (key == K_POWER){ // POWER KEY 입력시 OFF 모드로 이동한다.
				mode = POWER_OFF_MODE;
				PowerOffDisplayState = POWER_OFF_DISPLAY;
				key =0;
				_delay_ms(100);
			}
			if (key == K_MENU){
				if (ExamSelectCount == EXAM1)mode = EXAM1_MODE;
				else if(ExamSelectCount == EXAM2)mode = EXAM2_MODE;
				else if(ExamSelectCount == EXAM3)mode = EXAM3_MODE;
				else if(ExamSelectCount == EXAMRETURN)mode = MENU_SELECT_MODE;
				key=0;
				_delay_ms(100);
			}
		}
		else if(mode ==EXAM1_MODE){
			doremiPlay();
		}
		else if(mode == EXAM2_MODE){
			schoolPlay();
		}
		else if(mode == EXAM3_MODE){
			dogPlay();
		}
		
	}
}
