#include <stdio.h>
#include <string.h>

void readstring(FILE *f, char *p) {
	char c;
	c=fgetc(f);
	if(c!='\"')
		puts("ERRRRRRRR");
	while((c=fgetc(f))!='"' && !feof(f))
		*p++=c;
	*p++=0;
	while((c=fgetc(f))!=10 && !feof(f));
}


int main(int argc,char *argv[]) {
	char data[200000];
	char st[10240];
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

        g=fopen("translated_110.txt","rb");
	f=fopen("PF110JPJPN.LNG","rb");
	int start=0x40;
	unsigned short *idx=(unsigned short *)(data+0x40);
        fseek(f,0x40,SEEK_SET);
        short nstrings;
        int offset=0;
        int next=0;
        short ch;
        fread(&nstrings, 2, 1, f);
        nstrings=nstrings-3;
	unsigned short *strptr=idx+nstrings;
	int off=0x40+(nstrings+3)*2;
	int ntrans=0;
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
                fseek(f,offset,SEEK_SET);
                while(offset<next-2) {
                        fread(&ch,2,1,f);
                        offset+=2;
                        putchar(ch&127);
                }
                putchar('"');
                putchar(10);

		readstring(g,st);
		puts(st);
		idx[i]=(off-0x40)/2;
		if((off-0x40)/2>65535)
			printf("Error index too large %x\n",(off-0x40)/2);
		int j=0;
		for(j=0;st[j]!=0;j++) {
			data[off++]=st[j];
			data[off++]=0;
		}
		data[off++]=0;
		data[off++]=0;
        }
      idx[i]=(off-0x40)/2;
      id[8]=off;
      strcpy(data+off,"RG_VOICE_DATA");
      off+=strlen("RG_VOICE_DATA");
      data[off++]=0xf1; // this looks like a checksum at the end of the file but
      data[off++]=0x8d; // it doesn't seem to matter if it's wrong
      id[6]=off;
      printf("%d strings\n",nstrings);
      f=fopen("PF110JPJPN1.LNG","wb");
      fwrite(data,off,1,f);
      fclose(f);
}

