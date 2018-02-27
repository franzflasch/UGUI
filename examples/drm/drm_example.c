#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ugui.h"
#include <xf86drm.h>
#include <xf86drmMode.h>

#define DISPLAY_COLS 128
#define DISPLAY_ROWS 64

#define SECTIONS 64
#define PIXELS_PER_SECTION 64

static int drm_fd;
static uint32_t display[DISPLAY_COLS*DISPLAY_ROWS];

extern struct modeset_dev *modeset_list;
extern int prepare_drm_card(char *card, int *fd);
extern void modeset_draw(int fd, uint32_t *display);
extern void modeset_cleanup(int fd);


/* This is for the uc1701 controller which has some 
   strange pixel ordering
 */
void PixelFunc(UG_S16 x, UG_S16 y, UG_COLOR c)
{
	// if(x>127)
	// 	return;
	// if(y>63)
	// 	return;
	// int page = (y/8);
	// int screen_pixel = (x*8) + ((y-(page*8))+(page*PIXELS_PER_SECTION*16));
	// display[screen_pixel] = c;

	display[(y*DISPLAY_COLS)+x] = c;
}

int main(int argc, char **argv){

	UG_GUI gui;
	char *card;
	//int i = 0;

	/* check which DRM device to open */
	if (argc > 1)
		card = argv[1];
	else
	{
		fprintf(stderr, "Please specifiy drm card.\n");
		return 1;
	}

	prepare_drm_card(card, &drm_fd);
		

	UG_Init(&gui, PixelFunc, DISPLAY_COLS, DISPLAY_ROWS);
	UG_SelectGUI(&gui);

	UG_FillScreen(C_WHITE);

	while (1)
	{
		// for(i=0;i<9;i++)
		// 	UG_DrawPixel(i, 0, C_BLACK);

		/* Draw some circles */
		UG_DrawCircle(63, 31, 2, C_BLACK);
		UG_DrawCircle(63, 31, 5, C_BLACK);
		UG_DrawCircle(63, 31, 10, C_BLACK);
		UG_DrawCircle(63, 31, 15, C_BLACK);
		UG_DrawCircle(63, 31, 20, C_BLACK);
		UG_DrawCircle(63, 31, 25, C_BLACK);
		UG_DrawCircle(63, 31, 30, C_BLACK);

		UG_Update();
		modeset_draw(drm_fd, display);
	}

	modeset_cleanup(drm_fd);
	close(drm_fd);
}
