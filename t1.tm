* MiniC Compilation to TM Code
* File: t1.tm
* Standard prelude
  0:     LD  6,0(0)   load gp with maxaddress
  1:    LDA  5,-6(6)   copy offset to fp
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
* -> ArrayD
* <- ArrayD
* -> FunD
 13:     ST  0,-1(5)   funD: store return address to retFO
* -> Compound
* -> VarD
* <- VarD
* -> VarD
* <- VarD
* -> VarD
* <- VarD
* -> AssignE
* -> VarEK
* varE: address of arr
 14:    LDA  0,-7(5)   varE: cal address in local
* <- VarEK
 15:     ST  0,0(4)   assignE: push left
* -> VarEK
* varE: address of arr
 16:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 17:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 18:     LD  1,0(4)   assignE: load left
 19:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
 20:    LDA  0,-6(5)   varE: cal address in local
* <- VarEK
 21:     ST  0,1(4)   assignE: push left
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
 22:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 23:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 24:    LDA  1,-2(5)   arrE: cal the base address in local
 25:     LD  1,0(1)   arrE: redirect
 26:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
 27:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
 28:     LD  1,1(4)   assignE: load left
 29:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
 30:    LDA  0,-5(5)   varE: cal address in local
* <- VarEK
 31:     ST  0,2(4)   assignE: push left
* -> OpE
* -> VarEK
* varE: address of arr
 32:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
 33:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 34:     ST  0,3(4)   opE: push left
* -> Const
 35:    LDC  0,1(0)   const: load const
* <- Const
 36:     LD  1,3(4)   opE: load left
 37:    ADD  0,1,0   opE +
* <- Op
 38:     LD  1,2(4)   assignE: load left
 39:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> While
* -> OpE
* -> VarEK
* varE: address of arr
 40:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
 41:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 42:     ST  0,4(4)   opE: push left
* -> VarEK
* varE: address of arr
 43:    LDA  0,-4(5)   varE: cal address in local
* varE: if ! isAddress load the value
 44:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 45:     LD  1,4(4)   opE: load left
 46:    SUB  0,1,0   op <
 47:    JLT  0,2(7)   br if true
 48:    LDC  0,0(0)   false case
 49:    LDA  7,1(7)   unconditional jmp
 50:    LDC  0,1(0)   true case
* <- Op
* While: jump to end belongs here
* -> Compound
* -> If
* -> OpE
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
 52:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
 53:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 54:    LDA  1,-2(5)   arrE: cal the base address in local
 55:     LD  1,0(1)   arrE: redirect
 56:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
 57:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
 58:     ST  0,5(4)   opE: push left
* -> VarEK
* varE: address of arr
 59:    LDA  0,-6(5)   varE: cal address in local
* varE: if ! isAddress load the value
 60:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 61:     LD  1,5(4)   opE: load left
 62:    SUB  0,1,0   op <
 63:    JLT  0,2(7)   br if true
 64:    LDC  0,0(0)   false case
 65:    LDA  7,1(7)   unconditional jmp
 66:    LDC  0,1(0)   true case
* <- Op
* if: jump to else belongs here
* -> Compound
* -> AssignE
* -> VarEK
* varE: address of arr
 68:    LDA  0,-6(5)   varE: cal address in local
* <- VarEK
 69:     ST  0,6(4)   assignE: push left
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
 70:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
 71:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 72:    LDA  1,-2(5)   arrE: cal the base address in local
 73:     LD  1,0(1)   arrE: redirect
 74:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
 75:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
 76:     LD  1,6(4)   assignE: load left
 77:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
 78:    LDA  0,-7(5)   varE: cal address in local
* <- VarEK
 79:     ST  0,7(4)   assignE: push left
* -> VarEK
* varE: address of arr
 80:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
 81:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 82:     LD  1,7(4)   assignE: load left
 83:     ST  0,0(1)   assignE: store right to left
* <- assign
* <- Compound
* if: jump to end belongs here
 67:    JEQ  0,17(7)   If: jmp to else
 84:    LDA  7,0(7)   jf: jmp to end
* <- If
* -> AssignE
* -> VarEK
* varE: address of arr
 85:    LDA  0,-5(5)   varE: cal address in local
* <- VarEK
 86:     ST  0,8(4)   assignE: push left
* -> OpE
* -> VarEK
* varE: address of arr
 87:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
 88:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 89:     ST  0,9(4)   opE: push left
