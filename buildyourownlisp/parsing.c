#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpc.h"
#include <editline/readline.h>
#include <editline/history.h>
#include "eval.c"
int main(int argc, char** argv){
  
  /* Create the parser */
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new ("expr");
  mpc_parser_t* Lispy = mpc_new("lispy");

  /* Describe the Language */
  mpca_lang(MPCA_LANG_DEFAULT,
      "                                                      \
      number   : /-?[0-9]+/ ;                                \
      operator : '+' | '-' | '*' | '/' | '%' ;                     \
      expr     : <number> | '(' <operator> <expr>+ ')' ;     \
      lispy    : /^/ <operator> <expr>+ /$/ ;                \
      ",
  Number, Operator, Expr, Lispy);
    
  puts("Lispy version 0.0.0.1");
  puts("Press Ctrl+c to Exit\n");

  /* REPL (Read Evaluate Print Loop*/
  while(1){
      char* input = readline("lispy> ");
      if (!(strcmp(input,"") == 0)){
        add_history(input);
      }

      /* Parse user input */
      mpc_result_t r;
      if (mpc_parse("<stdin>", input, Lispy, &r)){
        lval result = eval(r.output);
        lval_println(result);
        mpc_ast_delete(r.output);
      } else {
        mpc_err_print(r.output);
        mpc_err_delete(r.output);
      }

      free(input);
    }
    
  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  
  return 0;


}
