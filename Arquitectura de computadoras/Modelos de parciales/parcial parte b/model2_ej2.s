.data
    cadena_1: .byte -45,0,20,23,-58,98,-100,24,49
    cadena_f:
.text
    la a0,cadena_1
    la a1,cadena_f
    jal negapos
fin:
    beq,x0,x0,fin
negapos:
    addi sp,sp,-1
    sb s0,0(sp)
    mv s0,ra
    mv t0,a0
    li t3,0
loop:
    lb t1,0(t0)
    beq t0,a1,finNegaPos
    mv t2,t0
    addi t0,t0,1
    bgez t1,loop
    addi t3,t3,1
    andi t4,t3,1
    beqz t4,posPar
    li a7,1
    mv a0,t1
    ecall
    j transform
posPar:
    li a7,11
    li a0,0x7c
    ecall
    li a7,1
    mv a0,t1
    ecall
    li a7,11
    li a0,0x7c
    ecall
transform:
    sub t1,zero,t1
    sb t1,0(t2)
    j loop
finNegaPos:
    mv ra,s0
    lb s0,0(sp)
    ret