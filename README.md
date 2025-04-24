# aplquat
APLquat is an APL extension that implements quaternions.

Quaternion are extensions of complex numbers.  A complex number is basically a
2-space vector where the elements are real coefficients of a pair of
orthogonal basis vectors.  Quaternions extend that to 3-space.  Quaternions
have many applications--quantum mechanics, kinematics, field theory, and so
on--but my thing is computer graphics so this extension is optimised for that.
(The extension can, of course, be extended...)

Quaternions are represented by a 4-element vector:

&nbsp;&nbsp;&nbsp;&nbsp; $[ s\, a_0i\, a_1j\, a_2k]$

where $s$ is a scalar quantity and $a_0$, $a_1$, and $a_2$ are elements of a
3-space vector.  This has many possible interpretations, but for my purposes
in computer graphics, a quaternion may represent either a position in space at
location $[ a_0\, a_1\, a_2 ]$ (the scalar component $s$ is ignored), or a
rotation of $s$ around an axis $[ a_0\, a_1\, a_2 ]$.

### Quaternion algebra

There is an algebra associated with quaternions.  They may be added,
subtracted, multplied, etc, and this package implements what I've found to be
a useful a subset ofthe possible operations.  The following tables list the
supported quaternion operations.  Assuming the quaternion extension has been
loaded with

&nbsp;&nbsp;&nbsp;&nbsp; 'libaplquat.so' ⎕fx 'quat'

(Use anything you like instead of 'quat'.)

the operations can be invoked in any of these four forms:

- quat[1]      	   (i.e., by the opcode)
- quat['pl']	   (by the abbreviation)
- quat['+']	   (by the symbol)
- quat['plus']	   (by the operation name)

| Opcode | Abbrev | Symbol | Operation       | Description      |
| ------ | ------ | ------ | --------------- | ---------------- |
1   |   'pl'   | '+'  |  'plus'          | Addition  |
2   |   'mi'   | '-'  |  'minus'         | Subtraction  |
3   |   'ti'   | '×'  |  'times'         | Multiplication  |
4   |   'di'   | '÷'  |  'divide'        | Division  |
5   |   'co'   | '*'  |  'conjugate'     | Conjugation  |
6   |   'mag'  | '+'  |  'magnitude'     | Norm  |
7   |   'neg'  | '-'  |  'negate'        | Negate  |
8   |   'inv'  | '÷'  |  'invert'        | Reciprocal  |
9   |   'eq'   | '='  |  'equal'         | Equal  |
10  |   'ne'   | '≠'  |  'not_equal'     | Not equal  |
11  |   'fmt'  | '⍕'  |  'format'        | Format  (not yet implemented)   |
12  |   'dot'  | '∘'  |  'dot_product'   | Dot product  |
13  |   'cross'| '○'  |  'cross_product' | Cross product  |
14  |   'ang'  | '<'  |  'interangle'    | Internal angle  |
15  |   'ro'   | '⌽'  |  'rotate'        | Rotate  |
16  |   'rad'  | ''   |  'radians'       | Radians  |
17  |   'deg'  | ''   |  'degrees'       | Degrees  |

## Installation

- ./autogen.sh
- make
- sudo make install

Note that the installation process queries apl itself to find the location
of the APL source code, so the source for the current apl must still be where
apl said it came from.

### A quaternion C++ library

The basis of this APL extension is a C++ library, libaplquat.so, which if you're
interested at

&nbsp;&nbsp;&nbsp;&nbsp; https://github.com/ChrisMoller/libquat

aplquat uses a slightly customised copy of the libquat source so you don't
have to install the libquat package before installing aplquat.



### An example
~~~
testquat;a;b;quat
⊣'libaplquat.so' ⎕fx 'quat'

⍝ creation:
 a←quat ⍳4
 b←6 quat 5+⍳3

⎕←''
⎕←'Dyadics:'
⎕←''

⍞←'Addition       +  '
 a quat['+'] b        ⍝ addition
 
 ⍞←'Subtraction    -  '
 a quat['-'] b	      ⍝ subtraction
 
 ⍞←'Multiplication ×  '
 a quat['×'] b	      ⍝ multiplication
 
 ⍞←'Division       ÷  '
 a quat['÷'] b	      ⍝ division
 
 ⍞←'Equality       == '
 a quat['=='] b	      ⍝ equality
 
 ⍞←'Inequality     != '
 a quat['!='] b	      ⍝ inequality
 
 ⍞←'Dot product    ∘  '
 a quat['∘'] b	      ⍝ dot product
 
 ⍞←'Cross product  ○  '
 a quat['○'] b	      ⍝ cross product
 
 ⍞←'Internal angle ,  '
 a quat['<'] b	      ⍝ interangle
 
 ⍞←'Transform      >< '
 a quat['><'] b	      ⍝ transform
 
⎕←''
⎕←'Monadics:'
⎕←''

 ⍞←'Norm/Magnitude +  '
 quat['+'] b	      ⍝ norm/magnitude
 
 ⍞←'Negate         -  '
 quat['-'] b	      ⍝ negate
 
 ⍞←'Conjugate      *  '
 quat['*'] b	      ⍝ conjugate
 
 ⍞←'Invert         ÷  '
 quat['÷'] b	      ⍝ invert

⎕←''
⎕←'Misc:'
⎕←''

 ⍞←'Multiplication identity  '
 a quat['×'] quat['÷'] a
 ~~~


