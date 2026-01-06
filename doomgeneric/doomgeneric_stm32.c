#include "doomkeys.h"
#include "doomgeneric.h"
#include "regs.h"

extern unsigned long systick_cnt;

void DG_Init()
{
    printf("DG_Init\r\n");
}


void DG_DrawFrame()
{
    printf("DG_DrawFrame not implemented\r\n");
    // while (1);
}

void DG_SleepMs(uint32_t ms)
{
    uint32_t cur = systick_cnt;
    while (systick_cnt - cur <= ms);
    return;
}

uint32_t DG_GetTicksMs()
{
    return systick_cnt;
}

int up = 0, down = 0, left = 0, right = 0, enter = 0, fire = 0;

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    if (((GPIOD->IDR >> 8) & 1) != up) {
        up ^= 1;
        *pressed = up;
        *doomKey = KEY_UPARROW;
        return 1;
    }
    if (((GPIOD->IDR >> 9) & 1) != down) {
        down ^= 1;
        *pressed = down;
        *doomKey = KEY_DOWNARROW;
        return 1;
    }
    if (((GPIOD->IDR >> 10) & 1) != left) {
        left ^= 1;
        *pressed = left;
        *doomKey = KEY_LEFTARROW;
        return 1;
    }
    if (((GPIOD->IDR >> 14) & 1) != right) {
        right ^= 1;
        *pressed = right;
        *doomKey = KEY_RIGHTARROW;
        return 1;
    }
    if (((GPIOD->IDR >> 15) & 1) != enter) {
        enter ^= 1;
        *pressed = enter;
        *doomKey = KEY_ENTER;
        return 1;
    }
    if (((GPIOB->IDR >> 15) & 1) != fire) {
        fire ^= 1;
        *pressed = fire;
        *doomKey = KEY_FIRE;
        return 1;
    }
	return 0;
}

void DG_SetWindowTitle(const char * title)
{
    printf("DG_SetWindowTitle not implemented\n");
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);
    while(1)
    {
      doomgeneric_Tick(); 
    }

    return 0;
}
