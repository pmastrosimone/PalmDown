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
#include "PalmDown.h"



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
	
	
	fileBuffer = MemHandleNew(256);
	fileBufP = MemHandleLock(fileBuffer);
	
	
	fileInfo.attributes;
	fileInfo.nameP = fileBufP;
	fileInfo.nameBufLen = 256;
	
	volIterPtr = vfsIteratorStart;
	dirIterPtr = vfsIteratorStart;
	
	//turn this into while loop for mult. volumes (?)
	volEnumErr = VFSVolumeEnumerate(&volRefPtr, &volIterPtr);
	//if mult. volumes are implemented, find some way to get user input before passing volref
	volOpenErr = VFSFileOpen(volRefPtr, "/", 0x0002U, &dirRef);
	
	if (volOpenErr == errNone){
		while(dirIterPtr != vfsIteratorStop){
			dirEnumErr = VFSDirEntryEnumerate(dirRef, &dirIterPtr, &fileInfo);
			if (dirEnumErr == errNone){
				//Here we should check for .md files, true- add file to list or struct   false- don't
			} else{
			}
		}
	}
	
	//Get that shit out of here
	MemHandleUnlock(fileBuffer);
	MemHandleFree(fileBuffer);
	
	return 0;
}

