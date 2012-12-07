# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

j1:  	addiu   $t2, $0, 5
	j 	j1d
	addiu   $t2, $0, 4
j1d:    addiu	$t2, $t2, 0 # $t2 = 5

# End the program	
ori   $v0, $zero, 10
syscall
.end _start
