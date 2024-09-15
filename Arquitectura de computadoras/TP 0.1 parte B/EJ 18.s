#Ej 18
.data
    texto: .string "Hola mundo!"
.text
    li a7,4
    la a0,texto
    ecall
    nop