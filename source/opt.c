/*
 * author: An Lam
 * Program: find command
 * Created Date: 4/17/2022
 * Update By: An Lam
 * Modified Date: 4/17/2022
 *
 * This program provides an example of how to parse arguments using getopt().
 * This program can be integrated into your 2nd programming project about the
 * find utility to parse multiple arguments. Note that this example uses a
 * short option for command-line arguments, e.g. -n instead of -name, -m
 * instead of -mmin, and this is acceptable in your program too.
 * 
 * Compile this code: gcc -o getopt-example getopt-example.c
 * 
 * Sample run: 
 * $./getopt-example -w Document -n foo -a delete 
 * $./getopt-example -w Document -n myfile -a delete 
 * $./getopt-example -w Document -m -10
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

//copy a file 
void copyFile(char *source, char *dest);

//copy a dir
void copyDir(char *souce, char *dest);

//do action on a specific path
int act(char *where, char *action, char **argvRemain);

//check if a file or directory is in modified range
int is_mmin(char *where, char *mmin);

//check if a path is valid
int isFound(char *where, char *subName,  char *name, char *mmin, char *inum);

//find a files/dirs
void find(char *where, char *name, char *mmin, char *inum, char* delete,  char *action, char **argvRemain);

//get directory from a file path
char *getDir(char *filePath);

//get the last section of a path
char *getLastDir(char *path);

//remove all non-empty directory
void removeDir(char *path);


void removeDir(char *path){
	//left, root, right: delete leaf first, then delete root
	DIR *dir;
	char slash[] = "/";
	char dot[] = ".";
	char dotdot[] = "..";
	struct dirent *de;
	if((dir = opendir(path)) != NULL){
		while((de = readdir(dir))){
			if(strcmp(dot, de->d_name) != 0 && strcmp(dotdot, de->d_name) != 0 ){
				DIR *tmp;
				
				char *tmpPath = malloc(sizeof(char)*256);
				tmpPath = strcpy(tmpPath, path);
				tmpPath = strcat(tmpPath, slash);
				tmpPath = strcat(tmpPath, de->d_name);

				if(opendir(tmpPath) != NULL){
					closedir(tmp);
					removeDir(tmpPath);		
				}else{
					remove(tmpPath);
				}
			}
		}
		remove(path);
	}
}
char *getDir(char *filePath){
	char *dirpath;
	int len = 0;
	//count length of filePath
	while(filePath[len++] != '\0');
	//count length of directory
	while(filePath[len--] != '/');
	//dirpath = filePath[0,l]

	len++;
	dirpath = malloc(sizeof(char)*len);
	int i;
	for(i=0; i<len; i++){
		dirpath[i] = filePath[i];
	}
	return dirpath;
}

char *getLastDir(char *path){
	int len=0, newLen = 0;
	while(path[len++] != '\0');	
	while(path[len] != '/'){
		len--;
		newLen++;
	}
	
	char *lastDir = malloc(sizeof(char)*newLen);
	int i=0;
	for(i = 0; i< newLen -1 ; i++){
		lastDir[i] = path[len + 1 + i];
	}	
	return lastDir;
}

void copyDir(char *source, char *dest){
	DIR *dirS, *dirD;
	struct dirent *deS;
	struct stat entryStat;

	char slash[] = "/";
	char currDir[] = ".";
	char prevDir[] = "..";
	
	//if sourcePath does not exist, then exit
	if((dirS = opendir(source)) == NULL){
		printf("Source Path does not exist\n");
		exit(0);
	}else{
			
		while(deS = readdir(dirS)){
			if(strcmp(currDir, deS->d_name) != 0 && strcmp(prevDir, deS->d_name) != 0){
				
				char *tmpSrc = malloc(sizeof(char)*256);
				char *tmpDes = malloc(sizeof(char)*256);
				
				tmpSrc = strcpy(tmpSrc, source);
				tmpDes = strcpy(tmpDes, dest);	
			
				tmpSrc = strcat(tmpSrc, slash);
				tmpSrc = strcat(tmpSrc, deS->d_name);
				tmpDes = strcat(tmpDes, slash);
				tmpDes = strcat(tmpDes, deS->d_name);
				
				stat(tmpSrc, &entryStat);
				if(S_ISDIR(entryStat.st_mode) == 1){
					mkdir(tmpDes, 0777);
					copyDir(tmpSrc, tmpDes);
				}else if(S_ISREG(entryStat.st_mode) ==1){
					copyFile(tmpSrc, tmpDes);
			
				}
			}

		}
	}


}
void copyFile(char *source, char *dest){
	//fp: source file, fpDes: Destination file			
	FILE *fp, *fpDes;
	fp = fopen(source, "r");
	fpDes = fopen(dest, "w");
	char ch;

	//copy content source file to destination file
	while(ch != EOF){
		ch = fgetc(fp);
		fputc(ch, fpDes);
	}
	fclose(fp);
	fclose(fpDes);
}

int act(char *where, char *action, char **argvRemain){
	char exce[] = "exec";
	char cat[] ="cat";
	char rm[] = "rm";
	char mv[] = "mv";	
	int isSuccess = 0;

	struct stat	pathStats;
	int isExist = stat(where, &pathStats);
	if(isExist ==0){	
		//if action = cat	
		if(strcmp(cat, action)==0){
			//if path is a regular file, then output file
			if(S_ISREG(pathStats.st_mode) == 1){
				FILE *fp;
				fp = fopen(where, "r");
				char ch;
				while(ch != EOF){
					ch = fgetc(fp);
					printf("%c", ch);
				}
				fclose(fp);
				isSuccess = 1;
			}else{
				printf("%s: Cannot open an directory\n", where);
			}
			exit(0);
		//if action = rm
		}else if(strcmp(rm, action)==0){
			//if path is a file, then remove file
			if(S_ISREG(pathStats.st_mode) == 1){
				remove(where);
				isSuccess = 1;
				printf("%s: Deleted successfully\n", where);
			}else if(S_ISDIR(pathStats.st_mode) == 1){
				printf("%s: This is an directory\n");
			}

		//if action = mv
		}else if(strcmp(mv, action) ==0){
			char *newDirPath = argvRemain[0];
			if(newDirPath== NULL){
				printf("mv: Missing argument \n");
			}else{
				printf("asdA");
				//if file is regular file then create file to new location, delete old file
				if(S_ISREG(pathStats.st_mode) == 1){
					char *df = getDir(newDirPath);
	
					if(opendir(df)){
						copyFile(where, newDirPath);
						remove(where);
						printf("%s: is new location\n", newDirPath);
					}
					else{
						printf("%s: does not exist\n", newDirPath);
					}
				//if file is directory, then copy content of dir to new loc, delete old dir
				}else if(S_ISDIR(pathStats.st_mode) == 1){
					if(!opendir(newDirPath)){
						printf("%s: new Location is not exist\n");
					}else{
						
						//copy source to destination
						char *lastOldDir = getLastDir(where);
						char slash[] ="/";
						strcat(newDirPath, slash);
						strcat(newDirPath, lastOldDir);
						mkdir(newDirPath, 0777);
						copyDir(where, newDirPath);
						removeDir(where);
					}
				}
			}
			exit(0);
		}
	}
	return isSuccess;
	
}

int is_mmin(char *where, char *mmin){
	int ismmin = 0;
	char mod; //modifier of mmin
	int mins; //minutes
			
	//get file status		
	struct stat status;
	int st =stat(where, &status);
	
	//if file exists
	if(st == 0){
		//if the first character is not digit, then mod = mmin[0], mins = mmin[1:end]
		if(!isdigit(mmin[0])){
			mod = mmin[0];
			mins = atoi(++mmin);
		}else{
			mins = atoi(mmin);
		}

		//get file's last modified time
		int duration = 0;
		duration = (int) (time(0) - status.st_mtime)/60;

		//if mod == -, then if file is modified after mins, then it is valid
		if(mod == '-'){
			if(duration < mins) ismmin = 1;	

		//if file is modified before mins
		}else if (mod == '+'){
			if(duration > mins) ismmin = 1;
		//if file is modified exact mins
		}else{
			if(duration == mins) ismmin = 1;
		}
	}
	return ismmin;

}

int isFound(char *where, char *subName,  char *name, char *mmin, char *inum){
	int  isFound = 0;
	if(name != NULL ){
		//if name exists, then $findf [path] -n [name] 
		if(strcmp(subName, name)==0){
			isFound = 1;
			if(mmin != NULL){
				isFound = is_mmin(where, mmin);
			}
		}else{
			return 0;
		}
	}else if(mmin != NULL){
		//check if is mmin {-num, num, +num} {before, exact, after}
		isFound = is_mmin(where, mmin);
	}else if(inum != NULL){
		struct stat fileStatus;
		stat(where, &fileStatus);
		long long fInum = (long long) fileStatus.st_ino;
		if( fInum == atoll(inum)) isFound = 1;
	}
	return isFound;
}


void find(char *where, char *name, char *mmin, char *inum, char *delete,  char *action, char **argvRemain)
{
/*Using deep first search method*/
//printf("%s, %s, %s, %s, %s, %s, %s\n\n\n ", where, name, mmin, inum, delete, action, argvRemain[0]);
	//initialize current directory, previous directory value
	char currDir[] = "."; 
	char prevDir[] = "..";

	//open directory 
	DIR *dir = opendir(where);
	struct dirent *de;
	//if directory exists
	if(dir != NULL){
		//read files/folder at current directory 	
		while((de = readdir(dir)) != NULL){
			//if not a dot or dot dot
			if(strcmp(de->d_name, currDir ) != 0 && strcmp(de->d_name, prevDir) != 0){		
					
				//build the path
				char slash[] = "/";
				char *subDir = slash;
				subDir = strcat(subDir, de->d_name); //surDir="/sub_dir"

				char *fullpath = malloc(sizeof(char)*2000);	
				fullpath = strcpy(fullpath, where);			
				fullpath  = strcat(fullpath, subDir); //where/surDir
				
				//get status of new path
				struct stat status;
				int st =stat(fullpath, &status);
			
				//if {criterias}\{delete} != NULL, then $findf [path] [crit] <-d> [exec] [argv]
								// else #findf [path]
				if(name!= NULL || mmin!= NULL || inum != NULL){	

					int  isPathExist = 0;
						if(name != NULL ){
							//if name exists, then $findf [path] -n [name] 
							if(strcmp(subName, name)==0){
								isPathExist = 1;
								if(mmin != NULL){
									isPathExist = finMin(where, mmin);
								}
							}else{
								return 0;
							}
						}else if(mmin != NULL){
							//check if is mmin {-num, num, +num} {before, exact, after}
							isPathExist = finMin(where, mmin);
						}else if(inum != NULL){
							struct stat fileStatus;
							stat(where, &fileStatus);
							long long fInum = (long long) fileStatus.st_ino;
							if( fInum == atoll(inum)) isPathExist = 1;
						}
					//if citerias meet,then $findf [path] [crit] <-d> [exec] [argv] 
					if(isPathExist != 0){

						//if delete == NULL, then $findf [path] [crit] [exec] [argv]
								//else $findf [path] [crit] -d
						if(delete == NULL){

							//if action == NULL, then $findf [path] [crit]
									//else $findf [path] [crit] [exec] [argv]
							if(action == NULL){
								printf("%s\n", fullpath);
							}else{
								act(fullpath, action, argvRemain);
							}
						}else{
							remove(fullpath);
						}
					}
					
				//$findf [path]
				}else{	
					printf("%s\n", fullpath);
				}
				
				//if action!= NULL, go next dir, else keep level
				if(action == NULL){
					//open next directory
					DIR *nextDir = opendir(fullpath);
				
					//if next directory exists, enter the nextdir
					if(nextDir != NULL ){
						closedir(nextDir);
						find(fullpath,name,  mmin, inum, delete,  action, argvRemain);
					}
				}
			}
		}
		closedir(dir);
	}else{
		printf("Could not find directory %s\n", where);
	}
}

