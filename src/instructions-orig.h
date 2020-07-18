// 00 - BRK
// 01 - ORA - (Indirect,X)
// 02 - Future Expansion
// 03 - Future Expansion
// 04 - Future Expansion
// 05 - ORA - Zero Page
// 06 - ASL - Zero Page
// 07 - Future Expansion
// 08 - PHP
// 09 - ORA - Immediate
// 0A - ASL - Accumulator
// 0B - Future Expansion
// 0C - Future Expansion
// 0D - ORA - Absolute
// 0E - ASL - Absolute
// 0F - Future Expansion
// 10 - BPL
// 11 - ORA - (Indirect),Y
// 12 - Future Expansion
// 13 - Future Expansion
// 14 - Future Expansion
// 15 - ORA - Zero Page,X
// 16 - ASL - Zero Page,X
// 17 - Future Expansion
// 18 - CLC
// 19 - ORA - Absolute,Y
// 1A - Future Expansion
// 1B - Future Expansion
// 1C - Future Expansion
// 1D - ORA - Absolute,X
// 1E - ASL - Absolute,X
// 1F - Future Expansion
// 20 - JSR
// 21 - AND - (Indirect,X)
// 22 - Future Expansion
// 23 - Future Expansion
// 24 - BIT - Zero Page
// 25 - AND - Zero Page
// 26 - ROL - Zero Page
// 27 - Future Expansion
// 28 - PLP
// 29 - AND - Immediate
// 2A - ROL - Accumulator
// 2B - Future Expansion
// 2C - BIT - Absolute
// 2D - AND - Absolute
// 2E - ROL - Absolute
// 2F - Future Expansion
// 30 - BMI
// 31 - AND - (Indirect),Y
// 32 - Future Expansion
// 33 - Future Expansion
// 34 - Future Expansion
// 35 - AND - Zero Page,X
// 36 - ROL - Zero Page,X
// 37 - Future Expansion
// 38 - SEC
// 39 - AND - Absolute,Y
// 3A - Future Expansion
// 3B - Future Expansion
// 3C - Future Expansion
// 3D - AND - Absolute,X
// 3E - ROL - Absolute,X
// 3F - Future Expansion
// 40 - RTI
// 41 - EOR - (Indirect,X)
// 42 - Future Expansion
// 43 - Future Expansion
// 44 - Future Expansion
// 45 - EOR - Zero Page
// 46 - LSR - Zero Page
// 47 - Future Expansion
// 48 - PHA
// 49 - EOR - Immediate
// 4A - LSR - Accumulator
// 4B - Future Expansion
// 4C - JMP - Absolute
// 4D - EOR - Absolute
// 4E - LSR - Absolute
// 4F - Future Expansion
// 50 - BVC
// 51 - EOR - (Indirect),Y
// 52 - Future Expansion
// 53 - Future Expansion
// 54 - Future Expansion
// 55 - EOR - Zero Page,X
// 56 - LSR - Zero Page,X
// 57 - Future Expansion
// 58 - CLI
// 59 - EOR - Absolute,Y
// 5A - Future Expansion
// 5B - Future Expansion
// 5C - Future Expansion
// 5D - EOR - Absolute,X
// 5E - LSR - Absolute,X
// 5F - Future Expansion
// 60 - RTS
// 61 - ADC - (Indirect,X)
// 62 - Future Expansion
// 63 - Future Expansion
// 64 - Future Expansion
// 65 - ADC - Zero Page
// 66 - ROR - Zero Page
// 67 - Future Expansion
// 68 - PLA
// 69 - ADC - Immediate
// 6A - ROR - Accumulator
// 6B - Future Expansion
// 6C - JMP - Indirect
// 6D - ADC - Absolute
// 6E - ROR - Absolute
// 6F - Future Expansion
// 70 - BVS
// 71 - ADC - (Indirect),Y
// 72 - Future Expansion
// 73 - Future Expansion
// 74 - Future Expansion
// 75 - ADC - Zero Page,X
// 76 - ROR - Zero Page,X
// 77 - Future Expansion
// 78 - SEI
// 79 - ADC - Absolute,Y
// 7A - Future Expansion
// 7B - Future Expansion
// 7C - Future Expansion
// 7D - ADC - Absolute,X
// 7E - ROR - Absolute,X
// 7F - Future Expansion
// 80 - Future Expansion
// 80 - STA - Absolute
// 81 - STA - (Indirect,X)
// 82 - Future Expansion
// 83 - Future Expansion
// 84 - STY - Zero Page
// 85 - STA - Zero Page
// 86 - STX - Zero Page
// 87 - Future Expansion
// 88 - DEY
// 89 - Future Expansion
// 8A - TXA
// 8B - Future Expansion
// 8C - STY - Absolute
// 8E - STX - Absolute
// 8F - Future Expansion
// 90 - BCC
// 91 - STA - (Indirect),Y
// 92 - Future Expansion
// 93 - Future Expansion
// 94 - STY - Zero Page,X
// 95 - STA - Zero Page,X
// 96 - STX - Zero Page,Y
// 97 - Future Expansion
// 98 - TYA
// 99 - STA - Absolute,Y
// 9A - TXS
// 9B - Future Expansion
// 9C - Future Expansion
// 9D - STA - Absolute,X
// 9E - Future Expansion
// 9F - Future Expansion
// A0 - LDY - Immediate
// A1 - LDA - (Indirect,X)
// A2 - LDX - Immediate
// A3 - Future Expansion
// A4 - LDY - Zero Page
// A5 - LDA - Zero Page
// A6 - LDX - Zero Page
// A7 - Future Expansion
// A8 - TAY
// A9 - LDA - Immediate
// AA - TAX
// AB - Future Expansion
// AC - LDY - Absolute
// AD - LDA - Absolute
// AE - LDX - Absolute
// AF - Future Expansion
// B0 - BCS
// B1 - LDA - (Indirect),Y
// B2 - Future Expansion
// B3 - Future Expansion
// B4 - LDY - Zero Page,X
// B5 - LDA - Zero Page,X
// B6 - LDX - Zero Page,Y
// B7 - Future Expansion
// B8 - CLV
// B9 - LDA - Absolute,Y
// BA - TSX
// BB - Future Expansion
// BC - LDY - Absolute,X
// BD - LDA - Absolute,X
// BE - LDX - Absolute,Y
// BF - Future Expansion
// C0 - Cpy - Immediate
// C1 - CMP - (Indirect,X)
// C2 - Future Expansion
// C3 - Future Expansion
// C4 - CPY - Zero Page
// C5 - CMP - Zero Page
// C6 - DEC - Zero Page
// C7 - Future Expansion
// C8 - INY
// C9 - CMP - Immediate
// CA - DEX
// CB - Future Expansion
// CC - CPY - Absolute
// CD - CMP - Absolute
// CE - DEC - Absolute
// CF - Future Expansion
// D0 - BNE
// D1 - CMP   (Indirect@,Y
// D2 - Future Expansion
// D3 - Future Expansion
// D4 - Future Expansion
// D5 - CMP - Zero Page,X
// D6 - DEC - Zero Page,X
// D7 - Future Expansion
// D8 - CLD
// D9 - CMP - Absolute,Y
// DA - Future Expansion
// DB - Future Expansion
// DC - Future Expansion
// DD - CMP - Absolute,X
// DE - DEC - Absolute,X
// DF - Future Expansion
// E0 - CPX - Immediate
// E1 - SBC - (Indirect,X)
// E2 - Future Expansion
// E3 - Future Expansion
// E4 - CPX - Zero Page
// E5 - SBC - Zero Page
// E6 - INC - Zero Page
// E7 - Future Expansion
// E8 - INX
// E9 - SBC - Immediate
// EA - NOP
// EB - Future Expansion
// EC - CPX - Absolute
// ED - SBC - Absolute
// EE - INC - Absolute
// EF - Future Expansion
// F0 - BEQ
// F1 - SBC - (Indirect),Y
// F2 - Future Expansion
// F3 - Future Expansion
// F4 - Future Expansion
// F5 - SBC - Zero Page,X
// F6 - INC - Zero Page,X
// F7 - Future Expansion
// F8 - SED
// F9 - SBC - Absolute,Y
// FA - Future Expansion
// FB - Future Expansion
// FC - Future Expansion
// FD - SBC - Absolute,X
// FE - INC - Absolute,X
// FF - Future Expansion
