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
magnitude is applied.



~~~
testquat;a;b;quat
⊣'libaplquat.so' ⎕fx 'quat'
 a←quat ⍳4
 b←quat 5+⍳4

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

