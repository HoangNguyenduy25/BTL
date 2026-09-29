MCU = atmega324p
F_CPU = 8000000UL
TARGET = btl_avr

AVR_BIN = C:/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin
CC = "$(AVR_BIN)/avr-gcc.exe"
OBJCOPY = "$(AVR_BIN)/avr-objcopy.exe"
SIZE = "$(AVR_BIN)/avr-size.exe"

BUILD_DIR = build

INCLUDES = \
	-I. \
	-IApp \
	-IBSP \
	-ICommon \
	-IData \
	-IDriver/Components \
	-IDriver/Devices \
	-IMiddleware

SRCS = \
	App/app_lcd.c \
	App/app_uart.c \
	App/main.c \
	BSP/bsp_button.c \
	BSP/bsp_dht.c \
	BSP/bsp_lcd.c \
	BSP/bsp_led.c \
	BSP/bsp_timer.c \
	BSP/bsp_uart.c \
	Data/data_button.c \
	Data/data_dht.c \
	Driver/Components/gpio.c \
	Driver/Components/twi.c \
	Driver/Components/uart.c \
	Driver/Devices/lcd_16x2.c \
	Middleware/fifo.c \
	Middleware/sys_time.c

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra $(INCLUDES)
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

all: $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

$(BUILD_DIR)/$(TARGET).elf: $(SRCS) | $(BUILD_DIR)
	@echo Compiling and linking project...
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo Generating HEX file: $@
	@$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo Memory usage:
	@$(SIZE) --format=avr --mcu=$(MCU) $<

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo Cleaned build directory.

.PHONY: all clean

