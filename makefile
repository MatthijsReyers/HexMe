
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncurses

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparse.o $(OBJECTFOLDER)exceptions.o $(OBJECTFOLDER)file.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)commandline.o $(OBJECTFOLDER)app.o $(OBJECTFOLDER)settings.o 

main: exceptions argparse file hdetect commandline app
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) -I $(SOURCEFOLDER) $(SOURCEFOLDER)$(MAINFILE)

exceptions:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

settings:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)settings/settings.cpp -o $(OBJECTFOLDER)settings.o 

app:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)app.cpp -o $(OBJECTFOLDER)app.o

# Utils namespace
# =======================================================
argparse:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/argparse/argparse.cpp -o $(OBJECTFOLDER)argparse.o 

file:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/file/file.cpp -o $(OBJECTFOLDER)file.o 

hdetect:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 

# Gui namespace
# =======================================================
commandline:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/commandline/commandline.cpp -o $(OBJECTFOLDER)commandline.o 

# maingui:
# 	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/main/main.cpp -o $(OBJECTFOLDER)main.o 

run:
	./bin/hexme README.md