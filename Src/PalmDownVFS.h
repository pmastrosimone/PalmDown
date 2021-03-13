/* PalmDownVFS.h 

File for handling filestreaming to/from VFS devices. In theory should be able to write changes directly
to the card, or can convert to the "internal DB"  freeing the user of the SD card. Should also be able to 
scan card relatively quickly for .md files.

2021- Phillip Mastrosimone

*/

/*
Includes:



*/

#include <PalmOS.h>
#include <VfsMgr.h>
#include <StringMgr.h>
#include "PalmDown.h"


//Should this return anything? If not change to void
//THE ARRAY DUMBASS RETURN THE ARRAY (or maybe not(?))
UInt16 openVolume(){
	Err volEnumErr;
	Err volOpenErr;
	Err dirEnumErr;
	UInt16 volRefPtr;
	UInt32 volIterPtr;
	UInt32 dirIterPtr;
	FileRef dirRef;
	
	MemHandle fileBuffer;
	MemPtr fileBufP;
	FileInfoType fileInfo;
	UInt16 n;
	const Char *mdext;
	/*apparently you can't make it check for the null terminator (for checking eostr ext)
	  we'll take care of this further down*/
	mdext = ".md";
	 
	
	fileBuffer = MemHandleNew(256);
	fileBufP = MemHandleLock(fileBuffer);
	
	
	fileInfo.attributes;
	fileInfo.nameP = fileBufP;
	fileInfo.nameBufLen = 256;
	
	volIterPtr = vfsIteratorStart;
	dirIterPtr = vfsIteratorStart;
	n = 0;
	
	//turn this into while loop for mult. volumes (?)
	volEnumErr = VFSVolumeEnumerate(&volRefPtr, &volIterPtr);
	//if mult. volumes are implemented, find some way to get user input before passing volref
	volOpenErr = VFSFileOpen(volRefPtr, "/", 0x0002U, &dirRef);
	
	if (volOpenErr == errNone){
		while(dirIterPtr != vfsIteratorStop){
			dirEnumErr = VFSDirEntryEnumerate(dirRef, &dirIterPtr, &fileInfo);
			if (dirEnumErr == errNone){
				//Here we should check for .md files, true- add file to list or struct   false- don't
				UInt16 size;
				const UInt16 extSize = 3;
				UInt16 compareStart;
				const Char *fileNameChk;
				size = StrLen(fileInfo.nameP);
				//Getting around StrStr not checking for null term by subtracting and comparing from the
				//pos in the array
				compareStart = size - extSize;
				fileNameChk = StrStr(&fileInfo.nameP[compareStart], mdext);
				if (fileNameChk == mdext){
					/*Throw the file into something here
						need to find out how much mem to init the array with*/
				}
				fileNameChk = NULL;
				n++;
			//Err handling for dirEnumErr
			} else{
			}
		}
	//Err handling for volOpenErr
	} else
	
	//Get that shit out of here (as in the memory, not as in removing the free func)
	MemHandleUnlock(fileBuffer);
	MemHandleFree(fileBuffer);
	
	return 0;
}

