# SCJSH
COP4610 project 1

Division of Labor:

Chris Osterer: 
	Part 1: set up the parser given in the notes
	Part 5: Execution
	Part 6: I/O Redirection
	Part 7: Pipes

	Built-ins:
			exit - easily implemented by chekcing for exit command and recording time at start and end of program with timeofday()
			echo - echo function decently right now
			cd - uses chdir(), had to change prompt so it displays cwd not $PWD, $PWD is the cwd for the outer shell not our program (parser2.c)
			io command - goes into the /proc directory and reads the file for that process id, putput formatted to table form

	
Jonathan Molina:
	Part 4: Path Resolution

Steven Perez:
	Part 2: Environemental Variables
	Part 3: Prompt
	Integration of parts 1 & 5
	makefile

	Built-ins:
			cd, echo

How to Compile executable using makefile:
						- type "make"
						- can also run clean to get rid of files you do not want to keep
Simply call the make command to have gcc compile the parser.c file w/ other files

Known Bugs and unfinished portions of the project:
Currently does not update prompt with changes in PWD - fixed in parser2.c 9/13
Currently does not support relative pathnames or PATH searches
Currently does not support I/O Redirection or Pipes - basic functionaily included in parser2.c still needs work 9/13
Currently lacks io built in - included in parser2.c, -> io ls -l -a will show the table for io stats 9/13
Curently lacks functionality for background processing

Content of Tar Archive and description of files:

Special Considerations:
