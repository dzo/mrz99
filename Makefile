all: decode recode recode110 extractbmp makever fixbmpheader
decode:
recode: recode.c
	gcc     recode.c   -o recode -l unistring
recode110:
extractbmp:
makever:
fixbmpheader:
