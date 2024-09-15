#Ej 20
.data
    texto: .string "Hola mundo!"
.text
    la a1,texto
    jal StrLen
    li a7,1
    ecall
    li a7,10
    ecall
    nop
StrLen:
    lb t0,0(a1)
    addi t1,t1,1
    addi a1,a1,1
    bnez t0,StrLen
    addi a0,t1,-1
    sub a1,a1,t1
    ret