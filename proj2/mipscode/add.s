.text
.set noat

.ent _start
.global _start
_start:

add1:	lui     $t0, 0x0FFF
	ori     $t0, $t0, 0xABCD
	lui     $t0, 0x1FFF
	ori     $t0, $t0, 0xABCD
	add     $t2, $t0, $t1		# $t2 = 0x2FFF579A
	
add2:	lui     $t0, 0x7FFF
	ori     $t0, $t0, 0xABCD
	add     $t2, $t0, $t0		# ERR: overflow

.end _start
	# the above instruction causes an error and exit the program
	# we don't need the following line any more
#done:	addi 	$0 $0 0			#unsupported by simulator, terminates
