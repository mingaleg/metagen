METAGEN
===
Metagen is universal competitive programming problems' test cases generator.

## Examples

> ./metagen print abcd
```
abcd
```

> ./metagen print 123 print 456
```
123456
```

> ./metagen print 123 _ print 456 endl print 789
```
123 456
789
```

> ./metagen times 3 abc
```
abcabcabc
```

> ./metagen next "[a-z]{10}"
```
ipgipyojtr
```

> ./metagen next "[a-z]{5}" _ next "[a-z]{7}"
```
vofvc ktcxonf
```

> ./metagen repeat 3 next "[a-z][{5}]" end
```
vpilf tmxol wdvbw 
```

> ./metagen repeat 3 repeat 4 next "[a-z]{2}" _ end endl end
```
xl pn li ir 
vz zc od sc 
gp in pi ks 
```

> ./metagen print 123 comment something anything print 456
```
123
```

> ./metagen inext 10
```
7
```

> ./metagen inext2 5 10
```
8
```

> ./metagen wnext 10 3
```
9
```

> ./metagen thuemorse 20 a 0
```
a00a0aa00aa0a00a0aa0
```

> ./metagen rotate 5 "abc test contest"
```
est contestabc t
```

> ./metagen shuffle 012345678912
```
731290564281
```

> ./metagen reverse 0123456789
```
9876543210
```

> ./metagen lowercase
```
abcdefghijklmnopqrstuvwxyz
```

> ./metagen uppercase
```
ABCDEFGHIJKLMNOPQRSTUVWXYZ
```

> ./metagen digits
```
0123456789
```

> ./metagen cycle 10 abcd
```
abcdabcdab
```

> ./metagen times 3 abcd
```
abcdabcdabcd
```

> ./metagen shift 1 abc baracuda
```
cbrbaudb
```

> ./metagen leftpad 6 a 0123
```
aa0123
```

> ./metagen times 3 [ next "[a-z]{10}" _ ]
```
mmgzjzywey mmgzjzywey mmgzjzywey
```

> ./metagen repeat [ inext2 2 10 ] next "[a-z]{5}" endl end
```
whfov
lyiym
vtgfx
cxdbp
```

> ./metagen [ print left print pad ] 20 "=" [ cycle [ print 10 ] [ lowercase ] ]
```
==========abcdefghij
```

> ./metagen set a abcd var a
```
abcd
```

> ./metagen set n 10 repeat [ var n ] repeat [ var n ] next "[a-z]" end endl end
```
xzlehmoqjn
bclolhqydw
xqgkmigbxd
rxjupeqnpj
ncsezxzdio
uzyotiqgfz
fanbwzlquz
pgdtircuca
dezzcoxejm
sxocbxppqf
```

> ./metagen set a [ next "[a-z]{10}" ] set b 0 repeat 10 rotate [ var b ] [ var a ] _ leftpad 2 0 [ var b ] inc b endl end
```
ghxndpvwqq 00
hxndpvwqqg 01
xndpvwqqgh 02
ndpvwqqghx 03
dpvwqqghxn 04
pvwqqghxnd 05
vwqqghxndp 06
wqqghxndpv 07
qqghxndpvw 08
qghxndpvwq 09
```
