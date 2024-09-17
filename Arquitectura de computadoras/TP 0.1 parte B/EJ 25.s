.data 
    texto1: .string "UnA"
    texto2: .string "una frase"
.text
    li a7,1
    la a4,texto1
    la a5,texto2
    jal strCmpi
    ecall
    li a7,10
    ecall
strCmpi:
    addi a6,x1,0
    lb t1, 0(a4)            # Cargar el byte actual de la primera cadena
    lb t2, 0(a5)            # Cargar el byte actual de la segunda cadena
    beq t1, zero, end_cmpi  # Si alcanzamos el fin de la primera cadena, terminar
    beq t2, zero, end_cmpi  # Si alcanzamos el fin de la segunda cadena, terminar

    jal toLower        # Convertir a minusculas
   # jalr t2,a1,toLower
    addi x1,a6,0
    bne t1, t2, not_equali  # Si los caracteres son diferentes, saltar a not_equali
    addi a4, a4, 1          # Avanzar en la primera cadena
    addi a5, a5, 1          # Avanzar en la segunda cadena
    j strCmpi       # Repetir el ciclo
    nop
end_cmpi:
    li a0, 0                # Cadenas son iguales, devolver 0 en a0
    ret
not_equali:
    li a0, 1                # Cadenas son diferentes, devolver 1 en a0
    ret
toLower:
    #32 letras hay entre la minuscula y la mayuscula
    #de una letra
    #Las mayusculas vienen antes
    li t4, 0x41             # Cargar valor ASCII de 'A'
    li t5, 0x5A             # Cargar valor ASCII de 'Z'
    li t6, 0x20             # Diferencia entre mayúscula y minúscula en ASCII
    blt t1, t4, end_toLower # Si es menor que 'A', ya está en minúscula
    bgt t1, t5, end_toLower # Si es mayor que 'Z', ya está en minúscula
    or t1,t1,t6
    
    blt t2, t4, end_toLower # Si es menor que 'A', ya está en minúscula
    bgt t2, t5, end_toLower # Si es mayor que 'Z', ya está en minúscula
    or t2, t2, t6           # Convertir a minúscula sumando el offset
        sb a5,0(t2)
end_toLower:
    ret