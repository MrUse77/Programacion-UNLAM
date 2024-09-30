#Ej 17
.data
    var: .word 100,1000,65536,65537,1000000,0
    finvar:
.text
    la a1,var
    la a2, finvar
    li x2,0
    sub a2,a2,a1
    srai a2,a2,2
    jal x1,printVector
    li a7,10
    ecall
    nop
printVector:
    lw a0,0(a1)
    li a7,1
    ecall
    addi a1,a1,4
    addi x2,x2,1
    blt x2,a2,coma
    bne x2,a2,printVector
    ret
coma:
    li a0,59
    li a7,11
    ecall
    j printVector