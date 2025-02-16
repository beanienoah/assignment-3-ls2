#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"
#include <unistd.h>

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		printf("Usage: %s <path> [exact-match-pattern]\n", argv[0]);
		return 0;
	}

	char *path = argv[1];
	if (argc == 2) { // using mode 1
		mode1(path);
	} else if (argc == 3) { // using mode 2
		char *lookingfor = argv[2];
		mode2(path, lookingfor);
	}
	return 0;
}
