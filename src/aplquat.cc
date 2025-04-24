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
#include <search.h>
#include <strings.h>


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
static bool is_quat (Value_P V);
static Value_P create_quat (int c_mode, Value_P A, Value_P B);

#include "kwds.h"
typedef struct {
  const char *key;
  void *loc;
} dictionary_ety_s;
dictionary_ety_s *dictionary = NULL;
int dictionary_max = 0;
int dictionary_nxt = 0;
#define DICTIONARY_INCR 64

enum {
  A_IS_QUAT,
  B_IS_QUAT
};

enum {
  A_IS_QUAT_BIT = 1 << A_IS_QUAT,
  B_IS_QUAT_BIT = 1 << B_IS_QUAT
};

enum {
  OP_SCALAR_SCALAR = 0,
  OP_SCALAR_QUAT   = B_IS_QUAT_BIT,
  OP_QUAT_SCALAR   = A_IS_QUAT_BIT,
  OP_QUAT_QUAT     = A_IS_QUAT_BIT | B_IS_QUAT_BIT
};

#define R2D(r)  (((r) / M_PI) * 180.0)
#define D2R(d)  (((d) / 180.0) * M_PI)

enum {
  CONSTRUCT_RAW,
  CONSTRUCT_RADIANS,
  CONSTRUCT_DEGREES
};

static int
set_mode (Value_P A, Value_P B)
{
  int mode = 0;
  mode |= is_quat (A) ? A_IS_QUAT_BIT : 0;
  mode |= is_quat (B) ? B_IS_QUAT_BIT : 0;
  return mode;
}

static int
dictionary_search_compare (const void *a, const void *b)
{
  const char *av = (char *)a;
  const char *bv = (*(dictionary_ety_s *)b).key;
  return strcasecmp (av, bv);
}

static operation_s *
lookup_kwd (const char *kwd)
{
  operation_s *op_data = NULL;
  void *res = bsearch (kwd, dictionary, dictionary_nxt,
			sizeof (dictionary_ety_s), dictionary_search_compare);
  if (res) {
    dictionary_ety_s *ety = (dictionary_ety_s *)res;
    op_data = ((operation_s*)(ety->loc));
  }
  return op_data;
}

static Quat
quatify (Value_P V)
{
  double vec[4];
  for (int i = 0; i < 4; i++) 
    vec[i] = (V->get_cravel (i)).get_real_value ();
  return Quat (vec);
}

static Value_P
valify (Quat V)
{
  double *v = V.qvec ();
  Shape shape_Z (4);
  auto rc = Value_P (shape_Z, LOC);
  for (int i = 0; i < 4; i++)
    (*rc).set_ravel_Float (i, v[i]);
  return rc;
}

static bool
is_quat (Value_P V)
{
  bool rc = false;

  if ((V->get_cfirst ()).is_numeric () && !(*V).is_complex (true)) {
    if (V->get_rank () == 1 && V->element_count () == 4)
      rc = true;
  }
  
  return rc;
}

static bool
is_real_scalar (Value_P V)
{
  bool rc = false;

  if ((V->get_cfirst ()).is_numeric () && !(*V).is_complex (true)) {
    if (V->get_rank () == 0 && V->element_count () == 1)
      rc = true;
  }
  
  return rc;
}

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

static Value_P
do_norm (Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (B)) {
    double S = +quatify (B);
    rc = FloatScalar (S, LOC);
  }

  return rc;  
}

static Value_P
do_negate (Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (B)) {
    Quat S = -quatify (B);
    return valify (S);
  }

  return rc;  
}

static Value_P
do_conjugate (Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (B)) {
    Quat S = *quatify (B);
    return valify (S);
  }

  return rc;  
}

static Value_P
do_invert (Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (B)) {
    if (0.0 == +quatify (B)) {		// get norm
      MORE_ERROR () << "Null quat.";
      DOMAIN_ERROR;
    }
    Quat S = ~quatify (B);
    return valify (S);
  }

  return rc;  
}

