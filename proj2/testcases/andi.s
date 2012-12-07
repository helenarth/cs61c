# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


andi1:	lui     $t0, 0x1234
	ori     $t0, $t0, 0x5678
	andi	$t2, $t0, 0x0FAB	# $t2 = 0x0628

andi2:	lui     $t0, 0x1234
	ori     $t0, $t0, 0x5678
	andi	$t2, $t0, 0xFFAB	# $t2 = 0x5628, no sign extends

# End the program	
ori   $v0, $zero, 10
syscall
.end _start

