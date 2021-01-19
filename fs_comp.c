#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PATH     1000

char logfile[]="fs_comp.log";
FILE *fpl;

void cmp_dir(char *dir1,char *dir2)
{
    FILE	  *fp;
    DIR           *d;
    struct dirent *dir;
    struct stat   st,st2;
    char path1[MAX_PATH],path2[MAX_PATH],parent[MAX_PATH];
    char *p1,*p2;
    int l,l2,dev_parent,ld1,ld2;

    // printf("dir1=%s\n",dir1);
    // printf("dir2=%s\n",dir2);
    ld1 = strlen(dir1);
    if (ld1 > MAX_PATH)
	fprintf(fpl,"%s Länge=%d\n",dir1,ld1);
    ld2 = strlen(dir2);
    if (ld2 > MAX_PATH)
	fprintf(fpl,"%s Länge=%d\n",dir2,ld2);
    d = opendir(dir1);
    if (d == NULL) {
	printf("%s\n",dir1);
	perror("dir1");
	exit(8);
	}
    strcpy(path1,dir1);
    l = strlen(dir1);
    p1 = &path1[l];
    if (dir1[l-1] != '/') {
	strcat(path1,"/");
	p1++;
	}
    // printf("path1=%s",path1);
    // fprintf(fpl,"path1=%s",path1);
    strcpy(path2,dir2);
    l = strlen(dir2);
    p2 = &path2[l];
    if (! (dir2[l-1] == '/')) {
	strcat(path2,"/");
	p2++;
	}
    // printf(" path2=%s\n",path2);
    // fprintf(fpl," path2=%s\n",path2);
    // exit(0);
    while ((dir = readdir(d)) != NULL) {
	// sprintf(path1,"%s/%s",dir1,dir->d_name);
	strcpy(p1,dir->d_name);
	stat(path1,&st);
	if (!strcmp(dir->d_name,"."))
	    continue;
	if (!strcmp(dir->d_name,"..")) {
	    // dev_parent = st.st_dev;
	    continue;
	    }
	strcpy(p2,dir->d_name);
	// if (dir->d_type == DT_REG || dir->d_type == DT_LNK) {
	if (dir->d_type == DT_REG) {
	    if (stat(path2,&st2) == -1) {
		printf("%s File not found.\n",path2);
		fprintf(fpl,"%s File not found.\n",path2);
		perror("stat");
		continue;
		}
	    if (st.st_size != st2.st_size) {
		printf("%s %ld != %ld\n",path1,st.st_size,st2.st_size);
		fprintf(fpl,"%s %ld != %ld\n",path1,st.st_size,st2.st_size);
		}
	    }
	if (dir->d_type == DT_DIR) {
	    strcpy(parent,path1);
	    strcat(parent,"/..");
	    // printf("parent=%s\n",parent);
	    // exit(0);
	    if (stat(parent,&st2) == 0) {
		dev_parent = st2.st_dev;
		if (st.st_dev != dev_parent) {
		    printf("%s dev_change\n",path1);
		    fprintf(fpl,"%s dev_change\n",path1);
		    continue;
		    }
		}
	    if (stat(path2,&st2) == -1) {
		printf("%s Directory not found.\n",path2);
		fprintf(fpl,"%s Directory not found.\n",path2);
		perror("stat");
		continue;
		}
	    cmp_dir(path1,path2);
	    }
	}
    closedir(d);
}

void main(int argc, char **argv)
{
    time_t t1,t2;
    double s;

    if (argc == 1) {
	printf("fs_comp dir1 dir2\n\nCompares two filesystems or directories.\n");
	printf("\nWritten by jsffm\n");
	exit(0);
	}
    t1 = time(NULL);
    fpl = fopen(logfile,"w");
    // d2 = argv[2];
    // ld2 = strlen(d2);
    // fprintf(fpl,"%s %d\n",d2,ld2);
    cmp_dir(argv[1],argv[2]);
    t2 = time(NULL);
    s = difftime(t2,t1);
    printf("%.f Sekunden\n",s);
    fprintf(fpl,"%.f Sekunden\n",s);
    fclose(fpl);
}
