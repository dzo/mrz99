CFLAGS=-std=c99
all: decode recode extractbmp makever fixbmpheader
decode:
recode: recode.c
	gcc recode.c   -o recode -l unistring
translate: translate.c
	gcc translate.c -o translate -l unistring
extractbmp:
makever:
fixbmpheader:
clean:
	rm -f fixbmpheader extractbmp recode decode makever
trans: PF090JPJPN.LNG PF110JPJPN.LNG PF090JPJPN_0047.LNG PF090JPJPN_0047_RU.LNG PF090JPJPN_RU.LNG
PF090JPJPN.LNG: translated.txt recode
	./recode translated.txt PF090JPJPN.LNG
PF110JPJPN.LNG: translated_110.txt recode
	./recode translated_110.txt PF110JPJPN.LNG
PF090JPJPN_0047.LNG: translated_0047.txt recode
	./recode translated_0047.txt PF090JPJPN_0047.LNG
PF090JPJPN_0047_RU.LNG: translated_0047_ru.txt recode
	./recode translated_0047_ru.txt PF090JPJPN_0047_RU.LNG
PF090JPJPN_RU.LNG: translated_ru.txt recode
	./recode translated_ru.txt PF090JPJPN_RU.LNG
