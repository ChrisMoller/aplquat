qplot c;b;quat;p;r;q;f;A


⊣'libaplquat.so' ⎕fx 'quat'
p←quat 0 1 0
r←(360÷c) quat['deg'] 1 0 0
q←r
b←1 4⍴0

b←b⍪q quat['×'] p quat['÷'] q
b←b⍪1 4⍴0

q←r quat['×'] q
b←b⍪q quat['⌽'] p
b←b⍪1 4⍴0

top:
q←r quat['×'] q
b←b⍪q quat['⌽'] p
b←b⍪1 4⍴0

→((c←c-1) > 0)/top

b←0 2 ↓b
b←(1↑⍴b)⍴(0 ¯1↓b) + 0j1 × 0 1↓b
A ←"""
output_filename: qplot
pa_width:        600 pixel 
pa_height:       600 pixel
axisX_label:     Real
axisY_label:     Imag
legend_name-1:   Rotated
"""
f←A ⎕plot b


