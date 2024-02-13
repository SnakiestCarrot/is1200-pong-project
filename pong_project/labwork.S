  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

# declaring as global in order to be called from rest of directory
.globl delay
.globl time2string
.globl hexasc
.globl enable_interrupt

  #----------------------HEXASC---------------#
hexasc:
	andi $a0, $a0, 0xf
	slti $t0,$a0,10
	beq $t0,$zero,larger
	nop
	
	addi $v0, $a0, 48
	jr $ra
	nop
	
larger:
	addi $v0, $a0, 55
	jr $ra
	nop
  #----------------------time2str---------------#
time2string:
	PUSH $ra #POP RA since everytime you call JAL its changed. /August
	
	# First number
	PUSH $a0
	andi $a0, $a1, 0xf000
	srl $a0, $a0, 12
	jal hexasc
	nop
	POP $a0
	sb $v0, 0($a0)
	
	# Second number
	PUSH $a0
	andi $a0, $a1, 0x0f00
	srl $a0, $a0, 8
	jal hexasc
	nop
	POP $a0
	sb $v0, 1($a0)
	
	# colon (':') char
	li $v0, 0x3a
	sb $v0, 2($a0)
	
	# Third number
	PUSH $a0
	andi $a0, $a1, 0x00f0
	srl $a0, $a0, 4
	jal hexasc
	nop
	POP $a0
	sb $v0, 3($a0)
	
	# Fourth number
	PUSH $a0
	andi $a0, $a1, 0x000f
	jal hexasc
	nop
	POP $a0
	sb $v0, 4($a0)
	
	# Null character
	li $v0, 0x00
	sb $v0, 5($a0)
	
	POP $ra #Pop RA to get out of time2string /August.
	jr $ra #added jr ra 
	nop
#----------------------------------------------#

delay:			
	# takes $a0 as input for ms of delay
	li	$t1, 8950			# loads arbitrary delay for 1 ms

loopOuter:
	slti	$t2, $a0, 0x01		# UPDATE: Compares if $a0 is less than one ($a0 > 0)
	addi 	$a0, $a0, -1		# decrement outer loop counter after comparison.
	li	$t0, 0			# initializes counter for inner loop
	beq	$t2, $zero, loopInner	# If it is larger goes to loop inner.
	nop
	jr	$ra
	nop

loopInner:
	addi	$t0, $t0, 1		# increment inner loop counter $t0 +1
	bne 	$t0, $t1, loopInner	# UPDATE: Goes back to inner loop if condition is NOT met yet.
	nop
	j	loopOuter		# If it was met, goes back to outerLoop.
	nop
	# end of delay function

enable_interrupt:
	ei
	jr $ra
	nop





