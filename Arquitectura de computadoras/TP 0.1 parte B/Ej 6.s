#Usando la syscall PrintChar (A7=11), imprima los valores ASCII desde 33 hasta 57
.data 
    num: .word 33
.text
    la x5,num
    li x6,57
    lw a0,0(x5)
loop:
    li a7,11
    ecall
    addi a0, a0,1
    bge x6, a0,loop
    li a7,10
    ecall
