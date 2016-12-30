#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
        FILE *fin,*fout,*iout,*fin1;
        typedef struct idx_st {
                int start;
                int size;
                int uncompressed_size;
        } idx_type;


       typedef struct header_st {
                int magic;
                int count;
                int header_size;
                int zero1;
                int index_start;
                int index_size;
                int img_start;
                int img_size;
                int string_start;
                int string_size;
        } header_type;

	if(argc>2) {
		puts("usage: extractbmp [encode]");
		exit(1);
	}
	int encode=0;

	if(argc==2 && !strcmp(argv[1],"encode"))
		encode=1;
        fin=fopen("Image.img","rb");
        header_type header;
        fread(&header,4,10,fin);

        idx_type index[header.count];
        char fname[256];
        fread(index, sizeof(idx_type), header.count, fin);
        printf("%d bitmaps\n", header.count);
	int outpos=0;
	if(encode)  {
		iout=fopen("Image1.img","wb");
		fwrite(&header,4,10,iout);
		fwrite(index, sizeof(idx_type), header.count, iout);
		outpos=sizeof(idx_type)*header.count+sizeof(header_type);
        }
        for(int i=0;i<header.count;i++) {
                printf("bmp %d at %d, len %d\n",i, index[i].start, index[i].size);
                sprintf(fname,"bmp/b%d.bmp.gz",i);
                fout=fopen(fname,"wb");
                fseek(fin,index[i].start, SEEK_SET);
                for(int j=0;j<index[i].size;j++)
                        fputc(fgetc(fin),fout);
                fclose(fout);
		if(encode) {
			sprintf(fname,"bmp1/b%d.bmp.gz",i);
			fin1=fopen(fname,"rb");
			if(!fin1) {
				printf("%s not found\n",fname);
				exit(1);
			}
			int l=0;
			index[i].start=outpos;
			while(!feof(fin1)) {
				char c=fgetc(fin1);
				if(!feof(fin1)) {
					fputc(c,iout);
					l++;
				}
			}
			fclose(fin1);
			index[i].size=l;
			outpos+=l;
			sprintf(fname,"bmp1/b%d.bmp",i);
			fin1=fopen(fname,"rb");
			if(fin1) {
				fseek(fin1, 0L, SEEK_END);
				index[i].uncompressed_size = ftell(fin1);
				fclose(fin1);
			}
		}
//                sprintf(fname,"gzip -d bmp/b%d.bmp.gz",i);
//                system(fname);
        }
        char strings[header.string_size];
        fseek(fin,header.string_start,SEEK_SET);
        fout=fopen("bmp_names","wb");
        fread(strings,1,header.string_size,fin);
        fwrite(strings,1,header.string_size,fout);
        fclose(fout);

	if(encode) {
		fwrite(strings,1,header.string_size,iout);
		fseek(iout, 0, SEEK_SET);
		header.img_size=outpos-index[0].start;
		header.string_start=outpos;
		fwrite(&header,4,10,iout);
                fwrite(index, sizeof(idx_type), header.count, iout);
	}
}

