# SCJSH - The Steven, Chris, & John Shell

Division of Labor:

Chris Osterer: 
	Part 1: set up the parser given in the notes
	Part 5: Execution
	Part 6: I/O Redirection (implemented execution side)
	Part 8: Background processing (implemented execution side)

	Built-ins:
			exit - easily implemented by chekcing for exit command and recording time at start and end of program with timeofday()
			io command - goes into the /proc directory and reads the file for that process id, putput formatted to table form

	
Jonathan Molina:
	Part 4: Path Resolution, Special case handling 
	Part 8: Background processing (Debugging)
	Testing
	
	Built-ins:
			cd - Special Case Handling (~)

Steven Perez:
	Part 2: Environemental Variables
	Part 3: Prompt
	Integration of parts 1 & 5
	Part 6: I/O Redirection (implemented parser side)
	Part 7: Pipes
	Part 8: Background processing (implemented parser side)
	makefile

	Built-ins:
			cd - Basic functionality & environment variable updating
			echo - Implemented in execute function using simple loop

How to Compile executable using makefile:
       	       					- type "make"
						- can also run clean to get rid of files you do not want to keep

Known Bugs and unfinished portions of the project:
io built in returns all zeroes as directed by instructor (this was determined to be acceptable, only wanted to access the file)
Command is read into an array of fixed length 256, as such a command with more than 255 characters may cause issues
When io is run in the background, the io command will print its results before the inner command executes
Background queue maxes out at 100 bg processes at once (includes completed processes, resets if all processes complete) {Limit could easily be increased, with more memory consumption}
Does not run scripts, can only run builtins and binary executables
Cannot ctrl-Z to suspend a process, will suspend entire shell

Content of Tar Archive and description of necessary files:
bgparser.c - Driver file, handles all aspects of tokenizing user input, determining I/O redirection, setting up pipes, and checking for background processing
	   Also has cd and exit built in commands
env.c - Replaces string sections of format $NAME with VALUE from environemnt variable (used by parser and path resolution).  Also generates prompt
finalexec.c - Given command, arguments, and flags, runs given command with I/O redirection, in either forground or background.  Also has io and echo built in commands
getcmdpath.c - Path resolution
specialcases.c - Utility functions used for parsing ~ and other special cases in path resolution

Extra Credit Note: Piping is implemented to allow unlimited pipes through recursion.  Please grade for extra credit!

Tested to run on Linprog (fsu servers) and Ubuntu 16.04 LTS
