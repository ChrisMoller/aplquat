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

#include "kwds.h"
typedef struct {
  const char *key;
  void *loc;
} dictionary_ety_s;
dictionary_ety_s *dictionary = NULL;
int dictionary_max = 0;
int dictionary_nxt = 0;
#define DICTIONARY_INCR 64

static int
dictionary_search_compare (const void *a, const void *b)
{
  const char *av = (char *)a;
  const char *bv = (*(dictionary_ety_s *)b).key;
  return strcasecmp (av, bv);
}

static keyword_s *
lookup_kwd (const char *kwd)
{
  keyword_s *op_data = NULL;
  void *res = bsearch (kwd, dictionary, dictionary_nxt,
			sizeof (dictionary_ety_s), dictionary_search_compare);
  if (res) {
    dictionary_ety_s *ety = (dictionary_ety_s *)res;
    op_data = ((keyword_s*)(ety->loc));
  }
  return op_data;
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

static bool
is_quat (Value_P V)
{
  bool rc = false;

  if ((V->get_cfirst ()).is_numeric () && !(*V).is_complex (true)) {
    if (V->get_rank () == 1 && V->element_count () == 4)
      rc = true;
    else {
      MORE_ERROR () <<
	"Invalid argument.  Must be a quaternion";
      SYNTAX_ERROR;
    }
  }
  else {
    MORE_ERROR () <<
      "Invalid argument.  Must be a real numeric vector.";
    SYNTAX_ERROR;
  }
  
  return rc;
}

static void
quatify (double *vec, Value_P V)
{
  bool rc = false;
  for (int i = 0; i < 4; i++) {
    vec[i] = (V->get_cravel (i)).get_real_value ();
  }
}

static Value_P
do_plus (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq + Bq;
    double *v = S.qvec ();
    
    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Value_P
do_minus (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq - Bq;
    double *v = S.qvec ();
    
    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Value_P
do_times (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq * Bq;
    double *v = S.qvec ();
    
    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Value_P
do_divide (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq / Bq;
    double *v = S.qvec ();
    
    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Value_P
do_equal (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    bool S = Aq == Bq;
    rc = IntScalar ((int)S, LOC);
  }

  return rc;  
}

static Value_P
do_not_equal (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    bool S = Aq != Bq;
    rc = IntScalar ((int)S, LOC);
  }

  return rc;  
}

static Value_P
do_dot (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    double S = Aq.qdot (Bq);
    rc = FloatScalar ((int)S, LOC);
  }

  return rc;  
}

static Value_P
do_cross (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq.qcross (Bq);
    double *v = S.qvec ();

    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Value_P
do_ang (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    double S = Aq.qang (Bq);
    rc = FloatScalar ((int)S, LOC);
  }

  return rc;  
}

static Value_P
do_xform (Value_P A, Value_P B)
{
  Value_P rc = Str0(LOC);

  if (is_quat (A) && is_quat (B)) {
    double Av[4];
    quatify (Av, A);
    Quat Aq (Av);
    
    double Bv[4];
    quatify (Bv, B);
    Quat Bq (Bv);

    Quat S = Aq.qxform (Bq);
    double *v = S.qvec ();

    Shape shape_Z (4);
    rc = Value_P (shape_Z, LOC);
    for (int i = 0; i < 4; i++) {
      (*rc).set_ravel_Float (i, v[i]);
    }
  }

  return rc;  
}

static Token
eval_AXB(Value_P A, Value_P X, Value_P B,
	 const NativeFunction * caller)
{
  Value_P rc = Str0(LOC);

  int op = OP_NONE;
  
  if (X->is_numeric_scalar () &&
      !(*X).is_complex (true)) {
    op = (*X).get_sole_integer ();
    cout << "num = " << op << endl;
  }
  else if (X->is_char_array()) {
    const UCS_string  ustr = X->get_UCS_ravel();
    UTF8_string which (ustr);
    keyword_s *fnd = lookup_kwd (which.c_str ());
    if (fnd) {
      switch (fnd->opcode) {
      case OP_PLUS:
	rc = do_plus (A, B);
	break;
      case OP_NEGATE:
      case OP_MINUS:
	rc = do_minus (A, B);
	break;
      case OP_CONJUGATE:
      case OP_TIMES:
	rc = do_times (A, B);
	break;
      case OP_DIVIDE:
	rc = do_divide (A, B);
	break;
      case OP_EQUAL:
	rc = do_equal (A, B);
	break;
      case OP_NOT_EQUAL: 
	rc = do_not_equal (A, B);
	break;
      case OP_DOT_PRODUCT:
	rc = do_dot (A, B);
	break;
      case OP_CROSS_PRODUCT:
	rc = do_cross (A, B);
	break;
      case OP_INTERANGLE:
	rc = do_ang (A, B);
	break;
      case OP_XFORM:
	rc = do_xform (A, B);
	break;
      case OP_PLUS_ASSIGN:
      case OP_MINUS_ASSIGN:
      case OP_TIMES_ASSIGN:
      case OP_DIVIDE_ASSIGN:
      case OP_NORM:
      case OP_INVERT:
      case OP_FORMAT:
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
    if (B->get_rank () == 1 && B->element_count () == 3) {
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
    for (int i = 0; i < nr_keys; i++) {
      if (dictionary_max <= 3 + dictionary_nxt) {
	dictionary_max += DICTIONARY_INCR;
	dictionary =
	  (dictionary_ety_s *)realloc (dictionary,
				       dictionary_max *
				       sizeof (dictionary_ety_s));
      }
      dictionary[dictionary_nxt].key = keywords[i].keyword;
      dictionary[dictionary_nxt++].loc = &keywords[i];
      dictionary[dictionary_nxt].key = keywords[i].abbr;
      dictionary[dictionary_nxt++].loc = &keywords[i];
      dictionary[dictionary_nxt].key = keywords[i].symbol;
      dictionary[dictionary_nxt++].loc = &keywords[i];
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

