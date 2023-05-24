  8469  002AD2                     _get_DO:
  8470                                  opt stack 23
  8477                           ;dram.c: 116: return RD5;
  8478  002AD8  0E00                    movlw   0
  8479  002ADA  BA83                    btfsc   3971,5,c        ;volatile
  8480  002ADC  0E01                    movlw   1
  8481  002ADE  D002                    goto    l1904
  8482  002AE0                     l1905:
  8483  002AE0  FFFF                    dw      65535   ; assembler added errata NOP
  8484                           ; BSR set to: ?
  8485                           ;dram.c: 117: }
  8486                           ;dram.c: 118: return 0;
  8487  002AE2  0E00                    movlw   0
  8488                           ;dram.c: 119: }
  8489  002AE4                     l1904:
  8490  002AE4  FFFF                    dw      65535   ; assembler added errata NOP
  8491                           ; BSR set to: ?
  8492  002AE6  0012                    return
  8493  002AE8  FFFF                    dw      65535   ; errata NOP
  8494  002AEA                     __end_of_get_DO:
  8495                                  opt stack 0


  8467  002A9E                     _get_DO:
  8468                                  opt stack 23
  8475                           ;dram.c: 116: return (PORTD >> 5) & 1;
  8476  002AA4  3883                    swapf   3971,w,c        ;volatile
  8477  002AA6  42E8                    rrncf   wreg,f,c
  8478  002AA8  0B07                    andlw   7
  8479  002AAA  0B01                    andlw   1
  8480  002AAC  D002                    goto    l1904
  8481  002AAE                     l1905:
  8482  002AAE  FFFF                    dw      65535   ; assembler added errata NOP
  8483                           ; BSR set to: ?
  8484                           ;dram.c: 117: }
  8485                           ;dram.c: 118: return 0;
  8486  002AB0  0E00                    movlw   0
  8487                           ;dram.c: 119: }
  8488  002AB2                     l1904:
  8489  002AB2  FFFF                    dw      65535   ; assembler added errata NOP
  8490                           ; BSR set to: ?
  8491  002AB4  0012                    return
  8492  002AB6  FFFF                    dw      65535   ; errata NOP
  8493  002AB8                     __end_of_get_DO:
  8494                                  opt stack 0
