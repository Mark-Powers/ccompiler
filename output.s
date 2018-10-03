lvalue i
push 11
:=
label lbl0
rvalue i
gofalse lbl1
lvalue i
rvalue i
push 1
-
:=
rvalue i
print
goto lbl0
label lbl1
rvalue i
push 0
-
not
gofalse lbl2
push 1000
print
label lbl2
push 999
print
