TARGET_ELF =thm36_1.3
OUT_TARGET =hex
platform =M3
O = ./out/lst

# Startup code
STARTUP = ./Projects/THM36_1.3/Startup/startup_thm36.c


#下面把所有文件都归加入了不用再手动加入文件
SRC += $(wildcard ./Projects/THM36_1.3/User/src/*.c)
SRC += $(wildcard ./Certification/EMVL1/*.c)
SRC += $(wildcard ./Drivers/CCID/*.c)
SRC += $(wildcard ./Drivers/CDC/*.c)
SRC += $(wildcard ./Drivers/HAL/src/*.c)
SRC += $(wildcard ./Drivers/HID/*.c)
SRC += $(wildcard ./Drivers/PrintUART/*.c)
SRC += $(wildcard ./Drivers/TMC_Drivers/src/*.c)
SRC += $(wildcard ./Drivers/USB/*.c)
SRC += $(wildcard ./example/src/*.c)


#兼容原文件库，去掉多余的文件
#tem += $(wildcard ./example/src/crypto_demomain.c)


ALLSRC += $(filter-out $(tem),$(SRC)) 


#头文件所在目录
APPINCLUDE += -I./Projects/THM36_1.3/Startup/			\
			-I./Projects/THM36_1.3/User/inc/ 			\
			-I./Certification/EMVL1/		\
			-I./Drivers/CCID/ 				\
			-I./Drivers/CDC/				\
			-I./Drivers/HAL/inc/			\
			-I./Drivers/HAL/Lib/inc/		\
			-I./Drivers/HID/				\
			-I./Drivers/PrintUART/			\
			-I./Drivers/TMC_Drivers/inc/	\
			-I./Drivers/USB/ 				\
			-I./example/inc/				\


#库文件
MYDLLSINC += -L ./Drivers/HAL/Lib/
MYDLLS += $(MYDLLSINC) -static -lmag_drv_V100 -lPLL_V100 -lRamReset_V100 -ltmcECCEncDecPtchk_V100 -ltmcLowAPI_V100 -ltmcAES_CFB

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
AR=arm-none-eabi-ar
LD=arm-none-eabi-ld

OBJCOPY =arm-none-eabi-objcopy

ifeq ($(OUT_TARGET),hex)
	outfilename2 := $(TARGET_ELF).hex
else
	outfilename2 := $(TARGET_ELF).bin
endif


TARGET = $(outfilename2)

startup_obj := $(STARTUP:.c=.o)
startup_dep := $(STARTUP:.c=.d)

OBJS = $(patsubst %.c, %.o, $(ALLSRC)) $(startup_obj)
DEPS = $(patsubst %.c, %.d, $(ALLSRC)) $(startup_dep)

ARCH_FLAGS = -mthumb -mcpu=cortex-m3 -std=c99
CFLAGS += $(APPINCLUDE) $(ARCH_FLAGS)
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -msoft-float 
CFLAGS += -O0 -Wall


LDSCRIPTS := -T thm36_Flash_gcc.ld
MAP = -Wl,-Map,$(O)/$(TARGET_ELF).map
LDFLAGS += $(LDSCRIPTS)
LDFLAGS += $(MAP)
 
#不用系统库
#LDFLAGS += --specs=nosys.specs  

.PHONY: all 
all: $(O)/$(TARGET) $(O)/$(TARGET_ELF).bin

$(O)/$(TARGET_ELF).elf:$(OBJS) 
	$(CC)  $(LDFLAGS) $(CFLAGS)  -o $@ $(OBJS) $(MYDLLS)

-include $(DEPS)	
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -MMD -c $<
	
$(O)/$(TARGET_ELF).hex: $(O)/$(TARGET_ELF).elf
	$(OBJCOPY) -O ihex $< $@

$(O)/$(TARGET_ELF).bin: $(O)/$(TARGET_ELF).elf
	$(OBJCOPY) -O binary $< $@
	
$(O)/ :
	@mkdir -p $(O)/lst

.PHONY: install
install:
	cp $(O)/* $(O)/../

.PHONY: print
print:
	@echo outfilename:  $(outfilename)
	@echo TARGET:   $(TARGET)
	@echo OUT_TARGET:   $(OUT_TARGET)
	@echo CFLAGS:   $(CFLAGS)
	@echo CC:   $(CC)
	@echo AR:   $(AR)
	@echo ALLSRC:   $(ALLSRC)
	@echo OBJS:   $(OBJS)
	@echo LDFLAGS:   $(LDFLAGS)
	
.PHONY: clean
clean:
	-rm -f 	./Certification/EMVL1/*.o		\
			./Drivers/CCID/*.o				\
			./Drivers/CDC/*.o				\
			./Drivers/HAL/src/*.o			\
			./Drivers/HID/*.o				\
			./Drivers/PrintUART/*.o			\
			./Drivers/TMC_Drivers/src/*.o	\
			./Drivers/USB/*.o				\
			./example/src/*.o				\
			./Projects/THM36_1.3/User/src/*.o			\
			./Projects/THM36_1.3/Startup/*.o			\
			
	-rm -f 	./Certification/EMVL1/*.d		\
			./Drivers/CCID/*.d				\
			./Drivers/CDC/*.d				\
			./Drivers/HAL/src/*.d			\
			./Drivers/HID/*.d				\
			./Drivers/PrintUART/*.d			\
			./Drivers/TMC_Drivers/src/*.d	\
			./Drivers/USB/*.d				\
			./example/src/*.d				\
			./Projects/THM36_1.3/User/src/*.d			\
			./Projects/THM36_1.3/Startup/*.d			\
			./out/lst/thm36* 				 

