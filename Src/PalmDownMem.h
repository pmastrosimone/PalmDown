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

const Char *dbName;
const Char *testData;
UInt16 pdbIndex; 


DmOpenRef reference;
LocalID dbLoc;
const UInt16 internalCard = 0;

/*Here we check for PalmDownDB, returns false if not found, returns true if found
or if the file could not be found due to another error besides file not existing

pdbOpen, pdbCheck, and pdbCreate are now under one function pdbOpen; which checks for the db, if exists opens and
returns db ref, if cannot be found, creates, opens, and returns ref, if cannot find db for another reason displ. error 

db opened in R/W mode
*/

static DmOpenRef pdbOpen(){
	Err dbLocErr;
	Err createErr; 
	
	dbName = "PalmDownDB";
	dbLoc = DmFindDatabase(internalCard, dbName);
	
	if (dbLoc == 0){
 		dbLocErr = DmGetLastErr();
 		//0x0207 Inidicates file not found, should attempt to handle other errors differently
 		if (dbLocErr == 0x0207){
 			createErr = DmCreateDatabase(internalCard, dbName, appFileCreator, dbType, false);
 			dbLoc = DmFindDatabase(internalCard, dbName);
 			reference = DmOpenDatabase(internalCard, dbLoc, dmModeReadWrite);
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

//Creates a new record within the internal database of 512 bytes and returns its handle
static MemHandle pdbNewRec(){
	MemHandle handle;
	const UInt32 size512B = 512;

	
	pdbIndex = dmMaxRecordIndex; 
	handle = DmNewRecord(reference, &pdbIndex, size512B);
	return handle;
}

//Writes record, return t/f based on success, currently writing test data
Boolean pdbWriteRec(MemHandle recHandle){
 Err writeError;
 MemPtr lockedHandlePtr;
 testData = "1234 Is this thing on?";
 
 
 lockedHandlePtr = MemHandleLock(recHandle);
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
