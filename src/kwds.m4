dnl generator for xml-kwds.m4
define(`RQ',`changequote(<,>)dnl`
'changequote`'')
dnl start at 1000 just so we don't have to with a null pointer
define(`offset', 0)dnl
define(`cnt',    offset)dnl
define(`xinc', `define(`$1',incr($1))')dnl
define(`upcase', `translit($1, `a-z', `A-Z')')dnl
define(`entry', ``#'define `OPERATION_'upcase($1) "$1"
`#'define `OPCODE_'upcase($1) cnt xinc(`cnt')
divert(1)  `{' `OPERATION_'upcase($1), `OPCODE_'upcase($1), $2, $3, $4 `},'
divert(0)'
)dnl

`/********* DON'RQ()`T MODIFY THIS FILE! ********/'
`/**** Make all changes in kwds.m4. ****/'

#ifndef KWDS_H
#define KWDS_H

entry(none,	     "",       "⍬",  "No-op")
entry(plus, 	     "pl",     "+",  "Addition")
entry(minus,	     "mi",     "-",  "Subtraction")
entry(times,	     "ti",     "×",  "Multiplication")
entry(divide,        "di",     "÷",  "Division")
entry(conjugate,     "co",     "*",  "Conjugation")
entry(magnitude,     "mag",    "+",  "Norm")
entry(negate,        "neg",    "-",  "Negate")
entry(invert,        "inv",    "÷",  "Reciprocal")
entry(equal,         "eq",     "=",  "Equal")
entry(not_equal,     "ne",     "≠",  "Not equal")
entry(format,        "fmt",    "⍕",  "Format  (not yet implemented)")
entry(dot_product,   "dot",    "∘",  "Dot product")
entry(cross_product, "cross",  "○",  "Cross product")
entry(interangle,    "ang",    "<",  "Internal angle")
entry(rotate,        "ro",     "⌽",  "Rotate")
entry(radians,       "rad",    "",   "Radians")
entry(degrees,       "deg",    "",   "Degrees")

`typedef struct {
  const char *keyword;
  int   opcode;
  const char *abbr;
  const char *symbol;
  const char *desc;
} operation_s;

operation_s operations[] = {'
undivert
`};
int nr_operations =' eval(cnt - offset)`;'
#endif /* KWDS_H*/
