# Planning document for PalmDown

Inputs:
- File List
- Open file
- Text input
- Commands

Outputs:
- Display file (MD)
- Preview file (html)
- Write file
- Error and exception handling
- Open and display MD reference

Average use session:
1. Get file list
2. Display file list
3. Create / open file
4. Display file data
5. Input data
6. Write file
7. If called, display MD reference
8. If called, display HTML preview
9. Repeat 4-6 until termination; assume user will terminate app at any point, changes should be written continuously

Typical exceptions:
- Read error
- Write error
- Corrupted file
- Missing library (?)
- Missing reference files
- Permission error
- Doctype error

"Extra" features:
- Versioning/version control
- Export to html/pdf/(doc?)
- E-mail & beam
- Encryption
- Fonts for preview/export/display
- Hotsync conduit
- Tabbed and/or split and/or hot button preview

App should support OS4 std (160x160), OS4 HiRes (320x320), OS5 std & HiRes, OS5 DIA.