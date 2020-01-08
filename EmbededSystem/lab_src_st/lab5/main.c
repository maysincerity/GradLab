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
int is_on;
int p;
int p_colors[10] = {0x2a36b1, 0x3b50ce, 0x455ede, 0x4e6cef, 0x4b69ff, 0x4d73ff, 0x5677fc, 0x6889ff, 0x738ffe, 0x91a7ff};
int status;
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
struct fingers {
	int status;
	int x;
	int y;
};
int colors[] = { 0xff0000, 0xffff00, 0xffff, 0xff00, 0xff};
void my_line(int dx, int dy, int sx, int sy, int color)
{
/*---------------------------------------------------*/
	//printf("you need implement fb_draw_line()\n"); exit(0);
if(abs(dy-sy) < abs(dx-sx))
	{
		int x;
		double k = (double)(dy - sy) / (dx - sx);

		for(x=0; x<=abs(sx-dx); x++)
		{
			if(sx<dx)
                           fb_draw_rect(sx+x - 3,(int)(sy+k*x+0.5) - 3,6,6,color);
			else
                           fb_draw_rect(dx+x - 3,(int)(dy+k*x+0.5) - 3,6,6, color);

		}
	}
	else
	{
		int y;
		double k = (double)(dx - sx) / (dy - sy);

		for(y=0; y<=abs(sy-dy); y++)
		{
			if(sy<dy)

                            fb_draw_rect((int)(sx+k*y+0.5) - 3,sy+y -3 ,6,6, color);
			else

                              fb_draw_rect((int)(dx+k*y+0.5) - 3,dy+y -3,6,6, color);
		}
	}
	return;
}
/*=================================================================*/
void my_thread(){
	while (1) {
		if (p > 97) {led_set(0);usleep(100);}
		else if (p < 3) {led_set(1);usleep(100);}
		else { usleep(p * 1000); is_on = !is_on; led_set(is_on); }
	}
}

struct historyText{
	int x;
	int y;
} hisText;

/*绘制圆*/
void fb_draw_circle(int x, int y, int r,int color){
	int tempX=0;
	for(tempX=x-r; tempX<x+r;tempX++){
		int dy = (sqrt(r*r-abs(tempX-x)*abs(tempX-x))+0.5);
		if(tempX <=0) continue;
		/*绘制竖着的部分*/
		int tempY=0;
		for(tempY=y-dy; tempY<y+dy; tempY++){
		     if(tempY <= 0) continue;
		     if(tempY>= SCREEN_HEIGHT-1) break;
			 fb_draw_pixel(tempX, tempY, color);
		}
	}
}

int draw_process(int process){
	int i = 0;
	fb_draw_rect(204, 254, (process - i) * 6-8, 42 , p_colors[i / 10]);
	char * tips;
	char t[4] = "00%";
	if (p < 3 ) {
		tips = "ON";
 	}else if (p > 97) {
		tips = "OFF";
	}else{
		tips = t;
		tips[0] = p / 10 + '0';
		tips[1] = p % 10 + '0';
	}
	fb_draw_rect(hisText.x, hisText.y, 50, 25, FB_COLOR(255,255,255));
	fb_draw_text(183 + p * 6,230, tips, 25 , FB_COLOR(0,0,0));
	hisText.x = 183+p*6;
	hisText.y = 230-25;
}
int main(int argc, char* argv[])
{
    is_on = 0;

	fb_init("/dev/graphics/fb0");
	font_init("/data/local/font.ttc");
	fb_draw_rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FB_COLOR(255,255,255));

	// set background jpeg
	fb_image *img;
	img = fb_read_jpeg_image("/data/local/jpg_test.jpg");
	fb_draw_image(0,0,img,0);
	fb_update();
	fb_free_image(img);

	pthread_t tid;
	pthread_create(&tid, NULL, my_thread , NULL);
	fb_update();
	struct fingers f[5];
	memset(f,0, sizeof(struct fingers) * 5);
	touch_init("/dev/input/event3");
	int type,x,y,finger,i;
	int flag;
	p = 0;
	while(1){
		fb_draw_rect(200,250,600,50, 0x000);
		fb_draw_rect(202,252,596,46, 0xffffff);
		draw_process(p);
		fb_update();
			type = touch_read(&x,&y,&finger);
			switch(type){
				case TOUCH_PRESS:
					//printf("TOUCH_PRESS：x=%d,y=%d,finger=%d\n",x,y,finger);
					if (x < 800 && x > 200 && y < 300 && y > 250) status = 1;
					break;
				case TOUCH_MOVE:
					//printf("TOUCH_MOVE：x=%d,y=%d,finger=%d\n",x,y,finger);
					if (status)
						if (x >= 800) p = 100;
						else if (x <= 200) p = 0;
						else p = (x - 200 ) / 6;
					break;
				case TOUCH_RELEASE:
					//printf("TOUCH_RELEASE：x=%d,y=%d,finger=%d\n",x,y,finger);
					if ( x < 100 && y < 50) {is_on = !is_on; led_set(is_on);}
					status = 0;
					break;
				default:
					break;
			}
			fb_update();
	}
	return 0;
}
