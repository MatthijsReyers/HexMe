
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncursesw

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparser.o $(OBJECTFOLDER)exceptions.o $(OBJECTFOLDER)file.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)converters.o $(OBJECTFOLDER)textbox.o $(OBJECTFOLDER)viewer.o

main: exceptions argparser file hdetect converters textbox viewer
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) $(SOURCEFOLDER)$(MAINFILE)

setup:
	clear
	mkdir $(OBJECTFOLDER)

cleanup:
	clear
	rm -rf $(OBJECTFOLDER)

run:
	./bin/hexme README.md

# App spesific stuff.
# =======================================================
exceptions:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

settings:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)settings/settings.cpp -o $(OBJECTFOLDER)settings.o 

# Utils namespace
# =======================================================
argparser:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/argparser/argparser.cpp -o $(OBJECTFOLDER)argparser.o 

file:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/file/file.cpp -o $(OBJECTFOLDER)file.o 

hdetect:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 

converters:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/converters/converters.cpp -o $(OBJECTFOLDER)converters.o 

# Gui namespace
# =======================================================
textbox:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/textbox/textbox.cpp -o $(OBJECTFOLDER)textbox.o 

viewer:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/viewer/viewer.cpp -o $(OBJECTFOLDER)viewer.o 
