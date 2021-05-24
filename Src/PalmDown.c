/*PalmDown.c
	Main file for PalmDown. Definitely what I would call spaghetti code. See the license
	file that I probably haven't written yet; in that case just ask before using 
	or modifying.
	
	2021- Phillip Mastrosimone	
 */
 
#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <Form.h>
#include <VfsMgr.h>
#include <ErrorBase.h>
#include <MemoryMgr.h>
#include <Table.h>
#include <DataMgr.h>
#include <StringMgr.h>
#include <TextMgr.h>
#include "PalmDown.h"
#include "PalmDown_ConRsc.h"
#include "PalmDownMem.h"
#include "PalmDownVFS.h"

/*********************************************************************
 * Entry Points
 *********************************************************************/

/*********************************************************************
 * Global variables
 *********************************************************************/
//file List ref (entry 10 to 10+n in PDDB)
DmOpenRef dbRef;
//n is number of .md files found 
UInt16 n;
UInt16 fRecStop;
const Char *testData;

//

/*********************************************************************
 * Internal Constants
 *********************************************************************/

/* Define the minimum OS version we support */
#define ourMinVersion    sysMakeROMVersion(3,5,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

/*********************************************************************
 * Internal Functions
 *********************************************************************/

/*		 
 * FUNCTION: GetObjectPtr
 *
 * DESCRIPTION:									 
 *
 * This routine returns a pointer to an object in the current form.
 *
 * PARAMETERS:
 *
 * formId
 *     id of the form to display
 *
 * RETURNED:
 *     address of object as a void pointer
 */

static void *GetObjectPtr(UInt16 objectID)
{
	FormType *frmP;

	frmP = FrmGetActiveForm();
	return FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID));
	
}

/*
 * FUNCTION: MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:
 *
 * frm
 *     pointer to the MainForm form.
 */

static void MainFormInit(FormType *frmP)
{
	
	FieldType *field;

	TableType *fileTablePtr;

	fileTablePtr = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainFileTable));

	populateTable(frmP, fileTablePtr, n);
    //FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFileTable));

}

/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id			  
 */

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	switch (command)
	{

	}

	return handled;
}

/*
 * FUNCTION: MainFormHandleEvent
 *
 * DESCRIPTION:
 *
 * This routine is the event handler for the "MainForm" of this 
 * application.
 *
 * PARAMETERS:
 *
 * eventP
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed to
 *     FrmHandleEvent
 */

static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	FormType *frmP;
    TableType *fileTablePtr;
	Boolean visible;
	switch (eventP->eType) 
	{
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			
			MainFormInit(frmP);
			
			FrmDrawForm(frmP);
			//fileTablePtr = GetObjectPtr(MainFileTable);
			//TblMarkTableInvalid(fileTablePtr);
			//TblRedrawTable(fileTablePtr);
			FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFileTable));
			
			handled = true;
			break;
            
        case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			 
			 FrmDrawForm(frmP);
			 MainFormInit(frmP);	
			
			 handled = true;	
			break;
			
		case ctlSelectEvent:
		{
			if (eventP->data.ctlSelect.controlID == MainTestNewRecButton){
			 	MemHandle recHandle =  pdbNewRec(512);
			 	//Currently calls pdbWriteRec as a test, and passes testData
			 	Boolean writeSuccess = pdbWriteRec(recHandle, testData);
			 	break;
			}
			if (eventP->data.ctlSelect.controlID == MainEditTestButton){
				FrmGotoForm(EditorForm);
			}
			break;
		}
	}
    
	return handled;
}

static void EditorFormInit(FormType *frmP){
	
}

static Boolean EditorFormDoCommand(UInt16 command){
	Boolean handled = false;

	switch (command)
	{

	}

	return handled;
}

