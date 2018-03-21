#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
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
      return -tokenstream->val;
    } else if (tokenstream->operator == '~') {
      next_token();
      return ~tokenstream->val;
    }
  } else {
    return (int)tokenstream->val;
  }
}

int parse_term(void) {
  int result = parse_factor();
  next_token();
  while (tokenstream->kind == TOKEN_OPERATOR_LVL_1) {
    switch (tokenstream->operator) {
    case '*': next_token(); result *=parse_factor(); next_token(); break;
    case '/': next_token(); result /=parse_factor(); next_token(); break;
    case '%': next_token(); result %=parse_factor(); next_token(); break;
    case '&': next_token(); result &=parse_factor(); next_token(); break;
    case '>': next_token(); result >>=parse_factor(); next_token(); break;
    case '<': next_token(); result <<=parse_factor(); next_token(); break;
    }
  }
  return result;
}

int parse_sum(void) {
  next_token();
  int result = parse_term();
  while (tokenstream->kind  == TOKEN_OPERATOR_LVL_2) {
    switch (tokenstream->operator) {
    case '+': next_token(); result +=parse_term(); break;
    case '-': next_token(); result -=parse_term(); break;
    case '|': next_token(); result |=parse_term(); break;
    case '^': next_token(); result ^=parse_term(); break;
    }
  }
  return result;
}

void start_parsing(void) {
  char test_char_stream[] = "8>>1*-4+1+1+14+2<<1";
  charstream = test_char_stream;
  Token test_token;
  tokenstream = &test_token;

  int result =  parse_sum();  
  printf("%d\n", result); 
}


int main(int argc, char **argv)
{
  start_parsing();
}
