# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

subu1:	lui     $t0, 0x0FFF
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0x1FFF
	ori     $t1, $t1, 0xABCD
	subu    $t2, $t0, $t1		# $t2 = 0xF0000000
	
subu2:	lui     $t0, 0x7ABC
	ori     $t0, $t0, 0xDEF0
	lui     $t1, 0x1234
	ori     $t1, $t1, 0x5678
	subu    $t2, $t0, $t1		# $t2 = 0x68888878

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start	
