// badapple_dzip.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "zlib.h"
using namespace std;
int main()
{
	FILE* fp = fopen("badapple.dat", "rb");
	uLongf slen;
	fread(&slen, 1, sizeof(int), fp);
	Bytef* buf = NULL;
	Bytef* dst = NULL;
	fseek(fp, 0, SEEK_END);
	uLongf flen = ftell(fp);
	fseek(fp, 4, SEEK_SET);
	flen -= 4;
	uLongf blen = compressBound(slen);
	if ((dst = (Bytef*)malloc(sizeof(Bytef) * slen)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}
	if ((buf = (Bytef*)malloc(sizeof(Bytef) * blen)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}
	fread(buf, flen, sizeof(char), fp);
	if (uncompress(dst, &slen, buf, blen) != Z_OK)
	{
		printf("uncompress failed!\n");
		return -1;
	} 
	free(buf);
	fclose(fp);
	for (int i = 0; i<slen; i++)
		printf("%c", dst[i]);
}