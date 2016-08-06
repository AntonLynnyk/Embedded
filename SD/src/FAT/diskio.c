/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include <stddef.h>
#include <stdbool.h>
#include "diskio.h"
#include "sd.h"

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
    SD_Initialize();
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* buff,		/* Pointer to the destination object */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count (bit15:destination) */
)
{
    if (buff == NULL)
    {
        return RES_NOTRDY;
    }

    if (SD_ReadDataBlock(buff, sector, offset, count))
    {
        return RES_ERROR;
    }

	return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
	const BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		        /* Sector number (LBA) or Number of bytes to send */
)
{
    static uint16_t sectorNumber = 0;

	if (!buff)
	{
		if (sc)
        {
            sectorNumber = sc;
		}
		else
		{
            sectorNumber = 0;
		}

		return RES_OK;
	}
	else
	{
        SD_WriteDataBlock(buff, sectorNumber, sc);
	}

	return RES_OK;
}

