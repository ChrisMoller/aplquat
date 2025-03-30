/*
    This file is part of GNU APL, a free implementation of the
    ISO/IEC Standard 13751, "Programming Language APL, Extended"

    mtx Copyright (C) 2024  Dr. C. H. L. Moller

compl    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// https://www.microapl.com/apl_help/ch_020_030_030.htm
// https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions

#include <stdio.h>
#include <math.h>
#include <values.h>

#include "Native_interface.hh"
#include "APL_types.hh"
#include "Shape.hh"
#include "Value.hh"

#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#undef VERSION

#include "../aplquat_config.h"

#include "Quat.hh"

using namespace std;

class NativeFunction;

extern "C" void * get_function_mux(const char * function_name);

enum {
  OP_NONE,				//  0
  OP_PLUS,				//  1 dyadic +
  OP_PLUS_ASSIGN,			//  2 dyadic +=
  OP_MINUS,				//  3 dyadic -
  OP_MINUS_ASSIGN,			//  4 dyadic -=
  OP_TIMES,				//  5 dyadic *
  OP_TIMES_ASSIGN,			//  6 dyadic *=
  OP_DIVIDE,				//  7 dyadic /
  OP_DIVIDE_ASSIGN,			//  8 dyadic /=
  OP_CONJUGATE,				//  9 monadic *
  OP_NORM,				// 10 monadic +
  OP_NEGATE,				// 11 monadic -
  OP_INVERT,				// 12 monadic ~
  OP_EQUAL,				// 13 dyadic ==
  OP_NOT_EQUAL,				// 14 dyadic !=
  OP_FORMAT,				// 15 monadic 'fmt'
  OP_DOT_PRODUCT,			// 16 dyadic 'dot'
  OP_CROSS_PRODUCT,			// 17 dyadic 'cross'
  OP_INTERANGLE				// 18 dyadic 'ang'
};

static bool
close_fun(Cause cause, const NativeFunction * caller)
{
   return true;
}

bool (*close_fun_is_unused)(Cause, const NativeFunction *) = &close_fun;

Fun_signature
get_signature()
{
  return SIG_Z_A_F2_B;
}

static Token
eval_fill_B(Value_P B, const NativeFunction * caller)
{
  fprintf (stderr, "eval_fill_B\n");
  return Token(TOK_APL_VALUE1, Str0(LOC));
}

static Token
eval_fill_AB(Value_P A, Value_P B, const NativeFunction * caller)
{
  fprintf (stderr, "eval_fill_AB\n");
  return Token(TOK_APL_VALUE1, Str0(LOC));
}

static Token
eval_ident_Bx(Value_P B, sAxis x)
{
  fprintf (stderr, "eval_ident_B\n");
  return Token(TOK_APL_VALUE1, Str0(LOC));
}

static Token
eval_XB(Value_P X, Value_P B, const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  return Token(TOK_APL_VALUE1, rc);
}

static Token
eval_B(Value_P B, const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  bool is_okay = false;
  if ((B->get_cfirst ()).is_numeric () && !(*B).is_complex (true)) {
    if (B->is_numeric_scalar()) {
      Shape shape_Z (4);
      rc = Value_P (shape_Z, LOC);
      const Cell &Bv = B->get_cscalar ();
      (*rc).set_ravel_Float (0, Bv.get_real_value ());
      for (int i = 1; i <= 3; i++)
	(*rc).set_ravel_Float (i, 0);
      is_okay = true;
    }
    else {
      const ShapeItem B_count = B->element_count();
      if ((B_count >= 1) && B->get_rank () == 1) {
	switch(B_count) {
	case 1:
	  {	
	    Shape shape_Z (4);
	    rc = Value_P (shape_Z, LOC);
	    const Cell &Bv = B->get_cravel (0);
	    (*rc).set_ravel_Float (0, Bv.get_real_value ());
	    for (int i = 1; i <= 3; i++)
	      (*rc).set_ravel_Float (i, 0);
	    is_okay = true;
	  }
	  break;
	case 3:
	  {
	    Shape shape_Z (4);
	    rc = Value_P (shape_Z, LOC);
	    (*rc).set_ravel_Float (0, 0);
	    for (int i = 0; i < B_count; i++) {
	      const Cell &Bv = B->get_cravel (i);
	      (*rc).set_ravel_Float (i+1, Bv.get_real_value ());
	    }
	    is_okay = true;
	  }
	  break;
	case 4:
	  {
	    Shape shape_Z (4);
	    rc = Value_P (shape_Z, LOC);
	    for (int i = 0; i < B_count; i++) {
	      const Cell &Bv = B->get_cravel (i);
	      (*rc).set_ravel_Float (i, Bv.get_real_value ());
	    }
	    is_okay = true;
	  }
	  break;
	default:
	  MORE_ERROR () <<
	    "Invalid vector argument.  Must be length 1, 2, 3.";
	  SYNTAX_ERROR;
	}
      }
    }
  }
  if (!is_okay) {
    MORE_ERROR () <<
      "Invalid argument.  Must be real numeric scalar or vector.";
    SYNTAX_ERROR;
  }
  else rc->check_value(LOC);

  return Token(TOK_APL_VALUE1, rc);
}

static Token
eval_AXB(Value_P A, Value_P X, Value_P B,
	 const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  return Token(TOK_APL_VALUE1, rc);
}

static Token
eval_AB(Value_P A, Value_P B, const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  double scalar = NAN;
  double vector[3] = {NAN, NAN, NAN};;
  
  if (A->is_numeric_scalar() &&
      !(*A).is_complex (true)) {
    scalar = (A->get_cscalar ()).get_real_value ();
  }
  else {
    MORE_ERROR () <<
      "Invalid left argument.  Must be real scalar";
    SYNTAX_ERROR;
  }
  
  if ((B->get_cfirst ()).is_numeric () && !(*B).is_complex (true)) {
    if (B-> get_rank () == 1 && B->element_count () == 3) {
      for (int i = 0; i < 3; i++) {
	vector[i] = (B->get_cravel (i)).get_real_value ();
      }
    }
    else {
      MORE_ERROR () <<
	"Invalid right argument.  Must be a length-3 vector.";
      SYNTAX_ERROR;
    }
  }
  else {
    MORE_ERROR () <<
      "Invalid right argument.  Must be real numeric vector.";
    SYNTAX_ERROR;
  }

  if (!isnan (scalar) && !isnan (vector[0])) {
    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    (*rc).set_ravel_Float (0, scalar);
    for (int i = 0; i < 3; i++) {
      (*rc).set_ravel_Float (i+1, vector[i]);
    }
    rc->check_value(LOC);
  }

  return Token(TOK_APL_VALUE1, rc);
}

void *
get_function_mux(const char * function_name)
{
  // mandatory
  if (!strcmp(function_name, "get_signature"))
    return reinterpret_cast<void *>(&get_signature);
  if (!strcmp(function_name, "eval_fill_B"))
    return reinterpret_cast<void *>(&eval_fill_B);
  if (!strcmp(function_name, "eval_fill_AB"))
    return reinterpret_cast<void *>(&eval_fill_AB);
  if (!strcmp(function_name, "eval_ident_Bx"))
    return reinterpret_cast<void *>(&eval_ident_Bx);

  // ad hoc
  if (!strcmp(function_name, "eval_B"))
    return reinterpret_cast<void *>(&eval_B);
  if (!strcmp(function_name, "eval_AB"))
    return reinterpret_cast<void *>(&eval_AB);
  if (!strcmp(function_name, "eval_XB"))
    return reinterpret_cast<void *>(&eval_XB);
  if (!strcmp(function_name, "eval_AXB"))
    return reinterpret_cast<void *>(&eval_AXB);

  return 0;
}

