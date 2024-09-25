.text
    li a1,3
    li a2,5
    jal mult
    li a7,1
    ecall
    li a1,3
    li a2,0
    jal mult
    ecall
    li a7,10
    ecall
    nop
mult:
    mv t0,a2
    li t1,0
    bnez t0,loop
    li a0,0
    ret
loop:
    addi t0,t0,-1
    add t1,t1,a1
    bgtz t0,loop
    mv a0,t1
    ret
