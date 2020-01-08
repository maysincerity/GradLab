#include <stdio.h>
#include "../common/common.h"

int main(int argc, char *argv[])
{
	fb_init("/dev/graphics/fb0");
	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FB_COLOR(255,255,255));
	fb_update();

	touch_init("/dev/input/event3");
	int type,x,y,finger,i;
	int x_old[5], y_old[5];
	while(1){
		type = touch_read(&x,&y,&finger);
		switch(type){
			case TOUCH_PRESS:
				switch(finger){
					case 0:
					fb_draw_circle(x,y,50,FB_COLOR(255,190,200));break;
					case 1:
					fb_draw_circle(x,y,50,FB_COLOR(200,190,255));break;
					case 2:
					fb_draw_circle(x,y,50,FB_COLOR(135,206,250));break;
					case 3:
					fb_draw_circle(x,y,50,FB_COLOR(127,255,170));break;
					case 4:
					fb_draw_circle(x,y,50,FB_COLOR(255,255,0));break;
					default:
					break;					
				}
				//fb_update();
				fb_myupdate(x,y,100,100);
				printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n",x,y,finger);
				break;
				
			case TOUCH_MOVE:
				fb_draw_circle(x_old[finger],y_old[finger],50,FB_COLOR(255,255,255));
				//fb_update();
				//fb_myupdate(x_old[finger],y_old[finger],100,100);
				switch(finger){
					case 0:
					fb_draw_circle(x,y,50,FB_COLOR(255,190,200));break;
					case 1:
					fb_draw_circle(x,y,50,FB_COLOR(200,190,255));break;
					case 2:
					fb_draw_circle(x,y,50,FB_COLOR(135,206,250));break;
					case 3:
					fb_draw_circle(x,y,50,FB_COLOR(127,255,170));break;
					case 4:
					fb_draw_circle(x,y,50,FB_COLOR(255,255,0));break;
					default:
					break;					
				}
				fb_myupdate2(x,y,200,200,x_old[finger],y_old[finger]);
				//fb_update();
				printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n",x,y,finger);
				break;
			case TOUCH_RELEASE:
				fb_draw_circle(x_old[finger],y_old[finger],50,FB_COLOR(255,255,255));
				fb_myupdate2(x_old[finger],y_old[finger],200,200,x_old[finger],y_old[finger]);
				//fb_update();
				printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n",x,y,finger);
				break;
			default:
				break;
		}
		
		x_old[finger] = x;
		y_old[finger] = y;
		
	}
	return 0;
	
}
