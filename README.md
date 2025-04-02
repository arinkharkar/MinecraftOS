Operating System that only runs Minecraft.

To build, ensure you have an i686 Cross Compiler Set up at /usr/local/opt/cross/bin, or change CC, CXX, ASM, and LD in the Makefile, and simply run "sudo make all" then use an emulator to run "minecraftOS.iso".

The operating system uses GRUB as a bootloader and its sole purpose is to run Minecraft, there is no filesystem support, no multitasking, just Minecraft.
