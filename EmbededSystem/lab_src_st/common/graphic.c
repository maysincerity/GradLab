#include "common.h"
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

static int LCD_MEM_BUFFER[SCREEN_WIDTH * SCREEN_HEIGHT];
static int *LCD_FRAME_BUFFER = NULL;

static struct {
	int x1, y1, x2, y2;
} update_area = {0,0,0,0};



void fb_init(char *dev)
{
	int fd;
	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;

	if(LCD_FRAME_BUFFER != NULL) return; /*already done*/

	//First: Open the device
	if((fd = open(dev, O_RDWR)) < 0){
		printf("Unable to open framebuffer %s, errno = %d\n", dev, errno);
		return;
	}
	if(ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix) < 0){
		printf("Unable to FBIOGET_FSCREENINFO %s\n", dev);
		return;
	}
	if(ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0){
		printf("Unable to FBIOGET_VSCREENINFO %s\n", dev);
		return;
	}

	printf("framebuffer info: bits_per_pixel=%u  width=%u  height=%u  line_length=%u  smem_len=%u\n",
			fb_var.bits_per_pixel, fb_var.xres, fb_var.yres, fb_fix.line_length, fb_fix.smem_len);

	//Second: mmap
	int *addr;
	size_t size = fb_var.xres * fb_var.yres * fb_var.bits_per_pixel/8;
	addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if((int)addr == -1){
		printf("failed to mmap memory for framebuffer.\n");
		return;
	}
	LCD_FRAME_BUFFER = addr;
	return;
}

/** copy data from mem buffer to frame buffer */
void fb_update(void)
{
	if(LCD_FRAME_BUFFER == NULL){
		printf("error: not allocate space for frame buffer\n");
		return;
	}

	if((update_area.x1 >= SCREEN_WIDTH)
			|| (update_area.x2 <= 0)
			|| (update_area.y1 >= SCREEN_HEIGHT)
			|| (update_area.y2 <= 0)) return;

	int x,y,w,h;
	x = (update_area.x1 < 0) ? 0 : update_area.x1;
	y = (update_area.y1 < 0) ? 0 : update_area.y1;
	w = (update_area.x2 > SCREEN_WIDTH)?
		SCREEN_WIDTH - x : update_area.x2 - x;
	h = (update_area.y2 > SCREEN_HEIGHT)?
		SCREEN_HEIGHT - y : update_area.y2 - y;

	int *src, *dst;
	src = LCD_MEM_BUFFER + y*SCREEN_WIDTH + x;
	dst = LCD_FRAME_BUFFER + y*SCREEN_WIDTH + x;
	while(h-- > 0){
		memcpy(dst, src, w*4);
		src += SCREEN_WIDTH;
		dst += SCREEN_WIDTH;
	}

	update_area.x2 = 0;
	return;
}

void fb_myupdate(int ux, int uy, int uw, int uh)
{
	ux-=50;
	uy-=50;
	uw = ux+uw;
	uh = uy+uh;


	if(LCD_FRAME_BUFFER == NULL){
		printf("error: not allocate space for frame buffer\n");
		return;
	}

	if((ux >= SCREEN_WIDTH)
			|| (uw <= 0)
			|| (uy >= SCREEN_HEIGHT)
			|| (uh <= 0)) return;

	int x,y,w,h;
	x = (ux < 0) ? 0 : ux;
	y = (uy < 0) ? 0 : uy;
	w = (uw > SCREEN_WIDTH)?
		SCREEN_WIDTH - x : uw - x;
	h = (uh > SCREEN_HEIGHT)?
		SCREEN_HEIGHT - y : uh - y;



	int *src, *dst;
	src = LCD_MEM_BUFFER + y*SCREEN_WIDTH + x;
	dst = LCD_FRAME_BUFFER + y*SCREEN_WIDTH + x;
	while(h-- > 0){

		memcpy(dst, src, w*4);
		src += SCREEN_WIDTH;
		dst += SCREEN_WIDTH;


	}

	ux = 0;
	return;
}

