#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/ioctl.h>
#include "../common/common.h"

/*=================================================================*/

#define LED_IOC_MAGIC	'L'
#define LED_ON	_IO(LED_IOC_MAGIC, 0)
#define LED_OFF	_IO(LED_IOC_MAGIC, 1)

static int led_fd = -1;
void led_set(int on)
{
	if(led_fd == -1)
	{
		led_fd = open("/dev/led", O_RDWR);
		if(led_fd < 0){
        	        printf("open /dev/led failed, errno = %d\n", errno);
			return;
	        }
	}
	on? ioctl(led_fd,LED_ON) : ioctl(led_fd,LED_OFF);
	return;
}

/**/
int frequency = 0;
int main(int argc, char* argv[])
{
	fb_init("/dev/graphics/fb0");
	font_init("/data/local/font.ttc");
	touch_init("/dev/input/event3");
	
	int flag = 0;
	while(1){
		led_set(flag);
		if(flag ==0) flag = 1; else flag = 0;
		usleep(50000);	
	}
	return;
}

/*control led thread*/
void * led_thread(void){
	int flag = 0;		
	while(1){
		if(frequency ==0){
			led_set(1);	
		}
		else if(frequency == 100){
			led_set(0);		
		}
		else{
			led_set(flag);
			if(flag = 0) flag = 1; else flag = 0;	
			sleep(frequency);		
		}	
	}
	return NULL;	
}

/*void draw UI*/
void * drawProgressBar(){
			
						
	return NULL;
}

