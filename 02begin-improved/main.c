/*
 ***************************************
 AUTHOR: Lorenzo Campanile
 ***************************************
 Thanks to the following guides:
 Setup and begin: https://www.reinterpretcast.com/writing-a-game-boy-advance-game
 GBA Programming guide: http://www.coranac.com/tonc/text/toc.htm
 ***************************************
 MOST OF THIS MATERIAL/KNOWLEDGE IS TAKEN FROM TONC (2nd Link above)
 ALL CREDITS TO ITS AUTHOR (Jasper Vijn)
 ***************************************
*/

// Useful types definition
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef u16             COLOR;

// Useful shortcuts definitions
#define INLINE static inline

// VRAM and IO addresses defines
#define MEM_IO   0x04000000
#define MEM_VRAM 0x06000000

// For set video mode and and background type (IO registers)
#define REG_DISPCNT *((volatile u32*)(MEM_IO+0x0000))

// List of video modes
#define DCNT_MODE0 0x0000
#define DCNT_MODE1 0x0001
#define DCNT_MODE2 0x0002
#define DCNT_MODE3 0x0003
#define DCNT_MODE4 0x0004
#define DCNT_MODE5 0x0005

// List of background types
#define DCNT_BG0 0x0100
#define DCNT_BG1 0x0200
#define DCNT_BG2 0x0400
#define DCNT_BG3 0x0800
#define DCNT_OBJ 0x1000

// Screen dimension
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// For referring to VRAM
#define vid_mem ((u16*)MEM_VRAM)

// Write indicated pixels (for video mode 3)
INLINE void m3_plot(int x, int y, COLOR clr)
{   *(vid_mem + y * SCREEN_WIDTH + x) = clr;   }


// Obtain RGB color from RGB values
INLINE COLOR RGB15(u32 red, u32 green, u32 blue)
{   return red | (green<<5) | (blue<<10);   }

int main()
{
    
    // Activate video mode 3 and background 2
    // Only background 2 is compatible with video mode 3
    REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;
    
    // Write pixels at indicated positions
    m3_plot( 120, 80, RGB15(31, 0, 0) );
    m3_plot( 136, 80, RGB15( 0,31, 0) );
    m3_plot( 120, 96, RGB15( 0, 0,31) );
    
    // Wait forever
    while(1);
    
    return 0;
}
