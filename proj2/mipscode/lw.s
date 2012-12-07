.text
.set noat

.ent _start
.global _start
_start:

	addiu	$sp,$sp,-12
	
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($sp)		# 0x9876ABCD
	lui     $t1, 0x1234
	ori     $t1, $t1, 0x0405
	sw      $t1, 4($sp)		# 0x12340405
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0xEF01
	sw      $t1, 8($sp)		# 0xABCDEF01

lw1:	lw	$t0, 0($sp)		# $t2 = 0x9876ABCD

lw2:	lw	$t1, 8($sp)		# $t2 = 0xABCDEF01

.end _start
