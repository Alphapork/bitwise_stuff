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
int not_null_token = 1;
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
  case '-':
  case '+':
  case '|':
  case '^':
    tokenstream->kind = TOKEN_OPERATOR_LVL_2;
    tokenstream->operator = *charstream;
    charstream++;
    break;
  case ' ': while(*charstream == ' ') charstream++; next_token(); break;
  case 0: not_null_token = 0; return 0;
  default: printf("ERROR\n"); break;
  }
  return 1;
}

char sum_string[300]; //TODO: implement support for expression of arbitrary size.
char term_string[100];
char factor_string[50];

void parse_factor(void) {
  if ((tokenstream->kind == TOKEN_OPERATOR_LVL_0) ||((tokenstream->kind == TOKEN_OPERATOR_LVL_2) && (tokenstream->operator == '-')) ) {
    char temp = tokenstream->operator;
    next_token();
    sprintf(factor_string, "(%c %d)", temp, tokenstream->val);
  } else {
    sprintf(factor_string, "%d", tokenstream->val);
  }
}

int parse_term(void) {
  parse_factor();
  next_token();
  if (tokenstream->kind == TOKEN_OPERATOR_LVL_1) {
    char temp[50];
    sprintf(temp, "(%c %s",tokenstream->operator, factor_string);
    next_token();
    parse_factor();
    sprintf(term_string, "%s %s)", temp, factor_string);
    return 1;
  } else {
    sprintf(term_string, "%s", factor_string);
    return 0;
  }
}

void parse_sum(void) {
  next_token();
  parse_term();
  sprintf(sum_string, "%s", term_string);
  next_token();
  while (tokenstream->kind  == TOKEN_OPERATOR_LVL_2) {
    char temp[300];
    char op = tokenstream->operator;
    sprintf(temp, "%s", sum_string);
    next_token();
    int twofactors = parse_term();
    sprintf(sum_string, "(%c %s %s)",op, temp, term_string);
    if (twofactors)
      next_token();
  }
}

void start_parsing(void) {
  char test_char_stream[] = "12*-34 + 45/56 + -25";
  charstream = test_char_stream;
  Token test_token;
  tokenstream = &test_token;
  while (not_null_token) {
    parse_sum();
  }
  
  printf("%s\n", sum_string); 
}










int main(int argc, char **argv)
{
  start_parsing();
}