static Token
eval_XB(Value_P X, Value_P B, const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  int op = OPCODE_NONE;
  
  if (X->is_numeric_scalar () &&
      !(*X).is_complex (true)) {
    op = (*X).get_sole_integer ();
    cout << "num = " << op << endl;
  }
  else if (X->is_char_array()) {
    const UCS_string  ustr = X->get_UCS_ravel();
    UTF8_string which (ustr);
    operation_s *fnd = lookup_kwd (which.c_str ());
    if (fnd) {
      switch (fnd->opcode) {
      case OPCODE_PLUS:
      case OPCODE_MAGNITUDE:
	rc = do_norm (B);
	break;
      case OPCODE_NEGATE:
      case OPCODE_MINUS:
	rc = do_negate (B);
	break;
      case OPCODE_CONJUGATE:
      case OPCODE_TIMES:
	rc = do_conjugate (B);
	break;
      case OPCODE_INVERT:
      case OPCODE_DIVIDE:
	rc = do_invert (B);
	break;
      case OPCODE_EQUAL:
      case OPCODE_NOT_EQUAL: 
      case OPCODE_DOT_PRODUCT:
      case OPCODE_CROSS_PRODUCT:
      case OPCODE_INTERANGLE:
      case OPCODE_ROTATE:
      case OPCODE_FORMAT:
	MORE_ERROR () <<
	  "No monadic use of operator: " << which.c_str ();
	SYNTAX_ERROR;
	break;
      }
    }
    else {
      MORE_ERROR () <<
	"Unknown operator: " << which.c_str ();
      SYNTAX_ERROR;
    }
  }

  rc->check_value(LOC);
  return Token(TOK_APL_VALUE1, rc);
}

static Token
eval_B (Value_P B, const NativeFunction * caller)
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
	case 1:				// set scalar s 0 0 0
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
	  {				// set vec 0 i j k
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
	  {				// set whole thing
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
	    "Invalid vector argument.  Must be length 1, 3, or 4.";
	  SYNTAX_ERROR;
	}
      }
    }
  }
  if (!is_okay) {
#if 1
    int prev_opcode = -1;
    for (int i = 1; i < dictionary_nxt; i++) {
      dictionary_ety_s *ety = &dictionary[i];
      operation_s *operation = (operation_s*)(ety->loc);
      if (operation->opcode != OPCODE_NONE &&
	  operation->opcode != prev_opcode) {
	int padding = 16 - strlen (operation->keyword);
	//                 c   a   s    k
	fprintf (stderr, "%d\t'%s'\t'%s'\t'%s'%*s%s\n",
		 operation->opcode,
		 operation->abbr,
		 operation->symbol,
		 operation->keyword,
		 padding,
		 " ",
		 operation->desc
		 );
	prev_opcode = operation->opcode;
      }
    }
    fprintf (stderr,
	     "\nUse any of the first four columns as the index value \
to get the\ncorresponding operation.\n");
#else
    MORE_ERROR () <<
      "Invalid argument.  Must be real numeric scalar or vector.";
    SYNTAX_ERROR;
#endif
  }
  else rc->check_value(LOC);

  return Token(TOK_APL_VALUE1, rc);
}

static Value_P
do_plus (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
    {
      double Av = (A->get_cravel (0)).get_real_value ();
      double Bv = (B->get_cravel (0)).get_real_value ();
      double R = Av + Bv;
      return FloatScalar (R, LOC);
    }
    break;
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Mismatched arguments.";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      Quat S = Aq + Bq;
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_minus (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
    {
      double Av = (A->get_cravel (0)).get_real_value ();
      double Bv = (B->get_cravel (0)).get_real_value ();
      double R = Av - Bv;
      return FloatScalar (R, LOC);
    }
    break;
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Mismatched arguments.";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      Quat S = Aq - Bq;
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_times (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
    {
      if (is_real_scalar (A) && is_real_scalar (B)) {
	double Av = (A->get_cravel (0)).get_real_value ();
	double Bv = (B->get_cravel (0)).get_real_value ();
	double R = Av * Bv;
	return FloatScalar (R, LOC);
      }
    }
    break;
  case OP_SCALAR_QUAT:
    {
      if (is_real_scalar (A)) {
	double Av = (A->get_cravel (0)).get_real_value ();
	Quat Bq = quatify (B);
	Quat S = Bq * Av;
	return valify (S);
      }
    }
    break;
  case OP_QUAT_SCALAR:
    {
      if (is_real_scalar (B)) {
	Quat Aq = quatify (A);
	double Bv = (B->get_cravel (0)).get_real_value ();
	Quat S = Aq * Bv;
	return valify (S);
      }
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      Quat S = Aq * Bq;
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_divide (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
    {
      if (is_real_scalar (A) && is_real_scalar (B)) {
	double Bv = (B->get_cravel (0)).get_real_value ();
	if (0.0 == Bv) {
	  MORE_ERROR () << "Zero divide.";
	  DOMAIN_ERROR;
	}
	double Av = (A->get_cravel (0)).get_real_value ();
	double R = Av / Bv;
	return FloatScalar (R, LOC);
      }
    }
    break;
  case OP_SCALAR_QUAT:
    {
      if (is_real_scalar (A)) {
	if (0.0 == +quatify (B)) {		// get norm
	  MORE_ERROR () << "Null quat.";
	  DOMAIN_ERROR;
	}
	Quat Bq = quatify (B);
	double Av = (A->get_cravel (0)).get_real_value ();
	Quat S = ~Bq * Av;
	return valify (S);
      }
    }
    break;
  case OP_QUAT_SCALAR:
    {
      if (is_real_scalar (B)) {
	double Bv = (B->get_cravel (0)).get_real_value ();
	if (0.0 == Bv) {
	  MORE_ERROR () << "Zero divide.";
	  DOMAIN_ERROR;
	}
	Quat Aq = quatify (A);
	Quat S = Aq / Bv;
	return valify (S);
      }
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Bq = quatify (B);
      if (0.0 == +quatify (B)) {		// get norm
	MORE_ERROR () << "Null quat.";
	DOMAIN_ERROR;
      }
      Quat Aq = quatify (A);
      Quat S = Aq / Bq;
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_equal (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      bool S = Aq == Bq;
      return IntScalar ((int)S, LOC);
    }
    break;
  }

  return rc;  
}

static Value_P
do_not_equal (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      bool S = Aq != Bq;
      return IntScalar ((int)S, LOC);
    }
    break;
  }

  return rc;  
}

static Value_P
do_dot (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      double S = Aq.qdot (Bq);
      return FloatScalar (S, LOC);
    }
    break;
  }

  return rc;  
}

