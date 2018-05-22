
CXX      = g++   


CXXFLAGS = -Wall -lm -O3 -lz -Ilibgab -Ilibgab/gzstream/ -c
LDFLAGS  = -lz

all: dist2nj

%.o: %.cpp
	${CXX} ${CXXFLAGS} $^ -o $@

libgab/utils.h:
	rm -rf libgab/
	git clone --recursive https://github.com/grenaud/libgab.git

libgab/utils.o:  libgab/utils.h
	cd libgab && make utils.o && make -C gzstream/ && cd ..



dist2nj.o:	dist2nj.cpp
	${CXX} ${CXXFLAGS} dist2nj.cpp

dist2nj:	dist2nj.o NjTree.o Tree.o NodeTree.o UnrootedNode.o mistarArmadillo.o libgab/utils.o  libgab/gzstream/libgzstream.a
	${CXX} -o $@ $^ $(LDLIBS) $(LDFLAGS)

clean :
	rm -f *.o dist2nj

