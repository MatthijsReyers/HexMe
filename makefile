
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncurses

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparse.o

main: argparse
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) -I $(SOURCEFOLDER) $(SOURCEFOLDER)$(MAINFILE)

argparse:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)argparse/argparse.cpp -o $(OBJECTFOLDER)argparse.o 