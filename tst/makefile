CC     =g++
IDIR   =../include
SDIR   =../src

IFLAGS =-I$(IDIR) -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE\mswu"
CFLAGS =-MMD -std=c++11 -ffast-math -pipe -mthreads -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -O3 $(IFLAGS) -c
LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE"

all: mb01.exe

mb01.exe: mb01.o FractalBitmap.o Mandelbrot.o
	$(CC) $(LFLAGS) -o mb01.exe mb01.o FractalBitmap.o Mandelbrot.o -s -mthreads -lwxmsw30u -mwindows
mb01.o: mb01.cpp
	$(CC) $(CFLAGS) mb01.cpp -o mb01.o
FractalBitmap.o: $(SDIR)/FractalBitmap.cpp
	$(CC) $(CFLAGS) $(SDIR)/FractalBitmap.cpp -o FractalBitmap.o
Mandelbrot.o:    $(SDIR)/Mandelbrot.cpp
	$(CC) $(CFLAGS) $(SDIR)/Mandelbrot.cpp    -o Mandelbrot.o
