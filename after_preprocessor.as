MAIN:     mov S1.1 ,LENGTH
          add r2,STR
          prn r7
LOOP:     jmp END
          prn #-5
          sub r1,r4
          inc k
          prn #7
          inc r1
          mov S1.2 ,r3
          bne LOOP
END:      hlt
STR:      .string "abcdef"
          dec r1
          mov #2, r2
          prn r5
          mov #-2, r3
LENGTH:   .data 6,-9,15
K:        .data 22
S1:       .struct 8, "ab"