#Escriba un programa que imprima el tercer elemento del vector
.data 
    var: .byte 129 #Valor en hexa 0x81
.text
    la x5,var
    lb a0,0(x5) # Tiene signo 0xffffff81
    li a7,1
    ecall # Signado Rango (-128 a 127), al pasarse de valor positivo da la vuelta y sigue en los negativos
    li a7,10
    ecall