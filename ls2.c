#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"
 
/**
* Counted slashes for indentation. Probably a more efficient way to
* do this, but this is what I settled on.
 */
int indentation(const char *path) {
    int tab = 0;
    for(const char *t = path; *t; t++) {
        if (*t == '/') {
            tab++;
        }
    }
    return tab;
}

/**
* Same as below, but adapted slightly for files.
*
 */
void files(const char *durandal, DIR *dir) {
    struct dirent *tycho;
    while((tycho = readdir(dir)) != NULL){
        // skip "." + ".."
        if(strcmp(tycho->d_name, ".") == 0 || strcmp(tycho->d_name, "..") == 0){
            continue;
        }

        // allocate mem for directory + / + filename
        int mem = strlen(durandal) + strlen(tycho->d_name) + 2; // +2, forgot about null
        char *path = (char*) malloc(mem);

        // constructing the path
        strcpy(path, durandal);
        strcat(path, "/");
        strcat(path, tycho->d_name);

        /**
        * okay, yeah, so this is a rough translation from some online resources. 
        * i think it's distinct enough :D
        */
        struct stat sizes;
        if(stat(path, &sizes) == 0 && !S_ISDIR(sizes.st_mode)) { // if file exists
            int tab = indentation(path);
            for(int i = 1; i < tab; i++) {
                printf("    ");
            }
            printf("%s (%ld bytes)\n", tycho->d_name, sizes.st_size);
        }
        free(path); // byebye malloc
    }
}

/**
* Given some directory, grab subdirectories and print their paths. Skips
* current and parent directories, and makes mem space for the full path of
* a directory + a subdirectory. The path is constructed in my dumb monkey 
* way, and we use stat to see if it's a directory. Indentation is applied, 
* and we recurse as needed.
 */
void directories(const char *durandal, DIR *dir) {
    struct dirent *tycho;
    while((tycho = readdir(dir)) != NULL){
        // skip "." + ".."
        if(strcmp(tycho->d_name, ".") == 0 || strcmp(tycho->d_name, "..") == 0){
            continue;
        }

        // allocate mem for directory + / + filename
        int mem = strlen(durandal) + strlen(tycho->d_name) + 2; // +2, forgot about null
        char *path = (char*) malloc(mem);

        // constructing the path
        strcpy(path, durandal);
        strcat(path, "/");
        strcat(path, tycho->d_name);

        /**
        * okay, yeah, so this is a rough translation from some online resources. 
        * i think it's distinct enough :D
        */
        struct stat sizes;
        if(stat(path, &sizes) == 0 && S_ISDIR(sizes.st_mode)) { // if is directory
            int tab = indentation(path);
            for(int i = 1; i < tab; i++) {
                printf("    ");
            }
            printf("%s/ (directory)\n", tycho->d_name);
            mode1(path); // now we recurse
        }
        free(path); // byebye malloc
    }
}

/**
* This was initially one giant block of code, kept running into
* formatting issues, so I decided to break it up into blocks to 
* (hopefully) fix some of them. mode2 should benefit from this as
* well, when implemented.
*
* Has some odd formatting issues compared to your solution, but it
* all seems to be there, just jumbled up a bit.
* */
void mode1(const char* durandal) {
    DIR* dir = opendir(durandal); // open a directory
    if (dir == NULL) { // if bad directory...
        return;
    }

    directories(durandal, dir);
    rewinddir(dir); // reset since directory has already advanced (cool)
    files(durandal, dir);
    closedir(dir); // neat and tidy
}

/**
* I lied. mode2 did not benefit. but mode1 is clean asf so I'm still
* happy-ish. Decided to just brute force it again because the stack does
* most of the actual logic, which makes helper methods like file/
* directories mostly useless - I could probably refactor this to make
* it look pretty, but I'm tired.
* 
* Well, I tried. Left it as is, but trust me when I say that I tried (a
* LOT) to get the formatting right. It's all in the stack somewhere, but
* I can't for the life of me figure out how the hell to get it out and
* look pretty. I think it stems from me trying to reuse what worked above,
* and just trying to brute force it into working, but honestly I'm probably
* going about it all wrong. Obv not full credit, but it works, and I'm
* calling it quits.
*
* Oh yeah: Builds (or, tries to) a list of paths that match a given directory
* and keyword. Uses a stack to store matching paths, separating files and
* directories using stat. If it's a directory, keep searching, if it's a file
* and it matches the keyword, add it to the stack. Print out the paths on the 
* stack, format them neatly, and we're done. Simple, right? BLEHHH
*/

void mode2(const char* durandal, const char* lookingfor) {
    DIR* dir = opendir(durandal);
    if (dir == NULL) {
        return;
    }
    
    stack_t *roland = initstack(); // store matching paths
    int match = 0; // matching directories
    struct dirent *tycho;
    while((tycho = readdir(dir)) != NULL) {
        if(strcmp(tycho->d_name, ".") == 0 || strcmp(tycho->d_name, "..") == 0){
            continue;
        }

        // allocate mem for directory + / + filename
        int mem = strlen(durandal) + strlen(tycho->d_name) + 2; // +2, forgot about null
        char *path = (char*) malloc(mem);

        // constructing the path
        strcpy(path, durandal);
        strcat(path, "/");
        strcat(path, tycho->d_name);

        // code salad
        struct stat sizes;
        if(stat(path, &sizes) == 0) { // exists
            if(S_ISDIR(sizes.st_mode)) {
                mode2(path, lookingfor); // check subdirectories
                if(match) {
                    push(roland, strdup(path)); // push a !duplicate! path name
                }
            } else if (strcmp(tycho->d_name, lookingfor) == 0) {  // not a directory, does it match?
                push(roland, strdup(path)); // ^^
                match = 1; // match found in this directory
            }
        }
        free(path);
    }

    // sighhhhhhhhhhhhhh. brute force attempt
    if(match) {
        push(roland, strdup(durandal)); // push entire directory since it contains match
    }
    closedir(dir);
    
    // and format! dunno how to fix this! i give up!
    while(roland->size > 0) {
        char *print = (char*) pop(roland);

        int tab = indentation(print); // thank god it wsn't useless (it was)
        for(int i = 1; i< tab; i++) {
            printf("    ");
        }

        struct stat sizes2;
        if(stat(print, &sizes2) == 0) {
            if(S_ISDIR(sizes2.st_mode)) {
                printf("%s/ (directory)\n", print);
            } else {
                printf("%s (%ld bytes)\n", print, sizes2.st_size);
            }
        }
    }
    freestack(roland); // neet and tiedy
}