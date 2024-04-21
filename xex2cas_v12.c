/*--------------------------------------------------------------------*/
/* XEX2CAS - GienekP                                                  */
/*--------------------------------------------------------------------*/
#include <stdio.h>
/*--------------------------------------------------------------------*/
typedef unsigned char U8;
/*--------------------------------------------------------------------*/
/* mono loader */
const U8 mono[128]={
  0x00, 0x01, 0x7a, 0x04, 0x71, 0xe4, 0x88, 0xf0, 0x3f, 0x38, 0x60, 0x85,
  0x04, 0x20, 0xcf, 0x04, 0x85, 0x05, 0x25, 0x04, 0xc9, 0xff, 0xf0, 0x30,
  0x20, 0xcf, 0x04, 0x85, 0x02, 0x20, 0xcf, 0x04, 0x85, 0x03, 0xa9, 0xf9,
  0x8d, 0xe2, 0x02, 0xa9, 0x04, 0x8d, 0xe3, 0x02, 0x20, 0xcf, 0x04, 0x81,
  0x04, 0xa5, 0x04, 0xc5, 0x02, 0xa5, 0x05, 0xe5, 0x03, 0xe6, 0x04, 0xd0,
  0x02, 0xe6, 0x05, 0x90, 0xeb, 0x84, 0x3d, 0x20, 0xcc, 0x04, 0xa4, 0x3d,
  0xa2, 0x00, 0x20, 0xcf, 0x04, 0xb0, 0xbc, 0x6c, 0xe0, 0x02, 0x6c, 0xe2,
  0x02, 0xcc, 0x8a, 0x02, 0x90, 0x1c, 0xc0, 0x80, 0x90, 0x1d, 0x20, 0x7a,
  0xe4, 0x10, 0x0a, 0xc0, 0x88, 0xd0, 0x15, 0xad, 0x7f, 0x04, 0x8d, 0x8a,
  0x02, 0xa9, 0x3c, 0x8d, 0x02, 0xd3, 0xa2, 0x00, 0xa0, 0x00, 0xb9, 0x00,
  0x04, 0xc8, 0x38, 0x60, 0x68, 0x68, 0x38, 0x60
  };
/*--------------------------------------------------------------------*/
const U8 shortesgap[128]={
  0xff, 0xff, 0x00, 0x06, 0x73, 0x06, 0xa5, 0x00, 0x48, 0xa5, 0x01, 0x48,
  0x78, 0xa9, 0x00, 0x8d, 0x00, 0xd4, 0x8d, 0x0e, 0xd4, 0x8d, 0x0e, 0xd2,
  0x8d, 0x0e, 0xd2, 0xad, 0x01, 0xd3, 0x48, 0xa9, 0xc0, 0x85, 0x01, 0xa0,
  0x00, 0x84, 0x00, 0xa2, 0xff, 0x8e, 0x01, 0xd3, 0xb1, 0x00, 0xca, 0x8e,
  0x01, 0xd3, 0x91, 0x00, 0xe8, 0xc8, 0xd0, 0xf1, 0xe6, 0x01, 0xa5, 0x01,
  0xc9, 0xd0, 0xd0, 0x04, 0xa9, 0xd8, 0x85, 0x01, 0xc9, 0x00, 0xd0, 0xe1,
  0x68, 0x29, 0xfe, 0x8d, 0x01, 0xd3, 0xa9, 0x40, 0x8d, 0x0e, 0xd4, 0xa9,
  0xf7, 0x8d, 0x0e, 0xd2, 0xad, 0x2f, 0x02, 0x8d, 0x00, 0xd4, 0x58, 0x68,
  0x85, 0x01, 0x68, 0x85, 0x00, 0xa2, 0x04, 0x8e, 0xd4, 0xbf, 0xca, 0xca,
  0xca, 0x8e, 0x17, 0xee, 0x8e, 0x18, 0xee, 0xca, 0x8e, 0x42, 0xbc, 0x60,
  0xea, 0xea, 0xe2, 0x02, 0xe3, 0x02, 0x00, 0x06
  };
/*--------------------------------------------------------------------*/
void addRecord(const U8 *record, U8 mode, FILE *pf)
{
	const U8 datal[8]={'d', 'a', 't', 'a', 0x84, 0x00, 0xB0, 0x36};
	const U8 datam[8]={'d', 'a', 't', 'a', 0x84, 0x00, 0xA6, 0x01};
	const U8 datas[8]={'d', 'a', 't', 'a', 0x84, 0x00, 0xFA, 0x00};
	const U8 dataf[8]={'d', 'a', 't', 'a', 0x84, 0x00, 0x14, 0x00};
	unsigned int i;
	switch (mode)
	{
		case 1: {fwrite(datam,sizeof(U8),sizeof(datam),pf);} break;
		case 2: {fwrite(datas,sizeof(U8),sizeof(datas),pf);} break;
		case 3: {fwrite(dataf,sizeof(U8),sizeof(dataf),pf);} break;
		default: {fwrite(datal,sizeof(U8),sizeof(datal),pf);} break;	
	};
	fputc(0x55,pf);
	fputc(0x55,pf);
	if (record==NULL) // EOF
	{
		fputc(0xFE,pf);
		for (i=0; i<128; i++) {fputc(0x00,pf);};
		fputc(0xA9,pf);
	}
	else
	{
		unsigned int sum=0xA7;
		for (i=0; i<128; i++) 
		{
			sum+=record[i];
			if (sum>255)
			{
				sum&=0xFF;
				sum++;
			};
		};
		fputc(0xFC,pf);
		fwrite(record,sizeof(U8),128,pf);
		fputc(sum&0xFF,pf);
	};
};
/*--------------------------------------------------------------------*/
void addData(U8 gap, const U8 *buf, unsigned int size, FILE *pf)
{
	U8 record[128];
	unsigned int i,j,k,nor;
	if (size)
	{
		if ((size%128)==0) {nor=size/128;} else {nor=(size/128)+1;};
		for (i=0; i<nor; i++)
		{
			for (j=0; j<128; j++) {record[j]=0;};
			if (i==(nor-1))
			{
				k=(size-(i*128));
			}
			else
			{
				k=128;
			};
			for (j=0; j<k; j++) {record[j]=buf[i*128+j];};
			addRecord(record,gap,pf);
		};
	}
	else
	{
		addRecord(NULL,gap,pf);
	};
}
/*--------------------------------------------------------------------*/
void addHeader(FILE *pf)
{
	const U8 header[]={'F','U','J','I',0x1D,0x00,0x00,0x00,'F','i','l','e',' ','w','a','s',
			' ','g','e','n','e','r','a','t','e','d',' ','b','y',' ','X','E','X','2','C','A','S'};	
	fwrite(header,sizeof(U8),sizeof(header),pf);
}
/*--------------------------------------------------------------------*/
void addBoud(FILE *pf, unsigned int boudval)
{
	const U8 boud[6]={'b','a','u','d', 0x00, 0x00};
	U8 val[2];
	val[0]=(boudval&0xFF);
	val[1]=((boudval>>8)&0xFF);
	fwrite(boud,sizeof(U8),sizeof(boud),pf);
	fwrite(val,sizeof(U8),sizeof(val),pf);
}
/*--------------------------------------------------------------------*/
unsigned int loadBIN(const char *filename, U8 *buf, unsigned int size)
{
	unsigned int ret=0;
	FILE *pf;
	pf=fopen(filename,"rb");
	if (pf)
	{
		ret=fread(buf,sizeof(U8),size,pf);
		fclose(pf);
	};
	return ret;
}
/*--------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	U8 buf[256*256];
    FILE *casfile;
    unsigned int size;
	printf("XEX2CAS v1.2 (c)GienekP\n");
	switch (argc)
	{
		case 3:
		{
			casfile=fopen(argv[2],"wb");
			if (casfile)
			{
				size=loadBIN(argv[1],buf,sizeof(buf));
				addHeader(casfile);
				addBoud(casfile,600);
				addData(0,mono,sizeof(mono),casfile);
				addData(2,shortesgap,sizeof(shortesgap),casfile);
				addBoud(casfile,800);
				addData(1,buf,128,casfile);
				addData(3,&buf[128],size-128,casfile);
				addData(3,NULL,0,casfile);
				fclose(casfile);
			};
		} break;
		default:
		{
			printf("use:\n");
			printf("   xex2cas file.xex file.cas\n");	
		} break;
	};
	return 0;
}
/*--------------------------------------------------------------------*/