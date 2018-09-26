lvalue i
push 5
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
push 999
print
