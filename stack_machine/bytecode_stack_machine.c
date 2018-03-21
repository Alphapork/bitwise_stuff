#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "bytecode_stack_machine.h"
#define POP()					\
    (*--top)

#define PUSH(x) \
    (*top++ = (x))

#define POPS(n) \
    assert(top - stack >= (n))

#define PUSHES(n) \
    assert(top + (n) <= stack + MAX_STACK)




int32_t vm_exec(const uint8_t *code) {
    enum { MAX_STACK = 1024 };
    int32_t stack[MAX_STACK];
    int32_t *top = stack;
    for (;;) {
        uint8_t op = *code++;
        switch (op) {
        case SUB: {
            POPS(2);
            // Note the stack's operand order!
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left - right);
            break;
        }
	case ADD: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left + right);
            break;
        }
	case OR: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left | right);
            break;
        }
	case XOR: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left ^ right);
            break;
        }
	case MUL: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left * right);
            break;
        }
	case DIV: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left / right);
            break;
        }
	case MOD: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left % right);
            break;
        }
	case RSHIFT: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left >> right);
            break;
        }
	case LSHIFT: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left << right);
            break;
        }
	case AND: {
            POPS(2);
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left & right);
            break;
        }
	case NEG: {
            POPS(1);
            int32_t operand = POP();
            PUSHES(1);
            PUSH(-operand);
            break;
        }
	case NOT: {
            POPS(1);
            int32_t operand = POP();
            PUSHES(1);
            PUSH(~operand);
            break;
        }
        case LIT:
            PUSHES(1);
            PUSH(*(uint32_t *)code);
            code += sizeof(uint32_t);
            break;
        case HALT:
            POPS(1);
            return POP();
        default:
            printf("vm_exec: illegal opcode");
            return 0;
        }
    }
}

/*int main(void)
{
  const uint8_t testcode[] = {13,1,0,0,0,13,4,0,0,0,2,0}; // 1 + 4
  int32_t result = vm_exec(testcode);
  printf("%d", result);
  }*/
