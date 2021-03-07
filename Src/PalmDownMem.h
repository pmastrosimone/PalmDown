/*PalmDownMem.h

File for working with memory structures and .pdb files on internal memory.
Also handles filestreaming on the internal storage.

2021- Phillip Mastrosimone
*/

/*
Includes: 
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

//Here we check for PalmDownDB, returns false if not found, returns true if found
//or if the file could not be found due to another error besides file not existing

static Boolean pdbCheck(){
	Err dbLocErr;
	dbName = "PalmDownDB";
	dbLoc = DmFindDatabase(internalCard, dbName);
	if (dbLoc == 0){
 		dbLocErr = DmGetLastErr();
 		//0x0207 Inidicates file not found, should attempt to handle other errors differently
 		if (dbLocErr == 0x0207){
 			return false;
 		} else {
 			
 			/* If another error is found, we return true
 			so that way a new DB is not being created when one
 			may or may not already exist
 			
 			NOTE: Maybe the pdbCheck should be changed from bool to some type of int
 			or err to handle more than 2 cases*/
 			
 			ErrAlert (dbLocErr);
 			return true; 
 		}
 	} else {
 		return true;
	 }
}

//Function for creating the pdb file on internal storage 
Err pdbCreate(){
	Err createErr;
	
	createErr = DmCreateDatabase(internalCard, dbName, appFileCreator, dbType, false);
	return createErr;
}

//Function for opening the main pdb file
static DmOpenRef pdbOpen(){

	Err openErr;
	//May return no open, but at least it doesn't crash the system if there's a null db
	if (dbLoc == 0){
		return 0;
	}
	reference = DmOpenDatabase(internalCard, dbLoc, dmModeReadWrite);
	
	//Error handling
	openErr = DmGetLastErr();
	if (openErr != errNone){
		ErrAlert (openErr);
		//Return 0 so that way we don't write to a file that "half opened" 
		return 0;
	}
	return reference;
}

static MemHandle pdbNewRec(){
	MemHandle handle;
	const UInt32 size512B = 512;

	
	pdbIndex = dmMaxRecordIndex; 
	handle = DmNewRecord(reference, &pdbIndex, size512B);
	return handle;
}

Err pdbWriteRec(MemHandle recHandle){
 Err writeError;
 MemPtr lockedHandlePtr;
 
 testData = "1234 Is this thing on?";
 
 
 lockedHandlePtr = MemHandleLock(recHandle);
 writeError = DmWrite(lockedHandlePtr, 0, testData, 8);
 if (writeError != errNone){
 	DmReleaseRecord(reference, pdbIndex, true);
 	Err unlockErr = MemHandleUnlock(recHandle);
 	ErrAlert(unlockErr);
 	return writeError;
 } else{
 	Err releaseErr = DmReleaseRecord(reference, pdbIndex, true);
 	Err unlockErr = MemHandleUnlock(recHandle); 
 	ErrAlert(unlockErr);
 	return releaseErr;
 }
} 
