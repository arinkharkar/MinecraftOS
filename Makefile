CC=/usr/local/opt/cross/bin/i686-elf-gcc
CXX=/usr/local/opt/cross/bin/i686-elf-g++
ASM=/usr/local/opt/cross/bin/i686-elf-as
LD=/usr/local/opt/cross/bin/i686-elf-gcc #Want to use the gcc for linking rather than ld, as ld may cause problems
GRUB=grub-mkrescue
CFLAGS= -ffreestanding -Wwrite-strings -m32 -fno-pie -fno-builtin -mno-red-zone -fno-exceptions -I src -I src/video -I src/linalg -I src/video/character_data -I src/gdt -I src/libc -I src/idt -I src/ps2 -I src/game
XORRISO=/src/xorriso-1.5.6/xorriso/xorriso
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

$(BIN)/floatarith.o: $(BIN) $(SRC)/libc/floatarith.S
	$(ASM) $(SRC)/libc/floatarith.S -o $(BIN)/floatarith.o

$(BIN)/math.o: $(BIN) $(SRC)/libc/math.cpp
	$(CC) $(CFLAGS) -c $(SRC)/libc/math.cpp -o $(BIN)/math.o

$(BIN)/kernel_main.o: $(BIN) $(SRC)/kernel_main.cpp
	$(CC) $(CFLAGS) -c $(SRC)/kernel_main.cpp -o $(BIN)/kernel_main.o

$(BIN)/error_handler.o: $(BIN) $(SRC)/error_handler.cpp
	$(CC) $(CFLAGS) -c $(SRC)/error_handler.cpp -o $(BIN)/error_handler.o

$(BIN)/pit.o: $(BIN) $(SRC)/pit.cpp
	$(CC) $(CFLAGS) -c $(SRC)/pit.cpp -o $(BIN)/pit.o

$(BIN)/video.o: $(BIN) $(SRC)/video/video.cpp
	$(CC) $(CFLAGS) -c $(SRC)/video/video.cpp -o $(BIN)/video.o

$(BIN)/gdt_manager.o: $(BIN) $(SRC)/gdt/gdt_manager.cpp
	$(CC) $(CFLAGS) -c $(SRC)/gdt/gdt_manager.cpp -o $(BIN)/gdt_manager.o

$(BIN)/gdt_enabler.o: $(BIN) $(SRC)/gdt/gdt_enabler.S
	$(ASM) $(SRC)/gdt/gdt_enabler.S -o $(BIN)/gdt_enabler.o

$(BIN)/idt.o: $(BIN) $(SRC)/idt/idt.cpp
	$(CC) $(CFLAGS) -c $(SRC)/idt/idt.cpp -o $(BIN)/idt.o

$(BIN)/idt_enabler.o: $(BIN) $(SRC)/idt/idt_enabler.S
	$(ASM) $(SRC)/idt/idt_enabler.S -o $(BIN)/idt_enabler.o

$(BIN)/ps2keyboard.o: $(BIN) $(SRC)/ps2/ps2keyboard.cpp
	$(CC) $(CFLAGS) -c $(SRC)/ps2/ps2keyboard.cpp -o $(BIN)/ps2keyboard.o

$(BIN)/ps2mouse.o: $(BIN) $(SRC)/ps2/ps2mouse.cpp
	$(CC) $(CFLAGS) -c $(SRC)/ps2/ps2mouse.cpp -o $(BIN)/ps2mouse.o

$(BIN)/game_main.o: $(BIN) $(SRC)/game/game_main.cpp
	$(CXX) $(CFLAGS) -c $(SRC)/game/game_main.cpp -o $(BIN)/game_main.o

$(BIN)/projection.o: $(BIN) $(SRC)/game/projection.cpp
	$(CXX) $(CFLAGS) -c $(SRC)/game/projection.cpp -o $(BIN)/projection.o

$(BIN)/draw.o: $(BIN) $(SRC)/game/draw.cpp
	$(CC) $(CFLAGS) -c $(SRC)/game/draw.cpp -o $(BIN)/draw.o

$(BIN)/ps2mouse.o: $(BIN) $(SRC)/ps2/ps2mouse.cpp
	$(CC) $(CFLAGS) -c $(SRC)/ps2/ps2mouse.cpp -o $(BIN)/ps2mouse.o

$(BIN)/matrix.o: $(BIN) $(SRC)/linalg/matrix.cpp
	$(CXX) $(CFLAGS) -c $(SRC)/linalg/matrix.cpp -o $(BIN)/matrix.o

$(BIN)/memory.o: $(BIN) $(SRC)/memory.cpp
	$(CC) $(CFLAGS) -c $(SRC)/memory.cpp -o $(BIN)/memory.o

$(BIN)/isr.o: $(BIN) $(SRC)/idt/isr.S
	$(ASM) $(SRC)/idt/isr.S -o $(BIN)/isr.o

$(BIN)/doomos.bin: $(BIN)/start.o $(BIN)/floatarith.o $(BIN)/math.o $(BIN)/kernel_main.o $(BIN)/ps2mouse.o $(BIN)/draw.o $(BIN)/projection.o $(BIN)/game_main.o $(BIN)/ps2keyboard.o $(BIN)/pit.o $(BIN)/error_handler.o $(BIN)/video.o $(BIN)/gdt_enabler.o $(BIN)/isr.o $(BIN)/gdt_manager.o $(BIN)/idt_enabler.o $(BIN)/idt.o $(BIN)/matrix.o $(BIN)/memory.o
	$(LD) $(LDFLAGS) $(BIN)/start.o $(BIN)/floatarith.o $(BIN)/math.o $(BIN)/projection.o $(BIN)/kernel_main.o $(BIN)/ps2mouse.o $(BIN)/draw.o $(BIN)/game_main.o $(BIN)/ps2keyboard.o $(BIN)/pit.o $(BIN)/error_handler.o $(BIN)/gdt_enabler.o $(BIN)/gdt_manager.o $(BIN)/isr.o $(BIN)/video.o $(BIN)/idt.o $(BIN)/matrix.o $(BIN)/memory.o $(BIN)/idt_enabler.o -o $(BIN)/doomos.bin 
	
$(ISO)/boot/doomos.bin: $(BIN)/doomos.bin $(ISO)/boot
	cp $(BIN)/doomos.bin $(ISO)/boot/doomos.bin

$(BOOT): $(ISO)/boot/doomos.bin
	$(GRUB) --xorriso=$(XORRISO) -o $(BOOT) $(ISO)

	
clean:
	rm -rf $(BIN)
	rm doomos.iso
	rm iso/boot/doomos.bin

.PHONY: all clean
