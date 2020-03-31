
CXX = g++
CXXFLAGS = -xc++ -Wall -shared-libgcc -lncursesw

SOURCEFOLDER = ./source/
OBJECTFOLDER = ./obj/
OUTPUTFOLDER = ./bin/

OUTPUTEXEC = hexme
MAINFILE = main.cpp

OBJECTS = $(shell find . -name "*.cpp" | grep "./source/" | grep -v "main.cpp" | xargs basename -a | cut -d "." -f 1 | sed 's,^,$(OBJECTFOLDER),' | sed 's/$$/.o/' | xargs)

main: gui utils exceptions app cmdparser
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
exceptions:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)exceptions/exceptions.cpp -o $(OBJECTFOLDER)exceptions.o 

# Utils namespace
# =======================================================
utils:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/argparser/argparser.cpp -o $(OBJECTFOLDER)argparser.o 
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/file/file.cpp -o $(OBJECTFOLDER)file.o 
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/hdetect/hdetect.cpp -o $(OBJECTFOLDER)hdetect.o 
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/converters/converters.cpp -o $(OBJECTFOLDER)converters.o 
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)utils/stringtools/escape.cpp -o $(OBJECTFOLDER)escape.o 

# Gui namespace
# =======================================================
gui:
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)gui/textbox/textbox.cpp -o $(OBJECTFOLDER)textbox.o 
	$(CXX) $(CXXFLAGS) -c $(SOURCEFOLDER)gui/viewer/viewer.cpp -o $(OBJECTFOLDER)viewer.o 
