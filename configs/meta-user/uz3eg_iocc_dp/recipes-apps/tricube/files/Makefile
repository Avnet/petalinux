APP = tricube

# Add any other object files to this list below
APP_OBJS = tricube.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS) -lm -lpthread -ldl \
	-lX11 -lxcb -lXau -lXdmcp -ldrm -lXfixes -lXext -lXdamage -lexpat -lglapi \
	-lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lEGL -lGLESv2 -lGL