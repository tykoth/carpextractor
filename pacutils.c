/*PAC UTILS*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;


int ltob(int i)
{
    return((i&0xff)<<24)+((i&0xff00)<<8)+((i&0xff0000)>>8)+((i>>24)&0xff);
}

void inf(int cmpsize, int ucmpsize, char *source, FILE *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[cmpsize];
    unsigned char out[ucmpsize];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    
	

    /* decompress until deflate stream ends or end of file */
    
        strm.avail_in = cmpsize;
        if (strm.avail_in == 0);
	
        strm.next_in = source;
	
        /* run inflate() on input until output buffer not full */
       
            strm.avail_out = ucmpsize;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
		
          //  assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                printf("error 1\n");    /* and fall through */
            case Z_DATA_ERROR: printf("error 2\n");
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);printf("error 3\n");
                
            }
		
            have = ucmpsize - strm.avail_out;
            fwrite(out, ucmpsize, 1, dest);
                (void)inflateEnd(&strm);
                
		printf("decomp ok!\n");
        

        /* done when inflate() says it's done */
    

    /* clean up and return */
    (void)inflateEnd(&strm);
    //return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}






int extract(char *file){

	FILE *fp,*fnew;
	int datasize;
	char *img,*buffer;
	
	char filename[255];
	char filesource[255];
	char dir[50];
	int cmpsz;
	int ucmpsz;
	char output[100];
	int count=0;
	int i=0;
	int magicsize=12;
	int cmpsize=4;
	int ucmpsize=4;
	int fnamesize=20;
	uint32 ver;
	char *head,*prev;
	char fname[255];
	int isfile;
	


	strcpy(filesource,file);

	

	fprintf(stderr, "OPEN:File %s\n",filesource);

	/*open file*/
	fp = fopen(filesource, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR:File %s not found\n",filesource);
		return 0;
	}

	fseek ( fp, 12, SEEK_SET );

	mkdir("data");
	chmod("data",0777);

	getcwd(&dir,100);

	strcat(dir,"/");
	strcat(dir,"data/");
	

	/*file info
	12 bytes:$FILENAME (magic)
	280 bytes:?
	filename: 256 bytes
	compressed size:4 bytes
	uncompressed size:4 bytes
	file data: (compressed size) bytes
	*/
	memset(fname,0,255);	
	memset(filename,0,255);
	//memset(isfile,0,4);


	fprintf(stderr, "READING File\n");
	/*skip header*/


	for(;;){
		
		fread( &isfile, 4, 1, fp );
		printf("file: %d\n",isfile);
		if(feof(fp)) break;

		/*detect file or dir*/
		if(isfile>0){

			/*read filename*/
			fread( filename, fnamesize, 1, fp );
			strcat(fname,dir);
			strcat(fname,filename);

			fseek ( fp, 236, SEEK_CUR );

			/*create file*/
			printf("filename: %s\n",fname);
			fnew = fopen(fname, "wb");
			if (fnew == NULL) {
				fprintf(stderr, "ERROR:Cannot Open File %s \n",fnew);
				return 0;
			}
			/*skip null data*/
		//	fseek ( fp, 248, SEEK_CUR );

			/*read size*/
			fread( &cmpsz, cmpsize, 1, fp );
			fread( &ucmpsz, ucmpsize, 1, fp );

			/*skip null data*/
			fseek ( fp, 8, SEEK_CUR );

			printf("compressed size: %d\n",cmpsz);
			printf("uncompressed size: %d\n",ucmpsz);

			/*read file data*/
			buffer=malloc(cmpsz);
			fread( buffer, cmpsz, 1, fp );

			/*decompress data*/
			inf(cmpsz,ucmpsz,buffer,fnew);

			/*write data into file*/
			//fwrite(buffer, 1028, 1, fnew);
		
			free(buffer);
			fclose(fnew);

			//fseek ( fp, 4, SEEK_CUR );
			memset(filename,0,255);
			memset(fname,0,255);
			//memset(isfile,0,4);
			isfile=0;

		}else{
			
			fread( filename, fnamesize, 1, fp );
			printf("directory found: %s\n",filename);
			if (filename=="."){
				//strcat(dir,"/");
				strcat(dir,filename);
				mkdir(dir);
				chmod(dir,0777);
				strcat(dir,"/");
				
			}
			fseek ( fp, 252, SEEK_CUR );
		}

	}
				
		strcat(fname,filename);
				
			
			
		


	fclose(fp);

}


