# SCJSH - The Steven, Chris, & John Shell
COP4610 project 1

Division of Labor:

Chris Osterer: 
	Part 1: set up the parser given in the notes
	Part 5: Execution
	Part 6: I/O Redirection (implemented execution side)

	Built-ins:
			exit - easily implemented by chekcing for exit command and recording time at start and end of program with timeofday()
			io command - goes into the /proc directory and reads the file for that process id, putput formatted to table form

	
Jonathan Molina:
	Part 4: Path Resolution, Special case handling 
	Part 8: Background processing
	Testing
	
	Built-ins:
			cd - Special Case Handling (~)

Steven Perez:
	Part 2: Environemental Variables
	Part 3: Prompt
	Integration of parts 1 & 5
	Part 6: I/O Redirection (implemented parser side)
	Part 7: Pipes
	makefile

	Built-ins:
			cd - Basic functionality & environment variable updating
			echo - Implemented in execute function using simple loop

How to Compile executable using makefile:
       	       					- type "make"
						- can also run clean to get rid of files you do not want to keep

Known Bugs and unfinished portions of the project:
io built in returns all zeroes
Command is read into an array of fixed length 256, as such a command with more than 255 characters may cause issues

Content of Tar Archive and description of files:
bgparser.c - Driver file, handles all aspects of tokenizing user input, determining I/O redirection, setting up pipes, and checking for background processing
	   Also has cd and exit built in commands
env.c - Replaces string sections of format $NAME with VALUE from environemnt variable (used by parser and path resolution).  Also generates prompt
finalexec.c - Given command, arguments, and flags, runs given command with I/O redirection, in either forground or background.  Also has io and echo built in commands
getcmndpath.c - Path resolution
specialcases.c - Uitility functions used for parsing ~ and other special cases in path resolution

Special Considerations:
