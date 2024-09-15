#Ej 19
.data
    texto: .string "Hola mundo!"
.text
    la a1,texto
    li t0,0
    jal PrintLen
    li a7,10
    ecall
    nop
PrintLen:
    lb t0,0(a1)
    addi t1,t1,1
    addi a1,a1,1
    bnez t0,PrintLen
    addi a0,t1,-1
    li a7,1
    ecall
    ret