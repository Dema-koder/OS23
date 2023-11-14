#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define LEN_NAME 16
#define MAX_EVENTS 1024
#define BUF_LEN (MAX_EVENTS * ((sizeof(struct inotify_event)) + LEN_NAME))

int fileDescriptor, watchDescriptor;
char filePath[PATH_MAX];

void handleSignal(int signal) {
    if (signal == SIGINT) {
        printf("\n*****************\n");
        traverseDirectory(filePath);
        inotify_rm_watch(fileDescriptor, watchDescriptor);
        close(fileDescriptor);
        exit(0);
    }
}

void traverseDirectory(const char *directoryPath) {
    char entryPath[PATH_MAX];
    struct stat fileInfo;
    struct dirent *entry;
    DIR *dir = opendir(directoryPath);
    if (!dir) return;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            sprintf(entryPath, "%s/%s", directoryPath, entry->d_name);
            if (stat(entryPath, &fileInfo) == -1) {
                perror("stat");
                continue;
            }
            if (S_ISDIR(fileInfo.st_mode))
                traverseDirectory(entryPath);
            printf("File: %s, i-node: %lu, links: %lu \n", entry->d_name, fileInfo.st_ino, fileInfo.st_nlink);
        }
    }
    closedir(dir);
}

void printFileInfo(const char *source) {
    struct stat fileInfo;
    if (stat(filePath, &fileInfo) == -1) {
        perror("stat");
        return;
    }
    printf("*****************\n");
    printf("Name: %s, i-node: %lu, links: %lu \n", source, fileInfo.st_ino, fileInfo.st_nlink);
    printf("*****************\n");

}

int main(int argc, char *argv[]) {
    int length;
    struct stat fileInfo;
    strcpy(filePath, argv[1]);
    traverseDirectory(filePath);
    printf("*****************\n");
    char buffer[BUF_LEN];
    fileDescriptor = inotify_init();
    watchDescriptor = inotify_add_watch(fileDescriptor, argv[1], IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    signal(SIGINT, handleSignal);
    
    while(1) {
        length = read(fileDescriptor, buffer, BUF_LEN);
        if (length < 0) 
            perror("read");
        char *ptr = buffer;
        while (ptr < buffer+length) {
            struct inotify_event *event = (struct inotify_event *)ptr;
            if (event->len) {
                char absolutePath[PATH_MAX];
                sprintf(absolutePath, "%s/%s", argv[1], event->name);
                
                if (strcmp(event->name, ".") == 0) {
                    ptr+= sizeof(struct inotify_event) + event->len;
                    continue;
                }
                
                if (event->mask & IN_ACCESS) {
                    printf("%s accessed\n", event->name);
                } else if (event->mask & IN_CREATE) {
                    printf("%s created\n", event->name);
                    printFileInfo(absolutePath);
                } else if (event->mask & IN_DELETE) {
                    printf("%s deleted\n", event->name);
                } else if (event->mask & IN_MODIFY) {
                    printf("%s modified\n", event->name);
                    printFileInfo(absolutePath);
                } else if (event->mask & IN_OPEN) {
                    printf("%s opened\n", event->name);
                    printFileInfo(absolutePath);
                } else if (event->mask & IN_ATTRIB) {
                    printf("%s metadata changed\n", event->name);
                    printFileInfo(absolutePath);
                }
            }
            ptr+= sizeof(struct inotify_event) + event->len;
        }
    }
    
    inotify_rm_watch(fileDescriptor, watchDescriptor);
    close(fileDescriptor);
    return 0;
}

