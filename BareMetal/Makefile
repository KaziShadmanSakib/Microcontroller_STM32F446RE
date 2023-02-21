CrossCompiler = arm-none-eabi-gcc
MACH = cortex-m4
CompilerFlags = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LinkerFlags = -nostdlib -T stm32_ls.ld -Wl,-Map=final.map

all:main.o stm32_startup.o final.elf

main.o:main.c
	$(CrossCompiler) $(CompilerFlags) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CrossCompiler) $(CompilerFlags) -o $@ $^

final.elf:main.o stm32_startup.o
	$(CrossCompiler) $(LinkerFlags) -o $@ $^

clean: 
	del -rf *.o *.elf

load:
	openocd -f board/stm32f4discovery.cfg