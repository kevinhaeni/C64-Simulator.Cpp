/*

  bho1 - init 23.8.2015
  bho1 - change to SDL"

  Compile with  `sdl2-config --cflags --libs`
  


  --------> x
  |
  |
  |
  |
  |
  |
  \/
  
  y

  
*/


//#define MEMORY_SIZE 65536
//#define MEMORY_CELL_SIZE 8

#include <stdlib.h>
#include "SDL.h"

//#include "memory.h"
#include "cpu-util.h"
#include "graphic.h"

#include <time.h>



SDL_Surface *sdl_init_display(unsigned int x_resolution, unsigned int y_resolution);
void sdl_exit_display();


int colortable[16][3]= {
  
  {  //Black 0x0
	0x00, 0x00, 0x00
  },
  {  //White 0x1
	0xFF, 0xFF, 0xFF
  },
  {  //Red 0x2
	0x89, 0x40, 0x36
  },
  {  //Cyan 0x3
	0x7A, 0xBF, 0xC7
  },
  {  //Purple 0x4
	0x8A, 0x46, 0xAE
  },
  {  //Green 0x5
	0x68, 0xA9, 0x41
  },
  {  //Blue 0x6
	0x3E, 0x31, 0xA2
  },
  {  //Yellow 0x7
	0xD0, 0xDC, 0x71
  },
  {  //Orange 0x8
0x90, 0x5F, 0x25
  },
  {  //Brown 0x9
	0x5C, 0x47, 0x00
  },
  {  //Light red 0xa
	0xBB, 0x77, 0x6D
  },
  {  //Dark gray 0xb
	0x55, 0x55, 0x55
  },
  {  //Gray 0xc
0x80, 0x80, 0x80
  },
  {  //Light green 0xd
	0xAC, 0xEA, 0x88
  },
  {  //Light blue 0xe
	0x7C, 0x70, 0xDA
  },
  { //Light gray 0xf
	0xAB, 0xAB, 0xAB
  }
};

int colortable_WTF[16][3]= {
  
  {  //Black 0x0
	0x00, 0x00, 0x00
  },
  {  //White 0x1
	0xFF, 0xFF, 0xFF
  },
  {  //Red 0x2
	0x68, 0x37, 0x2B
  },
  {  //Cyan 0x3
	0x70, 0xA4, 0xB2
  },
  {  //Purple 0x4
	0x6F, 0x3D, 0x86
  },
  {  //Green 0x5
	0x58, 0x8D, 0x43
  },
  {  //Blue 0x6
	0x35, 0x28, 0x79
  },
  {  //Yellow 0x7
	0xB8, 0xC7, 0x6F
  },
  {  //Orange 0x8
	0x6F, 0x4F, 0x25
  },
  {  //Brown 0x9
	0x43, 0x39, 0x00
  },
  {  //Light red 0xa
	0x9A, 0x67, 0x59
  },
  {  //Dark gray 0xb
	0x44, 0x44, 0x44
  },
  {  //Gray 0xc
	0x6C, 0x6C, 0x6C
  },
  {  //Light green 0xd
	0x9A, 0xD2, 0x84
  },
  {  //Light blue 0xe
	0x6C, 0x5E, 0xB5
  },
  { //Light gray 0xf
	0x95, 0x95, 0x95
  }
};


void init_display(struct _vic* vic){
  srand(time(NULL));
  vic->virtual_x_resolution = 32;
  vic->virtual_y_resolution = 32;
  /* virtual_x_resolution = 200; */
  /* virtual_y_resolution = 100; */
  
  vic->x_stretch = 10;
  vic->y_stretch = 10;
  //colortable = colortablefix;
  vic->device_x_resolution = vic->x_stretch*vic->virtual_x_resolution;
  vic->device_y_resolution = vic->y_stretch*vic->virtual_y_resolution;
  vic->le_screen = sdl_init_display(vic->device_x_resolution, vic->device_y_resolution);
}

void exit_display(){
  sdl_exit_display();
}
void switch_flip(struct _vic* vic){
  SDL_Flip(vic->le_screen);
}


