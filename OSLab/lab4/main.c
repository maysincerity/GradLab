#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char cur_dir[50];

void printinfo(struct stat statbuf) {
    //文件类型
    //S_IFMT是一个掩码，它的值是017000（八进制），用来过滤出前四位表示的文件类型
    switch (statbuf.st_mode & S_IFMT) {
        case S_IFSOCK:
            printf("socket ");
            break;
        case S_IFLNK:
            printf("symbolic link ");
            break;
        case S_IFREG:
            printf("regular file ");
            break;
        case S_IFBLK:
            printf("block device ");
            break;
        case S_IFDIR:
            printf("directory ");
            break;
        case S_IFCHR:
            printf("character device ");
            break;
        case S_IFIFO:
            printf("FIFO<pipe> ");
            break;
        default:
            break;
    }

    //权限信息
    //个人
    if(statbuf.st_mode & S_IRUSR) {
        printf("r");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IWUSR) {
        printf("w");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IXUSR) {
        printf("x");
    } 
    else {
        printf("-");
    }

    //同组
    if(statbuf.st_mode & S_IRGRP) {
        printf("r");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IWGRP) {
        printf("w");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IXGRP) {
        printf("x");
    } 
    else {
        printf("-");
    }

    //他人
    if(statbuf.st_mode & S_IROTH) {
        printf("r");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IWOTH) {
        printf("w");
    } 
    else {
        printf("-");
    }

    if(statbuf.st_mode & S_IXOTH) {
        printf("x");
    } 
    else {
        printf("-");
    }

    //硬连接的数目
    printf(" %ld ",(long) statbuf.st_nlink);

    //文件所有者名字，组名字
    struct passwd *uid = getpwuid(statbuf.st_uid);
    struct group *gid = getgrgid(statbuf.st_gid);
    printf("%s %s ", uid->pw_name, gid->gr_name);

    //文件大小
    printf("%ld ", statbuf.st_size);

    //文件最后修改时间
    char *time = ctime(&statbuf.st_mtime);
    time[strlen(time) - 1]='\0';
    printf("%s ", time);
}


void printdir(char *dir, int depth) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    //打开目录
    if ((dp = opendir(dir)) == NULL) {
        perror("Fail to open dir.\n");
        return;
    }

    //将dir设置为当前目录
    if (chdir(dir) != 0) {
        perror("Fail to change dir.\n");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if ((statbuf.st_mode & S_IFMT) == S_IFDIR) 
        {
            if(strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) 
            {
                continue;
            } 
            else 
            {
                printf("%d ", depth);
                printinfo(statbuf);
                printf("%s\n", entry->d_name);
                printdir(entry->d_name, depth + 1);
            }
        } 
        else 
        {
            printf("%d ", depth);
            printinfo(statbuf);
            printf("%s\n", entry->d_name);
        }
    }

    //返回父目录
    if (chdir("..") < 0) 
    {
        perror("Fail to change dir.\n");
        return;
    }

    //关闭目录项
    closedir(dp);
}

int main() {
    if (getcwd(cur_dir, 50) == NULL) 
    {
        perror("Fail to get current work directory.\n");
        exit(-1);
    }

    printdir(cur_dir, 0);

    return 0;
}
