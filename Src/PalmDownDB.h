/*PalmDownDB.h

Small (hopefully) file that should only deal with interpreting of
the internal DB, does not handle reading/writing of records directly

2021- Phillip Mastrosimone
*/

#include <PalmOS.h>
#include <DateTime.h>
#include "PalmDown.h"

struct fileInfo {
	Char name;
	Char path;
	UInt16 dbIndex;
}