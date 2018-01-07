#include "stdafx.h"
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <windows.h>
int32_t width, height;
RGBQUAD *pixels;
bool OpenBitmap(char const *filename)
{
	FILE *file = fopen(filename, "rb");
	if (file)
	{
		width = 0;
		height = 0;
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;
		fread(&bf, sizeof(bf), 1, file);
		fread(&bi, sizeof(bi), 1, file);
		if (bi.biBitCount != 24)
			return false;
		if (bi.biCompression != BI_RGB)
			return false;
		width = bi.biWidth;
		height = bi.biHeight;
		pixels = new RGBQUAD[width*height];
		uint32_t rowSize = (bi.biBitCount * width + 31) / 32 * 4;
		uint8_t *line = new uint8_t[rowSize];
		for (int y = 0; y < height; y++)
		{
			fread(line, rowSize, 1, file);
			for (int x = 0; x < width; x++)
			{
				uint8_t *color = line + x * 3;
				RGBQUAD *pixel = &pixels[(height - y - 1) * width + x];
				pixel->rgbBlue = color[0];
				pixel->rgbGreen = color[1];
				pixel->rgbRed = color[2];
			}
		}
		delete[] line;
		fclose(file);
		return true;
	}
	return false;
}
RGBQUAD GetColor(int x, int y, int w, int h)
{
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < w; i++)
	{
		if (i + x >= width) continue;
		for (int j = 0; j < h; j++)
		{
			if (j + y >= height) continue;
			RGBQUAD const& color = pixels[(y + j) * width + (x + i)];
			r += color.rgbRed;
			g += color.rgbGreen;
			b += color.rgbBlue;
		}
	}
	return RGBQUAD{(BYTE) (r / (w * h)), (BYTE)(g / (w * h)),(BYTE)(b / (w * h)) };
}
char ColorToCharacter(RGBQUAD const& color)
{
	int brightness = (color.rgbRed + color.rgbGreen + color.rgbBlue) / 3;
	static char const *characters = "8O&*dboc:_. ";
	int len = strlen(characters);
	int span = 0xFF / len;
	int cidx = brightness / span;
	if (cidx == len)
		cidx--;
	return characters[cidx];
}
void OutputAscii(const char* filename, int w, int h)
{
	FILE *file = fopen(filename, "a+");
	int x = width / w;
	int y = height / h;
	for (int i = 0; i < height; i += y)
	{
		for (int j = 0; j < width; j += x)
		{
			RGBQUAD color = GetColor(j, i, x, y);
			fprintf(file, "%c", ColorToCharacter(color));
			//printf("%c", ColorToCharacter(color));
		}
		fprintf(file, "\n");
		//printf("\n");
	}
	delete[] pixels;
	fclose(file);
}
int main()
{
	char filename[1024];
	printf("转换中，请稍后~\n");
	for (int i = 1; i <= 5262; i++)
	{
		sprintf(filename, "image/badapple_%d.bmp", i);
		if (OpenBitmap(filename))
			OutputAscii("badapple.txt", width / 6, height / 12);
	}
	printf("转换完成！");
}