.text
.set noat

.ent _start
.global _start
_start:

j1:  	addiu   $t2, $0, 5
	j 	j1d
	addiu   $t2, $0, 4
j1d:    add	$t2, $t2, $0		# $t2 = 5

.end _start	

