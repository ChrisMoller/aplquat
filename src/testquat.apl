∇testquat v;a;b;quat
 'libaplquat.so' ⎕fx 'quat'
 a←quat ⍳4
 b←quat 5+⍳4
 a quat['+'] b	⍝ addition 
 a quat['-'] b	⍝ subtraction 
 a quat['*'] b	⍝ multiplication
 a quat['÷'] b	⍝ division
 a quat['=='] b	⍝ equality 
 a quat['!='] b	⍝ inequality 
 a quat['∘'] b	⍝ dot product
 a quat['○'] b	⍝ cross product
 a quat['<'] b	⍝ interangle
 a quat['><'] b	⍝ transform
∇
