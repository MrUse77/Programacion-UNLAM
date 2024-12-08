#mismo que el 33 solo que buscar el minimo
.data
    vector: .word 100,-10, 20,1000,66500,1000000,-20,34,56
    finvec:
.text
    la a3,vector
    la a4,finvec
    sub t5,a4,a3
    srai t5,t5,2
mainLoop:
    la a3,vector
    la a4,finvec
    jal pushMax
    bnez a0,fin_main
    la a1,vector
    la a2,finvec
    jal printVector
    li a7,11
    li a0,10
    ecall
    addi t5,t5,-1

    bgtz t5,mainLoop
fin_main:
    li a7,10
    ecall
    nop
printVector:
    li t0,0
    sub t1,a2,a1
    srai s0,t1,2
loop:
    lw a0,0(a1)
    li a7,1
    ecall
    addi a1,a1,4
    addi t0,t0,1
    blt t0,s0,coma
    bne t0,s0,loop
    j fin
coma:
    li a0,59,
    li a7,11
    ecall
    j loop
pushMax:
    li t2,0xb4
loop_push:
    lw t0,0(a3)
    lw t1,4(a3)
    addi s0,a4,-4
    bgt t0,t1,swap
    addi a3,a3,4
    blt a3,s0,loop_push
fin:
    ret
swap:
    sw t0,4(a3)
    sw t1,0(a3)
    li a0,0
    jalr zero,t2,0
