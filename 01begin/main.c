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

int main()
{
 
    /*
     ********************
     VERY IMPORTANT:
     This is voluntary crappy code (no #define, typedef, ecc..)
     The purpose of this example is give a rapid example of how things works.
     An improved version of this example is inside the next folder.
     ********************
    */
    
    // 0x04000000 is the IO registers address
    // To make pointer out of it is used 'unsigned int *'
    // but the type is arbitrary
    unsigned int *io_registers_ptr = (unsigned int *)0x04000000;
    
    // Write 0x0403 in the content of this address means:
    // activate video mode 3 and activate background 2
    *io_registers_ptr  = 0x0403; // Set some bits
    
    // 0x06000000 is the VRAM address
    // A pointer is make out of it using 'unsigned short *'
    // Again, the type is arbitraty
    unsigned short *vram = (unsigned short *)0x06000000;
    
    // In video mode 3 each value of VRAM is a pixel of the screen (240*160)
    // I write 3 colors in three different pixels of the screen
    vram[120+80*240] = 0x001F;
    vram[136+80*240] = 0x03E0;
    vram[120+96*240] = 0x7C00;
    
    // Wait forever
    while(1);
    
    return 0;
}
