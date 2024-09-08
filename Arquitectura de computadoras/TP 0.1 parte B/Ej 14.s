#Escriba un programa que imprima el tercer elemento del vector
.data 
    var: .word 100,1000,65536,65537,1000000
.text
    la x5,var
    addi x6,x5,8
    lw a0,0(x6)
    li a7,1
    ecall
    li a7,10
    ecall