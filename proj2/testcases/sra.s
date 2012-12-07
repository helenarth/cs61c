# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

sra1:	addiu   $t0, $0, 0x1BCD
	sra	$t2, $t0, 2		# $t2 = 0x6F3
	
sra2:	lui     $t0, 0x5643
	ori     $t0, $t0, 0xCDAB
	sra	$t2, $t0, 0		# $t2 = 0x5643CDAB
	sra	$t2, $t0, 3		# $t2 = 0xAC879B5
	sra	$t2, $t0, 13		# $t2 = 0x2B21E
	
sra3:	lui     $t0, 0xF643
	ori     $t0, $t0, 0xCDAB
	sra	$t2, $t0, 0		# $t2 = 0xF643CDAB
	sra	$t2, $t0, 3		# $t2 = 0xFEC879B5
	sra	$t2, $t0, 13		# $t2 = 0xFFFFB21E

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					

