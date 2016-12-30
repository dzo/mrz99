#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct header_st {
        int magic; // 0xa55a5aa5
        int len;   // len of ver file
        int version; 
        int volume; //1
        int zero1;
        int a2c; //2c
        int zeros[5];
        int nfiles; //1
        int dirdepth; //1
        char dirname[64];
        int filestoprocess; //1
	char filename[56];
	int file_length;
	int file_crc;
        int magic1; // 0xa55a5aa5
        int ver_crc; // up to and including magic1
} header_type;

void wscpy(char *d, char *s) {
	while (*s) {
		*d++=*s++;
		d++;
        }
}
int crc16a(unsigned char * data_p, int len) {
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (len--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}


unsigned short crc16(const unsigned char *data, int len) { 
  unsigned short crc = 0xFFFF; 
  int i; 
  if (len) do { 
    crc ^= *data++; 
    for (i=0; i<8; i++) { 
      if (crc & 1) crc = (crc >> 1) ^ 0x8408; 
      else crc >>= 1; 
    } 
  } while (--len); 
  return(~crc); 
} 


int main(int argc, char *argv[]) {

	header_type h;
        FILE *fin,*fout;

	memset(&h, 0, sizeof(h));

        char *dir="OPN090";
	char *datfile="NA090OPN.DAT";

	if(argc>=2)
		datfile=argv[1];

	if(argc>=3)
                dir=argv[2];

	h.magic=0xa55a5aa5;
	h.len=sizeof(h);
	h.version=0x04000000;
	h.volume=1;
	h.a2c=0x2c;
	h.nfiles=1;
	h.dirdepth=1;
	wscpy(h.dirname,dir);
	h.filestoprocess=1;
	wscpy(h.filename,datfile);
	fin=fopen(datfile,"rb");
        fseek(fin, 0L, SEEK_END);
        h.file_length = ftell(fin);
        unsigned char *p=malloc(h.file_length);
        rewind(fin);
        fread(p,1,h.file_length,fin);
	h.file_crc=crc16(p,h.file_length);
        h.magic1=0xa55a5aa5;
	h.ver_crc=crc16((unsigned char *)&h, h.len-4);

	fout=fopen("NA090OPN.VER","wb");
	fwrite(&h,1,h.len,fout);
	fclose(fout);
}
