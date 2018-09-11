APP = pl-gpio-loopback-test

# Add any other object files to this list below
APP_OBJS = gpio.o
APP_OBJS += linux_pl_gpio_loopback.o
APP_OBJS += pl_gpio_udriver.o


all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o pl-gpio-loopback-test $(APP_OBJS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -f $(APP) *.elf *.gdb *.o
