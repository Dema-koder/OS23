#include <stdio.h>
#include <dirent.h>

int main(void) {
    DIR *d = opendir("/");
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) 
        printf("%s\n", dir->d_name);
    closedir(d);
}
