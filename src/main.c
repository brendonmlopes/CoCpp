#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

struct stat fileInfo;

int getCompileCommand(char** configFileName, char** compileCommand);

int main(int argc, char** argv){
	printf("FUNCTION CALL:\t\tmain");
	char projectPath[256];
	char filePath[256];
	char* configFilePath = (char*) malloc(256);
	char* compileCommand = (char*) malloc(256);

	time_t lastModTime;

	memset(projectPath,'\0',sizeof(projectPath));
	memset(filePath,'\0',sizeof(filePath));

	if(argc <2){
		printf("Not enough arguments\n");
		return EXIT_FAILURE;
	}


	printf("%s\n",projectPath);
	//Get project path
	snprintf(projectPath,sizeof(projectPath),"%s",argv[1]);
	

	//Add src/main.c on project path
	
	char fileBuffer[13] = "src/main.c\0";
	snprintf(filePath,sizeof(projectPath),"%s/src/main.c",projectPath);

	//Get the compile command from the Config file
	snprintf(configFilePath,256,"%s/config.txt",projectPath);
	getCompileCommand(&configFilePath,&compileCommand);

	printf("PROJECT DIR PATH:\t%s\n",argv[1]);
	printf("MAIN FILE PATH:\t\t%s\n",filePath);
	printf("CONFIG FILE PATH:\t%s\n",configFilePath);
	printf("COMMAND:\t\t%s\n",compileCommand);

	//Listen for changes
	while(true){
		//Save last modification time into lastModTime
		if (stat(filePath, &fileInfo) == 0) {
		    if (lastModTime != fileInfo.st_mtime) {
			lastModTime = fileInfo.st_mtime;

			printf("Compiling...\n");
			system(compileCommand);
			printf("Compiled!\n");
		    }
		}
		sleep(1);
	}
	return 0;
}

int getCompileCommand(char** configFileName,char** compileCommand){
	char line[256];
	printf("FUNCTION CALL:\t\tgetCompileCommand\n");
	printf("\tCONFIG FILE NAME:\t%s\n",*configFileName);

	//open the file given the filename
	FILE* file = fopen(*configFileName,"r");

	if(file == NULL){
		perror("Command file couldn't be opened");
		return 1;
	}

	//read lines from the file
	char* buffer = (char*) malloc(256);
	buffer = fgets(buffer,256,file);
	printf("\tCONFIG FILE CONTENT:\t%s",buffer);

	//fill buffer with everything from after "compile_command=" to "\n"
	sscanf(buffer,"compile_command = %255[^\n]",*compileCommand);
	printf("\tCOMMAND:\t%s\n",*compileCommand);
	return 0;
}

