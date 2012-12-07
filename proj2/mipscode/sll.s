.text
.set noat

.ent _start
.global _start
_start:

sll1:	addiu   $t0, $0, 0x1BCD
	sll	$t2, $t0, 2		# $t2 = 0x6F34
	
sll2:	lui     $t0, 0x5643
	ori     $t0, $t0, 0xCDAB
	sll	$t2, $t0, 0		# $t2 = 0x5643CDAB
	sll	$t2, $t0, 3		# $t2 = 0xB21E6D58
	sll	$t2, $t0, 13		# $t2 = 0x79b56000

.end _start

