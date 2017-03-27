#include <stdio.h>
#include <string.h>
#include <unistr.h>
#include <stdlib.h>

void readstring(FILE *f, char *p) {
	char c;
	c=fgetc(f);
	if(feof(f)) return;
	if(c!='\"') {
		puts("ERROR: mismatched quotes");
		exit(1);
	}
	while((c=fgetc(f))!='"' && !feof(f))
		*p++=c;
	*p++=0;
//	c=fgetc(f);
	if((c=fgetc(f))!=10 && !feof(f)) {
		puts("ERROR: mismatched quotes");
		exit(1);
	}
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

int main(int argc,char *argv[]) {
	char data[200000];
	char st[1024];
	short st_u16[1024];
	char *infile="translated.txt",*outfile="PF090JPJPN.LNG";
        FILE *f,*g;
        int i;
	unsigned *id=(unsigned *) data;
	id[0]=0xa55a5aa5;
	id[1]=0x01000001;
	id[2]=0x39304650;
	id[3]=0x45504a30;
	id[4]=0;
	id[5]=0;
	id[6]=0;// len
	id[7]=0x40;
	id[8]=0;//last
	id[9]=0xaaaaaa02;
	id[10]=0xaaaaaaaa;
	id[11]=0xaaaaaaaa;
        id[12]=0xaaaaaaaa;
        id[13]=0xaaaaaaaa;
        id[14]=0xaaaaaaaa;
        id[15]=0xaaaaaaaa;

        if(argc>1)
                infile=argv[1];

	if(argc>2)
                outfile=argv[2];

        g=fopen(infile,"rb");
	int start=0x40;
	unsigned short *idx=(unsigned short *)(data+0x40);
        short nstrings=0;
        int offset=0;
        int next=0;
        short ch;
	while(!feof(g)) {
		readstring(g,st);
//		printf("%d %s\n",nstrings,st);
		nstrings++;
	}
        nstrings+=2;
	rewind(g);
        nstrings=nstrings-3;
	printf("Found %d strings\n",nstrings);
	unsigned short *strptr=idx+nstrings;
	int off=0x40+(nstrings+3)*2;
	int ntrans=0;
	char **strings=malloc(sizeof(char *)*nstrings);
        for(i=0;i<nstrings;i++) {
		readstring(g,st);
		strings[i]=malloc(strlen(st)+1);
		strcpy(strings[i],st);
		int j;
		for(j=0;j<i;j++)
			if(!strcmp(st,strings[j]))
				break;
		if(j<i) {
			idx[i]=idx[j];
		} else {
//		puts(st);
		idx[i]=(off-0x40)/2;
		if((off-0x40)/2>65535) {
			printf("ERROR: index too large %x\n",(off-0x40)/2);
			exit(1);
		}
		size_t sz=512;
		u8_to_u16(st,strlen(st),(short *)(data+off),&sz);
		off+=sz*2;
		data[off++]=0;
		data[off++]=0;
		}
        }
      idx[i]=(off-0x40)/2;
      id[8]=off;
      strcpy(data+off,"RG_VOICE_DATA");
      off+=strlen("RG_VOICE_DATA");

      data[off++]=0xf1; // this looks like a checksum at the end of the file but
      data[off++]=0x8d; // it doesn't seem to matter if it's wrong
      id[6]=off;
      printf("%d strings\n",nstrings);
      f=fopen(outfile,"wb");
      fwrite(data,off,1,f);
      fclose(f);
      return(0);
}

