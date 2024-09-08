#Modifique el programa anterior para que cada elemento se encuentre rodeado entre
#corchetes [ ] (verifique la tabla ASCII para los valores de los corchetes)
#Corchetes [=91 (Dec) o 5B(Hex);]=93(Dec) o 5D(Hex)
.data 
    num: .word 33
.text
    la x5,num
    li x6,57
    lw x7,0(x5)
loop:
    li a0, 91
    li a7,11
    ecall
    add a0,x0,x7 
    li a7,11
    ecall
    li a0, 93 
    li a7,11
    ecall

    addi x7, x7,1
    bge x6, x7,loop
    li a7,10
    ecall
