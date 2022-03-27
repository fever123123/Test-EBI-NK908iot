/**************************************************************************//**
*
* @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date            Author       Notes
* 2022-03-26       yzh         First version
*
******************************************************************************/

#include <rtconfig.h>
#include <rtdevice.h>
#include <nu_ebi.h>
#include <stdio.h>


static void ebi_open(void)
{
    EBI_Open(EBI_BANK0,EBI_BUSWIDTH_8BIT,EBI_TIMING_NORMAL,EBI_OPMODE_NORMAL,EBI_CS_ACTIVE_LOW);/*初始化ebi总线cs0*/
    EBI_SetBusTiming(EBI_BANK0,0x03003318U,EBI_MCLKDIV_128);/*TAHD=3,TACC=3*/
}
static void ebi_close(void)
{
    EBI_Close(EBI_BANK0);/*关闭ebi总线cs0*/
}
static void ebi_read(int argc, char**argv)
{
    uint32_t addr;
    uint8_t bus_val;

    sscanf(argv[1],"0x%x",&addr);
    if(addr < 0x60000000 || addr > 0x600fffff)
    {
        rt_kprintf("%s@nk980iot: illegal address.\n",argv[0],addr);
        return;
    }
    ebi_open();
    bus_val = *((volatile uint8_t *)addr);
    ebi_close();

    rt_kprintf("%s@nk980iot: addr - 0x%x, data - 0x%x\n",argv[0],addr,bus_val);
}

MSH_CMD_EXPORT(ebi_read, ebi_read sample: ebi_read <Hex_addr>);

static void ebi_write(int argc, char**argv)
{
    uint32_t addr;
    uint32_t bus_val;
/*
    for(int i; i<argc; i++)
    {
        rt_kprintf("argc:%d, argv:%s\n",i,argv[i]);
    }
*/
    sscanf(argv[1],"0x%x",&addr);
    if(addr < 0x60000000 || addr > 0x600fffff)
    {
        rt_kprintf("%s@nk980iot: illegal address.\n",argv[0],addr);
        return;
    }

    sscanf(argv[2],"0x%x",&bus_val);

    ebi_open();
    *((volatile uint8_t *)addr) = (uint8_t)bus_val;
    ebi_close();

    rt_kprintf("%s@nk980iot: addr - 0x%x, data - 0x%x\n",argv[0],addr,bus_val);
}

MSH_CMD_EXPORT(ebi_write, ebi_write sample: ebi_write <Hex_addr Hex_data>);
