#!/usr/local/bin/apl --script
 ⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝
⍝                                                                    ⍝
⍝ ./testquat.apl                       2025-03-31  19:08:41 (GMT-4)  ⍝
⍝                                                                    ⍝
 ⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝⍝

∇a←acos x
 ⍝ arccosine of x
 a←¯2○x
∇

∇a←acosd x
 a←(¯2○x)÷(○÷180)
∇

∇a←acosh x
 a←¯6○x
∇

∇a←acosr x
 a←(¯2○x)÷(○1)
∇

∇a←asin x
 a←¯1○x
∇

∇a←asind x
 a←(¯1○x)÷(○÷180)
∇

∇a←asinh x
 a←¯5○x
∇

∇a←asinr x
 a←(¯1○x)÷(○1)
∇

∇a←atan x
 a←¯3○x
∇

atan2←{('○.5××', ⍕⍵)  ⎕ea '¯3○', (⍕⍵), '÷', ⍕⍺}

atan2d←{r2d (('○.5××', ⍕⍵)  ⎕ea '¯3○', (⍕⍵), '÷', ⍕⍺)}

atan2r←{(('○.5××', ⍕⍵)  ⎕ea '¯3○', (⍕⍵), '÷', ⍕⍺)÷○1}

∇a←atand x
 a←(¯3○x)÷(○÷180)
∇

∇a←atanh x
 a←¯7○x
∇

∇a←atanr x
 a←(¯3○x)÷(○1)
∇

∇a←cos x
 a←2○x
∇

∇a←cosd x
 a←2○○x÷180
∇

∇a←cosh x
 a←6○x
∇

∇a←cosr x
 a←2○○x
∇

cov←{⎕mx[11] ⍵}

d2r←{○1×⍵÷180}

det←{⎕MX[1] ⍵}

⍝ function em has ufun1 pointer 0!

eval←{⎕mx[5] ⍵}

evec←{⎕mx[4] ⍵}

gaussian←{⍺ ⎕mx[10] ⍵}

hist←{⍺ ⎕mx[12] ⍵}

ident←{⎕MX[6] ⍵}

norm←{⎕mx[9] ⍵}

pcaval←{eval cov ⍵}

pcavec←{evec cov ⍵}

⍝ function quat has ufun1 pointer 0!

r2d←{180×⍵÷○1}

∇a←tan x
 a←3○x
∇

∇a←tand x
 a←3○○x÷180
∇

∇a←tanh x
 a←7○x
∇

∇a←tanr x
 a←3○○x
∇

⎕CT←1E¯13

⎕FC←".,⋆0_¯"

⎕IO←1

⎕L←0

⎕LX←""

⎕PP←10

⎕PR←' '

⎕PS←0 0

⎕PW←106

⎕R←0

⎕RL←7596157243904707097

⎕TZ←-4

⎕X←0