// shameless copy from net
void DrawPixel(struct _vic* vic, int x, int y,Uint8 R, Uint8 G,Uint8 B){
  SDL_Surface *screen = vic->le_screen;
  Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  
    if ( SDL_MUSTLOCK(screen) )
    {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }

    switch (screen->format->BytesPerPixel) {
        case 1: { /* vermutlich 8 Bit */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;

        case 2: { /* vermutlich 15 Bit oder 16 Bit */
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;

        case 3: { /* langsamer 24-Bit-Modus, selten verwendet */
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            } else {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;

        case 4: { /* vermutlich 32 Bit */
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }

    if ( SDL_MUSTLOCK(screen) )
    {
        SDL_UnlockSurface(screen);
    }
}


void dot(struct _vic* vic, unsigned int x, unsigned int y, int color){
  
  // todo
  // loop over stretch factor and draw stretchfactor pixels
  
  unsigned int i;
  unsigned int j;
  int r,g,b;
  r = colortable[color][0];
  g = colortable[color][1];
  b = colortable[color][2];

  for (i=0; i<vic->x_stretch;i++){
	for (j=0; j<vic->y_stretch;j++){
	  DrawPixel(vic, x*vic->x_stretch+i, y*vic->y_stretch+j, r,g,b);
	}
  }
}


void draw_bitmap_memory(struct _6510_cpu* cpu, struct _vic* vic, char memory[][9]){
  int r = rand() % 0x100;
  memory[0xFE][0] = (0x1 & (r>>0)) == 1 ? '1' : '0'; 
  memory[0xFE][1] = (0x1 & (r>>1)) == 1 ? '1' : '0'; 
  memory[0xFE][2] = (0x1 & (r>>2)) == 1 ? '1' : '0'; 
  memory[0xFE][3] = (0x1 & (r>>3)) == 1 ? '1' : '0'; 
  memory[0xFE][4] = (0x1 & (r>>4)) == 1 ? '1' : '0'; 
  memory[0xFE][5] = (0x1 & (r>>5)) == 1 ? '1' : '0'; 
  memory[0xFE][6] = (0x1 & (r>>6)) == 1 ? '1' : '0'; 
  memory[0xFE][7] = (0x1 & (r>>7)) == 1 ? '1' : '0'; 

  int color;
  unsigned int i,x,y;
  if(cpu->cycles %1000 <10){
    //    draw_bitmap_memory(&cpu, &vic, memory);
    for(i=0; i<vic->virtual_x_resolution*vic->virtual_y_resolution;i++){
      //    index = i + j*32;
      //    y = (unsigned int)index/virtual_x_resolution;
      //    x = index%virtual_x_resolution;
      
      color = conv_bitstr2int(memory[vic->videobase+i],4,7);
      x = i%vic->virtual_x_resolution; 
      y = (int) i/vic->virtual_x_resolution;
      
      dot(vic, x, y, color);
      
    }
    switch_flip(vic);
  }

}  


void clr_screen(struct _vic* vic, char memory[][9]){
  int color;
  unsigned int i, j, x, y;
  for(i=0; i<vic->virtual_x_resolution*vic->virtual_y_resolution;i++){
	//    index = i + j*32;
    //    y = (unsigned int)index/virtual_x_resolution;
    //    x = index%virtual_x_resolution;

    color = 0;//color = conv_bitstr2int(memory[base+i],4,7);
	for(j=0;j<8; j++){
	  memory[i][j] = '0';
	}
	memory[i][j] = '\0';
    x = i%vic->virtual_x_resolution; 
    y = (int) i/vic->virtual_x_resolution;

    dot(vic,x,y,color);

  }

  switch_flip(vic);
}

  


SDL_Surface *sdl_init_display(unsigned int x_resolution, unsigned int y_resolution){
  SDL_Surface *screen;

//SDL_Surface *screen;
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
	printf("Can't init SDL:  %s\n", SDL_GetError());
	exit(1);
  }
  
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(x_resolution, y_resolution, 16, SDL_HWSURFACE);
  if (screen == NULL) {
	printf("Can't set video mode: %s\n", SDL_GetError());
	exit(1);
  }
  return screen;
}

void sdl_exit_display(){
  SDL_Quit();
}










