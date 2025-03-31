dnl generator for xml-kwds.m4
define(`RQ',`changequote(<,>)dnl`
'changequote`'')
dnl start at 1000 just so we don't have to with a null pointer
define(`offset', 0)dnl
define(`cnt',    offset)dnl
define(`xinc', `define(`$1',incr($1))')dnl
define(`upcase', `translit($1, `a-z', `A-Z')')dnl
define(`entry', ``#'define `KEYWORD_'upcase($1) "$1"
`#'define `OP_'upcase($1) cnt xinc(`cnt')
divert(1)  `{' `KEYWORD_'upcase($1), `OP_'upcase($1), $2, $3, $4 `},'
divert(0)'
)dnl

`/********* DON'RQ()`T MODIFY THIS FILE! ********/'
`/**** Make all changes in kwds.m4. ****/'

#ifndef KWDS_H
#define KWDS_H

entry(none,	     "",      "⍬",  "No-op")
entry(plus, 	     "pl",    "+",  "Addition")
entry(plus_assign,   "pa",    "+=", "Add-assign")
entry(minus,	     "mi",    "-",  "Subtraction")
entry(minus_assign,  "pa",    "-=", "Subtraction-assign")
entry(times,	     "ti",    "×",  "Multiplication")
entry(times_assign,  "ta",    "×=", "Multiplication-assign")
entry(divide,        "di",    "÷",  "Division")
entry(divide_assign, "da",    "÷=", "Division-assign")
entry(conjugate,     "co",    "*",  "Conjugation")
entry(norm,          "no",    "+",  "Norm (magnitude)")
entry(negate,        "neg",   "-",  "Negate")
entry(invert,        "inv",   "÷",  "Invert")
entry(equal,         "eq",    "==", "Equal")
entry(not_equal,     "ne",    "!=", "Not equal")
entry(format,        "fmt",   "⍕",  "Format")
entry(dot_product,   "dot",   "∘",  "Dot product")
entry(cross_product, "cross", "○",  "Cross product")
entry(interangle,    "ang",   "<",  "Internal angle")
entry(xform,         "xform", "><", "Transform")

`typedef struct {
  const char *keyword;
  int   opcode;
  const char *abbr;
  const char *symbol;
  const char *desc;
} keyword_s;

keyword_s keywords[] = {'
undivert
`};
int nr_keys =' eval(cnt - offset)`;'
#endif /* KWDS_H*/
