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

static Err pdbCreate(){
	Err createErr;
	
	createErr = DmCreateDatabase(internalCard, dbName, appFileCreator, dbType, false);
	return createErr;
}