* -> Const
 90:    LDC  0,1(0)   const: load const
* <- Const
 91:     LD  1,9(4)   opE: load left
 92:    ADD  0,1,0   opE +
* <- Op
 93:     LD  1,8(4)   assignE: load left
 94:     ST  0,0(1)   assignE: store right to left
* <- assign
* <- Compound
 95:    LDA  7,-56(7)   jmp to begin
* While: jump to begin belongs here
 51:    JEQ  0,44(7)   jmp to end
* <- While
* -> Return
* -> VarEK
* varE: address of arr
 96:    LDA  0,-7(5)   varE: cal address in local
* varE: if ! isAddress load the value
 97:     LD  0,0(0)   varE: load value to ac
* <- VarEK
 98:     LD  7,-1(5)   retS: return
* <- Return
* <- Compound
 99:     LD  7,-1(5)   funD: return
* <- FunD
* -> FunD
100:     ST  0,-1(5)   funD: store return address to retFO
* -> Compound
* -> VarD
* <- VarD
* -> VarD
* <- VarD
* -> AssignE
* -> VarEK
* varE: address of arr
101:    LDA  0,-5(5)   varE: cal address in local
* <- VarEK
102:     ST  0,10(4)   assignE: push left
* -> VarEK
* varE: address of arr
103:    LDA  0,-3(5)   varE: cal address in local
* varE: if ! isAddress load the value
104:     LD  0,0(0)   varE: load value to ac
* <- VarEK
105:     LD  1,10(4)   assignE: load left
106:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> While
* -> OpE
* -> VarEK
* varE: address of arr
107:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
108:     LD  0,0(0)   varE: load value to ac
* <- VarEK
109:     ST  0,11(4)   opE: push left
* -> OpE
* -> VarEK
* varE: address of arr
110:    LDA  0,-4(5)   varE: cal address in local
* varE: if ! isAddress load the value
111:     LD  0,0(0)   varE: load value to ac
* <- VarEK
112:     ST  0,12(4)   opE: push left
* -> Const
113:    LDC  0,1(0)   const: load const
* <- Const
114:     LD  1,12(4)   opE: load left
115:    SUB  0,1,0   opE -
* <- Op
116:     LD  1,11(4)   opE: load left
117:    SUB  0,1,0   op <
118:    JLT  0,2(7)   br if true
119:    LDC  0,0(0)   false case
120:    LDA  7,1(7)   unconditional jmp
121:    LDC  0,1(0)   true case
* <- Op
* While: jump to end belongs here
* -> Compound
* -> VarD
* <- VarD
* -> AssignE
* -> VarEK
* varE: address of arr
123:    LDA  0,-6(5)   varE: cal address in local
* <- VarEK
124:     ST  0,13(4)   assignE: push left
* -> CallE
* callE: cal every arguments
* -> VarEK
* varE: address of arr
125:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
126:     LD  0,0(0)   varE: load value to ac
* <- VarEK
127:     ST  0,-10(5)   callE: store argument
* -> VarEK
* varE: address of arr
128:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
129:     LD  0,0(0)   varE: load value to ac
* <- VarEK
130:     ST  0,-10(5)   callE: store argument
* -> VarEK
* varE: address of arr
131:    LDA  0,-4(5)   varE: cal address in local
* varE: if ! isAddress load the value
132:     LD  0,0(0)   varE: load value to ac
* <- VarEK
133:     ST  0,-10(5)   callE: store argument
134:     ST  5,-8(5)   store current fp
135:    LDA  5,-8(5)   push new frame
136:    LDA  0,1(7)   save return in ac
137:    LDC  7,13(0)   jump to function entry
138:     LD  5,0(5)   pop current frame
* <- CallE
139:     LD  1,13(4)   assignE: load left
140:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
141:    LDA  0,-7(5)   varE: cal address in local
* <- VarEK
142:     ST  0,14(4)   assignE: push left
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
143:    LDA  0,-6(5)   varE: cal address in local
* varE: if ! isAddress load the value
144:     LD  0,0(0)   varE: load value to ac
* <- VarEK
145:    LDA  1,-2(5)   arrE: cal the base address in local
146:     LD  1,0(1)   arrE: redirect
147:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
148:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
149:     LD  1,14(4)   assignE: load left
150:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
151:    LDA  0,-6(5)   varE: cal address in local
* varE: if ! isAddress load the value
152:     LD  0,0(0)   varE: load value to ac
* <- VarEK
153:    LDA  1,-2(5)   arrE: cal the base address in local
154:     LD  1,0(1)   arrE: redirect
155:    SUB  0,1,0   arrE: cal the absolute address
* <- ArrE
156:     ST  0,15(4)   assignE: push left
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
157:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
158:     LD  0,0(0)   varE: load value to ac
* <- VarEK
159:    LDA  1,-2(5)   arrE: cal the base address in local
160:     LD  1,0(1)   arrE: redirect
161:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
162:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
163:     LD  1,15(4)   assignE: load left
164:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
165:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
166:     LD  0,0(0)   varE: load value to ac
* <- VarEK
167:    LDA  1,-2(5)   arrE: cal the base address in local
168:     LD  1,0(1)   arrE: redirect
169:    SUB  0,1,0   arrE: cal the absolute address
* <- ArrE
170:     ST  0,16(4)   assignE: push left
* -> VarEK
* varE: address of arr
171:    LDA  0,-7(5)   varE: cal address in local
* varE: if ! isAddress load the value
172:     LD  0,0(0)   varE: load value to ac
* <- VarEK
173:     LD  1,16(4)   assignE: load left
174:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
175:    LDA  0,-5(5)   varE: cal address in local
* <- VarEK
176:     ST  0,17(4)   assignE: push left
* -> OpE
* -> VarEK
* varE: address of arr
177:    LDA  0,-5(5)   varE: cal address in local
* varE: if ! isAddress load the value
178:     LD  0,0(0)   varE: load value to ac
* <- VarEK
179:     ST  0,18(4)   opE: push left
* -> Const
180:    LDC  0,1(0)   const: load const
* <- Const
181:     LD  1,18(4)   opE: load left
182:    ADD  0,1,0   opE +
* <- Op
183:     LD  1,17(4)   assignE: load left
184:     ST  0,0(1)   assignE: store right to left
* <- assign
* <- Compound
185:    LDA  7,-79(7)   jmp to begin
* While: jump to begin belongs here
122:    JEQ  0,63(7)   jmp to end
* <- While
* <- Compound
186:     LD  7,-1(5)   funD: return
* <- FunD
* -> FunD
187:     ST  0,-1(5)   funD: store return address to retFO
* -> Compound
* -> VarD
* <- VarD
* -> AssignE
* -> VarEK
* varE: address of arr
188:    LDA  0,-2(5)   varE: cal address in local
* <- VarEK
189:     ST  0,19(4)   assignE: push left
* -> Const
190:    LDC  0,0(0)   const: load const
* <- Const
191:     LD  1,19(4)   assignE: load left
192:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> While
* -> OpE
* -> VarEK
* varE: address of arr
193:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
194:     LD  0,0(0)   varE: load value to ac
* <- VarEK
195:     ST  0,20(4)   opE: push left
* -> Const
196:    LDC  0,10(0)   const: load const
* <- Const
197:     LD  1,20(4)   opE: load left
198:    SUB  0,1,0   op <
199:    JLT  0,2(7)   br if true
200:    LDC  0,0(0)   false case
201:    LDA  7,1(7)   unconditional jmp
202:    LDC  0,1(0)   true case
* <- Op
* While: jump to end belongs here
* -> Compound
* -> AssignE
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
204:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
205:     LD  0,0(0)   varE: load value to ac
* <- VarEK
206:    LDA  1,-6(5)   arrE: cal the base address in local
207:     LD  1,0(1)   arrE: redirect
208:    SUB  0,1,0   arrE: cal the absolute address
* <- ArrE
209:     ST  0,21(4)   assignE: push left
* -> CallE
* callE: cal every arguments
210:     ST  5,-3(5)   store current fp
211:    LDA  5,-3(5)   push new frame
212:    LDA  0,1(7)   save return in ac
213:    LDC  7,6(0)   jump to function entry
214:     LD  5,0(5)   pop current frame
* <- CallE
215:     LD  1,21(4)   assignE: load left
216:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> AssignE
* -> VarEK
* varE: address of arr
217:    LDA  0,-2(5)   varE: cal address in local
* <- VarEK
218:     ST  0,22(4)   assignE: push left
* -> OpE
* -> VarEK
* varE: address of arr
219:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
220:     LD  0,0(0)   varE: load value to ac
* <- VarEK
221:     ST  0,23(4)   opE: push left
* -> Const
222:    LDC  0,1(0)   const: load const
* <- Const
223:     LD  1,23(4)   opE: load left
224:    ADD  0,1,0   opE +
* <- Op
225:     LD  1,22(4)   assignE: load left
226:     ST  0,0(1)   assignE: store right to left
* <- assign
* <- Compound
227:    LDA  7,-35(7)   jmp to begin
* While: jump to begin belongs here
203:    JEQ  0,24(7)   jmp to end
* <- While
* -> CallE
* callE: cal every arguments
* -> VarEK
* varE: address of arr
228:    LDA  0,-6(5)   varE: cal address in local
* varE: if ! isAddress load the value
229:     LD  0,0(0)   varE: load value to ac
* <- VarEK
230:     ST  0,-5(5)   callE: store argument
* -> Const
231:    LDC  0,0(0)   const: load const
* <- Const
232:     ST  0,-5(5)   callE: store argument
* -> Const
233:    LDC  0,10(0)   const: load const
* <- Const
234:     ST  0,-5(5)   callE: store argument
235:     ST  5,-3(5)   store current fp
236:    LDA  5,-3(5)   push new frame
237:    LDA  0,1(7)   save return in ac
238:    LDC  7,100(0)   jump to function entry
239:     LD  5,0(5)   pop current frame
* <- CallE
* -> AssignE
* -> VarEK
* varE: address of arr
240:    LDA  0,-2(5)   varE: cal address in local
* <- VarEK
241:     ST  0,24(4)   assignE: push left
* -> Const
242:    LDC  0,0(0)   const: load const
* <- Const
243:     LD  1,24(4)   assignE: load left
244:     ST  0,0(1)   assignE: store right to left
* <- assign
* -> While
* -> OpE
* -> VarEK
* varE: address of arr
245:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
246:     LD  0,0(0)   varE: load value to ac
* <- VarEK
247:     ST  0,25(4)   opE: push left
* -> Const
248:    LDC  0,10(0)   const: load const
* <- Const
249:     LD  1,25(4)   opE: load left
250:    SUB  0,1,0   op <
251:    JLT  0,2(7)   br if true
252:    LDC  0,0(0)   false case
253:    LDA  7,1(7)   unconditional jmp
254:    LDC  0,1(0)   true case
* <- Op
* While: jump to end belongs here
* -> Compound
* -> CallE
* callE: cal every arguments
* -> ArrayEK
* ArrayEK: cal address of arr(base + index)
* -> VarEK
* varE: address of arr
256:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
257:     LD  0,0(0)   varE: load value to ac
* <- VarEK
258:    LDA  1,-6(5)   arrE: cal the base address in local
259:     LD  1,0(1)   arrE: redirect
260:    SUB  0,1,0   arrE: cal the absolute address
* arrE: if ! isAddress load the value
261:     LD  0,0(0)   arrE: put value in ac
* <- ArrE
262:     ST  0,-5(5)   callE: store argument
263:     ST  5,-3(5)   store current fp
264:    LDA  5,-3(5)   push new frame
265:    LDA  0,1(7)   save return in ac
266:    LDC  7,9(0)   jump to function entry
267:     LD  5,0(5)   pop current frame
* <- CallE
* -> AssignE
* -> VarEK
* varE: address of arr
268:    LDA  0,-2(5)   varE: cal address in local
* <- VarEK
269:     ST  0,26(4)   assignE: push left
* -> OpE
* -> VarEK
* varE: address of arr
270:    LDA  0,-2(5)   varE: cal address in local
* varE: if ! isAddress load the value
271:     LD  0,0(0)   varE: load value to ac
* <- VarEK
272:     ST  0,27(4)   opE: push left
* -> Const
273:    LDC  0,1(0)   const: load const
* <- Const
274:     LD  1,27(4)   opE: load left
275:    ADD  0,1,0   opE +
* <- Op
276:     LD  1,26(4)   assignE: load left
277:     ST  0,0(1)   assignE: store right to left
* <- assign
* <- Compound
278:    LDA  7,-34(7)   jmp to begin
* While: jump to begin belongs here
255:    JEQ  0,23(7)   jmp to end
* <- While
* <- Compound
279:     LD  7,-1(5)   funD: return
* <- FunD
  3:    LDC  0,280(0)   main: load return address to ac
  4:     ST  0,-1(5)   main: store return address to retFO
  5:    LDC  7,187(0)   jump to main
* End of execution.
280:   HALT  0,0,0   
