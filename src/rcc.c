#include <rcc.h>

void pll3_init(void)
{
    RCC_CR &= ~RCC_CR_PLL3ON;
    while (RCC_CR & RCC_CR_PLL3RDY)
        ;

    RCC_PLL3DIVR = ((31 - 1) << 0) | ((20 - 1) << 24);
    RCC_PLL3FRACR = 3840 << 3;

    RCC_PLLCKSELR &= ~RCC_PLLCKSELR_DIVM3_MASK;
    RCC_PLLCKSELR |= (4 << 20);
    RCC_PLLCFGR |= RCC_PLLCFGR_DIVR3EN | RCC_PLLCFGR_PLL3FRACEN;
    RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL3RGE_MASK;
    RCC_PLLCFGR |= RCC_PLL3VCIRANGE_3;
    RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL3VCOSEL;

    RCC_CR |= RCC_CR_PLL3ON;
    while (!(RCC_CR & RCC_CR_PLL3RDY))
        ;
}

void rcc_init(void)
{
    RCC_APB4ENR |= RCC_APB4ENR_PWREN;
    PWR_CR3 |= PWR_CR3_LDOEN;
    while (!(PWR_CSR1 & PWR_CSR1_ACTVOSRDY))
        ;

    PWR_D3CR &= ~PWR_D3CR_VOS_MASK;
    PWR_D3CR |= PWR_D3CR_VOS_SCALE1;
    while (!(PWR_CSR1 & PWR_CSR1_ACTVOSRDY))
        ;

    FLASH_ACR = 0x38;
    while ((FLASH_ACR & 0xF) != 8)
        ;

    RCC_CR &= ~RCC_CR_HSIDIV_MASK;
    RCC_CR |= RCC_CR_HSION;
    while (!(RCC_CR & RCC_CR_HSIRDY))
        ;

    RCC_CR &= ~RCC_CR_PLL1ON;
    while (RCC_CR & RCC_CR_PLL1RDY)
        ;

    RCC_PLLCKSELR &= ~(RCC_PLLCKSELR_PLLSRC_MASK | RCC_PLLCKSELR_DIVM1_MASK);
    RCC_PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSI | (4 << 4);

    RCC_PLL1DIVR = ((40 - 1) << 0) |  // N
                   ((2 - 1) << 9) |   // P
                   ((2 - 1) << 16) |  // Q
                   ((2 - 1) << 24);   // R

    RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL1FRACEN;
    RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL1RGE_MASK;
    RCC_PLLCFGR |= RCC_PLL1VCIRANGE_3;

    RCC_PLLCFGR |=
        RCC_PLLCFGR_DIVP1EN | RCC_PLLCFGR_DIVQ1EN | RCC_PLLCFGR_DIVR1EN;

    RCC_CR |= RCC_CR_PLL1ON;
    while (!(RCC_CR & RCC_CR_PLL1RDY))
        ;

    RCC_D1CFGR = (0x8UL << 0) | (0x4UL << 4);
    RCC_D2CFGR = (0x4UL << 4) | (0x4UL << 8);
    RCC_D3CFGR = (0x4UL << 4);

    RCC_CFGR &= ~RCC_CFGR_SW_MASK;
    RCC_CFGR |= RCC_CFGR_SW_PLL1;
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL1)
        ;
}
