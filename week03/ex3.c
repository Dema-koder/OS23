#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct Directory {
	char name[70];
	struct File* files[3000];
	struct Directory* sub_dirs[3000];
	int nf;
	int nd;
	char path[3000];
};

struct File {
	int id;
	char name[70];
	int size;
	char data[3000];
	struct Directory* directory;
};

struct File* create_file(char* name) {
	struct File* file = (struct File*)(malloc(sizeof(struct File)));
	strcpy(file->name, name);
	return file;
}

struct Directory* create_dir(char* name) {
	struct Directory* dir = (struct Directory*)(malloc(sizeof(struct Directory)));
	strcpy(dir->name, name);
	return dir;
}

void add_file(struct File* file, struct Directory* dir) {
	dir->nf++;
	file->directory = dir;
	dir->files[dir->nf-1] = file;
}

void overwrite_to_file(struct File* file, const char* str) {
	strcpy(file->data, str);
	file->size = sizeof(str);
}

void append_to_file(struct File* file, const char* str) {
	file->size += sizeof(str);
	strcat(file->data, str);
}

void printp_file(struct File* file) {
	printf("%s/%s\n", file->directory->path, file->name);
}

void show_file(struct File* file) {
	printf("%s ", file->name);
}
void show_dir(struct Directory* dir) {
	printf("\nDIRECTORY\n");
	printf(" path: %s\n", dir->path);
	printf(" files:\n");
	printf(" [ ");
	for (int i = 0; i < dir->nf; i++) {
		show_file(dir->files[i]);
	}
	printf("]\n");
	printf(" directories:\n");
	printf(" { ");
	for (int i = 0; i < dir->nd; i++) {
		show_dir(dir->sub_dirs[i]);
	}
	printf("}\n");
}

#define MAX_PATH 2048
void add_dir(struct Directory* dir1, struct Directory* dir2) {
	if (dir1 && dir2) {
		dir2->sub_dirs[dir2->nd] = dir1;
		dir2->nd++;
		char temp_path[MAX_PATH];
		if (strcmp(dir2->path, "/")) {
			strcpy(temp_path, dir2->path);
			strcat(temp_path, "/");
			strcat(temp_path, dir1->name);
			strcpy(dir1->path, temp_path);
		} else {
			strcpy(temp_path, "/");
			strcat(temp_path, dir1->name);
			strcpy(dir1->path, temp_path);
		}
	}
}

int main(void) {
	struct Directory* root = create_dir("/");
	struct Directory* home = create_dir("home");
	struct Directory* bin = create_dir("bin");
	add_dir(home, root);
	add_dir(bin, root);
	struct File* bash = create_file("bash");
	add_file(bash, bin);
	struct File* ex3_1 = create_file("ex3_1.c");
	struct File* ex3_2 = create_file("ex3_2.c");
	add_file(ex3_1, home);
	add_file(ex3_2, home);
	overwrite_to_file(ex3_1, "int printf(const char * format, ...);");
	overwrite_to_file(ex3_2, "“//This is a comment in C language");
	overwrite_to_file(bash, "Bourne Again Shell!!");
	append_to_file(ex3_1, "int main(){printf(\"Hello World!\")}");
	printp_file(bash);
	printp_file(ex3_1);
	printp_file(ex3_2);
	show_dir(root);
}
