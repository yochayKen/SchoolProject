;this is commant
.entry W
MAIN:     mov S1.1 ,LENGTH
          add r2,STR
          macro m2
          prn #7
          inc r1
          endmacro
          macro m1
          dec r1
          mov #2, r2
          prn r5
          mov #-2, r3
          endmacro
          prn r7
LOOP:     jmp END
          prn #-5
          sub r1,r4
          inc K
          m2
          prn W
          prn BLABLABLA
          mov S1.2 ,r3
          bne LOOP
END:      hlt
STR:      .string "abcdef"
          m1
LENGTH:   .data 6,-9,15
K:        .data 22
S1:       .struct 8, "ab"