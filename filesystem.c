#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "support.h"
#include "structs.h"
#include "filesystem.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>

struct fileSystem *fsys;
int nextPage;
unsigned int nextFreePage(){
  for(unsigned int i = nextPage; i < 7983; i++){
	if(isFree(i) == 0){
  	nextPage = i;
  	return i;
	}
  }
  nextPage = 0;
  return 8000;
}

void mkdir(char* name, int currentDirectory){
unsigned int pageNum = nextFreePage();
fsys->allocPages[pageNum] = 1;
	fsys->pages[pageNum].nextPage = pageNum;
	newDirEntry((char *) name, pageNum, currentDirectory);
	newDirEntry(".", pageNum, pageNum);
	newDirEntry("..", currentDirectory, pageNum);
}
/*
 * generateData() - Converts source from hex digits to
 * binary data. Returns allocated pointer to data
 * of size amt/2.
 */
char* generateData(char *source, size_t size)
{
	char *retval = (char *)malloc((size >> 1) * sizeof(char));

	for(size_t i=0; i<(size-1); i+=2)
	{
		sscanf(&source[i], "%2hhx", &retval[i>>1]);
	}
	return retval;
}

struct fileSystem* createFileSystem(char* file) {
int fd;
  fd = creat(file, S_IRWXU);
  close(fd);
  fd = open(file, O_RDWR);
  struct fileSystem *retSystem = (struct fileSystem*) mmap(NULL, sizeof(struct fileSystem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  ftruncate(fd, sizeof(struct fileSystem));
  struct rootSector root;
  root.rootSectLoc = 7999;
  root.rootDirLoc = 0;
  root.allocStartLoc = 7983;
  root.numAllocPages = 16;
  root.bytesPerSector = 512;
  retSystem->root = root;
  retSystem->allocPages[0] = 1;
  for(int i = 1; i < 7983; i++){
	retSystem->allocPages[i] = 0;
  }
  close(fd);
  return retSystem;
}



/*
 * filesystem() - loads in the filesystem and accepts commands
 */
void filesystem(char *file)
{
	/* pointer to the memory-mapped filesystem */
	int currentDirectoryPage = 0;
	nextPage = 0;
	FILE *f;
      f = fopen(file, "r");
      if(f == NULL){
        fprintf(stderr, "Creating filesystem!\n");
        fsys = createFileSystem(file);
        //newDirEntry(".", 0, 0);
        //newDirEntry("..", 0, 0);
        fprintf(stderr, "Filesystem created!\n");
      }
      else {
        int fd = open(file, O_RDWR);
        fsys = (struct fileSystem*) mmap(NULL, sizeof(struct fileSystem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        fprintf(stderr, "Validating filesystem!\n");
        
      }
	
	//int currentDirectoryPage = 0;
	//nextPage = 0;
	



	/*
	 * open file, handle errors, create it if necessary.
	 * should end up with map referring to the filesystem.
	 */


	/* You will probably want other variables here for tracking purposes */


	/*
	 * Accept commands, calling accessory functions unless
	 * user enters "quit"
	 * Commands will be well-formatted.
	 */
	char *buffer = NULL;
	size_t size = 0;
	while(getline(&buffer, &size, stdin) != -1)
	{
		/* Basic checks and newline removal */
		size_t length = strlen(buffer);
		if(length == 0)
		{
			continue;
		}
		if(buffer[length-1] == '\n')
		{
			buffer[length-1] = '\0';
		}

		/* TODO: Complete this function */
		/* You do not have to use the functions as commented (and probably can not)
		 *	They are notes for you on what you ultimately need to do.
		 */

		if(!strcmp(buffer, "quit"))
		{
			break;
		}
		else if(!strncmp(buffer, "dump ", 5))
		{
			if(isdigit(buffer[5]))
			{
				//dump(stdout, atoi(buffer + 5));
			}
			else
			{
				char *filename = buffer + 5;
				char *space = strstr(buffer+5, " ");
				*space = '\0';
				//open and validate filename
				//dumpBinary(file, atoi(space + 1));
			}
		}
		else if(!strncmp(buffer, "usage", 5))
		{
			//usage();
		}
		else if(!strncmp(buffer, "pwd", 3))
		{
			//pwd();
		}
		else if(!strncmp(buffer, "cd ", 3))
		{
			//cd(buffer+3);
		}
		else if(!strncmp(buffer, "ls", 2))
		{
			//ls();
		}
		else if(!strncmp(buffer, "mkdir ", 6))
		{
			mkdir(buffer+6, currentDirectoryPage);
			//mkdir(buffer+6);
		}
		else if(!strncmp(buffer, "cat ", 4))
		{
			//cat(buffer + 4);
		}
		else if(!strncmp(buffer, "write ", 6))
		{
			char *filename = buffer + 6;
			char *space = strstr(buffer+6, " ");
			*space = '\0';
			size_t amt = atoi(space + 1);
			space = strstr(space+1, " ");

			char *data = generateData(space+1, amt<<1);
			//write(filename, amt, data);
			free(data);
		}
		else if(!strncmp(buffer, "append ", 7))
		{
			char *filename = buffer + 7;
			char *space = strstr(buffer+7, " ");
			*space = '\0';
			size_t amt = atoi(space + 1);
			space = strstr(space+1, " ");

			char *data = generateData(space+1, amt<<1);
			//append(filename, amt, data);
			free(data);
		}
		else if(!strncmp(buffer, "getpages ", 9))
		{
			//getpages(buffer + 9);
		}
		else if(!strncmp(buffer, "get ", 4))
		{
			char *filename = buffer + 4;
			char *space = strstr(buffer+4, " ");
			*space = '\0';
			size_t start = atoi(space + 1);
			space = strstr(space+1, " ");
			size_t end = atoi(space + 1);
			//get(filename, start, end);
		}
		else if(!strncmp(buffer, "rmdir ", 6))
		{
			//rmdir(buffer + 6);
		}
		else if(!strncmp(buffer, "rm -rf ", 7))
		{
			//rmForce(buffer + 7);
		}
		else if(!strncmp(buffer, "rm ", 3))
		{
			//rm(buffer + 3);
		}
		else if(!strncmp(buffer, "scandisk", 8))
		{
			//scandisk();
		}
		else if(!strncmp(buffer, "undelete ", 9))
		{
			//undelete(buffer + 9);
		}



		free(buffer);
		buffer = NULL;
	}
	free(buffer);
	buffer = NULL;

}

/*
 * help() - Print a help message.
 */
void help(char *progname)
{
	printf("Usage: %s [FILE]...\n", progname);
	printf("Loads FILE as a filesystem. Creates FILE if it does not exist\n");
	exit(0);
}

/*
 * main() - The main routine parses arguments and dispatches to the
 * task-specific code.
 */
int main(int argc, char **argv)
{
	/* for getopt */
	long opt;

	/* run a student name check */
	check_student(argv[0]);

	/* parse the command-line options. For this program, we only support */
	/* the parameterless 'h' option, for getting help on program usage. */
	while((opt = getopt(argc, argv, "h")) != -1)
	{
		switch(opt)
		{
		case 'h':
			help(argv[0]);
			break;
		}
	}

	if(argv[1] == NULL)
	{
		fprintf(stderr, "No filename provided, try -h for help.\n");
		return 1;
	}

	filesystem(argv[1]);
	return 0;
}
