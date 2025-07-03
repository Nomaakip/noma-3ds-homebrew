#include <citro2d.h>
#include <3ds.h>
#include <stdio.h>
#include <string.h>

C2D_TextBuf g_staticBuf, g_dynamicBuf;
C2D_Text g_staticText[3];

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

int main(int argc, char **argv)
{
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C2D_SpriteSheet sheet = C2D_SpriteSheetLoad("romfs:/gfx/a.t3x");
	C2D_Image pfp, carSwitch;
	pfp = C2D_SpriteSheetGetImage(sheet, 0);
	carSwitch = C2D_SpriteSheetGetImage(sheet, 1);

	g_staticBuf = C2D_TextBufNew(4096);
	g_dynamicBuf = C2D_TextBufNew(4096);

	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	C2D_TextParse(&g_staticText[0], g_staticBuf, "colored text hahhahahah");
	C2D_TextParse(&g_staticText[1], g_staticBuf, "this is peak");
	C2D_TextParse(&g_staticText[2], g_staticBuf, "i think");

	C2D_TextOptimize(&g_staticText[0]);
	C2D_TextOptimize(&g_staticText[1]);
	C2D_TextOptimize(&g_staticText[2]);

	u32 clrClear = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrBlack = C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f);

	u32 clrRec1 = C2D_Color32(0x9A, 0x6C, 0xB9, 0xFF);
	u32 clrRec2 = C2D_Color32(0xFF, 0xFF, 0x2C, 0xFF);
	/* u32 clrRec3 = C2D_Color32(0xD8, 0xF6, 0x0F, 0xFF);
	u32 clrRec4 = C2D_Color32(0x40, 0xEA, 0x87, 0xFF); */

	while (aptMainLoop())
	{
		hidScanInput();
		if (hidKeysDown() & KEY_START)
			break;

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);
		// C2D_DrawRectangle(0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, clrRec1, clrRec2, clrRec3, clrRec4);
		C2D_DrawImageAt(carSwitch, 40.0f, 20.0f, 0.0f, NULL, 0.5f, 0.5f);

		C2D_TargetClear(bottom, clrBlack);
		C2D_SceneBegin(bottom);

		C2D_DrawText(&g_staticText[0], C2D_WithColor, 5, 6.0f, 0.0f, 1.0f, 1.0f, clrRec1);
		C2D_DrawText(&g_staticText[1], C2D_WithColor, 5, 30.0f, 0.0f, 1.0f, 1.0f, clrClear);
		C2D_DrawText(&g_staticText[2], C2D_WithColor, 5, 60.0f, 0.0f, 1.0f, 1.0f, clrClear);

		touchPosition touch;
		hidTouchRead(&touch);

		C2D_TextBufClear(g_dynamicBuf);

		char touchText[64];
		C2D_Text g_touchC2DText;

		if (touch.px >= 5 && touch.px <= 55 && touch.py >= 100 && touch.py <= 150)
		{
			snprintf(touchText, sizeof(touchText), "hi");
		}
		
		C2D_TextParse(&g_touchC2DText, g_dynamicBuf, touchText);
		C2D_TextOptimize(&g_touchC2DText);
		C2D_DrawText(&g_touchC2DText, C2D_WithColor, 5, 200.0f, 0.0f, 1.0f, 1.0f, clrRec2);

		C2D_DrawImageAt(pfp, 5.0f, 100.0f, 0.0f, NULL, 1.0f, 1.0f);

		C3D_FrameEnd(0);
		gspWaitForVBlank();
	}

	C2D_TextBufDelete(g_staticBuf);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
