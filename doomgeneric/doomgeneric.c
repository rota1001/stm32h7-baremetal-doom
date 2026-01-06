#include <stdio.h>

#include "m_argv.h"

#include "doomgeneric.h"

pixel_t* DG_ScreenBuffer = NULL; // unused

void M_FindResponseFile(void);
void D_DoomMain (void);

extern unsigned long _frame_buffer;

void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
    myargc = argc;
    myargv = argv;

	// M_FindResponseFile();

	// DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
    DG_ScreenBuffer = &_frame_buffer;

	DG_Init();

	D_DoomMain ();
}

