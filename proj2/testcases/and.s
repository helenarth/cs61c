# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


and1:	lui     $t0, 0x8765
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0xEFAB
	ori     $t1, $t1, 0xCD12
	and	$t2, $t0, $t1		# $t2 = 0x87218900
	
and2:	lui     $t0, 0xF0F0
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0x1234
	and	$t2, $t0, $t1		# $t2 = 0xA0C00204

	
# End the program	
ori   $v0, $zero, 10
syscall
.end _start
