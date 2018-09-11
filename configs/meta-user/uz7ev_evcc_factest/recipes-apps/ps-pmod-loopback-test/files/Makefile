APP = ps-pmod-loopback-test

# Add any other object files to this list below
APP_OBJS = linux_ps_pmod_loopback_test.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o ps-pmod-loopback-test $(APP_OBJS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -f $(APP) *.elf *.gdb *.o
