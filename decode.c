#include <stdio.h>

// decode a pioneer .LNG file (old format with 16 bit index)

int main(int argc,char *argv[]) {
	FILE *f;
	int i;
	f=fopen(argv[1],"rb");
	fseek(f,0x40,SEEK_SET);
	short nstrings;
	int offset=0;
	int next=0;
	short ch;
	fread(&nstrings, 2, 1, f);
	nstrings=nstrings-3;
	for(i=0;i<nstrings;i++) {
		fseek(f,0x40+i*2,SEEK_SET);
		offset=0;
		fread(&offset, 2, 1, f);
		offset=offset*2+0x40;
		next=0;
		fread(&next, 2, 1, f);
		next=next*2+0x40;
		ch=1;
		putchar('"');
		putchar(0);
		fseek(f,offset,SEEK_SET);
		while(offset<next-2) {
			fread(&ch,1,1,f);
			offset++;
			putchar(ch&255);
		}
                putchar('"');
                putchar(0);
		putchar(13);
		putchar(0);
                putchar(10);
                putchar(0);
	}
	printf("%d strings\n",nstrings);
}
