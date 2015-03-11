// freeimageEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdl.h>
#include <iostream>

#include "FreeImage.h"

#ifdef _DEBUG
#pragma comment(lib, "freeimaged.lib")
#else
#pragma comment(lib, "freeimage.lib")
#endif

#pragma comment(lib, "sdl2.lib")

// ----------------------------------------------------------

fi_handle g_load_address;

// ----------------------------------------------------------

inline unsigned _stdcall
_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	BYTE *tmp = (BYTE *)buffer;

	for (unsigned c = 0; c < count; c++) {
		memcpy(tmp, g_load_address, size);

		g_load_address = (BYTE *)g_load_address + size;

		tmp += size;
	}

	return count;
}

inline unsigned _stdcall
_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	// there's not much use for saving the bitmap into memory now, is there?

	return size;
}

inline int _stdcall
_SeekProc(fi_handle handle, long offset, int origin) {
	assert(origin != SEEK_END);

	if (origin == SEEK_SET) {
		g_load_address = (BYTE *)handle + offset;
	}
	else {
		g_load_address = (BYTE *)g_load_address + offset;
	}

	return 0;
}

inline long _stdcall
_TellProc(fi_handle handle) {
	assert((int)handle > (int)g_load_address);

	return ((int)g_load_address - (int)handle);
}

int invert_image(int pitch, int height, void* image_pixels)
{
	int index;
	void* temp_row;
	int height_div_2;

	temp_row = (void *)malloc(pitch);
	if (NULL == temp_row)
	{
		SDL_SetError("Not enough memory for image inversion");
		return -1;
	}
	//if height is odd, don't need to swap middle row 
	height_div_2 = (int)(height * .5);
	for (index = 0; index < height_div_2; index++)    {
		//uses string.h 
		memcpy((Uint8 *)temp_row,
			(Uint8 *)(image_pixels)+
			pitch * index,
			pitch);

		memcpy(
			(Uint8 *)(image_pixels)+
			pitch * index,
			(Uint8 *)(image_pixels)+
			pitch * (height - index - 1),
			pitch);
		memcpy(
			(Uint8 *)(image_pixels)+
			pitch * (height - index - 1),
			temp_row,
			pitch);
	}
	free(temp_row);
	return 0;
}

//This is the function you want to call! 
int SDL_InvertSurface(SDL_Surface* image)
{
	if (NULL == image)
	{
		SDL_SetError("Surface is NULL");
		return -1;
	}
	return(invert_image(image->pitch, image->h,
		image->pixels));
}

SDL_Surface *FiBitmapToSdlSurface(FIBITMAP *fb) {
	int h = FreeImage_GetHeight(fb), w = FreeImage_GetWidth(fb);
	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(FreeImage_GetBits(fb),
		w, h, 32, w*4 , 0xff0000, 0x00ff00, 0x0000ff, 0);
	return surf;
}


int _tmain(int argc, _TCHAR* argv[])
{
	FreeImageIO io;

	io.read_proc = _ReadProc;
	io.write_proc = _WriteProc;
	io.tell_proc = _TellProc;
	io.seek_proc = _SeekProc;

	//SDL 초기화.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 0;
	}
	SDL_Window *pWindow;
	SDL_Renderer *pRenderer;
	if (SDL_CreateWindowAndRenderer(640, 480, 0, &pWindow, &pRenderer) < 0)
	{
		std::cout << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
		return 0;
	}
	
	FIBITMAP *dib = NULL;
	
	FILE *file = fopen("test1.png", "rb");
	fseek(file, 0, SEEK_END);	
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	BYTE *pImageBuffer = new BYTE[fileSize];

	fread(pImageBuffer, fileSize, 1, file);
	fclose(file);
	
	g_load_address = pImageBuffer;
	dib = FreeImage_LoadFromHandle(FIF_PNG, &io, (fi_handle)pImageBuffer);

	SDL_Surface* pSurface = FiBitmapToSdlSurface(dib);
	SDL_InvertSurface(pSurface);

	SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);	
	SDL_FreeSurface(pSurface);

	FreeImage_Unload(dib);
	delete[] pImageBuffer;

	if (pTexture == 0)
	{
		SDL_DestroyRenderer(pRenderer);
		SDL_DestroyWindow(pWindow);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return 0;
	}

	bool running = true;

	// 사용자 입력을 받는 코드이다.
	//윈도우의 경우 Escape 키를 누르면 해당 루프를 벗어날 수 있다.
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}
		//화면 클리어
		SDL_RenderClear(pRenderer);
		//렌더러에 텍스쳐를 카피
		SDL_Rect src;
		src.h = 128;
		src.w = 64;
		src.x = 0;
		src.y = 0;
		SDL_RenderCopy(pRenderer, pTexture, &src, &src);
		//화면을 출력한다.
		SDL_RenderPresent(pRenderer);
	}

	//생성하 객체를 모두 제거한다.	
	SDL_DestroyTexture(pTexture);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}