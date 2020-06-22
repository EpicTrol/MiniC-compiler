* MiniC Compilation to TM Code
* File: t2.tm
* Standard prelude
  0:     LD  6,0(0)   load gp with maxaddress
  1:    LDA  5,-4(6)   copy offset to fp
  2:     ST  0,0(0)   clear location 0
* End of standard prelude
* -> Input
  6:     ST  0,-1(5)   input: store return address to retFO
  7:     IN  0,0,0   input: read integer value
  8:     LD  7,-1(5)   input: return
* <- Input
* -> Output
  9:     ST  0,-1(5)   output: store return address to retFO
 10:     LD  0,-2(5)   output: load first arg to ac
 11:    OUT  0,0,0   output: print
 12:     LD  7,-1(5)   output: return
* <- Output
* -> FunD
 13:     ST  0,-1(5)   funD: store return address to retFO
* -> Compound
* -> If
* -> OpE
* -> VarEK
* varE: address of arr
 14:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 15:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 16:     ST  0,0(4)   opE: push left
* -> Const
 17:    LDC  0,0(0)   const: load const
* <- Const
 18:     LD  1,0(4)   opE: load left
 19:    SUB  0,1,0   op ==
 20:    JEQ  0,2(7)   br if true
 21:    LDC  0,0(0)   false case
 22:    LDA  7,1(7)   unconditional jmp
 23:    LDC  0,1(0)   true case
* <- Op
* if: jump to else belongs here
* -> Return
* -> VarEK
* varE: address of arr
 25:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
 26:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 27:     LD  7,-1(5)   retS: return
* <- Return
* if: jump to end belongs here
 24:    JEQ  0,4(7)   If: jmp to else
* -> Return
* -> CallE
* callE: cal every arguments
* -> VarEK
* varE: address of arr
 29:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 30:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 31:     ST  0,-6(5)   callE: store argument
* -> OpE
* -> VarEK
* varE: address of arr
 32:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
 33:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 34:     ST  0,1(4)   opE: push left
* -> OpE
* -> OpE
* -> VarEK
* varE: address of arr
 35:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
 36:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 37:     ST  0,2(4)   opE: push left
* -> VarEK
* varE: address of arr
 38:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 39:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 40:     LD  1,2(4)   opE: load left
 41:    DIV  0,1,0   opE /
* <- Op
 42:     ST  0,3(4)   opE: push left
* -> VarEK
* varE: address of arr
 43:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 44:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 45:     LD  1,3(4)   opE: load left
 46:    MUL  0,1,0   opE *
* <- Op
 47:     LD  1,1(4)   opE: load left
 48:    SUB  0,1,0   opE -
* <- Op
 49:     ST  0,-6(5)   callE: store argument
 50:     ST  5,-4(5)   store current fp
 51:    LDA  5,-4(5)   push new frame
 52:    LDA  0,1(7)   save return in ac
 53:    LDC  7,13(0)   jump to function entry
 54:     LD  5,0(5)   pop current frame
* <- CallE
 55:     LD  7,-1(5)   retS: return
* <- Return
 28:    LDA  7,27(7)   jf: jmp to end
* <- If
* <- Compound
 56:     LD  7,-1(5)   funD: return
* <- FunD
* -> FunD
 57:     ST  0,-1(5)   funD: store return address to retFO
* -> Compound
* -> VarD
* <- VarD
* -> VarD
* <- VarD
* -> AssignE
* -> VarEK
* varE: address of arr
 58:    LDA  0,-2(5)   varE: cal address in local
* <- VarEK
 59:     ST  0,4(4)   assignE: push left
* -> CallE
* callE: cal every arguments
 60:     ST  5,-4(5)   store current fp
 61:    LDA  5,-4(5)   push new frame
 62:    LDA  0,1(7)   save return in ac
 63:    LDC  7,6(0)   jump to function entry
 64:     LD  5,0(5)   pop current frame
* <- CallE
 65:     LD  1,4(4)   assignE: load left
 66:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
 67:    LDA  0,-3(5)   varE: cal address in local
* <- VarEK
 68:     ST  0,5(4)   assignE: push left
* -> CallE
* callE: cal every arguments
 69:     ST  5,-4(5)   store current fp
 70:    LDA  5,-4(5)   push new frame
 71:    LDA  0,1(7)   save return in ac
 72:    LDC  7,6(0)   jump to function entry
 73:     LD  5,0(5)   pop current frame
* <- CallE
 74:     LD  1,5(4)   assignE: load left
 75:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> CallE
* callE: cal every arguments
* -> CallE
* callE: cal every arguments
* -> VarEK
* varE: address of arr
 76:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
 77:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 78:     ST  0,-6(5)   callE: store argument
* -> VarEK
* varE: address of arr
 79:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 80:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 81:     ST  0,-6(5)   callE: store argument
 82:     ST  5,-4(5)   store current fp
 83:    LDA  5,-4(5)   push new frame
 84:    LDA  0,1(7)   save return in ac
 85:    LDC  7,13(0)   jump to function entry
 86:     LD  5,0(5)   pop current frame
* <- CallE
 87:     ST  0,-6(5)   callE: store argument
 88:     ST  5,-4(5)   store current fp
 89:    LDA  5,-4(5)   push new frame
 90:    LDA  0,1(7)   save return in ac
 91:    LDC  7,9(0)   jump to function entry
 92:     LD  5,0(5)   pop current frame
* <- CallE
* <- Compound
 93:     LD  7,-1(5)   funD: return
* <- FunD
  3:    LDC  0,94(0)   main: load return address to ac
  4:     ST  0,-1(5)   main: store return address to retFO
  5:    LDC  7,57(0)   jump to main
* End of execution.
 94:   HALT  0,0,0   
