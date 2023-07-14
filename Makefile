SRC = $(wildcard src/cpp/*.cpp)
OBJ = $(SRC:.cpp=.o)

PYTHON_INCLUDE = /usr/include/python3.10
MAGICK++_INCLUDE = /usr/include/ImageMagick

CPPFLAGS = -Wall -std=c++11 -I$(MAGICK++_INCLUDE) -I$(PYTHON_INCLUDE)

all: imageFilter

%.o: %.cpp
	g++ -c -o $@ $< $(CPPFLAGS)

imageFilter: $(OBJ)
	g++ -o $@ $^

clean:
	rm -rf src/*.o ./imageFilter