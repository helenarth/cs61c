# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


jr1: 	la   	$t0, jr1d
	addiu	$t2, $0, 1
	jr 	$t0
	addiu   $t2, $0, 1
jr1d:  	addiu	$t2, $t2, 1		# $t2 = 2

jr2:	addiu   $t2, $0, 1		# $t2 = 1
	jal	jr2d
	addiu   $t2, $t2, 1		# $t2 = 2
	la	$t0, done
	addiu   $t2, $t2, 1		# $t2 = 3
	jr	$t0
	addiu   $t2, $t2, 1		# not executed
jr2d: 	jr 	$ra		# $t2 = 3

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					
