CFLAGS = -O3
LDFLAGS =
appname = SorceryOfUvutu.exe

all: $(appname)
clean:
	rm -f $(appname) *.o
.PHONY: all clean

sdl_cflags := $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_image)
sdl_libs := $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image)
override CFLAGS += $(sdl_cflags)
override LIBS += $(sdl_libs)

$(appname): main.c outerSDL.o version.h
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) -lm
