# aplquat
An APL extension implementing quaternions

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

