/* PalmDownVFS.h 

File for handling filestreaming to/from VFS devices. In theory should be able to write changes directly
to the card, or can convert to the "internal DB"  freeing the user of the SD card. Should also be able to 
scan card relatively quickly for .md files.


TODO: Make openVolume recurse directories for compat. files
2021- Phillip Mastrosimone

*/

/*
Includes:



*/

#include <PalmOS.h>
#include <VfsMgr.h>
#include <StringMgr.h>
#include <MemoryMgr.h>
#include "PalmDown.h"

UInt16 openVolume(DmOpenRef dbRef){
	UInt16 fRec;
	UInt16 offset = 0;
	Err volEnumErr;
	Err volOpenErr;
	Err dirEnumErr;
	Err fileLErr;
	UInt16 volRefPtr;
	UInt32 volIterPtr;
	UInt32 dirIterPtr;
	FileRef dirRef;
	MemHandle fileBuffer;
	MemHandle recordBuffer;
	MemPtr fileBufP;
	MemPtr recordPtr;
	FileInfoType fileInfo;
	UInt32 attributes;
	UInt16 n;
	const Char *mdext;
	UInt16 size;
	const UInt16 extSize = 3;
	UInt16 compareStart;
	const Char *fileNameChk;
	/*apparently you can't make it check for the null terminator (for checking eostr ext)
	  we'll take care of this further down*/
	mdext = ".md";			 
	fRec = 9; 
	
	fileBuffer = MemHandleNew(256);
	fileBufP = MemHandleLock(fileBuffer);
	
	
	fileInfo.attributes = attributes;
	fileInfo.nameP = fileBufP;
	fileInfo.nameBufLen = 256;
	
	volIterPtr = vfsIteratorStart;
	dirIterPtr = vfsIteratorStart;
	n = 0;
	
	//turn this into while loop for mult. volumes (?)
	//Save for the Zodiac this may not be necessary
	volEnumErr = VFSVolumeEnumerate(&volRefPtr, &volIterPtr);
	//if mult. volumes are implemented, find some way to get user input before passing volref
	//Maybe just a different version for the Zodiac?
	volOpenErr = VFSFileOpen(volRefPtr, "/", 0x0002U, &dirRef);
	
	if (volOpenErr == errNone){
		while(dirIterPtr != vfsIteratorStop){
			dirEnumErr = VFSDirEntryEnumerate(dirRef, &dirIterPtr, &fileInfo);
			if (dirEnumErr == errNone){
				//Here we check for .md files
				//Redeclaring variables in loop every time seems unecessary (impact unknown), 
				//move these out eventually to be safe

				size = StrLen(fileInfo.nameP);
				/*Getting around StrStr not checking for null term by subtracting and comparing from the
				for example, test.md = 7 length, position 5 to pos 7 = .md then compared
				to mdExt (which is also .md), this way files named .md.exe (or something) won't be picked
				up by the program				
				*/
				compareStart = size - extSize;
				fileNameChk = StrStr(&fileInfo.nameP[compareStart], mdext);
				//would using && == mdext be good or unecessary?
				if (fileNameChk != NULL){
					//iterate n for number of entries
					recordBuffer = DmNewRecord(dbRef, &fRec, 256);
					recordPtr = MemHandleLock(recordBuffer);
					
					DmStrCopy(recordPtr, offset, fileBufP);
					
					//DmWrite(recordPtr, offset, &fileInfo.nameP, size);
					DmReleaseRecord(dbRef, fRec, true);
					fRec++;
					n++;
				}
				//ReNullify the fileNameChk before stepping through again
				fileNameChk = NULL;
			//Err handling for dirEnumErr
			} else{
			}
		}
	//Err handling for volOpenErr
	} else{
	
	}
	
	//Freeing the fileBuffer
	MemHandleUnlock(fileBuffer);
	MemHandleFree(fileBuffer);
	return n;
}