static Value_P
do_cross (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      double *S = Aq.qcross (Bq);
      Shape shape_Z (3);
      auto rc = Value_P (shape_Z, LOC);
      for (int i = 0; i < 3; i++)
	(*rc).set_ravel_Float (i, S[i]);
      return rc;
    }
    break;
  }

  return rc;  
}

static Value_P
do_ang (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      Quat S = Aq.qang (Bq);
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_rotate (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  switch (set_mode (A, B)) {
  case OP_SCALAR_SCALAR:
  case OP_SCALAR_QUAT:
  case OP_QUAT_SCALAR:
    {
      MORE_ERROR () <<
	"Invalid arguments.  Must be quaternions";
      SYNTAX_ERROR;
    }
    break;
  case OP_QUAT_QUAT:
    {
      Quat Aq = quatify (A);
      Quat Bq = quatify (B);
      Quat S = Aq.qrot (Bq);
      return valify (S);
    }
    break;
  }

  return rc;  
}

static Value_P
do_radians_construct (Value_P A, Value_P B)
{
  Value_P rc = create_quat (CONSTRUCT_RADIANS, A, B);
  return rc;
}

static Value_P
do_degrees_construct (Value_P A, Value_P B)
{
  Value_P rc = create_quat (CONSTRUCT_DEGREES, A, B);
  return  rc;
}

static Token
eval_AXB(Value_P A, Value_P X, Value_P B,
	 const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  int op = OPCODE_NONE;
  
  if (X->is_numeric_scalar () &&
      !(*X).is_complex (true)) {
    op = (*X).get_sole_integer ();
    cout << "num = " << op << endl;
  }
  else if (X->is_char_array()) {
    const UCS_string  ustr = X->get_UCS_ravel();
    UTF8_string which (ustr);
    operation_s *fnd = lookup_kwd (which.c_str ());
    if (fnd) {
      switch (fnd->opcode) {
      case OPCODE_PLUS:
      case OPCODE_MAGNITUDE:
	rc = do_plus (A, B);
	break;
      case OPCODE_NEGATE:
      case OPCODE_MINUS:
	rc = do_minus (A, B);
	break;
      case OPCODE_CONJUGATE:
      case OPCODE_TIMES:
	rc = do_times (A, B);
	break;
      case OPCODE_DIVIDE:
      case OPCODE_INVERT:
	rc = do_divide (A, B);
	break;
      case OPCODE_EQUAL:
	rc = do_equal (A, B);
	break;
      case OPCODE_NOT_EQUAL: 
	rc = do_not_equal (A, B);
	break;
      case OPCODE_DOT_PRODUCT:
	rc = do_dot (A, B);
	break;
      case OPCODE_CROSS_PRODUCT:
	rc = do_cross (A, B);
	break;
      case OPCODE_INTERANGLE:
	rc = do_ang (A, B);
	break;
      case OPCODE_ROTATE:
	rc = do_rotate (A, B);
	break;
      case OPCODE_RADIANS:
	rc = do_radians_construct (A, B);
	break;
      case OPCODE_DEGREES:
	rc = do_degrees_construct (A, B);
	break;
      case OPCODE_FORMAT:
	MORE_ERROR () <<
	  "No dyadic use of operator: " << which.c_str ();
	SYNTAX_ERROR;
	break;
      }
    }
    else {
      MORE_ERROR () <<
	"Unknown operator: " << which.c_str ();
      SYNTAX_ERROR;
    }
  }

  rc->check_value(LOC);
  return Token(TOK_APL_VALUE1, rc);
}

static Value_P
create_quat (int c_mode, Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  double scalar = NAN;
  double vector[3] = {NAN, NAN, NAN};
  double sa, ca;
  
  if (A->is_numeric_scalar() &&
      !(*A).is_complex (true)) {
    scalar = (A->get_cscalar ()).get_real_value ();
    switch (c_mode) {
    case CONSTRUCT_RAW:
      ca = scalar;
      sa = 1.0;
      break;
    case CONSTRUCT_RADIANS:
      {
	ca = cos (scalar/2.0);
	sa = sin (scalar/2.0);
      }
      break;
    case CONSTRUCT_DEGREES:
      {
	ca = cos (D2R (scalar)/2.0);
	sa = sin (D2R (scalar)/2.0);
      }
      break;
    }
  }
  else {
    MORE_ERROR () <<
      "Invalid left argument.  Must be real scalar";
    SYNTAX_ERROR;
  }
  
  if ((B->get_cfirst ()).is_numeric () && !(*B).is_complex (true)) {
    bool is_non_zero = false;
    if (B->get_rank () == 1 && B->element_count () == 3) {
      for (int i = 0; i < 3; i++) {
	vector[i] = (B->get_cravel (i)).get_real_value ();
	if (0.0 != vector[i]) is_non_zero = true;
      }
    }
    else {
      MORE_ERROR () <<
	"Invalid right argument.  Must be a length-3 vector.";
      SYNTAX_ERROR;
    }
    if (!is_non_zero) {
      MORE_ERROR () <<
	"Invalid right argument.  Must not be a null vector.";
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
    (*rc).set_ravel_Float (0, ca);
    double m2 = 0.0;
    if (c_mode == CONSTRUCT_RADIANS ||
	c_mode == CONSTRUCT_DEGREES) {
      cerr << "is norming c_mode = " << c_mode << endl;
      for (int i = 0; i < 3; i++)
	m2 += vector[i] * vector[i];
      m2 = sqrt (m2);
    }
    else m2 = 1.0;
    for (int i = 0; i < 3; i++) 
      (*rc).set_ravel_Float (i+1, sa * vector[i] / m2);
    rc->check_value(LOC);
  }
  return rc;
}

static Token
eval_AB (Value_P A, Value_P B, const NativeFunction * caller)
{
  Value_P rc = create_quat (CONSTRUCT_RAW, A, B);

  return Token(TOK_APL_VALUE1, rc);
}

static int
dictionary_insert_compare (const void *a, const void *b)
{
  const char *av = (*(dictionary_ety_s *)a).key;
  const char *bv = (*(dictionary_ety_s *)b).key;
  return strcasecmp (av, bv);
}

void *
get_function_mux(const char * function_name)
{
  static bool dict_initialised = false;

  if (!dict_initialised) {
    dict_initialised = true;
    for (int i = 0; i < nr_operations; i++) {
      if (dictionary_max <= 3 + dictionary_nxt) {
	dictionary_max += DICTIONARY_INCR;
	dictionary =
	  (dictionary_ety_s *)realloc (dictionary,
				       dictionary_max *
				       sizeof (dictionary_ety_s));
      }
      dictionary[dictionary_nxt].key = operations[i].keyword;
      dictionary[dictionary_nxt++].loc = &operations[i];
      dictionary[dictionary_nxt].key = operations[i].abbr;
      dictionary[dictionary_nxt++].loc = &operations[i];
      if (0 < strlen (operations[i].symbol)) {
	dictionary[dictionary_nxt].key = operations[i].symbol;
	dictionary[dictionary_nxt++].loc = &operations[i];
      }
    }
    qsort (dictionary, dictionary_nxt, sizeof (dictionary_ety_s),
	   dictionary_insert_compare);

#if 0
    for (int i = 0; i < dictionary_nxt++; i++) {
      dictionary_ety_s *ety = &dictionary[i];
      fprintf (stderr, "ety \"%s\"\n", ety->key);
    }
    keyword_s *fnd;
    fnd = lookup_kwd ("format");
    if (fnd) {
      fprintf (stderr, "found %s\n", fnd->keyword);
      fprintf (stderr, "found %d\n", fnd->opcode);
      fprintf (stderr, "found %s\n", fnd->abbr);
    }
    else fprintf (stderr, "not found\n");
    
    fnd = lookup_kwd ("fmt");
    if (fnd) {
      fprintf (stderr, "found %s\n", fnd->keyword);
      fprintf (stderr, "found %d\n", fnd->opcode);
      fprintf (stderr, "found %s\n", fnd->abbr);
    }
    else fprintf (stderr, "not found\n");
    
    fnd = lookup_kwd ("⍕");
    if (fnd) {
      fprintf (stderr, "found %s\n", fnd->keyword);
      fprintf (stderr, "found %d\n", fnd->opcode);
      fprintf (stderr, "found %s\n", fnd->abbr);
    }
    else fprintf (stderr, "not found\n");
#endif
  }

  
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

