.text
.set noat

.global _start
.ent    _start 
_start:
	lui     $t3, 0x0040		# if proj1
	ori     $t3, $t3, 0x1000	# if proj1
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($t3)		# 0x9876ABCD
	lui     $t1, 0x1234
	ori     $t1, $t1, 0x0405
	sw      $t1, 4($t3)		# 0x12340405
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0xEF01
	sw      $t1, 8($t3)		# 0xABCDEF01

sw1:	lui     $t0, 0x1234
	ori     $t0, $t0, 0x0405
	sw	$t0, 4($t3)
	
sw2:	lui     $t0, 0xA1B2
	ori     $t0, $t0, 0xC3D4
	sw	$t0, 8($t3)

sw3:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0x0000
	sw	$t0, 6($t3)		# ERR

.end _start

