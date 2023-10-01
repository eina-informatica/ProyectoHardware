	AREA codigo, CODE, READONLY
	PRESERVE8
	EXPORT conecta_K_buscar_alineamiento_arm_opt
		
conecta_K_buscar_alineamiento_arm_opt
        mov ip, sp
        stmdb r13!, {r4-r9, fp, lr}
        sub fp, ip, #4
        ; comprobamos si la fila es válida
		ldrsb r4, [fp, #4] ; r4  deltas_fila[i]
		ldrsb r5, [fp, #8] ; r5  deltas_columna[i]
		mov r9, #0 ; r9  long_linea = 0
bucle_ini
		mov r8,#0; inicializamos r8 a 0 (contador)
		cmp r1, #0
        blt fin ; si fila < 0
        cmp r1, #7
        bge fin ; si fila >= 7
        ; comprobamos si la columna es válida
        cmp r2, #0
        blt fin ; si columna < 0
        cmp r2, #7
        bge fin ; si columna >= 7
bucle
        ; si está la celda, obtenemos su valor
        cmp r8,#6
        beq fin
        add r6,r1,r1,LSL #1 
        add r7,r0,r6,LSL #1
        ldrb r7,[r7,r8]
        add r8,r8,#1 ;cont++
        ; comprobamos si está la celda
        cmp r7,r2
        bne bucle
        sub r8,r8,#1 ; cont-- para recuperar el valor correcto
        add r6,r1,r1,LSL #1
        add r7,r0,#0x0000002A ; buscar en la parte de abajo del tablero (dnde se indexan los colores)
        add r7,r7,r6,LSL #1
        ldrb r7,[r7,r8] ; valor del color
        and r7,r7,#0x03
        cmp r7,r3 ; comparamos el color con el parámetro
        bne fin
		add r9,r9,#1 ; long_linea++
        add r1, r1, r4 ; r1 - fila + deltas_fila[i]
        add r2, r2, r5 ; r2 - columna + deltas_columna[i]
		b bucle_ini
fin
        mov r0, r9 ; r0 - 0 (return 0)
        ldmia r13!, {r4-r9, fp, lr}
        bx r14
		END