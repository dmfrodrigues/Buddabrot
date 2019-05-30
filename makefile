CC     =g++

SDIR   =./src
IDIR   =-I./include -I../FractalApp/include
ODIR   =./obj
BDIR   =./bin

IFLAGS =$(IDIR) -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE\mswu" -I"D:\_ProgrammingLibraries\mcap"
CFLAGS =-MMD -std=c++11 -ffast-math -pipe -mthreads -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -O3 $(IFLAGS) -c

#LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE"
LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE" -L"D:\dmfro\ProgProjects\cpp\FractalApp\lib" -lfractalapp

all: makefolders $(BDIR)/main.exe

makefolders:
	mkdir -p obj
	mkdir -p bin

$(BDIR)/main.exe:                       $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o
	$(CC) -o $(BDIR)/main.exe $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o $(LFLAGS) -s -mthreads -lwxmsw30u -mwindows
$(ODIR)/Mandelbrot.o:          $(SDIR)/Mandelbrot.cpp
	$(CC) $(CFLAGS)            $(SDIR)/Mandelbrot.cpp          -o $(ODIR)/Mandelbrot.o
$(ODIR)/FractalApp.o:          $(SDIR)/FractalApp.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalApp.cpp          -o $(ODIR)/FractalApp.o

-include $(ODIR)/*.d
