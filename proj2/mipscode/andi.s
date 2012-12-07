.text
.set noat

.ent _start
.global _start
_start:

andi1:	lui     $t0, 0x1234
	ori     $t0, $t0, 0x5678
	andi	$t2, $t0, 0x0FAB	# $t2 = 0x0628

andi2:	lui     $t0, 0x1234
	ori     $t0, $t0, 0x5678
	andi	$t2, $t0, 0xFFAB	# $t2 = 0x5628, no sign extends

.end _start	

