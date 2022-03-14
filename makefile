CXX=g++

CDEFFLAGS=-std=c++17 -Wall -Wextra -Wpedantic
CFLAGS=-O3 -Wl,--strip-all,--build-id=none,--gc-sections -static


default: test.cpp
	$(CXX) $(CDEFFLAGS) $^ -o test.exe $(CFLAGS)
