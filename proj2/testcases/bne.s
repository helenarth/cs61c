# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

bne1:	addiu   $t0, $0, 1
	addiu   $t1, $0, 1
	addiu	$t2, $0, 1
	bne	$t0, $t1, bne1d
	addiu	$t2, $t2, 1
bne1d:	addiu	$t2, $t2, 1		# $t2 = 3
	
bne2:	addiu   $t0, $0, 0
	addiu   $t1, $0, 1
	addiu	$t2, $0, 1
	bne	$t0, $t1, bne2d
	addiu	$t2, $t2, 1
bne2d:	addiu	$t2, $t2, 1		# $t2 = 2

# End the program	
ori   $v0, $zero, 10
syscall
.end _start

