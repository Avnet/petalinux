APP = mac-eeprom-test

# Add any other object files to this list below
APP_OBJS = mac_eeprom_demo.o
APP_OBJS += mac_eeprom_test.o
APP_OBJS += zed_iic_axi.o
APP_OBJS += main.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o mac-eeprom-test $(APP_OBJS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -f $(APP) *.elf *.gdb *.o
