		AREA datos, DATA, READWRITE
dfila 	DCB 0x00, 0xFF, 0xFF, 0x01
dcolum	DCB 0xFF, 0x00, 0xFF, 0xFF
ksize   EQU 0x4
		AREA codigo, CODE, READONLY
		PRESERVE8
		IMPORT conecta_K_buscar_alineamiento_arm_opt
		EXPORT conecta_K_hay_linea_arm_arm

conecta_K_hay_linea_arm_arm
    mov ip, sp
    stmdb r13!, {r4-r10, fp, lr}
    sub fp, ip, #4
    ; copia de parámetros
    mov r4, r0 ; r4  cuadricula
    mov r5, r1 ; r5  fila
    mov r6, r2 ; r6  columna
    mov r7, r3 ; r7  color
    ; variables locales
    ; mov r12 ,#4 ; r12  K_size = 4   no me deja pq r11 esta reservado para el fp
    ; mov r11, #4; r11  numDeltas = 4
    mov r8, #0 ; r8  i = 0
    mov r9, #0 ; r9  linea = FALSE
    mov r10, #0 ; r10  long_linea = 0
ini_bucle
    cmp r8, #4 ; i < numDeltas
    bgt fin
    cmp r9, #1 ; linea == True
    beq fin
    ldr r0, =dfila ; r0  dfila
    ldrsb r1, [r0, r8] ; r1  dfila[i]
    ldr r0, =dcolum ; r1  dcolum
    ldrsb r2, [r0, r8] ; r2  dcolum[i]
    stmdb r13!,{r1-r2} ; apilamos dfila[i] y dcolum[i] para la siguiente llamda de buscar_alineamiento
    mov r0, r4 ; r0  cuadricula
    mov r1, r5 ;  r1  fila
    mov r2, r6 ;  r2  columna
    mov r3, r7 ;  r3  color
    bl conecta_K_buscar_alineamiento_arm_opt
    ldmia r13!,{r1-r2} ; desapilamos dfila[i] y dcolum[i]
    mov r10, r0 ; long_linea = long_linea + long_linea_actual
    cmp r10, #ksize ; long_linea == K_size
    movge r9, #1 ; linea = True
    bge fin
    rsb r1 ,r1, #0 ; dfila[i] = -dfila[i]
    rsb r2 ,r2, #0 ; dcolum[i] = -dcolum[i]
    stmdb r13!,{r1-r2} ; apilamos -dfila[i] y -dcolum[i] para la siguiente llamda de buscar_alineamiento
    mov r0, r4 ; r0 cuadricula
	add r1, r5, r1 ; r1  fila + (-deltas_fila[i])
    add r2, r6, r2 ; r2  columna + (-deltas_columna[i])
    mov r3, r7 ;  r3  color (PROBAR A QUITARLO)
	bl conecta_K_buscar_alineamiento_arm_opt ; llamada con la inversa
    ldmia r13!,{r1-r2} ; desapilamos dfila[i] y dcolum[i]
    add r10, r10, r0 ; long_linea = long_linea + long_linea_actual
    cmp r10, #4 ; long_linea == K_size
    movge r9, #1 ; linea = True
    add r8, r8, #1 ; i = i + 1
    b ini_bucle
fin
    mov r0, r9 ; r0  linea
    ldmia r13!, {r4-r10, fp, lr}
    bx r14
	END