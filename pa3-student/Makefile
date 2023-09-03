EXE = pa3
EXETest = testComp

OBJS_EXE = HSLAPixel.o lodepng.o PNG.o main.o imgtree.o imgtree_given.o stats.o
OBJS_EXET = HSLAPixel.o lodepng.o PNG.o testComp.o imgtree.o imgtree_given.o stats.o

CXX = g++
CXXFLAGS = -std=c++1y -c -g -O0 -Wall -Wextra -pedantic 
LD = g++
#LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lpthread -lm
LDFLAGS = -std=c++1y -lpthread -lm 

all : pa3 testComp

$(EXE) : $(OBJS_EXE)
	$(LD) $(OBJS_EXE) $(LDFLAGS) -o $(EXE)

$(EXETest) : $(OBJS_EXET)
	$(LD) $(OBJS_EXET) $(LDFLAGS) -o $(EXETest)

#object files
HSLAPixel.o : cs221util/HSLAPixel.cpp cs221util/HSLAPixel.h
	$(CXX) $(CXXFLAGS) cs221util/HSLAPixel.cpp -o $@

PNG.o : cs221util/PNG.cpp cs221util/PNG.h cs221util/HSLAPixel.h cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/PNG.cpp -o $@

lodepng.o : cs221util/lodepng/lodepng.cpp cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/lodepng/lodepng.cpp -o $@

stats.o : stats.h stats_private.h stats.cpp cs221util/HSLAPixel.h cs221util/PNG.h
	$(CXX) $(CXXFLAGS) stats.cpp -o $@

imgtree.o : imgtree.h imgtree_private.h imgtree.cpp stats.h cs221util/PNG.h cs221util/HSLAPixel.h
	$(CXX) $(CXXFLAGS) imgtree.cpp -o $@

imgtree_given.o : imgtree.h imgtree_private.h imgtree_given.cpp stats.h cs221util/PNG.h cs221util/HSLAPixel.h
	$(CXX) $(CXXFLAGS) imgtree_given.cpp -o $@

testComp.o : testComp.cpp cs221util/PNG.h cs221util/HSLAPixel.h imgtree.h
	$(CXX) $(CXXFLAGS) testComp.cpp -o testComp.o

main.o : main.cpp cs221util/PNG.h cs221util/HSLAPixel.h imgtree.h imgtree.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

clean :
	-rm -f *.o $(EXE) $(EXETest) images-output/*.png
