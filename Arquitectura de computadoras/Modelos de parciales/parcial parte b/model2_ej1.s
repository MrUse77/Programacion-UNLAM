.data
    numeros: .word 4,7,29,14,24,25,12
    vecfin:
.text
    la a0,numeros
    la a1,vecfin
    jal pares
fin:
    beq x0,x0,fin
pares:
    mv t0,a0
loop:
    lw a0, 0(t0)
    li t2,1
    and t1,a0,t2
    beq t0,a1,finPares
    addi t0,t0,4
    bnez t1,loop
    li a7,1
    ecall
    li a0,20
    li a7,11
    ecall
    j loop
finPares:
    ret