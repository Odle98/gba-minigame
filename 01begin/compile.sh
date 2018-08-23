# Cross-compile C code in Thumb instructions for GBA CPU
arm-none-eabi-gcc -c main.c -mthumb-interwork -mthumb -O2 -o main.o

# Link of .o files using 'specs' file for control the linker behavior
arm-none-eabi-gcc main.o -mthumb-interwork -mthumb -specs=gba.specs -o main.elf

# Strip not needed info from the executable file
arm-none-eabi-objcopy -v -O binary main.elf main.gba

# Utiliy for hearder fix to the rom (not installed in my case)
# gbafix main.gba



