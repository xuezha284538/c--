#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int dir_count = 0;
int file_count = 0;
long long total_lines = 0;

void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void count_lines(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return;
    
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            total_lines++;
        }
    }
    fclose(fp);
}

void traverse_dir(const char *name, int depth) {
    DIR *dir = opendir(name);
    if (!dir) {
        perror("无法打开目录");
        return;
    }
    
    struct dirent *entry;
    char path[10000];
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        sprintf(path, "%s/%s", name, entry->d_name);
        
        if (entry->d_type == DT_DIR) {
            dir_count++;
            print_indent(depth);
            printf("[%s]\n", entry->d_name);
            traverse_dir(path, depth + 1);
        } else if (entry->d_type == DT_REG) {
            file_count++;
            print_indent(depth);
            printf("%s\n", entry->d_name);
            count_lines(path);
        }
    }
    
    closedir(dir);
}

int main() {
    const char *root_path = "/home/xuezha/0612";
    printf("目录树:\n");
    printf("[%s]\n", root_path);
    traverse_dir(root_path, 1);
    
    printf("\n统计结果:\n");
    printf("文件夹数目: %d\n", dir_count);
    printf("文件数目: %d\n", file_count);
    printf("所有文件总行数: %lld\n", total_lines);
    
    return EXIT_SUCCESS;
}