void fb_myupdate2(int ux, int uy, int uw, int uh, int ox, int oy)
{
	ux-=50;
	uy-=50;
	uw = ux+uw;
	uh = uy+uh;

	int ow;
	int oh;
	ox -= 50;
	oy -= 50;
	ow = ox+uw;
	oh = oy+uh;

	if(LCD_FRAME_BUFFER == NULL){
		printf("error: not allocate space for frame buffer\n");
		return;
	}

	if((ux >= SCREEN_WIDTH)
			|| (uw <= 0)
			|| (uy >= SCREEN_HEIGHT)
			|| (uh <= 0)) return;
	if((ox >= SCREEN_WIDTH)
			|| (ow <= 0)
			|| (oy >= SCREEN_HEIGHT)
			|| (oh <= 0)) return;

	int x,y,w,h;
	x = (ux < 0) ? 0 : ux;
	y = (uy < 0) ? 0 : uy;
	w = (uw > SCREEN_WIDTH)?
		SCREEN_WIDTH - x : uw - x;
	h = (uh > SCREEN_HEIGHT)?
		SCREEN_HEIGHT - y : uh - y;

	int oox,ooy,oow,ooh;
	oox = (ox < 0) ? 0 : ox;
	ooy = (oy < 0) ? 0 : oy;
	oow = (ow > SCREEN_WIDTH)?
		SCREEN_WIDTH - oox : ow - oox;
	ooh = (oh > SCREEN_HEIGHT)?
		SCREEN_HEIGHT - ooy : oh - ooy;
	int *osrc, *odst;
	osrc = LCD_MEM_BUFFER + ooy*SCREEN_WIDTH + oox;
	odst = LCD_FRAME_BUFFER + ooy*SCREEN_WIDTH + oox;

	int tempy = y<ooy?y:ooy;
	int tempx = x<oox?x:oox;
	int temph = h<ooh?ooh:h;
	int tempw = w<oow?oow:w;

	int *src, *dst;
	src = LCD_MEM_BUFFER + tempy*SCREEN_WIDTH + tempx;
	dst = LCD_FRAME_BUFFER + tempy*SCREEN_WIDTH + tempx;
	while(temph-- > 0){

		memcpy(dst, src, tempw*4);
		src += SCREEN_WIDTH;
		dst += SCREEN_WIDTH;

		//memcpy(odst, osrc, ow*4);
		//osrc += SCREEN_WIDTH;
		//odst += SCREEN_WIDTH;
	}

	ux = 0;
	return;
}

static void _update_area(int x, int y, int w, int h)
{
	//if((w <= 0)||(h <= 0)) return; /* sure */
	int x2 = x+w;
	int y2 = y+h;
	if(update_area.x2 == 0) {
		update_area.x1 = x;
		update_area.y1 = y;
		update_area.x2 = x2;
		update_area.y2 = y2;
	} else {
		if(update_area.x1 > x) update_area.x1 = x;
		if(update_area.y1 > y) update_area.y1 = y;
		if(update_area.x2 < x2) update_area.x2 = x2;
		if(update_area.y2 < y2) update_area.y2 = y2;
	}
	return;
}

/*======================================================================*/

void fb_draw_pixel(int x, int y, int color)
{
	if(x<0 || y<0 || x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) return;
	_update_area(x,y,1,1);
	int *tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * y + x;
	*tmp = color;
	return;
}

void fb_draw_rect(int x, int y, int w, int h, int color)
{
	if(x < 0) { w += x; x = 0;}
	if(x+w > SCREEN_WIDTH) { w = SCREEN_WIDTH-x;}
	if(y < 0) { h += y; y = 0;}
	if(y+h >SCREEN_HEIGHT) { h = SCREEN_HEIGHT-y;}
	if(w<=0 || h<=0) return;
	_update_area(x,y,w,h);
	/*---------------------------------------------------*/
	//printf("you need implement fb_draw_rect()\n"); exit(0);
	int i, j;
	int *tmp;
	for(i = y; i < y + h; i++){
		if (x>=(x+w-5)){
			for(j = x; j < x + w; j++){
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j;
				*tmp = color;
			}
		}
		else{
			for(j = x; j < x + w-5; j+=5){
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j;
				*tmp = color;
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j+1;
				*tmp = color;
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j+2;
				*tmp = color;
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j+3;
				*tmp = color;
				tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * i + j+4;
				*tmp = color;
			}
		}
	}
	/*---------------------------------------------------*/
	return;
}

void fb_draw_circle(int x, int y, int r, int color)
{
    if (x - r < 0 || y - r < 0 || x + r > SCREEN_WIDTH || y + r > SCREEN_HEIGHT) return;
    _update_area(x - r, y - r, 2 * r , 2 * r);
    int i, j;
    for (i = y - r ; i <= y + r ; ++i) {
        for (j = x - r ; j < x + r ; ++j) {
            if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r) {
                fb_draw_pixel(j, i, color);
            }
        }
    }
}

void fb_draw_line(int x1, int y1, int x2, int y2, int color)
{
/*---------------------------------------------------*/
	//printf("you need implement fb_draw_line()\n"); exit(0);
	int x, y;
	int dx, dy;
	int ddx, ddy;
	int *tmp;
	int xmin, ymin, w, h;
	if(x1 < x2) xmin = x1, w = x2 - x1;
	else xmin = x2, w = x1 - x2;
	if(y1 < y2) ymin = y1, h = y2 - y1;
	else ymin = y2, h = y1 - y2;
	_update_area(xmin, ymin, w, h);
	dx = x2 - x1;
	dy = y2 - y1;
	ddx = (x2 > x1) ? 1: -1;
	ddy = (y2 > y1) ? 1: -1;
	if(dx > dy){
		for(x = x1; x != x2; x += ddx){
			y = (int)(((double)dy * (x - x1)) / dx) + y1;
			tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * y + x;
			*tmp = color;
		}
	}
	else{
		for(y = y1; y != y2; y += ddy){
			x = (int)(((double)dx * (y - y1)) / dy) + x1;
			tmp = LCD_MEM_BUFFER + SCREEN_WIDTH * y + x;
			*tmp = color;
		}
	}
/*---------------------------------------------------*/
	return;
}

