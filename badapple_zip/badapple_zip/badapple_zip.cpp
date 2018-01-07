// badapple_zip.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include "zlib.h"

int main()
{
	FILE* fp = fopen("badapple.txt", "r");
	Bytef* buf = NULL;
	Bytef* src = NULL;
	fseek(fp, 0, SEEK_END);
	uLong flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if ((src = (Bytef*)malloc(sizeof(Bytef) * flen)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}
	fread(src, flen, sizeof(char), fp);
	uLongf blen = compressBound(flen);
	if ((buf = (Bytef*)malloc(sizeof(Bytef) * blen)) == NULL)
	{
		printf("no enough memory!\n");
		return -1;
	}
	if (compress(buf, &blen, src, flen) != Z_OK)
	{
		printf("compress failed!\n");
		return -1;
	}
	fclose(fp);
	free(src);
	fp = fopen("badapple.dat", "wb"); //wb 只写打开或新建一个二进制文件；只允许写数据
	fwrite(&flen, 1, sizeof(int), fp);
	fwrite(buf, blen, sizeof(char), fp);
	fclose(fp);
	free(buf);
	return 0;
}