int 
main(int argc, char **argv)
{
	int w, n, m, i, a, d;
	char  *where, *name, *mmin, *inum, *delete,  *action;
	where = NULL; name = NULL; mmin = NULL; inum = NULL; action = NULL; delete = NULL;
	while (1) {
		char c;

		c = getopt(argc, argv, "w:n:m:i:a:d::");	/* A colon (‘:’) to
							 * indicate that it
							 * takes a required
							 * argument, e.g, -w
							 * testdir */
		if (c == -1) {
			/* We have finished processing all the arguments. */
			break;
		}
		switch (c) {
		case 'w':
			w=1;
			where = optarg;
			break;
		case 'n':
			n = 1;
			name = optarg;
			break;
		case 'm':
			m = 1;
			mmin = optarg;
			break;
		case 'i':
			i = 1;
			inum = optarg;
			break;
		case 'a':
			a = 2;
			action = optarg;
			break;
		case 'd':
			d = 0;
			delete = "delete";
			break;
		case '?':
		default:
			exit(0);
		}
	}

	argc -=optind;
	argv +=optind;

	//get where 
	//if where is null get current directory	
	if(w == 0){
		char currDir[] = ".";
		char mv[] = "mv";
		if(where == NULL && argc == 0){
			 where = currDir;
		}else if(where == NULL && argc == 1){
			where = argv[0];
		}

		 if(a!= 0 && strcmp(mv, action)==0){
			if(argc < 2){
				where = currDir;
			}else{
				where = argv[0];
				argc-=1;
				argv+=1;
			}
		}
	}
	find(where, name, mmin, inum, delete,  action, argv);
	return 0;
}

