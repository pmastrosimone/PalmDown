#File handler 
{
	Get file list; err
	Determine .md files
	Ensure .md files are really Markdown 
	Display file list to user; err
	
	Open file location; point to file for editor (or preview)
	OR
	Delete file
	OR
	Create file; point to file for editor (or preview)

}

#File editor
{
	Open file (rw); err
	Accept edits; syntax highlighting for color dev. 
	Quick buttons to var MD functions (i.e. a header button that inserts a "#" character)
	Write changes live; err
	
	End program
	OR
	Return to file handler/list
	OR
	Point to file for preview
	
}

#File preview
{
	Open .md file (r); err
	Convert .md syntax into html
	Create html page using md; err
	Display html page using style sheet + internal browser/renderer; err
	
	Point to file for editor
	OR
	Return to file handler/list
}