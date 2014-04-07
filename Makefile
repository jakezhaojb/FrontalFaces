CFLAGS = `pkg-config opencv --cflags`
LIBS = `pkg-config opencv --libs`

test : test.cpp
	g++ $(CFLAGS) -I lib/ -o $@ $< lib/*.cpp $(LIBS) 

#test : example2.cpp flandmark_detector.cpp  flandmark_detector.h  liblbp.cpp  liblbp.h  msvc-compat.h
#       g++ `pkg-config opencv --cflags` -I lib/ example2.cpp lib/*.cpp -o test `pkg-config opencv --libs`
