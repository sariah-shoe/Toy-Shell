# Default target: build both
all: shell addNum

# Build the shell
shell: shell.c
	gcc shell.c -o shell

# Build addNum
addNum: addNum.c
	gcc addNum.c -o addNum

# Clean up binaries
clean:
	rm -f shell addNum