static Boolean EditorFormHandleEvent(EventType * eventP){
	Boolean handled = false; 
	FormType *frmP;
	switch (eventP->eType){
		case menuEvent:
			return EditorFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm(frmP);
			EditorFormInit(frmP);
			handled = true;
			break;
            
        case frmUpdateEvent:
			/* 
			 * To do any custom drawing here, first call
			 * FrmDrawForm(), then do your drawing, and
			 * then set handled to true. 
			 */
			break;
			
		case ctlSelectEvent:
		{
			/*if (eventP->data.ctlSelect.controlID == MainClearTextButton)
			{
				
				FieldType * field = (FieldType*)GetObjectPtr(MainDescriptionField);
				if (field)
				{
					FldDelete(field, 0, 0xFFFF);					
					FldDrawField(field);
				}
				break;
			}
			if (eventP->data.ctlSelect.controlID == MainTestNewRec){
			 	MemHandle recHandle =  pdbNewRec();
			 	Boolean writeSuccess = pdbWriteRec(recHandle);
			 	break;*/
			//} 
			break;
		}
	}
	return handled;
}

/*
 * FUNCTION: AppHandleEvent
 *
 * DESCRIPTION: 
 *
 * This routine loads form resources and set the event handler for
 * the form loaded.
 *
 * PARAMETERS:
 *
 * event
 *     a pointer to an EventType structure
 *
 * RETURNED:
 *     true if the event was handled and should not be passed
 *     to a higher level handler.
 */

static Boolean AppHandleEvent(EventType * eventP)
{
	UInt16 formId;
	FormType *frmP;

	if (eventP->eType == frmLoadEvent)
	{
		/* Load the form resource. */
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		/* 
		 * Set the event handler for the form.  The handler of the
		 * currently active form is called by FrmHandleEvent each
		 * time is receives an event. 
		 */
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;
			case EditorForm:
				FrmSetEventHandler(frmP, EditorFormHandleEvent);
				break;
		}
		return true;
	}

	return false;
}

/*
 * FUNCTION: AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.
 */

static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;
	do 
	{
		/* change timeout if you need periodic nilEvents */
		EvtGetEvent(&event, evtWaitForever);
		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				if (! AppHandleEvent(&event))
				{
					FrmDispatchEvent(&event);
				}				
			}
		}
	} while (event.eType != appStopEvent);
}

/*
 * FUNCTION: AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * RETURNED:
 *     errNone - if nothing went wrong
 */

static Err AppStart(void)
{

	UInt32 bytes;
    dbRef = pdbOpen();
	n = openVolume(dbRef);
	bytes = n * 256;
	fRecStop = n + 10;
	return errNone;
}

/*
 * FUNCTION: AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 */

static void AppStop(void)
{
        
	/* Close all the open forms. */
	FrmCloseAllForms();

}

/*
 * FUNCTION: RomVersionCompatible
 *
 * DESCRIPTION: 
 *
 * This routine checks that a ROM version is meet your minimum 
 * requirement.
 *
 * PARAMETERS:
 *
 * requiredVersion
 *     minimum rom version required
 *     (see sysFtrNumROMVersion in SystemMgr.h for format)
 *
 * launchFlags
 *     flags that indicate if the application UI is initialized
 *     These flags are one of the parameters to your app's PilotMain
 *
 * RETURNED:
 *     error code or zero if ROM version is compatible
 */

static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	/* See if we're on in minimum required version of the ROM or later. */
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
	{
		if ((launchFlags & 
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
		{
			FrmAlert (RomIncompatibleAlert);

			/* Palm OS versions before 2.0 will continuously relaunch this
			 * app unless we switch to another safe one. */
			if (romVersion < kPalmOS20Version)
			{
				AppLaunchWithCommand(
					sysFileCDefaultApp, 
					sysAppLaunchCmdNormalLaunch, NULL);
			}
		}

		return sysErrRomIncompatible;
	}

	return errNone;
}

/*
 * FUNCTION: PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * 
 * PARAMETERS:
 *
 * cmd
 *     word value specifying the launch code. 
 *
 * cmdPB
 *     pointer to a structure that is associated with the launch code
 *
 * launchFlags
 *     word value providing extra information about the launch.
 *
 * RETURNED:
 *     Result of launch, errNone if all went OK
 */

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;

	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
	{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				return error;

			/* 
			 * start application by opening the main form
			 * and then entering the main event loop 
			 */
			FrmGotoForm(MainForm);
			AppEventLoop();

			AppStop();
			break;
	}
    
	return errNone;
}

