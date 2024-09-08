#Escriba un programa que imprima por consola el valor de un dato (32 bits signado)
# almacenado en memoria. Complete el programa a partir de la sección text.
.data 
    var: .word 123456
.text
    la x5,var
    lw a0,0(x5)
    li a7,1
    ecall
    li a7,10
    ecall