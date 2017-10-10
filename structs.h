#ifndef STRUCTS_H
#define STRUCTS_H

/*
 *
 * Define page/sector structures here as well as utility structures
 * such as directory entries.
 *
 * Sectors/Pages are 512 bytes
 * The filesystem is 4 megabytes in size.
 * You will have 8K pages total.
 *
 */
struct page {
  unsigned char data[512];
  unsigned int nextPage;
};

struct rootSector {
  int rootSectLoc;
  int rootDirLoc;
  int allocStartLoc;
  int numAllocPages;
  int bytesPerSector;
};

// 25 directory entries can fit in 1 page in the first 500 bytes
struct directoryEntry {
  unsigned char fileName[11];
  unsigned char fileOrDirectory;
  unsigned int size;
  unsigned int pageNumber;
};


struct fileSystem {
  struct rootSector root;
  unsigned char allocPages[7983];
  struct page pages[7983];
};


#endif
