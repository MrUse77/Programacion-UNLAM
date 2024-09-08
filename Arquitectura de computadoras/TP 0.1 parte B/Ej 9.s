#Escriba un programa que imprima por consola el valor dado (16 bits signado)
#almacenado en memoria
.data 
    var: .half 32769
.text
    la x5,var
    lh a0,0(x5)
    li a7,1
    ecall
    li a7,10
    ecall