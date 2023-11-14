#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

char src_path[PATH_MAX];

void process_file(const char* source, const char* newFiles, struct stat src_stat) {
    struct stat newFileStat;
    if (stat(newFiles, &newFileStat) == -1)
        return;
    FILE* file = fopen(newFiles, "r");
    char mess[100];
    fgets(mess, 100, file);
    if (newFileStat.st_ino == src_stat.st_ino && strcmp(source, newFiles) != 0)
        printf("Path: %s i-node: %lu Text: %s \n", newFiles, (unsigned long)newFileStat.st_ino, mess);
    fclose(file);
}

void all_hlinks(const char* source) {
    DIR *dir = opendir(src_path);
    struct stat src_stat;
    stat(source, &src_stat);
    ino_t inodeStat = src_stat.st_ino;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* newFiles = (char*)malloc((strlen(src_path) + strlen(entry->d_name) + 1) * sizeof(char));
            sprintf(newFiles, "%s/%s", src_path, entry->d_name);
            process_file(source, newFiles, src_stat);
            free(newFiles);
        }
    }
    closedir(dir);
}

void unlink_all(const char* source) {
    DIR *dir = opendir(src_path);
    struct stat src_stat;
    stat(source, &src_stat);
    ino_t inodeStat = src_stat.st_ino;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
            char* newFiles = (char*)malloc((strlen(src_path) + strlen(entry->d_name) + 1) * sizeof(char));
            sprintf(newFiles, "%s/%s", src_path, entry->d_name);
            struct stat newFileStat;
            stat(newFiles, &newFileStat);
            if (newFileStat.st_ino == inodeStat && strcmp(source, newFiles) != 0) {
                if (remove(newFiles) == 0) {
                    printf("Delete hard link: %s\n", newFiles);
                } else 
                    printf("Did not find duplicate hard links\n");
            }
            free(newFiles);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    strcpy(src_path, argv[1]);
    FILE *myfile1 = fopen("myfile1.txt", "w");
    fprintf(myfile1, "Hello world\n");
    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");
    link("myfile1.txt", src_path);
    link("myfile11.txt", src_path);
    link("myfile12.txt", src_path);
    all_hlinks("myfile1.txt");
    rename("myfile1.txt", "/tmp/myfile1.txt");
    FILE *myfile11 = fopen("myfile11.txt", "a");
    fprintf(myfile11, "something");
    char *fullPath = (char*) malloc((strlen(src_path) + strlen("myfile13.txt") + 1) *sizeof(char));
    sprintf(fullPath, "%s/%s", src_path, "myfile13.txt");
    symlink("/tmp/myfile1.txt", fullPath);
    FILE *tmpfile = fopen("/tmp/myfile1.txt", "a");
    fprintf(tmpfile, "\nsomething");
    char *pp = (char*) malloc((strlen(src_path) + strlen("myfile11.txt") + 1) *sizeof(char));
    sprintf(pp, "%s/%s", src_path, "myfile11.txt");
    printf("\n");
    unlink_all(pp);
    printf("\n");
    all_hlinks("myfile11.txt");

    fclose(myfile1);
    fclose(myfile11);
    return 0;
}

