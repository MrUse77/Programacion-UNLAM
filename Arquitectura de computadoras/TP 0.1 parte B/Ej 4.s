#Utilizando la system call ‘PrintInt’ (a7=1) imprima por consola el valor 1234.
.data 
    num: .word 1234
.text
    la x5,num
    lw x6,0(x5)
    li a7,1
    ecall