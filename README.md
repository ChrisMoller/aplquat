# aplquat
APLquat is an APL extension that implements quaternions.

A quaternion is an extension of a complex number.  A complex number is
basically a 2-space vector where the elements are real coefficients of
a pair of orthogonal basis vectors.  Quaternions extend that to 3-space.
A major difference though is that while complex numbers a cartesian in
interpretation, quaternions are more akin to a spherical coordinate system.

Quaternions are represented by a 4-element vector:

&nbsp;&nbsp;&nbsp;&nbsp; $[ s\, a_0i\, a_1j\, a_2k]$

where $s$ is a magnitude and $a_0$, $a_1$, and $a_2$ are a vector to which the
magnitude is applied.  This has many possible interpretations, but for my
purposes in computer graphics, a quaternion may represent a position
in space at distance $s$ from the local origin in direction
$[ a_0\, a_1\, a_2 ]$, or a rotation $s$ around an axis $[ a_0\, a_1\, a_2 ]$.

Quaternions have about as many applications as do complex numbers and can be
found in areas such as special relativity, electromagnetics, quantum
mechanics, and so on.  This package provides only a subset of quaternion
algebra I've found useful for what I do, bu I expect even as it is it can
be of use in other areas and nothing says the package can't be extended to
include additional function.


### Quaternion algebra

The following tables list the supported quaternion operations.  Assuming
the quaternion extension has been loaded with

&nbsp;&nbsp;&nbsp;&nbsp; 'libaplquat.so' ⎕fx 'quat'

the operations can be invoked in any of three forms such as:

- a quat[1] b
- a quat['pl'] b
- a quat['+'] b
- a quat['plus'] b


#### Dyadic

| Opcode | Abbrev | Symbol | Operation       | Description      |
| ------ | ------ | ------ | --------------- | ---------------- |
|  1     | 'pl'   | '+'    | 'plus'          | Addition |
|  2     | 'mi'   | '-'    | 'minus'         | Subtraction |
|  3     | 'ti'   | '×'    | 'times'         | Multiplication |
|  4     | 'di'   | '÷'    | 'divide'        | Division |
|  9     | 'eq'   | '=='   | 'equal'         | Equal |
| 10     | 'ne'   | '!='   | 'not_equal'     | Not equal |
| 12     | 'dot'  | '∘'    | 'dot_product'   | Dot product |
| 13     | 'cross'| '○'    | 'cross_product' | Cross product |
| 14     | 'ang'  | '<'    | 'interangle'    | Internal angle |
| 15     | 'xform'| '><'   | 'xform'         | Transform |

#### Monadic

| Opcode | Abbrev | Symbol | Operation       | Description      |
| ------ | ------ | ------ | --------------- | ---------------- |
|  5     | 'co'   | '*'    | 'conjugate'     | Conjugation |
|  6     | 'no'   | '+'    | 'norm'          | Norm (magnitude) |
|  7     | 'neg'  | '-'    | 'negate'        | Negate |
|  8     | 'inv'  | '÷'    | 'invert'        | Invert |
| 11     | 'fmt'  | '⍕'    | 'format'        | Format  (not yet implemented) |

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

