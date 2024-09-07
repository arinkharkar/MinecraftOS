CC=/usr/local/opt/cross/bin/i686-elf-gcc
CXX=/usr/local/opt/cross/bin/i686-elf-g++
ASM=/usr/local/opt/cross/bin/i686-elf-as
LD=/usr/local/opt/cross/bin/i686-elf-gcc #Want to use the gcc for linking rather than ld, as ld may cause problems
GRUB=grub-mkrescue
CFLAGS=-ffreestanding -m32 -fno-pie -mno-red-zone -fno-exceptions -I src -I src/video -I src/video/character_data
XORRISO=/home/kalix/download/xorriso-1.4.6/xorriso/xorriso
LDFLAGS=-T"link.ld" -ffreestanding -nostdlib -lgcc
BIN=bin
ISO=iso
SRC=src
BOOT=doomos.iso


all: $(BIN)/doomos.bin $(BOOT)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/start.o: $(BIN) $(SRC)/start.S
	$(ASM) $(SRC)/start.S -o $(BIN)/start.o

$(BIN)/kernel_main.o: $(BIN) $(SRC)/kernel_main.c
	$(CC) $(CFLAGS) -c $(SRC)/kernel_main.c -o $(BIN)/kernel_main.o

$(BIN)/error_handler.o: $(BIN) $(SRC)/error_handler.c
	$(CC) $(CFLAGS) -c $(SRC)/error_handler.c -o $(BIN)/error_handler.o

$(BIN)/video.o: $(BIN) $(SRC)/video/video.c
	$(CC) $(CFLAGS) -c $(SRC)/video/video.c -o $(BIN)/video.o

$(BIN)/doomos.bin: $(BIN)/start.o $(BIN)/kernel_main.o $(BIN)/error_handler.o $(BIN)/video.o
	$(LD) $(LDFLAGS) $(BIN)/start.o $(BIN)/kernel_main.o $(BIN)/error_handler.o $(BIN)/video.o -o $(BIN)/doomos.bin
	
$(ISO)/boot/doomos.bin: $(BIN)/doomos.bin $(ISO)/boot
	cp $(BIN)/doomos.bin $(ISO)/boot/doomos.bin

$(BOOT): $(ISO)/boot/doomos.bin
	$(GRUB) --xorriso=$(XORRISO) -o $(BOOT) $(ISO)
clean:
	rm -rf $(BIN)

.PHONY: all clean
