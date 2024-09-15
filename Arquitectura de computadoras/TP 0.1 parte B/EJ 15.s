.data
    var: .word 100,1000,65536,65537,1000000,0
    finvar:
.text
    la x5,var
    la x8,finvar
    add x9,x0,x8
    li x7,0
    li a7,1
loop:
    lw x6,0(x5)
    addi x5,x5,4
    addi x7,x7,1
    bne x8,x5,loop
    addi a0,x7,0
    ecall
    li a7,10
    ecall
    nop
    