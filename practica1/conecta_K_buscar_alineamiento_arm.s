	AREA codigo, CODE, READONLY
	PRESERVE8
	EXPORT conecta_K_buscar_alineamiento_arm

conecta_K_buscar_alineamiento_arm
        mov ip, sp
        stmdb r13!, {r4-r8, fp, lr}
        sub fp, ip, #4
        ; comprobamos si la fila es válida
        cmp r1, #0
        blt fin ; si fila < 0
        cmp r1, #7
        bge fin ; si fila >= 7
        ; comprobamos si la columna es válida
        cmp r2, #0
        blt fin ; si columna < 0
        cmp r2, #7
        bge fin ; si columna >= 7
        ; si la celda se encuentra en el tablero, obtenemos su valor
        mov r8,#0; inicializamos r8 a 0 (contador)
bucle
        cmp r8,#6
        beq fin
        add r6,r1,r1,LSL #1 
        add r7,r0,r6,LSL #1
        ldrb r7,[r7,r8]
        add r8,r8,#1 ;cont++
        ; comprobamos si esta tu celda
        cmp r7,r2
        bne bucle
        sub r8,r8,#1  ; cont-- para recuperar el valor correcto
        add r6,r1,r1,LSL #1
        add r7,r0,#0x0000002A ; buscar en la parte de abajo del tablero (donde se indexan los colores)
        add r7,r7,r6,LSL #1
        ldrb r7,[r7,r8] ; valor del color
        and r7,r7,#0x03
        cmp r7,r3 ; comparamos el color con el parámetro
        bne fin
        ldrsb r4, [fp, #4] ; r4 - deltas_fila[i]
        add r1, r1, r4 ; r1 - fila + deltas_fila[i]
        ldrsb r5, [fp, #8] ; r5 - deltas_columna[i]
        add r2, r2, r5 ; r2 - columna + deltas_columna[i]
        stmdb r13!, {r4-r5} ; se apilan los deltas para la siguiente llamada recursiva
        bl conecta_K_buscar_alineamiento_arm
        add sp, sp, #8
        add r0, r0, #1 ; r0 - r0 + 1 (return 1 + resultado funci?n)
        ldmia r13!, {r4-r8, fp, lr}
        bx r14
fin
        mov r0, #0 ; r0 - 0 (return 0)
        ldmia r13!, {r4-r8, fp, lr}
        bx r14
		END