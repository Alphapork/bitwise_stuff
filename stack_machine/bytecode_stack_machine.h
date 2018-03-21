#ifndef BYTECODE_H
#define BYTECODE_H
enum bytecodes {
  HALT = 0,
  SUB, // 1
  ADD, // 2
  OR, // 3
  XOR, // 4
  MUL, // 5
  DIV, // 6
  MOD, // 7
  RSHIFT, // 8
  LSHIFT, // 9
  AND, // 10
  NEG, // 11
  NOT, // 12
  LIT // 13
};

int32_t vm_exec(const uint8_t *code);
#endif
