#Modifique el programa anterior para que imprima -1234 y luego el programa termine
#(Syscall Exit A7=10)
.data 
    num: .word 1234
.text
    la x5,num
    lw a0,0(x5)
    sub a0,x0,a0
    li a7,1
    ecall
    li a7,10
    ecall