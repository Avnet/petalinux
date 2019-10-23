APP = i2csensor

# Add any other object files to this list below
APP_OBJS = i2csensor.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)
