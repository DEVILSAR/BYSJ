#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
 
 
/* ��������Right shift */
#define RTSHIFT(x,n)		( (x)>>(n) )
/* ѭ������Right rotate */
#define RTROT(xsize,x,n)	( ((x)>>(n)) | ( ((x)&((1<<(n+1))-1)) << ( ((xsize)<<3)-(n) )) )
 
/*----------------
      Main()
----------------*/
int main(int argc, char **argv)
{
	int i;
	unsigned char *input=NULL;
 
	int nk;
	uint8_t chunk_data[512/8]={0}; 	/* 512bits��Ϣ�� */
	unsigned int nch;		/* ��Ϣ��������Total number of 512bits_chunks */
	unsigned int mod;		/* ��Ϣ���ȹ���512��������Result of mod calculation: msgbitlen%512 */
 
	uint32_t words[64]={0};  	/* �����ֵ�Ԫ��32*64 = 2048 bits */
	unsigned long len;		/* length in bytes */
	unsigned long bitlen;		/* length in bits */
	unsigned char digest[8*8+1]={0}; /* convert u32 hv[0-7] to string by sprintf(%08x) */
 
	/* hv_primes[8]����ǰ����������ƽ��������ȡ�Ĺ�ϣ��ʼֵ */
	static uint32_t hv_primes[8]= {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
	};
 
	/* Variable hash values: ��ϣֵ������*/
	uint32_t hv[8];
 
	/* kv_primes[64]������ǰ����������������������ȡ��kv��ʼֵ round constants */
	const uint32_t kv_primes[64]= {
	  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
 
	/* Variable round constants kv[64] kV������*/
	uint32_t kv[64];
 
	/* SHA compression vars��������� */
	uint32_t a,b,c,d,e,f,g,h;
	uint32_t s0, s1, ch, temp1, temp2, maj;
 
 
 
	/* 0. ��ȡ������Ϣ��Get input string */
	if(argc>1)
		input=(unsigned char *)argv[1];
 
	/* 1. ǰ����Pre-processing */
	len=strlen((char *)input);
	bitlen=len*8;
 
	/* ������Ϣλ�������������� bitlen, nch, mod */
	mod=bitlen%512;
	/* mod > 448-1: fill the last chunk and need a new chunck */
	if(mod > 448-1) {
		nch=bitlen/512+2;
	}
	/* mod <= 448-1: fill the last chunk. */
	else
		nch=bitlen/512+1;
 
	printf("Input message has %lubits, with mod=%d %s 448-1!\n", bitlen, mod, mod>448-1?">":"<=");
 
	/* 2. ��ʼ����ϣֵ���� Init hash values hv[8] */
	for(i=0; i<8; i++)
		hv[i]=hv_primes[i];
 
	/* 3. ��ʼ��kv  Init round constants kv[64] */
	for(i=0; i<64; i++)
		kv[i]=kv_primes[i];
 
/* ---- ��ÿ����Ϣ����й�ϣ���㡡chunk loop ---- */
for(nk=0; nk<nch; nk++) {
	printf("\n\t--- nk=%d ---\n", nk);
 
	/* 4. ���������512λ��Ϣ�顡Loak chunck_data[64] */
	if( mod <= 448-1 ) {
		/* For complete chunck blocks */
		if( nk < nch-1 )
			memcpy((void *)chunk_data, input+(nk*(512>>3)), (512>>3));
		/* The last block, Need to fill 1+0s for the last chunk */
		else {  /* nk == nch -1 */
			memset(chunk_data, 0, sizeof(chunk_data));
			memcpy((void *)chunk_data, input+(nk*(512>>3)), mod>>3);
			/* Append a single 1, big_endian */
			chunk_data[mod>>3]=0x80;
			/* ������Ϣ���ȡ�Append 64bits to the end of chunk, as BIT_LENGTH of original message, in big_endian! */
			for(i=0; i<sizeof(bitlen); i++)
				*(chunk_data+(512>>3)-1-i) = (bitlen>>(i*8))&0xff; /* Big_endian */
		}
	}
	else if( mod > 448-1 ) {
		/* For complete chunck blocks */
		if( nk < nch-2 )
			memcpy((void *)chunk_data, input+(nk*(512>>3)), (512>>3));
		/* The last block of original msg, appended with 1+0s to complete a 512bits chunck. */
		else if( nk == nch-2 ) {
			memset(chunk_data, 0, sizeof(chunk_data));
			memcpy((void *)chunk_data, input+(nk*(512>>3)), mod>>3);
			/* Append a single 1, big_endian */
			chunk_data[mod>>3]=0x80;
		}
		/* Additional new chunk, as the last chunck. */
		else { /* nk == nch -1 */
			/* All 0s */
			memset(chunk_data, 0, sizeof(chunk_data));
			/* ������Ϣ���ȡ�Append 64bits to the end of chunk, as BIT_LENGTH of original message, in big_endian! */
			for(i=0; i<sizeof(bitlen); i++)
				*(chunk_data+(512>>3)-1-i) = (bitlen>>(i*8))&0xff; /* Big_endian */
		}
	}
 
	/* 5. ���������ֵ�Ԫ  words[64] */
	/* 5.1 words[0]~[15]:  u8 chunck_data[64] ---> u32 words[64] */
	for(i=0; i<16; i++) {
		words[i]=(chunk_data[4*i]<<24) +(chunk_data[4*i+1]<<16)+(chunk_data[4*i+2]<<8)+chunk_data[4*i+3];
	}
	/* 5.2 words[15]~[63]: 48 more words */
	for(i=16; i<64; i++) {
		/* s0 = (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3) */
		s0=RTROT(4,words[i-15],7) ^ RTROT(4,words[i-15],18) ^ RTSHIFT(words[i-15],3);
		/* s1 = (w[i- 2] rightrotate 17) xor (w[i- 2] rightrotate 19) xor (w[i- 2] rightshift 10) */
		s1=RTROT(4,words[i-2],17) ^ RTROT(4,words[i-2],19) ^ RTSHIFT(words[i-2],10);
		/* w[i] = w[i-16] + s0 + w[i-7] + s1 */
		words[i]=words[i-16]+s0+words[i-7]+s1;
	}
 
	/* 6. ����64�ֵĹ�ϣ���㡡SHA Compression, 64 rounds. */
	/* ���� a,b,c,d,e,f,g,h */
	a=hv[0]; b=hv[1]; c=hv[2]; d=hv[3]; e=hv[4]; f=hv[5]; g=hv[6]; h=hv[7];
	/* Compress for 64 rounds */
	for(i=0; i<64; i++) {
		/* S1 = (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25) */
		s1=RTROT(4,e,6)^RTROT(4,e,11)^RTROT(4,e,25);
 
		/* ch = (e and f) xor ((not e) and g) */
		ch= (e&f)^((~e)&g);
 
		/* temp1 = h + S1 + ch + kv[i] + w[i] */
		temp1=h+s1+ch+kv[i]+words[i];
 
		/* S0 = (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22) */
		s0=RTROT(4,a,2)^RTROT(4,a,13)^RTROT(4,a,22);
 
		/* maj = (a and b) xor (a and c) xor (b and c) */
		maj=(a&b)^(a&c)^(b&c);
 
		/* temp2 = S0 + maj */
		temp2=s0+maj;
 
		h=g;
		g=f;
		f=e;
		e=d+temp1;
		d=c;
		c=b;
		b=a;
		a=temp1+temp2;
	}
 
#if 1	/* ��ӡ������Print a~h */
	printf("a: %08x \n", a);
	printf("b: %08x \n", b);
	printf("c: %08x \n", c);
	printf("d: %08x \n", d);
	printf("e: %08x \n", e);
	printf("f: %08x \n", f);
	printf("g: %08x \n", g);
	printf("h: %08x \n", h);
#endif
 
	/* 7. �޸Ĺ��������Modify final values */
	hv[0] += a;
	hv[1] += b;
	hv[2] += c;
	hv[3] += d;
	hv[4] += e;
	hv[5] += f;
	hv[6] += g;
	hv[7] += h;
 
} /* ---- ����������Ϣ��ļ��㡡END: chunk loop ---- */
 
	/* 8. �������յĹ�ϣ��ϢժҪ��Generate final hash digest */
	for(i=0; i<8; i++)
		sprintf((char *)digest+8*i,"%08x",hv[i]); /*Convert to string */
 
	printf("\nDigest: %s\n", digest);
 
 
return 0;
}
 
 