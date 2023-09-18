12
11
10
9
8
7
6
5
4
3
2
1




    10:                 // comprobar si la celda es valida y del mismo color 
0x000001E8  E92D5FFC  STMDB     R13!,{R2-R12,R14}
0x000001EC  E1A0A000  MOV       R10,R0
0x000001F0  E1A04001  MOV       R4,R1
0x000001F4  E1A05002  MOV       R5,R2
0x000001F8  E1A0B003  MOV       R11,R3
0x000001FC  E28D7030  ADD       R7,R13,#0x00000030
0x00000200  E89700C0  LDMIA     R7,{R6-R7}
    11:                 if (tablero_buscar_color(t, fila, columna, color) != EXITO) { 
0x00000204  E1A0300B  MOV       R3,R11
0x00000208  E1A02005  MOV       R2,R5
0x0000020C  E1A01004  MOV       R1,R4
0x00000210  E1A0000A  MOV       R0,R10
0x00000214  EB00011D  BL        tablero_buscar_color(0x00000690)
0x00000218  E3500000  CMP       R0,#0x00000000
0x0000021C  0A000002  BEQ       0x0000022C
    12:                         return 0; 
    13:                 } 
    14:                  
    15:     // encontrada, entonces avanzar Ã­ndices 
    16:     uint8_t nueva_fila = fila + delta_fila; 
    17:     uint8_t nueva_columna = columna + delta_columna; 
    18:  
    19:     // incrementar longitud y visitar celda vecina 
    20:     return 1 + conecta_K_buscar_alineamiento_c(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna); 
0x00000220  E3A00000  MOV       R0,#0x00000000
0x00000224  E8BD5FFC  LDMIA     R13!,{R2-R12,R14}
    21: } 
    22:  
    23: // devuelve true si encuentra una lÃ­nea de longitud mayor o igual a _K 
    24: uint8_t 
    25: conecta_K_hay_linea_c_c(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color) 
0x00000228  E12FFF1E  BX        R14
    16:     uint8_t nueva_fila = fila + delta_fila; 
0x0000022C  E0840006  ADD       R0,R4,R6
0x00000230  E20080FF  AND       R8,R0,#0x000000FF
    17:     uint8_t nueva_columna = columna + delta_columna; 
    18:  
    19:     // incrementar longitud y visitar celda vecina 
0x00000234  E0850007  ADD       R0,R5,R7
0x00000238  E20090FF  AND       R9,R0,#0x000000FF
    20:     return 1 + conecta_K_buscar_alineamiento_c(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna); 
    21: } 
    22:  
    23: // devuelve true si encuentra una lÃ­nea de longitud mayor o igual a _K 
    24: uint8_t 
    25: conecta_K_hay_linea_c_c(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color) 
0x0000023C  E1A0300B  MOV       R3,R11
0x00000240  E1A02009  MOV       R2,R9
0x00000244  E1A01008  MOV       R1,R8
0x00000248  E1A0000A  MOV       R0,R10
0x0000024C  E88D00C0  STMIA     R13,{R6-R7}
0x00000250  EBFFFFE4  BL        conecta_K_buscar_alineamiento_c(0x000001E8)
0x00000254  E2800001  ADD       R0,R0,#0x00000001
0x00000258  E20000FF  AND       R0,R0,#0x000000FF
0x0000025C  EAFFFFF0  B         0x00000224