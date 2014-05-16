cc = g++
CFLAGS = `pkg-config opencv --cflags`
LDFLAGS = `pkg-config opencv --libs`
INCLUDE = -Isrc/
FLAGS = -std=c++11

OBJ = main.o cpdb_compute.o flandmark_detector.o liblbp.o

test : $(OBJ)
	$(cc) $(CFLAGS) -o test $(OBJ) $(LDFLAGS) $(FLAGS)
	rm $(OBJ)

main.o : main.cpp
	$(cc) -c $(INCLUDE) main.cpp $(FLAGS)

cpdb_compute.o : src/cpdb_compute.cpp
	$(cc) -c src/cpdb_compute.cpp

flandmark_detector.o : src/flandmark_detector.cpp
	$(cc) -c src/flandmark_detector.cpp

liblbp.o : src/liblbp.cpp
	$(cc) -c src/liblbp.cpp

.PHONY : clean
clean:
	rm test
