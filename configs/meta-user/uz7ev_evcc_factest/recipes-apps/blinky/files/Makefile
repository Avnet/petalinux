APP = blinky

# Add any other object files to this list below
APP_OBJS = linux_ps_led_blink.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o blinky $(APP_OBJS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean:
	-rm -f $(APP) *.elf *.gdb *.o
