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

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

#define MEM_IO   0x04000000
#define MEM_PAL  0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM  0x07000000

#define REG_DISPLAY        (*((volatile uint32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT      (*((volatile uint32 *)(MEM_IO + 0x0130)))

#define KEY_UP    0x0040
#define KEY_DOWN  0x0080
#define KEY_RIGHT 0x0010
#define KEY_LEFT  0x0020
#define KEY_ANY   0x03FF

#define OBJECT_ATTR0_Y_MASK 0x0FF
#define OBJECT_ATTR1_X_MASK 0x1FF

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef uint16 rgb15;

typedef struct obj_attrs {
    uint16 attr0;
    uint16 attr1;
    uint16 attr2;
    uint16 pad;
} __attribute__((packed, aligned(4))) obj_attrs;

typedef uint32    tile_4bpp[8];
typedef tile_4bpp tile_block[512];

#define oam_mem            ((volatile obj_attrs *)MEM_OAM)
#define tile_mem           ((volatile tile_block *)MEM_VRAM)
#define object_palette_mem ((volatile rgb15 *)(MEM_PAL + 0x200))

// Obtain RGB color in hex from RGB values
static inline rgb15 RGB15(int r, int g, int b)
{   return r | (g << 5) | (b << 10);   }

// Set the position of an object to specified x and y coordinates
static inline void set_object_position(volatile obj_attrs *object, int x,
                                       int y)
{
    object->attr0 = (object->attr0 & ~OBJECT_ATTR0_Y_MASK) |
    (y & OBJECT_ATTR0_Y_MASK);
    object->attr1 = (object->attr1 & ~OBJECT_ATTR1_X_MASK) |
    (x & OBJECT_ATTR1_X_MASK);
}

// Clamp 'value' in the range 'min' to 'max' (inclusive)
static inline int clamp(int value, int min, int max)
{
    return (value < min ? min
            : (value > max ? max : value));
}

int main(void)
{
    
    // Create the ball tile for the ball sprite
    // 0x2222 is four pixels of colour index 2
    // Write pixel of color into VRAM
    volatile uint16 *ball_tile_mem   = (uint16 *)tile_mem[4][5];
    for (int i = 0; i < (sizeof(tile_4bpp) / 2); ++i)
        ball_tile_mem[i] = 0x2222;
    
    // Write the color for the ball
    object_palette_mem[2] = RGB15(74, 214, 181);
    
    // Create ball sprite writing object attributes into OAM
    volatile obj_attrs *ball_attrs = &oam_mem[1];
    ball_attrs->attr0 = 0; // 4bpp tiles, SQUARE shape
    ball_attrs->attr1 = 0; // 8x8 size when using the SQUARE shape
    ball_attrs->attr2 = 5; // Start at the fifth tile in tile block four
                           // use color palette zero
    
    // Initialize variables to keep track of the state of the paddle and
    // ball, and set their initial positions (by modifying their
    // attributes in OAM)
    const int ball_width = 8, ball_height = 8;
    int ball_velocity = 1;
    int ball_x = 120, ball_y = 60;
    set_object_position(ball_attrs, ball_x, ball_y);
    
    // Set the display parameters to enable objects, and use a 1D
    // object->tile mapping
    REG_DISPLAY = 0x1000 | 0x0040;
    
    // The main game loop
    uint32 key_states = 0;
    while (1) {
        
        // Skip past the rest of any current V-Blank, then skip past
        // the V-Draw
        while(REG_DISPLAY_VCOUNT >= 160);
        while(REG_DISPLAY_VCOUNT <  160);
        
        // Get current key states (REG_KEY_INPUT stores the states inverted)
        key_states = ~REG_KEY_INPUT & KEY_ANY;
        
        // Set position limits for the player
        int ball_max_clamp_x = SCREEN_WIDTH  - ball_width;
        int ball_max_clamp_y = SCREEN_HEIGHT - ball_height;
        
        // Get new ball position using Key direction
        // Ensure also that player x and y coordinates remains in the screen
        // X -> 0 and Y -> 0 is top left corner
        if (key_states & KEY_UP)
            ball_y = clamp(ball_y - ball_velocity, 0, ball_max_clamp_x);
        if (key_states & KEY_DOWN)
            ball_y = clamp(ball_y + ball_velocity, 0, ball_max_clamp_y);
        if (key_states & KEY_LEFT)
            ball_x = clamp(ball_x - ball_velocity, 0, ball_max_clamp_x);
        if (key_states & KEY_RIGHT)
            ball_x = clamp(ball_x + ball_velocity, 0, ball_max_clamp_x);
        
        // Set the position on screen
        set_object_position(ball_attrs, ball_x, ball_y);
    }
    
    return 0;
}
