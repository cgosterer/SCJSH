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

How to Compile executable using makefile:
						- type "make"
						- can also run clean to get rid of files you do not want to keep

Known Bugs and unfinished portions of the project:

Content of Tar Archive and description of files:

Special Considerations:
