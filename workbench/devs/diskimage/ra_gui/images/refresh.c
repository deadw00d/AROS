/*
	Data for "refresh" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>

UWORD __chip refreshData[48] =
{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,0x8000,
	0x007F,0xC000,0x01FF,0xE000,0x03E0,0x7000,0x0B80,0x3800,
	0x1F00,0x1000,0x1F80,0x0000,0x1FC0,0x2000,0x0F80,0xF000,
	0x0F03,0xF000,0x0407,0xF800,0x0003,0xF800,0x0400,0xF800,
	0x0E01,0xD000,0x0783,0x8000,0x03FF,0x0000,0x01FE,0x0000,
	0x007C,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image refresh =
{
	0, 0,		/* LeftEdge, TopEdge */
	24, 24, 1,	/* Width, Height, Depth */
	refreshData,	/* ImageData */
	0x0001, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
