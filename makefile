
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncurses

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparse.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)settings.o $(OBJECTFOLDER)hexme.o $(OBJECTFOLDER)exceptions.o 

main: exceptions argparse hdetect settings hexme
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) -I $(SOURCEFOLDER) $(SOURCEFOLDER)$(MAINFILE)

exceptions:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

argparse:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)argparse/argparse.cpp -o $(OBJECTFOLDER)argparse.o 

hdetect:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 

settings:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)settings/settings.cpp -o $(OBJECTFOLDER)settings.o 

hexme:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)hexme/hexme.cpp -o $(OBJECTFOLDER)hexme.o 