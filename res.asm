.data
String0:.asciiz "come in\n"
.space 3
static:	.space 0
exp:	.space 512
stack:	.space 512
.text
la $sp,stack
la $fp,stack
jal main
li $v0 10
syscall
f1:
sw $ra,4($fp)
sw $v0,8($fp)
sw $sp,12($fp)
addi $sp,$fp,4
addi $fp,$fp,8
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
f0:
sw $ra,4($fp)
sw $v0,8($fp)
sw $sp,12($fp)
addi $sp,$fp,4
addi $fp,$fp,12
li $8,20
sgt $10,$9,$8
li $9,0
beq $10,$9,ifEnd0
li $9,0
bne $10,$9,ifStart0
ifStart0:
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
ifEnd0:
li $v0,1
move $a0 $8
syscall
li $9,1
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
la $10,exp+4
lw $10,0($10)
sw $10,12($sp)
jal f1
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
f2:
sw $ra,4($fp)
sw $v0,8($fp)
sw $sp,12($fp)
addi $sp,$fp,4
addi $fp,$fp,12
li $9,'x'
seq $10,$8,$9
li $9,0
beq $10,$9,ifEnd1
li $9,0
bne $10,$9,ifStart1
ifStart1:
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
ifEnd1:
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
main:
sw $ra,4($fp)
sw $v0,8($fp)
sw $sp,12($fp)
addi $sp,$fp,4
addi $fp,$fp,8
la $11,stack+16
li $8,4
sw $8,0($11)
la $11,stack+20
li $8,2
sw $8,0($11)
li $9,0
la $11,stack+20
lw $8,0($11)
slt $10,$8,$9
li $9,0
beq $10,$9,ifEnd2
li $9,0
bne $10,$9,ifStart2
ifStart2:
la $11,stack+20
lw $9,0($11)
li $8,-1
mul $10,$8,$9
la $11,exp+4
sw $10,0($11)
la $11,stack+20
la $8,exp+4
lw $8,0($8)
sw $8,0($11)
ifEnd2:
la $11,stack+16
lw $9,0($11)
la $11,stack+20
lw $8,0($11)
sle $10,$8,$9
li $9,0
beq $10,$9,ifEnd3
li $9,0
bne $10,$9,ifStart3
ifStart3:
la $11,stack+20
li $8,0
sw $8,0($11)
li $v0,4
la $a0,String0
syscall
ifEnd3:
la $11,stack+16
lw $8,0($11)
la $11,stack+20
lw $9,0($11)
sgt $10,$9,$8
li $9,0
beq $10,$9,ifEnd4
li $9,0
bne $10,$9,ifStart4
ifStart4:
la $11,stack+20
li $8,1
sw $8,0($11)
ifEnd4:
case0:
la $11,stack+20
lw $9,0($11)
li $8,0
bne $8,$9,case1
la $11,stack+20
li $8,0
sw $8,0($11)
j switchEnd0
case1:
la $11,stack+20
lw $9,0($11)
li $8,1
bne $8,$9,case2
la $11,stack+20
li $8,1
sw $8,0($11)
j switchEnd0
case2:
switchEnd0:
li $v0,1
la $11,stack+20
lw $8,0($11)
move $a0 $8
syscall
la $11,stack+20
lw $9,0($11)
li $8,1
sub $10,$8,$9
la $11,exp+4
sw $10,0($11)
la $11,stack+20
la $8,exp+4
lw $8,0($8)
sw $8,0($11)
case3:
la $11,stack+20
lw $9,0($11)
li $8,0
bne $8,$9,case4
la $11,stack+20
li $8,11
sw $8,0($11)
j switchEnd1
case4:
la $11,stack+20
lw $9,0($11)
li $8,1
bne $8,$9,case5
la $11,stack+20
li $8,666
sw $8,0($11)
j switchEnd1
case5:
switchEnd1:
li $v0,1
la $11,stack+20
lw $8,0($11)
move $a0 $8
syscall
li $10,12
sw $10,12($sp)
jal f0
la $11,stack+16
sw $8,0($11)
li $v0,1
la $11,stack+16
lw $8,0($11)
move $a0 $8
syscall
la $11,stack+32
lw $10,0($11)
sw $10,12($sp)
jal f2
la $11,stack+40
sw $8,0($11)
li $9,'n'
la $11,stack+40
lw $8,0($11)
seq $10,$8,$9
li $9,0
beq $10,$9,ifEnd5
li $9,0
bne $10,$9,ifStart5
ifStart5:
li $9,1
la $11,stack+16
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
ifEnd5:
case6:
la $11,stack+40
lw $9,0($11)
li $8,'n'
bne $8,$9,case7
li $9,1
la $11,stack+16
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
j switchEnd2
case7:
la $11,stack+40
lw $9,0($11)
li $8,'y'
bne $8,$9,case8
li $9,2
la $11,stack+16
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
j switchEnd2
case8:
switchEnd2:
la $11,stack+36
lw $10,0($11)
sw $10,12($sp)
jal f2
la $11,stack+40
sw $8,0($11)
case9:
la $11,stack+40
lw $9,0($11)
li $8,'n'
bne $8,$9,case10
li $9,1
la $11,stack+16
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
j switchEnd3
case10:
la $11,stack+40
lw $9,0($11)
li $8,'y'
bne $8,$9,case11
li $9,2
la $11,stack+16
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
j switchEnd3
case11:
switchEnd3:
li $11,60
li $8,12
sw $8,0($11)
li $9,2
li $11,60
lw $8,0($11)
mul $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $11,56
la $8,exp+4
lw $8,0($8)
sw $8,0($11)
li $11,60
lw $9,0($11)
li $11,56
lw $8,0($11)
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
li $9,-2
li $8,27
add $10,$8,$9
la $11,exp+4
sw $10,0($11)
li $v0,1
la $8,exp+4
lw $8,0($8)
move $a0 $8
syscall
li $v0,1
li $8,+26
move $a0 $8
syscall
li $v0,1
li $8,-27
move $a0 $8
syscall
lw $31,0($sp)
lw $v0,4($sp)
subi $fp,$sp,4 
lw $sp,8($sp)
jr $31
