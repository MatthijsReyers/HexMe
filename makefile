
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncurses

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparse.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)exceptions.o $(OBJECTFOLDER)app.o $(OBJECTFOLDER)settings.o 

main: exceptions argparse hdetect settings app
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) -I $(SOURCEFOLDER) $(SOURCEFOLDER)$(MAINFILE)

exceptions:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

argparse:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/argparse/argparse.cpp -o $(OBJECTFOLDER)argparse.o 

hdetect:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 

settings:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)settings/settings.cpp -o $(OBJECTFOLDER)settings.o 

app:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)app.cpp -o $(OBJECTFOLDER)app.o



run:
	./bin/hexme README.md