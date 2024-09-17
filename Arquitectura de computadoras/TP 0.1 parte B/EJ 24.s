.data 
    letraA: .string "A"
    letraB: .string "B"
.text
    li a7,11
    lb a0,letraA
    jal toLower
    ecall
    lb a0,letraB
    jal toLower
    ecall
    li a7,10
    ecall
    nop
toLower:
    #32 letras hay entre la minuscula y la mayuscula
    #de una letra
    #Las mayusculas vienen antes
    addi a0,a0,32
    ret