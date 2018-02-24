TARGET = Krakout
OBJS = csound.o csoundcontrol.o video.o csprite.o cobject.o cfont.o cmenu.o  basis.o cballmap.o cblockmap.o cmissilemap.o cpaddle.o cgame.o main.o ccharactermap.o ccharacter1map.o  ccharacter2map.o ccharacter3map.o ccharacter4map.o ccharacter5map.o ccharacter6map.o ccharacter7map.o ccharacter8map.o ccharacter9map.o
INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR = 
LDFLAGS = 
LIBS = -lm -lstdc++ -lpspaudiolib -lpspaudio -lpsprtc
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Krakout
PSP_EBOOT_ICON = ICON.PNG
PSP_EBOOT_ICON1 =
PSP_EBOOT_UNKPNG = PIC.PNG
PSP_EBOOT_PIC1 =
PSP_EBOOT_SND0 =
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak