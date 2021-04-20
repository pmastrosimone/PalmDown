/*PalmDownMem.h

File for working with memory structures and .pdb files on internal memory.
Also handles filestreaming on the internal storage.

2021- Phillip Mastrosimone
*/

/*
Includes: 
PalmOS.h- some sort of dark magic
MemoryMgr.h- working with memory allocation and structures
ErrorMgr.h + ErrorBase.h- error and exception handling
DataMgr.h- working with data and databases
FileStream.h- what salmon do to lay eggs; or for filestreaming I can't remember which
*/
#include <PalmOS.h>
#include <MemoryMgr.h>
#include <ErrorMgr.h>
#include <ErrorBase.h>
#include <DataMgr.h>
#include <FileStream.h>
#include "PalmDown.h"

/*GLOBALS

Should see if these can become locals*/
const Char *dbName;
const Char *testData;
UInt16 pdbIndex; 


DmOpenRef reference;
LocalID dbLoc;
const UInt16 internalCard = 0;

/*Here we check for PalmDownDB

pdbOpen, pdbCheck, and pdbCreate are now under one function pdbOpen; which checks for the db, if exists opens and
returns db ref, if cannot be found, creates, opens, and returns ref, if cannot find db for another reason displ. error 

db opened in R/W mode
*/

static DmOpenRef pdbOpen(){
	Err dbLocErr;
	Err createErr; 
	UInt16 recordIter;
	dbName = "PalmDownDB";
	dbLoc = DmFindDatabase(internalCard, dbName);
	
	if (dbLoc == 0){
 		dbLocErr = DmGetLastErr();
 		//0x0207 Inidicates file not found, should attempt to handle other errors differently
 		if (dbLocErr == 0x0207){
 			createErr = DmCreateDatabase(internalCard, dbName, appFileCreator, dbType, false);
 			//Should do something with createErr 
			//200IQ solution: you can't have an error if you don't handle the error
			dbLoc = DmFindDatabase(internalCard, dbName);
 			reference = DmOpenDatabase(internalCard, dbLoc, dmModeReadWrite);
 			//Here record numbers 0-8 (16Byte) are made for application preferences, while unused for now
 			//pre-allocating means the program shouldn't have to search for data in the sea of files
 			for(recordIter = 0; recordIter != 8; recordIter++){
 				DmNewRecord(reference, &recordIter, 16);
 			}
 			return reference;	
 		} else {
 			//Alert for errors other than file NF; return no ref.
 			ErrAlert (dbLocErr);
 			return 0; 
 		}
 	} else {
 		reference = DmOpenDatabase(internalCard, dbLoc, dmModeReadWrite);
 		return reference;	
	 }
}

//Creates a new record within the internal database of 512 bytes (arbitrary) and returns its handle
static MemHandle pdbNewRec(){
	MemHandle handle;
	const UInt32 size512B = 512;

	
	pdbIndex = dmMaxRecordIndex; 
	handle = DmNewRecord(reference, &pdbIndex, size512B);
	return handle;
}

//Writes record, return t/f based on success, currently writing test data
//This function needs a prototype, should also be passed data from form or vfs rather than using internal testData
Boolean pdbWriteRec(MemHandle recHandle){
 Err writeError;
 MemPtr lockedHandlePtr;
 //Should move testData to another function, maybe as a flag for debugging or its own header or something
 testData = "1234 Is this thing on?";
 
 
 lockedHandlePtr = MemHandleLock(recHandle);
 
 //In theory this should only write the first 8 bytes of the test data "1234 Is "
 writeError = DmWrite(lockedHandlePtr, 0, testData, 8);
 if (writeError != errNone){
 	DmReleaseRecord(reference, pdbIndex, true);
 	MemHandleUnlock(recHandle);
 	ErrAlert(writeError);
 	return false;	
 	} else{
 		DmReleaseRecord(reference, pdbIndex, true);
 		MemHandleUnlock(recHandle);	
		return true;
 	}
}

//Freeing memory, should only be called in appStop but may be useful in other cases
Err memStop(){
	
}