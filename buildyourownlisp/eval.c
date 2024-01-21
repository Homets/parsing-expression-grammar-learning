#include "eval.h"

lval eval_op(lval x, char* op, lval y){
  if (strcmp(op, "+") == 0){return lval_num(x.num + y.num);}
  if (strcmp(op, "-") == 0){return lval_num(x.num + y.num);}
  if (strcmp(op, "*") == 0){return lval_num(x.num + y.num);}
  if (strcmp(op, "/") == 0){
    return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
  }
  if (strcmp(op, "%") == 0){
    return y.num == 0 ? lval_err(LERR_MOD_ZERO) : lval_num(x.num % y.num);
  }
  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t){
  /* If tagged as number return it directly. */ 
  if (strstr(t->tag, "number")){
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }
  
  //The operator is always the second child in an expression
  char* op = t->children[1]->contents;

  //store the first operand
  lval x = eval(t->children[2]);

  int i = 3;
  /* Iterate the remaining children and combining. */
  while (strstr(t->children[i]->tag, "expr")){
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

lval lval_num(long x){
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

lval lval_err(int x){
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

void lval_print(lval v){
  switch(v.type){
    case LVAL_NUM:
      printf("%li", v.num);
      break;
    case LVAL_ERR:
      if (v.err == LERR_DIV_ZERO){
        printf("Error: Divide by Zero");
      }
      if (v.err == LERR_BAD_OP){
        printf("Error: Invalid operator");
      }
      if (v.err == LERR_BAD_NUM){
        printf("Error: Invalid operand");
      }
      if (v.err == LERR_MOD_ZERO){
      printf("Error: Modulo by Zero");
      }
      break;
  }
}


void lval_println(lval v){
  lval_print(v);
  putchar('\n');
}
