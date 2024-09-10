CC=/usr/local/opt/cross/bin/i686-elf-gcc
CXX=/usr/local/opt/cross/bin/i686-elf-g++
ASM=/usr/local/opt/cross/bin/i686-elf-as
LD=/usr/local/opt/cross/bin/i686-elf-gcc #Want to use the gcc for linking rather than ld, as ld may cause problems
GRUB=grub-mkrescue
CFLAGS=-ffreestanding -m32 -fno-pie -mno-red-zone -fno-exceptions -I src -I src/video -I src/video/character_data -I src/gdt -I src/libc -I src/idt
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

$(BIN)/pit.o: $(BIN) $(SRC)/pit.c
	$(CC) $(CFLAGS) -c $(SRC)/pit.c -o $(BIN)/pit.o

$(BIN)/ps2keyboard.o: $(BIN) $(SRC)/ps2keyboard.c
	$(CC) $(CFLAGS) -c $(SRC)/ps2keyboard.c -o $(BIN)/ps2keyboard.o

$(BIN)/video.o: $(BIN) $(SRC)/video/video.c
	$(CC) $(CFLAGS) -c $(SRC)/video/video.c -o $(BIN)/video.o

$(BIN)/gdt_manager.o: $(BIN) $(SRC)/gdt/gdt_manager.c
	$(CC) $(CFLAGS) -c $(SRC)/gdt/gdt_manager.c -o $(BIN)/gdt_manager.o

$(BIN)/gdt_enabler.o: $(BIN) $(SRC)/gdt/gdt_enabler.S
	$(ASM) $(SRC)/gdt/gdt_enabler.S -o $(BIN)/gdt_enabler.o

$(BIN)/idt.o: $(BIN) $(SRC)/idt/idt.c
	$(CC) $(CFLAGS) -c $(SRC)/idt/idt.c -o $(BIN)/idt.o

$(BIN)/idt_enabler.o: $(BIN) $(SRC)/idt/idt_enabler.S
	$(ASM) $(SRC)/idt/idt_enabler.S -o $(BIN)/idt_enabler.o

$(BIN)/isr.o: $(BIN) $(SRC)/idt/isr.S
	$(ASM) $(SRC)/idt/isr.S -o $(BIN)/isr.o

$(BIN)/doomos.bin: $(BIN)/start.o $(BIN)/kernel_main.o $(BIN)/pit.o $(BIN)/error_handler.o $(BIN)/video.o $(BIN)/ps2keyboard.o $(BIN)/gdt_enabler.o $(BIN)/isr.o $(BIN)/gdt_manager.o $(BIN)/idt_enabler.o $(BIN)/idt.o
	$(LD) $(LDFLAGS) $(BIN)/start.o $(BIN)/kernel_main.o $(BIN)/pit.o $(BIN)/error_handler.o $(BIN)/gdt_enabler.o $(BIN)/ps2keyboard.o $(BIN)/gdt_manager.o $(BIN)/isr.o $(BIN)/video.o $(BIN)/idt.o $(BIN)/idt_enabler.o -o $(BIN)/doomos.bin 
	
$(ISO)/boot/doomos.bin: $(BIN)/doomos.bin $(ISO)/boot
	cp $(BIN)/doomos.bin $(ISO)/boot/doomos.bin

$(BOOT): $(ISO)/boot/doomos.bin
	$(GRUB) --xorriso=$(XORRISO) -o $(BOOT) $(ISO)
	
clean:
	rm -rf $(BIN)

.PHONY: all clean
