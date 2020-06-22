#pragma once
#ifndef _CODE_H
#define _CODE_H

/* 保存当前指令的寄存器 */
#define pc 7 // program counter程序计数器 tiny已有
#define gp 6 //global pointer “全局指针”指向（全局）变量存储的内存底部
#define fp 5 //函数作用域和过程调用
#define tp 4 //左值和右值计算的缓存temp
#define ac 0 //accumulator累加器 tiny已有 通常结果存放在ac中
#define ac1 1 //2nd accumulator tiny已有

/* emitRO emits a register-only TM instruction寄存器指令
 * op = opcode
 * r = target register
 * s,t = source register
 * c = print if TraceCode is TRUE
 */
void emitRO(char* op, int r, int s, int t, char* c);

/* emitRM emits a register-memory TM instruction寄存器存储
 * op = opcode
 * r = target register
 * d = offset
 * s = base register
 * c = print if TraceCode is TRUE
 */
void emitRM(char* op, int r, int d, int s, char* c);

/* emitSkip 跳过howMany条指令
 * 返回当前指令地址
 */
int emitSkip(int howMany);

/* emitBackup 倒退到之前跳过的一条指令地址
 */
void emitBackup(int loc);

/* emitRestore 恢复
 */
void emitRestore(void);

/* emitComment 打印一行注释
 */
void emitComment(char* c);

/* emitRM_Abs 当emit一条RM指令时将绝对引用转换为相对pc的引用
 * op = opcode
 * r = target register
 * a = absolute location in memory
 * c = print if TraceCode is TRUE
 */
void emitRM_Abs(char* op, int r, int a, char* c);

#endif //_CODE_H