#include <stdio.h>      // printf, scanf, etc.
#include <stdlib.h>     // malloc, free, exit
#include <string.h>     // strcmp, strlen, strcpy, etc.
#include <ctype.h>      // isdigit, isalpha, isspace
#include <math.h>       // pow, sqrt, cbrt, log, log10, exp
#include <stdbool.h>    // true, false, bool type

#define MAX_TOKENS 1000

typedef enum {
  TOKEN_NUMBER,
  TOKEN_VARIABLE,
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
} TokenType;

typedef struct {
  TokenType type;
  union{
    double number;
    char operator;
    char *func_name;
    char variable;
  } value;
} Token;

Token tokens[MAX_TOKENS];

void remove_token(Token* tokens_in, int *argc, int index) {
    for (int j = index; j < *argc - 1; j++) {
        tokens_in[j] = tokens_in[j + 1];
    }
    (*argc)--;
}

Token tokenize(char *arg) {
  Token t;
  if(atof(arg)){
    t.type = TOKEN_NUMBER;
    t.value.number = atof(arg);
    return t;
  }
  else if(arg[0] <= 90 && arg[0] >= 65){
    t.type = TOKEN_VARIABLE;
    t.value.variable = arg[0];
    return t;
  }
  else if(strcmp(arg, "+")||strcmp(arg, "-")||strcmp(arg, "/")||strcmp(arg, "*")||strcmp(arg, "^")){
    t.type = TOKEN_OPERATOR;
    t.value.operator = arg[0];
    return t;
  }
  else if(strcmp(arg, "(")){
    t.type = TOKEN_RPAREN;
    return t;
  }
  else if(strcmp(arg, ")")){
    t.type = TOKEN_LPAREN;
    return t;
  }
  else{
    t.type = TOKEN_FUNCTION;
    t.value.func_name = arg;
    return t; 
  }
}

void output_tokens(Token* tokens_in, int argc){
  for(int i = 0; i < argc; i++){
    if(tokens_in[i].type == TOKEN_NUMBER){
      printf("%f ", tokens_in[i].value.number);
    }
    if(tokens_in[i].type == TOKEN_FUNCTION){
      printf("%s ", tokens_in[i].value.func_name);
    }  
    if(tokens_in[i].type == TOKEN_VARIABLE){
      printf("%c ", tokens_in[i].value.variable);
    }  
    if(tokens_in[i].type == TOKEN_OPERATOR){
      printf("%c ", tokens_in[i].value.operator);
    }  
    if(tokens_in[i].type == TOKEN_RPAREN){
      printf("%c ", '(');
    } 
    if(tokens_in[i].type == TOKEN_LPAREN){
      printf("%c ", ')');
    }  
  }
}

char operator_prescedence[5] = {'^','*', '/', '+', '-'};

double eval_simple(Token* tokens_in, int argc){
  double final_number = 0;
  double result = 0;
  for(int pass = 0; pass < 5; pass++){
    for(int i = 0; i < argc; i++){
      result = 0; 
      if (tokens_in[i].type == TOKEN_OPERATOR) {
        if (operator_prescedence[pass] == tokens_in[i].value.operator) {
          switch(operator_prescedence[pass]) {
            case '^': tokens_in[i-1].value.number = pow(tokens_in[i-1].value.number, tokens_in[i+1].value.number); break;
            case '*': tokens_in[i-1].value.number *= tokens_in[i+1].value.number; break;
            case '/': tokens_in[i-1].value.number /= tokens_in[i+1].value.number; break;
            case '+': tokens_in[i-1].value.number += tokens_in[i+1].value.number; break;
            case '-': tokens_in[i-1].value.number -= tokens_in[i+1].value.number; break;
          }
          // remove operator and right number
          remove_token(tokens_in, &argc, i);   // remove operator
          remove_token(tokens_in, &argc, i);   // remove right number
          i--; // back up so loop doesn’t skip
        }
      }
      output_tokens(tokens_in,argc);
      printf("\n");
    }
  }
  double total = 0.0f;
  for(int i = 0; i < argc; i++){
    if(tokens[i].type == TOKEN_NUMBER){
      total += tokens[i].value.number;
    }
  }
  return total;
}

int main(int argc, char *argv[]){
  for(int i = 1; i < argc; i++){
    tokens[i-1] = tokenize(argv[i]);
  }
  printf("%f\n", eval_simple(tokens, argc));
}

/* scope
 - single variable
 - complex fractions
 - square and cube roots
 - logs
 - exponents
 - natural logs
*/
