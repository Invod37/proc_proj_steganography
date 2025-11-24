CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c99
CXXFLAGS = -Wall -Wextra -std=c++14 

TEST_LIBS = -lgtest -lgtest_main -lpthread

LIB_OBJ = src/bmp_utils.o src/steg_utils.o src/steg_encode.o src/steg_decode.o

MAIN_OBJ = main.o

TEST_OBJ = tests/test_logic.o

TARGET = stegtool
TEST_TARGET = run_tests


all: $(TARGET)


$(TARGET): $(MAIN_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_OBJ) $(LIB_OBJ)


tests: $(TEST_TARGET)
$(TEST_TARGET): $(TEST_OBJ) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJ) $(LIB_OBJ) $(TEST_LIBS)


main.o: main.c
	$(CC) $(CFLAGS) -c main.c

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@


tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o src/*.o tests/*.o $(TARGET) $(TEST_TARGET)

.PHONY: all tests clean