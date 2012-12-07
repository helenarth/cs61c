# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

addiu1:	addiu   $t0, $0, 32767
	addiu	$t2, $t0, 1	# $t2 = 0x00008000

addiu2:	addiu   $t0, $0, -1
	addiu	$t2, $t0, -1	# $t2 = 0xFFFFFFFE

addiu3:	addiu   $t0, $0, 32767
	addiu	$t2, $t0, 32767	# $t2 = 0x0000FFFE

addiu4:	addiu   $t0, $0, -32768
	addiu	$t2, $t0, -32768	# $t2 = 0xFFFF0000


# End the program	
ori   $v0, $zero, 10
syscall
.end _start
