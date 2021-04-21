/* PalmDownVFS.h 

File for handling filestreaming to/from VFS devices. In theory should be able to write changes directly
to the card, or can convert to the "internal DB"  freeing the user of the SD card. Should also be able to 
scan card relatively quickly for .md files.

2021- Phillip Mastrosimone

*/

/*
Includes:



*/

//SHOULD THIS BE REWRITTEN AS A LOOP (i.e., as each valid file is populated, a new row is added with fname)
//rather than dicking around with 30000+ functions doing similar things and adding complexity/mem consumption

#include <PalmOS.h>
#include <VfsMgr.h>
#include <StringMgr.h>
#include <MemoryMgr.h>
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
				//Here we check for .md files
				//Redeclaring variables in loop every time seems unecessary (impact unknown), 
				//move these out eventually to be safe
				UInt16 size;
				const UInt16 extSize = 3;
				UInt16 compareStart;
				const Char *fileNameChk;
				size = StrLen(fileInfo.nameP);
				/*Getting around StrStr not checking for null term by subtracting and comparing from the
				for example, test.md = 7 length, position 5 to pos 7 = .md then compared
				to mdExt (which is also .md), this way files named .md.exe (or something) won't be picked
				up by the program				
				*/
				compareStart = size - extSize;
				fileNameChk = StrStr(&fileInfo.nameP[compareStart], mdext);
				if (fileNameChk != NULL){
					//iterate n for number of entries
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

//Copied a lot of this code from openVolume
//Returns handle to PDDB rec containing file names
//This function may be able to be combined into openVolume, resizing entry 9 + offsetting in PDDB as needed
//May even be able to include table drawing
MemPtr getFiles(UInt16 size, UInt16 bytes, DmOpenRef dbRef){
	UInt16 fRec = 9;
	UInt16 offset = 0;
	Err volEnumErr;
	Err volOpenErr;
	Err dirEnumErr;
	UInt16 volRefPtr;
	UInt32 volIterPtr;
	UInt32 dirIterPtr;
	FileRef dirRef;
	MemHandle recordBuffer;
	MemHandle fileBuffer;
	MemPtr fileBufP;
	MemPtr recordPtr;
	FileInfoType fileInfo;
	Err fileLErr;
	const Char *mdext;	
    
	/*apparently you can't make it check for the null terminator (for checking eostr ext)
	  we'll take care of this further down*/
	mdext = ".md";
	 
	fileBuffer = MemHandleNew(256);
	fileBufP = MemHandleLock(fileBuffer);
	
	//FileInfoTag data structure
	fileInfo.attributes;
	fileInfo.nameP = fileBufP;
	fileInfo.nameBufLen = 256;
	
	volIterPtr = vfsIteratorStart;
	dirIterPtr = vfsIteratorStart;
	
	//turn this into while loop for mult. volumes (?)
	volEnumErr = VFSVolumeEnumerate(&volRefPtr, &volIterPtr);
	//if mult. volumes are implemented, find some way to get user input before passing volref
	volOpenErr = VFSFileOpen(volRefPtr, "/", 0x0002U, &dirRef);
	
	//Record 9 in DB for holding fNames (temp), bytes is = to # of md files found,
	//by 256 for max fname length then lock handle
	// NEED TO CLEAR THIS RECORD AT END OF PROGRAM OR NEXT STARTUP 
	recordBuffer = DmNewRecord(dbRef, &fRec, bytes);
	recordPtr = MemHandleLock(recordBuffer);
	if (volOpenErr == errNone){
		while(dirIterPtr != vfsIteratorStop){
			dirEnumErr = VFSDirEntryEnumerate(dirRef, &dirIterPtr, &fileInfo);
			if (dirEnumErr == errNone){
				//Here we should check for .md files, true- add file to list or struct   false- don't
				//Redeclaring variables in loop every time seems unecessary (impact unknown), 
				//move these out eventually to be safe
				UInt16 sizeStr;
				const UInt16 extSize = 3;
				UInt16 compareStart;
				const Char *fileNameChk;
				sizeStr = StrLen(fileInfo.nameP);
				/*Getting around StrStr not checking for null term by subtracting and comparing from the
				for example, test.md = 7 length, position 5 to pos 7 = .md then compared
				to mdExt (which is also .md), this way files named .md.exe (or something) won't be picked
				up by the program				
				*/
				compareStart = sizeStr - extSize;
				fileNameChk = StrStr(&fileInfo.nameP[compareStart], mdext);

				if (fileNameChk != NULL){
					//Here we write the name of the file, then offset by 256 for addnl. files

					DmStrCopy(recordPtr, offset, fileInfo.nameP); 
					
					offset += 256;
				
				} 
				
				if (offset == bytes){
					break;
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
	MemHandleUnlock(fileBuffer);
	MemHandleFree(fileBuffer);
	//HOORAY IT RETURNS THE RECORD
	return recordPtr;
}
