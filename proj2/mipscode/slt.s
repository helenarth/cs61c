.text
.set noat

.ent _start
.global _start
_start:

slt1:	addiu   $t0, $0, 0x1BCD
	addiu   $t1, $0, 0x2DEF
	slt     $t2, $t0, $t1		# $t2 = 1
	
slt2:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD	# -21555
	lui     $t1, 0x0BCD		
	ori     $t1, $t1, 0x1234	# 197988916
	slt	$t2, $t1, $t0		# $t2 = 0
	
slt3:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD	# -21555
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0x1234	# -1412623820
	slt	$t2, $t1, $t0		# $t2 = 1
	
slt4:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0x1234
	slt	$t2, $t0, $t1		# $t2 = 0

.end _start