/*======================================================================*/

void fb_draw_image(int x, int y, fb_image *image, int color)
{
	if(image == NULL) return;

	int ix = 0; //image x
	int iy = 0; //image y
	int w = image->pixel_w; //draw width
	int h = image->pixel_h; //draw height

	if(x<0) {w+=x; ix-=x; x=0;}
	if(y<0) {h+=y; iy-=y; y=0;}

	if(x+w > SCREEN_WIDTH) {
		w = SCREEN_WIDTH - x;
	}
	if(y+h > SCREEN_HEIGHT) {
		h = SCREEN_HEIGHT - y;
	}
	if((w <= 0)||(h <= 0)) return;

	_update_area(x,y,w,h);

	char *dst = (char *)(LCD_MEM_BUFFER + y*SCREEN_WIDTH + x);
	char *src = image->content + iy*image->line_byte + ix*4;
/*---------------------------------------------------------------*/

	int alpha;
	int ww;
	int i, j;
	char r, g, b;
	char *dst1, *src1, *dst2, *src2;
	int h1 = h * 4, w1 = w * 4;

	if(image->color_type == FB_COLOR_RGB_8880) /*lab3: jpg*/
	{
		//printf("you need implement fb_draw_image() FB_COLOR_RGB_8880\n"); exit(0);
		//memcpy(dst, src, w * h * 4);

		for(i = 0; i < h1; i += 4){
			dst1 = dst + i * SCREEN_WIDTH;
			src1 = src + i * image->pixel_w;
			memcpy(dst1, src1, w1);
			/*for(j = 0; j < w1; j += 4){
				*(int*)(dst1 + j) = *(int*)(src1 + j);
			}*/
		}
		/*for(i = 0; i < h; i++){
			for(j = 0; j < w; j++){
				*(int*)(dst + i * SCREEN_WIDTH * 4 + j * 4) = *(int*)(src + i * w * 4 + j * 4);
			}
		}*/
		return;
	}

	if(image->color_type == FB_COLOR_RGBA_8888) /*lab3: png*/
	{
		//printf("you need implement fb_draw_image() FB_COLOR_RGBA_8888\n"); exit(0);
		for(i = 0; i < h1; i += 4){
			src1 = src + i * w;
			dst1 = dst + i * SCREEN_WIDTH;
			for(j = 0; j < w1; j += 4){
				dst2 = dst1 + j;
				src2 = src1 + j;
				alpha = *(src2 + 3);
				switch(alpha){
				case 255:
					*(int*)(dst2) = *(int*)(src2);
					break;
				default:
					b = *(src2);
					g = *(src2 + 1);
					r = *(src2 + 2);
					*(dst2) += (((b - *(dst2)) * alpha) >> 8);
					*(dst2 + 1) += (((g - *(dst2 + 1)) * alpha) >> 8);
					*(dst2 + 2) += (((r - *(dst2 + 2)) * alpha) >> 8);
					//*(dst2 + 3) = 255;
				}
			}
		}

		return;
	}

	if(image->color_type == FB_COLOR_ALPHA_8) /*lab3: font*/
	{
		//printf("you need implement fb_draw_image() FB_COLOR_ALPHA_8\n"); exit(0);
		for(i = 0; i < h; i++){
			for(j = 0; j < w; j++){
				alpha = *(src + i * w + j);
				switch(alpha){
				case 255:
					*(int*)(dst + i * SCREEN_WIDTH * 4 + j * 4) = color;
					break;
				default:
					b = color & 0xff;
					g = (color >> 8) & 0xff;
					r = (color >> 16) & 0xff;
					*(dst + i * SCREEN_WIDTH * 4 + j * 4) += (((b - *(dst + i * SCREEN_WIDTH * 4 + j * 4)) * alpha) >> 8);
					*(dst + i * SCREEN_WIDTH * 4 + j * 4 + 1) += (((g - *(dst + i * SCREEN_WIDTH * 4 + j * 4 + 1)) * alpha) >> 8);
					*(dst + i * SCREEN_WIDTH * 4 + j * 4 + 2) += (((r - *(dst + i * SCREEN_WIDTH * 4 + j * 4 + 2)) * alpha) >> 8);
					*(dst + i * SCREEN_WIDTH * 4 + j * 4 + 3) = 255;
					break;
				}
			}
		}
		return;
	}
/*---------------------------------------------------------------*/
	return;
}

/** draw a text string **/
void fb_draw_text(int x, int y, char *text, int font_size, int color)
{
	fb_image *img;
	fb_font_info info;
	int i=0;
	int len = strlen(text);
	while(i < len)
	{
		img = fb_read_font_image(text+i, font_size, &info);
		if(img == NULL) break;
		fb_draw_image(x+info.left, y-info.top, img, color);
		fb_free_image(img);

		x += info.advance_x;
		i += info.bytes;
	}
	return;
}
