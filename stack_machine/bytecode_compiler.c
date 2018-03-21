#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "bytecode_stack_machine.h"
typedef enum TokenKind {
  TOKEN_INT = 128,
  TOKEN_OPERATOR_LVL_0,
  TOKEN_OPERATOR_LVL_1,
  TOKEN_OPERATOR_LVL_2
}TokenKind;

typedef struct Token {
  TokenKind kind;
  union {
    uint64_t val;
    char operator;
  };

} Token;
Token *tokenstream; 
char *charstream;
uint8_t *bytecode;

int next_token(void) {
  switch(*charstream) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':{
    uint64_t value = 0;
    while (isdigit(*charstream)) {
      value *= 10;
      value += *charstream - '0';
      charstream++;
    }
    tokenstream->kind = TOKEN_INT;
    tokenstream->val = value;
    break;
  }
  case '~':
    tokenstream->kind = TOKEN_OPERATOR_LVL_0;
    tokenstream->operator = *charstream;
    charstream++;
    break;    
  case '*':
  case '/':
  case '%':
  case '&':
    tokenstream->kind = TOKEN_OPERATOR_LVL_1;
    tokenstream->operator = *charstream;
    charstream++;
    break;
  case '>':
    charstream++;
    if(*charstream == '>') {
    tokenstream->kind = TOKEN_OPERATOR_LVL_1;
    tokenstream->operator = *charstream;
    charstream++;  
    } else {
      printf("ERROR\n");
    }
    break;
  case '<':
    charstream++;
    if(*charstream == '<') {
    tokenstream->kind = TOKEN_OPERATOR_LVL_1;
    tokenstream->operator = *charstream;
    charstream++;  
    } else {
      printf("ERROR\n");
    }
    break;
  case '-':
  case '+':
  case '|':
  case '^':
    tokenstream->kind = TOKEN_OPERATOR_LVL_2;
    tokenstream->operator = *charstream;
    charstream++;
    break;
  case ' ': while(*charstream == ' ') charstream++; next_token(); break;
  case 0:  return 0;
  default: printf("ERROR\n"); break;
  }
  return 1;
}



int parse_factor(void) {
  if ((tokenstream->kind == TOKEN_OPERATOR_LVL_0) ||((tokenstream->kind == TOKEN_OPERATOR_LVL_2) && (tokenstream->operator == '-')) ) {
    if (tokenstream->operator == '-') {
      next_token();
      *bytecode++ = LIT;
      *(uint32_t *)bytecode = (uint32_t)tokenstream->val;
      bytecode += sizeof(uint32_t);
      *bytecode++ = NEG;
      return -tokenstream->val;
    } else if (tokenstream->operator == '~') {
      next_token();
      *bytecode++ = LIT;
      *(uint32_t *)bytecode = (uint32_t)tokenstream->val;
      bytecode += sizeof(uint32_t);
      *bytecode++ = NOT;
      return ~tokenstream->val;
    }
  } else {
    *bytecode++ = LIT;
    *(uint32_t *)bytecode = (uint32_t)tokenstream->val;
    bytecode += sizeof(uint32_t);
    return (int)tokenstream->val;
  }
}

int parse_term(void) {
  int result = parse_factor();
  next_token();
  while (tokenstream->kind == TOKEN_OPERATOR_LVL_1) {
    switch (tokenstream->operator) {
    case '*': next_token(); result *=parse_factor(); *bytecode++ = MUL; next_token(); break;
    case '/': next_token(); result /=parse_factor(); *bytecode++ = DIV; next_token(); break;
    case '%': next_token(); result %=parse_factor(); *bytecode++ = MOD; next_token(); break;
    case '&': next_token(); result &=parse_factor(); *bytecode++ = AND; next_token(); break;
    case '>': next_token(); result >>=parse_factor(); *bytecode++ = RSHIFT; next_token(); break;
    case '<': next_token(); result <<=parse_factor(); *bytecode++ = LSHIFT; next_token(); break;
    }
  }
  return result;
}

int parse_sum(void) {
  next_token();
  int result = parse_term();
  while (tokenstream->kind  == TOKEN_OPERATOR_LVL_2) {
    switch (tokenstream->operator) {
    case '+': next_token(); result +=parse_term(); *bytecode++ = ADD; break;
    case '-': next_token(); result -=parse_term(); *bytecode++ = SUB; break;
    case '|': next_token(); result |=parse_term(); *bytecode++ = OR; break;
    case '^': next_token(); result ^=parse_term(); *bytecode++ = XOR; break;
    }
  }
  return result;
}

void start_parsing(void) {
  char test_char_stream[] = "1*-2  + 2<<1";
  charstream = test_char_stream;
  Token test_token;
  tokenstream = &test_token;
  uint8_t testingcode[1024];
  bytecode = testingcode;
  int result =  parse_sum();  
  printf("interpreter result: %d\n", result);
  *bytecode = 0;
  bytecode = testingcode;
}


int main(int argc, char **argv)
{
  start_parsing();
  int32_t vmresult = vm_exec(bytecode);
  printf("vm result via bytecode: %d\n\n", vmresult);
  for (int i = 0; i < 30; i++) { // for inspecting the bytecode
    printf("%d  ",bytecode[i]);
  }
  
}
