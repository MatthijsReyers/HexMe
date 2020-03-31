
CXX = g++
CXXFLAGS = -xc++ -Wall -lstdc++ -shared-libgcc -lncursesw

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(shell find . -name "*.cpp" | grep "./source/" | grep -v "main.cpp" | xargs basename -a | cut -d "." -f 1 | sed 's,^,$(OBJECTFOLDER),' | sed 's/$$/.o/' | xargs)

main: exceptions.o argparser cmdparser file hdetect converters stringtools textbox viewer app
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(OUTPUTFOLDER)$(OUTPUTEXEC) $(SOURCEFOLDER)$(MAINFILE)

app:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)app/app.cpp -o $(OBJECTFOLDER)app.o 

cmdparser:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)app/cmdparser/cmdparser.cpp -o $(OBJECTFOLDER)cmdparser.o 

setup:
	clear
	mkdir $(OBJECTFOLDER)
	mkdir $(OUTPUTFOLDER)

cleanup:
	clear
	rm -rf $(OBJECTFOLDER)
	rm -rf $(OUTPUTFOLDER)

run:
	./bin/hexme README.md

# App spesific stuff.
# =======================================================
exceptions.o:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

settings:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)settings/settings.cpp -o $(OBJECTFOLDER)settings.o 

# Utils namespace
# =======================================================
argparser:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/argparser/argparser.cpp -o $(OBJECTFOLDER)argparser.o 

file:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/file/file.cpp -o $(OBJECTFOLDER)file.o 

hdetect:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 

converters:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/converters/converters.cpp -o $(OBJECTFOLDER)converters.o 

stringtools:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/stringtools/escape.cpp -o $(OBJECTFOLDER)escape.o 

# Gui namespace
# =======================================================
textbox:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)gui/textbox/textbox.cpp -o $(OBJECTFOLDER)textbox.o 

viewer:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)gui/viewer/viewer.cpp -o $(OBJECTFOLDER)viewer.o 
