// badapple_exe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#include "zlib.h"
Bytef* dst = NULL;

struct fps_limit {

	int previous_time;
	int tpf_limit;
	int tpf;
	fps_limit(int fps = 60) : previous_time(GetTickCount()), tpf(0) {
		limit_fps(fps);
	}
	void reset() {
		previous_time = GetTickCount(),
			tpf = 0;
		tpf_limit = 60;
	}
	void limit_fps(int fps) {
		tpf_limit = (int)(1000.0f / (float)fps);
	}
	void delay() {
		tpf = GetTickCount() - previous_time;

		if (tpf < tpf_limit)
			Sleep(tpf_limit - tpf - 1);

		previous_time = GetTickCount();
	}
};
void Uncompressdata()
{
	FILE* fp = fopen("badapple.dat", "rb");
	uLongf slen;
	fread(&slen, 1, sizeof(int), fp);
	Bytef* buf = NULL;
	fseek(fp, 0, SEEK_END);
	uLongf flen = ftell(fp);
	fseek(fp, 4, SEEK_SET);
	flen -= 4;
	uLongf blen = compressBound(slen);
	if ((dst = (Bytef*)malloc(sizeof(Bytef) * slen)) == NULL)
	{
		printf("no enough memory!\n");
		exit(1);
	}
	if ((buf = (Bytef*)malloc(sizeof(Bytef) * blen)) == NULL)
	{
		printf("no enough memory!\n");
		exit(1);
	}
	fread(buf, flen, sizeof(char), fp);
	if (uncompress(dst, &slen, buf, blen) != Z_OK)
	{
		printf("uncompress failed!\n");
		exit(1);
	}
	dst[slen] = '\0';
	free(buf);
//	fclose(fp);

/*	HRSRC hrscdat = FindResource(NULL, MAKEINTRESOURCE(0), _T("TYPEDATA"));
					//NULL值指定模块句柄指向操作系统通常情况下创建最近过程的相关位图文件
	char *a = (char *)LockResource(LoadResource(NULL, hrscdat));
					//包含所需资源的模块句柄，如果是程序本身，可以置为NULL
	DWORD flen = SizeofResource(NULL, hrscdat);
	DWORD slen;
	memcpy(&slen, a, sizeof(int));
	Bytef* buf = NULL;
	flen -= 4;
	unsigned int blen = compressBound(slen);
	if ((dst = (Bytef*)malloc(sizeof(char) * (slen + 1))) == NULL)
	{
		printf("no enough memory!\n");
		exit(1);
	}
	if ((buf = (Bytef*)malloc(sizeof(char) * blen)) == NULL)
	{
		printf("no enough memory!\n");
		exit(1);
	}
	memcpy(buf, a + 4, flen * sizeof(char));
	if (uncompress(dst, &slen, buf, blen) != Z_OK)
	{
		printf("uncompress failed!\n");
		exit(1);
	}
	dst[slen] = '\0';
	free(buf);*/
}
int main()
{
	printf("Loading......");
//	HRSRC hrscwav = FindResource(NULL, MAKEINTRESOURCE(1), _T("WAVE"));
	Uncompressdata();
//	printf("OK\n把控制台的高度和宽度都调大点哦\n");
	system("PAUSE");
	system("CLS");
//	PlaySound((LPCWSTR)LockResource(LoadResource(NULL, hrscwav)), NULL, SND_MEMORY | SND_ASYNC);
	PlaySound(TEXT("C:\\opencv\\works\\others\\badapple_exe\\badapple_exe\\BadApple.wav"), NULL, SND_FILENAME || SND_ASYNC);
	fps_limit fps(25);//画面刷新帧数
	int i = 0, j = 0;
	char buf[3000];
//	Sleep(1000);
	while (1)
	{
		HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos;
		pos.X = 0;
		pos.Y = 0;
		SetConsoleCursorPosition(hConsoleOutput, pos);
		while (j<22 * 50) //确定每页刷新出来的字符总数，这里取1100时画面稳定
		{
			if (dst[i] == '\0')
				return 0;
			buf[j++] = dst[i++];
		}
		j = 0;
		printf("%s", buf);
		fps.delay();
	}
}