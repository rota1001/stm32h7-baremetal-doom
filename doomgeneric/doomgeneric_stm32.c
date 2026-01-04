#include "doomkeys.h"
#include "doomgeneric.h"

void DG_Init()
{
    printf("DG_Init\r\n");
}


void DG_DrawFrame()
{
    printf("DG_DrawFrame not implemented\r\n");
    while (1);
}

void DG_SleepMs(uint32_t ms)
{
    printf("DG_SleepMs not implemented\r\n");
    while (1);
}

uint32_t DG_GetTicksMs()
{
    printf("DG_GetTicksMs not implemented\r\n");
    while (1);
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    printf("DG_GetKey not implemented\r\n");
    while (1);
	return 0;
}

void DG_SetWindowTitle(const char * title)
{
    // printf("DG_SetWindowTitle not implemented\r\n");
    while (1);
}

int main(int argc, char **argv)
{
    // while (1) {
    //     printf("DOOM main\n");
    // }
    doomgeneric_Create(argc, argv);
    while (1) {
        printf("DOOM main\n");
    }
    while(1)
    {
      doomgeneric_Tick(); 
    }

    return 0;
}
