#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
        FILE *fin,*fout;
        typedef struct __attribute__((__packed__)) bmp_st {
                short sig;
                int size;
		int reserved;
                int offset;
		int dib_size;
		int width;
		int height;
		short planes;
		short bpp;
		int compression;
		int img_size;
		int xppm;
		int yppm;
		int cols;
		int ccnt;
        } bmp_h_type;

	if(argc!=3) {
		puts("error");
		exit(1);
	}

	fin=fopen(argv[1],"rb");
	fout=fopen(argv[2],"wb");

	bmp_h_type header;
	fread(&header,1,sizeof(header),fin);

	char z[16];
	fread(z,1,16,fin);
	
	header.size-=16;
	header.offset-=16;
	header.dib_size-=16;
	header.compression=0;
	header.xppm=0;
	header.yppm=0;
	header.cols=0;
	header.ccnt=0;
	
	fwrite(&header,1,sizeof(header),fout);
	while(!feof(fin)) {
		char c=fgetc(fin);
		if(!feof(fin))
			fputc(c,fout);
	}
}
