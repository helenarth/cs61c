# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

jal1:  	addiu   $t2, $0, 5
	jal 	jal1d
	addiu   $t2, $0, 4
jal1d:  addiu	$t2, $t2, 0		# $t2 = 5

jal2:  	addiu   $ra, $0, 0
	jal 	jal2d
	addiu   $t2, $0, 4
jal2d:  addiu	$t2, $ra, 0		# $t2 = 00400018 (not 5)


# End the program	
ori   $v0, $zero, 10
syscall
.end _start
