// MD5 hashing
#include<iostream>
#include<cstring>
#include "md5.hpp"
using namespace std;

//Initialization

const unsigned int s[64]= { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22, 
	       5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	       4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	       6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

const unsigned int K[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

unsigned int MD5::leftRotate(unsigned int s, int c) {
  return ((s<<c) | (s>>(32-c)));
}

void MD5::computeHash(string message, unsigned int hash[4]) {
  //Initialize variables:
  unsigned int a0 = 0x67452301;   //A
  unsigned int b0 = 0xefcdab89;   //B
  unsigned int c0 = 0x98badcfe;   //C
  unsigned int d0 = 0x10325476;   //D

  static const unsigned char padding[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  static const int blockSize = 64;

  long originalMsgLen = message.size();

  if (originalMsgLen == 0) return;

  if(originalMsgLen%blockSize >= 56) {
    message.append((char *)padding, 56+(blockSize-originalMsgLen%blockSize));
  }

  long msgLen = message.size();
    
  for(int k=0;k<=msgLen/blockSize;k++) {

    //Initialize hash value for this chunk:
    unsigned int A = a0;
    unsigned int B = b0;
    unsigned int C = c0;
    unsigned int D = d0;

    unsigned int F;
    unsigned int dTemp;
    unsigned int g;
    unsigned int M[blockSize/4];
    
    unsigned char input[blockSize];
    int bytesToFill;
    int paddingLen;

    unsigned int msg[blockSize/4];
    long i,t;

    bool isFinal =( k==msgLen/blockSize);
    if(isFinal) {
      bytesToFill = 14;
      paddingLen = 56-(msgLen%blockSize);
    }
    else {
      bytesToFill = blockSize/4; // wordsize is 4 bytes
      paddingLen = 0;
    }
    memcpy(input, message.c_str()+(k*blockSize), bytesToFill*4-paddingLen);
    memcpy(input+(56-paddingLen), padding, paddingLen);

    for(i=0,t=0;t<bytesToFill;i=i+4,t++) {
      msg[t]=input[i+3];
      cout<<hex<<(unsigned int)input[i+3]<<"," << input[i+3]<<"\t";
      for(int j=i+2;j>=i;j--) {
	msg[t] = msg[t]<<8;
	msg[t] =  msg[t] | input[j];
	cout<<hex <<(unsigned int)input[j] <<"," << input[j]<<"\t";
      }
      cout <<endl;
    }

    if(isFinal) {
      long msgLenInBits = originalMsgLen*8;
      msg[t]= msgLenInBits & 0xFFFFFFFF;
      t++;
      msgLenInBits = msgLenInBits >>32;
      msg[t]= msgLenInBits & 0xFFFFFFFF;
    }
    cout <<"Outputting input message " <<endl;
    for(int k=0;k<16;k++) {
      M[k] = msg[k];
      cout << hex << M[k] << ",";
    }
    cout <<endl;
    //Main loop:
    for(int i=0; i<64;i++) {
      if( (0 <= i) && (i <= 15)) {
	F = (B & C) | ((~B) & D);
        g = i;
      }
      else if ((16 <= i) && (i <= 31)) {
	F = (D & B) | ((~D) & C);
	g = (5*i + 1) % 16;
      }
      else if ((32 <= i) && (i <= 47)) {
	F = B ^ C ^ D;
	g = (3*i + 5) % 16;
      }
      else if ((48 <= i) && (i <= 63)) {
	F = C ^ (B | (~D));
	g = (7*i) % 16;
      }
      //Be wary of the below definitions of a,b,c,d
      dTemp = D;
      D = C;
      C = B;
      B = B + leftRotate((A + F + K[i] + M[g]), s[i]);
      A = dTemp;
    }
    //Add this chunk's hash to result so far:
    a0 = a0 + A;
    b0 = b0 + B;
    c0 = c0 + C;
    d0 = d0 + D;
  }
  hash[0] = a0;
  hash[1] = b0;
  hash[2] = c0;
  hash[3] = d0;
}
