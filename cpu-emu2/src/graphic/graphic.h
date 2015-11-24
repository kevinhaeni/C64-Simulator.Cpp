/*
  sdl-driver.h
  bho1 - init 8.7.2011
  GPL

  simulating a video chip

*/

#ifndef _SDLDRIVER_H_
#define _SDLDRIVER_H_
#include "SDL.h"

// video interface chip vic
struct _vic {
  unsigned int virtual_x_resolution;
  unsigned int virtual_y_resolution;
  unsigned int device_x_resolution;
  unsigned int device_y_resolution;

  unsigned int videobase ;
  unsigned int x_stretch;
  unsigned int y_stretch;

  SDL_Window *window;
  SDL_Renderer *render;

  Uint32 *pixels;
  SDL_Texture *texture;
};
 

void init_display(struct _vic* vic);
void exit_display();

void draw_bitmap_memory(struct _6510_cpu*, struct _vic* vic, char memory[][9]);
void clr_screen(struct _vic* vic, char memory[][9]);

//void DrawPixel(SDL_Surface *screen, int x, int y,Uint8 R, Uint8 G,Uint8 B);
//SDL_Surface *sdl_init_display(unsigned int x_resolution, unsigned int y_resolution);

//void sdl_exit_display();



#endif /* _SDLDRIVER_H_ */
