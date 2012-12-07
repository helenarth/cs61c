.text
.set noat

.ent _start
.global _start
_start:

addu1:	lui     $t0, 0x0FFF
	ori     $t0, $t0, 0xABCD
	lui     $t0, 0x1FFF
	ori     $t0, $t0, 0xABCD
	addu    $t2, $t0, $t1		# $t2 = 0x2FFF579A
	
addu2:	lui     $t0, 0x7FFF
	ori     $t0, $t0, 0xABCD
	addu    $t2, $t0, $t0		# no overflow error

	# the above instruction doesn't cause an error
	# so we need to exit
.end _start		#unsupported by simulator, terminates

