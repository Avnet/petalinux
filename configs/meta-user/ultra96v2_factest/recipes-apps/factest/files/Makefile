APP = factest

LDLIBS += -lm -lpthread -lbluetooth

DEPS = test_zcu100.h typedef.h

# Add any other object files to this list below
APP_OBJS = ultra96v2-production-test.o
APP_OBJS += common_util.o
APP_OBJS += test_bt.o
APP_OBJS += test_dp.o
APP_OBJS += test_eeprom.o
APP_OBJS += test_flash.o
APP_OBJS += test_gpio.o
APP_OBJS += test_sd.o
APP_OBJS += test_usb.o
APP_OBJS += test_wifi.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o factest $(APP_OBJS) $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean:
	-rm -f $(APP) *.elf *.gdb *.o