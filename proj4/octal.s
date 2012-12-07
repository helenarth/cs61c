
OCTAL: 
 
         lui $r0, 0x829f        

        sll	$r0, $r0, 7

	srlv  $r0, $r0, 7  

       srlv  	$r2, $r0, 6 

	sll	$r0, $r0, 10	

	srlv  $r1, $r0, 13

	sll  $r0, $r0,  3 

       srlv  $r0,$r0, 13

	sll	$r1,$r1, 4

	sll	$r2, $r2, 8

	add	$r0, $r0, $r1

	add	$r0, $r0, $r2

	disp	 $r0, 0

	jr	$r3

