.text
.set noat

.ent _start
.global _start
_start:

ori1:	addiu   $t0, $0, 0x1BCD
	ori	$t2, $t0, 0xEFAB	# $t2 = 0xEFEF
	
ori2:	lui     $t0, 0xCDEF
	ori     $t0, $t0, 0xABCD
	ori	$t2, $t0, 0x1234	# $t2 = 0xCDEFBBFD

.end _start	
