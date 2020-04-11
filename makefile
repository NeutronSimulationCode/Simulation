ifeq ($(OS),Windows_NT)
	CXX=.
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CXX=g++
	endif
	ifeq ($(UNAME_S),Darwin)
		CXX=/usr/local/opt/llvm/bin/clang++
	endif
endif

CXX_FLAGS=-O3 -fopenmp -I./include
ROOT_FLAGS=`root-config --cflags` -I./include -g -fpic
ROOT_LIBS=`root-config --glibs --libs`
HEADERS=Punto.h Retta.h Neutron.h Generatore.h Propagatore.h Rivelatore.h
OBJECTS=bin/Simulatore_dict.o bin/Punto.o bin/Retta.o bin/Neutron.o bin/Generatore.o bin/Propagatore.o bin/Rivelatore.o bin/Simulatore.o 

$(shell mkdir -p bin)
$(shell mkdir -p objects)

all : bin/Simulatore 

bin/%.o : ./bin/%.cxx
	$(CXX) $(CXX_FLAGS) $(ROOT_FLAGS) -c $< $(ROOT_LIBS) -o $@

bin/%.o : ./src/%.cxx
	$(CXX) $(CXX_FLAGS) $(ROOT_FLAGS) -c $< $(ROOT_LIBS) -o $@

bin/Simulatore_dict.cxx:  include/Simulatore_LinkDef.h $(include/HEADERS)
	rootcint -f $@ -I./include $(HEADERS) include/Simulatore_LinkDef.h

bin/Simulatore : $(OBJECTS)
	$(CXX) $(CXX_FLAGS) $(ROOT_FLAGS) $^ $(ROOT_LIBS) -o $@

clean : 
	rm -rf ./bin/*
	rm -f ./bin/Simulatore_dict.cxx ./bin/Simulatore_dict_rdict.pcm