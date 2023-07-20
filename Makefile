SRC = $(wildcard src/cpp/*.cpp)
OBJ = $(SRC:.cpp=.o)

PYTHON_INCLUDE = /usr/include/python3.10
PYTHON_LIB = python3.10
MAGICK++_INCLUDE = /usr/local/include/ImageMagick-7
MAGICK++_LIB = Magick++-7.Q16HDRI
MAGICK_LIB_PATH = /usr/local/lib

CPPFLAGS = -Wall -std=c++11 -I$(PYTHON_INCLUDE) -l$(PYTHON_LIB) -I$(MAGICK++_INCLUDE) -L$(MAGICK_LIB_PATH) -l$(MAGICK++_LIB)

all: ImageProcessing

%.o: %.cpp
	g++ -c -o $@ $< $(CPPFLAGS)

ImageProcessing: $(OBJ)
	g++ -o $@ $^ $(CPPFLAGS)

clean:
	rm -rf src/cpp/*.o ./ImageProcessing tmp/*