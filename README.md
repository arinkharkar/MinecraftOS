Operating System that only runs doom.

To build, ensure you have an i686 Cross Compiler Set up at /usr/local/opt/cross/bin, or change CC, CXX, ASM, and LD in the Makefile, and simply run "sudo make all" then use an emulator to run "doomos.iso".

The operating system uses GRUB as a bootloader and its sole purpose is to run doom, there is no filesystem support, no multitasking, just Doom.
