.text
.set noat

.ent _start
.global _start
_start:

or1:	addiu   $t0, $0, 0x1BCD
	addiu   $t1, $0, 0x2FAB
	or	$t2, $t0, $t1		# $t2 = 0x3FEF
	
or2:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0x1234
	or	$t2, $t0, $t1		# $t2 = 0xFFFFBBFD

.end _start

