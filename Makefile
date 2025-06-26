CC = gcc -std=c99
W_FLAGS = -Wall -Wextra -Wconversion -Wshadow -Wstrict-overflow -Wfloat-equal \
	-Wformat=2 -Wstrict-aliasing -Wcast-align \
	-Wstrict-prototypes -Wpointer-arith -Wundef -Wnull-dereference
MILD_W_FLAGS = $(W_FLAGS) -Wno-unused-variable -Wno-unused-parameter
EXTRA_W_FLAGS = $(W_FLAGS) -fanalyzer -Wno-analyzer-null-dereference
FAST_FLAGS = -O3 -march=native -ffast-math 
DEBUG_FLAGS = -O0 -DDBG -ggdb -fsanitize=address,undefined
LINK_FLAGS =

release: build
	$(CC) -o build/release src/main.c $(FAST_FLAGS) $(LINK_FLAGS)

debug: build 
	$(CC) -o build/debug src/main.c $(SLOW_FLAGS) $(DEBUG_FLAGS) $(MILD_W_FLAGS) $(LINK_FLAGS)

analyzer: build 
	$(CC) -fsyntax-only src/main.c $(DEBUG_FLAGS) $(EXTRA_W_FLAGS) $(LINK_FLAGS)

build: 
	mkdir -p build

.PHONY: clean
clean:
	rm -rf build
