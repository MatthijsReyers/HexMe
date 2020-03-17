
COMPILER = gcc
FLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncursesw

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(OBJECTFOLDER)argparse.o $(OBJECTFOLDER)exceptions.o $(OBJECTFOLDER)file.o $(OBJECTFOLDER)hdetect.o $(OBJECTFOLDER)textbox.o $(OBJECTFOLDER)viewer.o $(OBJECTFOLDER)app.o

main: setup exceptions argparse file hdetect textbox viewer app
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) $(SOURCEFOLDER)$(MAINFILE)

setup:
	mkdir $(OBJECTFOLDER)

cleanup:
	rm -rf $(OBJECTFOLDER)

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
textbox:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/textbox/textbox.cpp -o $(OBJECTFOLDER)textbox.o 

viewer:
	$(COMPILER) $(FLAGS) -c $(SOURCEFOLDER)gui/viewer/viewer.cpp -o $(OBJECTFOLDER)viewer.o 

run:
	./bin/hexme README.md