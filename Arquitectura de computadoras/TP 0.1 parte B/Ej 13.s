#Escriba un programa que cuente cuantos elementos componen un vector utilizando cero
#como ultimo elemento del vector.
.data 
    var: .word 100,1000,65536,65537,1000000,0
.text
    la x5,var

loop:
    lw x6,0(x5)
    beqz x6,fin
    addi x5,x5,4
    addi x7,x7,1
    j loop
fin:    
    add a0,x0,x7
    li a7,1
    ecall
    nop