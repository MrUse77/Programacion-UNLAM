.text
    li a1,154
    li a2,3
    jal divide
    li a7,1
    ecall
    li a1,10
    li a2,190
    jal divide
    ecall
    li a7,10
    ecall
    nop
divide:
    mv t0,a1
    li t1,0
    blt a1,a2,fin_zero
    bne a2,a1,loop
    li a0,1
    j fin
loop:
    sub t0,t0,a2
    addi t1,t1,1
    bge t0,a2,loop
    mv a0,t1
    j fin
fin_zero:
    li a0,0
fin:
    ret

    