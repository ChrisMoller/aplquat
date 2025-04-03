testquat;a;b;quat
⊣'libaplquat.so' ⎕fx 'quat'
 a←quat ⍳4
 b←6 quat 5+⍳3

⎕←''
⎕←'Dyadics:'
⎕←''

⍞←'Addition:         a quat[''+''] b  ==> '
 a quat['+'] b        ⍝ addition
 
 ⍞←'Subtraction:      a quat[''-''] b ==> '
 a quat['-'] b	      ⍝ subtraction
 
 ⍞←'Multiplication:   a quat[''×''] b ==>  '
 a quat['×'] b	      ⍝ multiplication
 
 ⍞←'Division:         a quat[''÷''] b ==> '
 a quat['÷'] b	      ⍝ division
 
 ⍞←'Equality:         a quat[''==''] b ==> '
 a quat['=='] b	      ⍝ equality
 
 ⍞←'Inequality:       a quat[''!=''] b ==> '
 a quat['!='] b	      ⍝ inequality
 
 ⍞←'Dot product:      a  quat[''∘''] b ==> '
 a quat['∘'] b	      ⍝ dot product
 
 ⍞←'Cross product:    a quat[''○''] b ==> '
 a quat['○'] b	      ⍝ cross product
 
 ⍞←'Internal angle:   a quat[''<''] b ==>'
 a quat['<'] b	      ⍝ interangle
 
 ⍞←'Rotate:           a quat[''⌽''] b ==> '
 a quat['⌽'] b	      ⍝ rotate
 
⎕←''
⎕←'Monadics:'
⎕←''

 ⍞←'Norm/Magnitude"   quat[''+''] b ==> '
 quat['+'] b	      ⍝ norm/magnitude
 
 ⍞←'Negate:           quat[''-''] b ==> '
 quat['-'] b	      ⍝ negate
 
 ⍞←'Conjugate:        quat[''*''] b ==> '
 quat['*'] b	      ⍝ conjugate
 
 ⍞←'Invert:           quat[''÷''] b ==> '
 quat['÷'] b	      ⍝ invert

⎕←''
⎕←'Misc:'
⎕←''

 ⍞←'Multiplication identity:  a quat[''×''] quat[''÷''] a ==> '
 a quat['×'] quat['÷'] a

⍞←'Rotation : p0 quat[''⌽''] p1 ==> ' 
  p1 ← quat (0, 1, 0, 0)
  p0 ← quat (1.8, 0, 0, 1)
p0 quat['⌽'] p1

