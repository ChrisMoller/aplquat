
/*
    aplquat Copyright (C) 2025  Dr C. H. L. Moller under the terms of the
    GNU General Public License Version 3.

    This program is free software: you can redistribute it and/or modify
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


#include <iostream>
#include <string>
#include <cmath>

#include "Quat.hh"

Quat::Quat ()
{
  a = 0;
  b = 0;
  c = 0;
  d = 0;
}

Quat::Quat (double ai, double bi, double ci, double di)
{
  a = ai;
  b = bi;
  c = ci;
  d = di;
}

Quat::Quat (double ai)
{
  a = ai;
  b = 0;
  c = 0;
  d = 0;
}

Quat::Quat (double *v)
{
  a = v[0];
  b = v[1];
  c = v[2];
  d = v[3];
}
    
Quat::~Quat ()
{
}

Quat
Quat::operator+(Quat v)	// add
{
  Quat s;
  s.a = a + v.a;
  s.b = b + v.b;
  s.c = c + v.c;
  s.d = d + v.d;
  return s;
}

Quat
Quat::operator+=(Quat &v)	// add-assign
{
  Quat s;
  a += v.a;
  b += v.b;
  c += v.c;
  d += v.d;
  return *this;
}

Quat
Quat::operator-(Quat v)	// subtract
{
  Quat s;
  s.a = a - v.a;
  s.b = b - v.b;
  s.c = c - v.c;
  s.d = d - v.d;
  return s;
}

Quat
Quat::operator-=(Quat &v)	// subtract-assign
{
  Quat s;
  a -= v.a;
  b -= v.b;
  c -= v.c;
  d -= v.d;
  return *this;
}

Quat
Quat::operator*(Quat v)	// multiply
{
  Quat s;
  s.a = (a * v.a) - (b * v.b) - (c * v.c) - (d * v.d);
  s.b = (a * v.b) + (b * v.a) + (c * v.d) - (d * v.c);
  s.c = (a * v.c) - (b * v.d) + (c * v.a) + (d * v.b);
  s.d = (a * v.d) + (b * v.c) - (c * v.b) + (d * v.a);
  return s;
}

Quat
Quat::operator*=(Quat &v)	// multiply-assign
{
  Quat s;
  s.a = (a * v.a) - (b * v.b) - (c * v.c) - (d * v.d);
  s.b = (a * v.b) + (b * v.a) + (c * v.d) - (d * v.c);
  s.c = (a * v.c) - (b * v.d) + (c * v.a) + (d * v.b);
  s.d = (a * v.d) + (b * v.c) - (c * v.b) + (d * v.a);
  a = s.a;
  b = s.b;
  c = s.c;
  d = s.d;
  return *this;
}

Quat
Quat::operator*(double v)	// multiply by double
{
  Quat s;
  s.a = a * v;
  s.b = b;
  s.c = c;
  s.d = d;
  return s;
}

Quat
Quat::operator*=(double &v)	// multiply by double assign
{
  a = a * v;
  b = b * v;
  c = c * v;
  d = d * v;
  return *this;
}

Quat
Quat::operator/(Quat v)	// divide
{
  Quat s = ~v;			// invert denominator
  Quat t = s * *this;
  return t;
}

Quat
Quat::operator/=(Quat &v)	// divide-assign
{
  Quat s = ~v;			// invert denominator
  Quat t = s * *this;
  *this = t;
  return t;
}

Quat
Quat::operator/(double v)	// divide by double
{
  Quat s;
  s.a = a / v;
  s.b = b;
  s.c = c;
  s.d = d;
  return s;
}

// I'm using monadic * because quaternions have no use for
// a dereference operator
Quat
Quat::operator*()		// conjugation
{
  Quat s;
  s.a = a;
  s.b = -b;
  s.c = -c;
  s.d = -d;
  return s;
}

double
Quat::operator+()		// norm
{
  double s = sqrt ((a * a) + (b * b) + (c * c) + (d * d));
  return s;
}

Quat
Quat::operator-()		// negation
{
  Quat s;
  s.a = -a;
  s.b = -b;
  s.c = -c;
  s.d = -d;
  return s;
}

// Note that c++ doesn't support monadic / which would
// have made more sense, so I'm using monadic tilde (~)
// because quaternions have no use for a complement operator
Quat
Quat::operator~()		// reciprocal
{
  Quat s;
  double m2 = (a * a) + (b * b) + (c * c) + (d * d);
  s.a = a/m2;
  s.b = -b/m2;
  s.c = -c/m2;
  s.d = -d/m2;
  return s;
}

bool
Quat::operator==(Quat v)	// eq
{
  bool s = (a == v.a) && (b == v.b) && (c == v.c) && (d == v.d);
  return s;
}

bool
Quat::operator!=(Quat v)	// neq
{
  bool s = (a != v.a) || (b != v.b) || (c != v.c) || (d != v.d);
  return s;
}

string
Quat::qstr ()			// stringify
{
  char *s;
  asprintf (&s, "[%#08g [%#08g %#08g %#08g]]", a, b, c, d);
  string t (s);
  free (s);
  return t;
}

double *
Quat::qvec ()			// convert to array
{
  double *v = new double[4];
  v[0] = a;
  v[1] = b;
  v[2] = c;
  v[3] = d;
  return v;
}

#if 0
string
Quat::qstr (Quat v)		// stringify
{
  char *s;
  asprintf (&s, "[%#08g [%#08g %#08g %#08g]]", v.a, v.b, v.c, v.d);
  string t (s);
  free (s);
  return t;
}
#endif

ostream&
operator<<(ostream& os, const Quat &v)
{
  os << "[ " << v.a << " " << v.b << " " << v.c << " " << v.d << " ]";
    return os;
}

#if 0
double
Quat::qdot (Quat &a, Quat &b)
{
  return (a.a * b.a) + (a.b * b.b) + (a.c * b.c) + (a.d * b.d);
}

Quat
Quat::qcross (Quat &a, Quat &b)
{
  Quat s;
  s.a = 0.0;
  s.b = (a.c * b.d) - (a.d * b.c);
  s.c = (a.d * b.b) - (a.b * b.d);
  s.d = (a.b * b.c) - (a.c * b.b);
  return s;
}

double
Quat::qang (Quat &a, Quat &b)
{
  return acos (Quat::qdot (a, b) / (+a * +b));
}
#endif

double
Quat::qdot (Quat &v)
{
  return (a * v.a) + (b * v.b) + (c * v.c) + (d * v.d);
}

Quat
Quat::qxform (Quat &v)
{
  Quat xform = this->qcross (v);
  double ang = this->qang (v);
  xform.a = ang;
  return xform;
}

Quat
Quat::qcross (Quat &v)
{
  Quat s;
  s.a = 0.0;
  s.b = (c * v.d) - (d * v.c);
  s.c = (d * v.b) - (b * v.d);
  s.d = (b * v.c) - (c * v.b);
  return s;
}

double
Quat::qang (Quat &v)
{
  double dt = (a * v.a) + (b * v.b) + (c * v.c) + (d * v.d);
  return acos (dt / ((+*this) * +v));
}

void
Quat::show ()
{
  fprintf (stdout, "[ %g %g %g %g ]\n",
	   this->a,
	   this->b,
	   this->c,
	   this->d);
}

void
Quat::show (string l)
{
  fprintf (stdout, "%s = [ %g %g %g %g ]\n",
	   l.c_str (),
	   this->a,
	   this->b,
	   this->c,
	   this->d);
}
