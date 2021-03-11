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
	
	volEnumErr = VFSVolumeEnumerate(&volRefPtr, &volIterPtr);
	
	volOpenErr = VFSFileOpen(volRefPtr, "/", 0x0002U, &dirRef);
	
	
	dirEnumErr = VFSDirEntryEnumerate(dirRef, &dirIterPtr, &fileInfo);
	
	MemHandleUnlock(fileBuffer);
	MemHandleFree(fileBuffer);
	
	return 0;
}

