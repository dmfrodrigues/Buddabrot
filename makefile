CC     =g++

SDIR   =./src
IDIR   =-I./include -I../fractal-app/include
ODIR   =./obj
BDIR   =./bin

IFLAGS =$(IDIR) -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE\mswu" -I"D:\_ProgrammingLibraries\mcap"
CFLAGS =-MMD -std=c++11 -ffast-math -O3 $(IFLAGS) -c

#LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE"
LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE" -L"D:\dmfro\ProgProjects\cpp\fractal-app\lib" -lfractalapp

all: makefolders $(BDIR)/main.exe

makefolders:
	mkdir -p obj
	mkdir -p bin

$(BDIR)/main.exe:              $(ODIR)/Buddhabrot.o $(ODIR)/FractalApp.o
	$(CC) -o $(BDIR)/main.exe $(ODIR)/Buddhabrot.o $(ODIR)/FractalApp.o $(LFLAGS) -s -mthreads -lwxmsw30u -mwindows
$(ODIR)/Buddhabrot.o:          $(SDIR)/Buddhabrot.cpp
	$(CC) $(CFLAGS)            $(SDIR)/Buddhabrot.cpp          -o $(ODIR)/Buddhabrot.o
$(ODIR)/FractalApp.o:          $(SDIR)/FractalApp.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalApp.cpp          -o $(ODIR)/FractalApp.o

-include $(ODIR)/*.d
