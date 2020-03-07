
COMPILER = g++
FLAGS = -lncurses
OUTPUTEXEC = hexme
SOURCEFOLDER = ./source/
MAIN = hexme.cpp

main:
	$(COMPILER) $(FLAGS) -o $(OUTPUTEXEC) -I $(SOURCEFOLDER) $(SOURCEFOLDER)$(MAIN)