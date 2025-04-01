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

| Opcode | Abbrev | Symbol | Operation   | Description      |
| ------ | ------ | ------ | ----------- | ---------------- |
| 10     | 'ne'   | '!='   | 'not_equal'     | Not equal        |
|  5     | 'co'   | '*'    | 'conjugate'     | Conjugation      |
|  1     | 'pl'   | '+'    | 'plus'          | Addition         |
|  6     | 'no'   | '+'    | 'norm'          | Norm (magnitude)) |
|  2     | 'mi'   | '-'    | 'minus'         | Subtraction |
|  7     | 'neg'  | '-'    | 'negate'        | Negate |
| 14     | 'ang'  | '<'    | 'interangle'    | Internal angle |
|  9     | 'eq'   | '=='   | 'equal'         | Equal |
| 15     | 'xform'| '><'   | 'xform'         | Transform |
| 14     | 'ang'  | '<'    | 'interangle'    | Internal angle |
|  5     | 'co'   | '*'    | 'conjugate'     | Conjugation |
| 13     | 'cross'| '○'    | 'cross_product' | Cross product |
|  4     | 'di'   | '÷'    | 'divide'        | Division |
| 12     | 'dot'  | '∘'    | 'dot_product'   | Dot product |
|  9     | 'eq'   | '=='   | 'equal'         | Equal |
| 11     | 'fmt'  | '⍕'    | 'format'        | Format  (not yet implemented) |
| 14     | 'ang'  | '<'    | 'interangle'    | Internal angle |
|  8     | 'inv'  | '÷'    | 'invert'        | Invert |
|  2     | 'mi'   | '-'    | 'minus'         | Subtraction |
| 10     | 'ne'   | '!='   | 'not_equal'     | Not equal |
|  7     | 'neg'  | '-'    | 'negate'        | Negate |
|  6     | 'no'   | '+'    | 'norm'          | Norm (magnitude) |
| 10     | 'ne'   | '!='   | 'not_equal'     | Not equal |
|  1     | 'pl'   | '+'    | 'plus'          | Addition |
|  3     | 'ti'   | '×'    | 'times'         | Multiplication |
| 15     | 'xform'| '><'   | 'xform'         | Transform |
|  3     | 'ti'   | '×'    | 'times'         | Multiplication |
|  4     | 'di'   | '÷'    | 'divide'        | Division |
|  8     | 'inv'  | '÷'    | 'invert'        | Invert |
| 12     | 'dot'  | '∘'    | 'dot_product'   | Dot product |
| 11     | 'fmt'  | '⍕'    | 'format'        | Format  (not yet implemented) |
| 13     | 'cross'| '○'    | 'cross_product' | Cross product |



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

