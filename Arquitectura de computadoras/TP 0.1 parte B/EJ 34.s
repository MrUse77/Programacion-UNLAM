#mismo que el 33 solo que buscar el minimo
.data
    vector: .word 100,-10, 20,1000,66500,1000000,-20,34,56
    finvec:
.text
    la a3,vector
    addi a3,a3,4
    addi a4,a3,12
    jal swap
    la a1,vector
    la a2,finvec
    jal printVector
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
swap:
    lw t0,0(a3)
    lw t1,0(a4)
    sw t0,0(a4)
    sw t1,0(a3)
fin:
    ret
