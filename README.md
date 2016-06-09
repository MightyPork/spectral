# spectral

A simple C program for frequency analysis of text.

Just pipe your analyzed file into it.

```none
$ cc main.c
$ cat main.c | ./a.out
20 	' '	392
74 	't'	297
09 	'\t'	219
0a 	'\n'	187
6e 	'n'	186
72 	'r'	186
65 	'e'	171
69 	'i'	161
63 	'c'	145
6f 	'o'	145
75 	'u'	124
73 	's'	107
66 	'f'	105
61 	'a'	100
28 	'('	86
29 	')'	86
3b 	';'	78
5f 	'_'	74
6c 	'l'	65
3d 	'='	64
68 	'h'	60
70 	'p'	57
64 	'd'	50
2a 	'*'	49
2f 	'/'	38
30 	'0'	37
7b 	'{'	29
7d 	'}'	29
62 	'b'	27
6d 	'm'	27
2d 	'-'	26
43 	'C'	24
3e 	'>'	23
2c 	','	22
78 	'x'	19
2b 	'+'	16
67 	'g'	14
5c 	'\'	14
27 	'''	13
4c 	'L'	12
...
```
