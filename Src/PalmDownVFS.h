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