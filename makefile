
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncursesw -std=c++17

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)logging.o $(OBJECTFOLDER)argparser.o $(OBJECTFOLDER)cmdparser.o $(OBJECTFOLDER)exceptions.o $(OBJECTFOLDER)file.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)converters.o $(OBJECTFOLDER)escape.o $(OBJECTFOLDER)textbox.o $(OBJECTFOLDER)viewer.o $(OBJECTFOLDER)app.o

main: exceptions logging argparser cmdparser file hdetect converters stringtools textbox viewer app
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) $(SOURCEFOLDER)$(MAINFILE)

app:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)app/app.cpp -o $(OBJECTFOLDER)app.o 

logging:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)logging.cpp -o $(OBJECTFOLDER)logging.o 

cmdparser:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)app/cmdparser/cmdparser.cpp -o $(OBJECTFOLDER)cmdparser.o 

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

stringtools:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)utils/stringtools/escape.cpp -o $(OBJECTFOLDER)escape.o 

# Gui namespace
# =======================================================
textbox:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/textbox/textbox.cpp -o $(OBJECTFOLDER)textbox.o 

viewer:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/viewer/viewer.cpp -o $(OBJECTFOLDER)viewer.o 
