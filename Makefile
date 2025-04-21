CC=/usr/local/opt/cross/bin/i686-elf-gcc
CXX=/usr/local/opt/cross/bin/i686-elf-g++
ASM=/usr/local/opt/cross/bin/i686-elf-as
LD=/usr/local/opt/cross/bin/i686-elf-gcc #Want to use the gcc for linking rather than ld, as ld may cause problems
GRUB=grub-mkrescue
CFLAGS= -ffreestanding -m32 -fno-pie -fno-builtin -mno-red-zone -fno-exceptions -I src -I src/video -I src/linalg -I src/video/character_data -I src/gdt -I src/libc -I src/idt -I src/ps2 -I src/game
XORRISO=/src/xorriso-1.5.6/xorriso/xorriso
LDFLAGS=-T"link.ld" -ffreestanding -nostdlib -lgcc
BIN=bin
ISO=iso
SRC=src
BOOT=minecraftOS.iso

# Find all source files
C_FILES := $(shell find src -type f -name '*.c')
ASM_FILES := $(shell find src -type f -name '*.S')

# Convert source files to object files
OBJ_FILES := $(C_FILES:src/%.c=$(BIN)/%.o) $(ASM_FILES:src/%.S=$(BIN)/%.o)

# Ensure bin directories exist
$(shell mkdir -p $(BIN) $(shell find src -type d | sed "s|src|$(BIN)|"))

all: $(BIN)/minecraftOS.bin $(BOOT)

# Compilation rules
$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)/%.o: src/%.S
	$(ASM) $< -o $@

# Link the kernel
$(BIN)/minecraftOS.bin: $(OBJ_FILES)
	$(LD) $(LDFLAGS) $^ -o $@

# Create ISO
$(ISO)/boot/minecraftOS.bin: $(BIN)/minecraftOS.bin $(ISO)/boot
	cp $< $@

$(BOOT): $(ISO)/boot/minecraftOS.bin
	$(GRUB) --xorriso=$(XORRISO) -o $@ $(ISO)
	cp minecraftOS.iso /mnt/c/users/arin/documents

clean:
	rm -rf $(BIN)
	rm minecraftOS.iso
	rm iso/boot/minecraftOS.bin


# Used to clean everything except screensaver .cpp files
clean_fast:
	find $(BIN) -type f -name '*.o' ! -path '$(BIN)/video/screensavers/*' -exec rm {} +


.PHONY: all clean