/*dirrrrrrrrrrrrty 
detect file size
haha, it works  */
int detect(char* file,int position){


	FILE *fp;
	char *hdr,*tmp;
	int size=4;
	
	

	fp = fopen(file, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR:File %s not found\n",file);
		return 0;
	}

	hdr=malloc(4);
	tmp=malloc(4);

	fseek ( fp, position, 0 );

	fread( hdr, 4, 1, fp );
	//fread( tmp, 12, 1, fp );
	
	

	while(fread( tmp, 4, 1, fp )==1){
		if(strcmp(hdr,tmp)==0) break;
		size+=4;
	}

	fclose(fp);

return size;
	

	

}


/*++++++ENCODE FILES+++++++*/
int encode(char* file){

	FILE *fp,*ftex,*fout;
	int size;
	char *img,*buffer;
	char filename[30];
	char dir[100];
	char temp[100];
	char output[100];
	int count=0;
	int i=0;
	uint32 ver;
	char *head,*prev,*header;
	


	strcpy(filename,file);

	


;

	/*open file*/
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR:File %s not found\n",filename);
		return 0;
	}
	


	sscanf(filename,"%[^.]",temp);
	printf("using directory %s\n",temp);
	sprintf(filename,"new%s",file);
	printf("creating file %s\n",filename);

	/*create output file*/
	fout = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR:Cannot create file %s\n",filename);
		return 0;
	}
	//mkdir(temp);
	//chmod(temp,0777);

	getcwd(&dir,100);

	strcat(dir,"/");
	strcat(dir,temp);
	

	img=malloc(32768);
	header=malloc(24);
	buffer=malloc(4);
	head=malloc(4);
	prev=malloc(4);

	memset(img,0,32768);
	memset(buffer,0,4);
	memset(prev,0,4);
	memcpy(prev,buffer,4);
	/*read header, 8 bytes*/
	fread( head, 4, 1, fp );
	fseek ( fp, -4, SEEK_CUR );

	

	
	for(;;){
				
		/*end of block*/
		if((memcmp(head,buffer,4)==0)&&(!ver)){
			fseek ( fp, 8, SEEK_CUR );
			fread( buffer, 4, 1, fp );
			fseek ( fp, -12, SEEK_CUR );
			ver=1;
			ver=*buffer^0xFFFFFFEB;
			printf("%x\n",*buffer);
			if(ver==0xFFFFFFFF){ 	//header found!
			
				fseek ( fp, -4, SEEK_CUR );
				fread( header, 24, 1, fp );
				sprintf(output,"%s/imagen%d.pvr",dir,i);
				printf("openning file %s\n",output);
				ftex = fopen(output, "rb");
				if (ftex == NULL) {
					fprintf(stderr, "ERROR:Cannot open file %s\n",output);
					return 0;
				}

				fseek(ftex, 0, SEEK_END); // seek to end of file
				size = ftell(ftex); // get current file pointer
				fseek(ftex, 0, SEEK_SET); // seek back to beginning of file

				
				
				fseek ( ftex, 36, SEEK_CUR ); //skip pvr header
				fread( img, size-36, 1, ftex ); //read entire texture
							
				
				if (size==18464) size=20492; //fix export of pvr export


				fread( buffer, 4, 1, fp );
				printf("writing file %s, size %d\n",output,size);

				/*write header into file*/				
				//hdr.filelength=count;
				fwrite(header, 24, 1, fout);

				/*write data into file*/
				fwrite(img, size-36, 1, fout);
			
				fclose(ftex);
				memset(img,0,32768);

				i++;
				//fseek ( fp, -12, SEEK_CUR );
			}
			
		}

			memcpy(prev,buffer,4);
			ver=*prev;
			fread( buffer, 4, 1, fp );
			if(feof(fp)) break;
		
		
				
	}

			/*if(ftex){
				printf("writing file %s, size %d\n",output,size);
				//hdr.filelength=count;
				//fwrite(header, 20, 1, fout);

				
				fwrite(img, size, 1, fout);
			
				fclose(fout);
			}*/


	fclose(fp);


}

int main(int argc, char *argv[]) {

	

	if (argc != 3) {
		printf("pac files decoder/encoder\nusage: pacutils option filename\noptions:\n  -x extract\n  -e encode\n");
		return 0;
	}



	if (!strcmp(argv[1],"x")) {
			
		extract(argv[2]);

	}else if (!strcmp(argv[1],"e")) {
		encode(argv[2]);

	}


	

return 0;
	


}

