.text
.set noat

.ent _start
.global _start
_start:

lui1:	lui     $t0, 0xCDEF
	srl     $t1, $t0, 16		# $t2 = 0x0000CDEF

lui2:	lui     $t1, 0x0DEF		# $t2 = 0x0DEF0000

.end _start	
