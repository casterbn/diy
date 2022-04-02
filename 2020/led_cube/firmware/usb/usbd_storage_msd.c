#include <string.h>
#include "usbd_msc_mem.h"
#include "platform.h"

#define STORAGE_LUN_NBR                  1 
const int8_t STORAGE_Inquirydata[] = {0x00, 0x80, 0x02, 0x02,
    (USBD_STD_INQUIRY_LENGTH - 5), 0x00, 0x00, 0x00, 'S', 'T', 'M', ' ', ' ',
    ' ', ' ', ' ', 'm', 'i', 'c', 'r', 'o', 'S', 'D', ' ', 'F', 'l', 'a', 's',
    'h', ' ', ' ', ' ', '1', '.', '0', '0', };
__IO uint32_t count = 0;
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t* block_num,
    uint32_t* block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);
USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops = {
    //
    STORAGE_Init, STORAGE_GetCapacity, STORAGE_IsReady,
    STORAGE_IsWriteProtected, STORAGE_Read, STORAGE_Write, STORAGE_GetMaxLun,
    (int8_t*)STORAGE_Inquirydata    //
    };
USBD_STORAGE_cb_TypeDef* USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;

int8_t STORAGE_Init(uint8_t lun)
{
    return (0);
}

int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t* block_num,
    uint32_t* block_size)
{
    *block_size = 4096;
    *block_num = 2048;
    return (0);
}

int8_t STORAGE_IsReady(uint8_t lun)
{
//    static uint8_t status = 0;
    return (0);
}

int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
    return 0;
}

int8_t STORAGE_Read(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len)
{
    SPIFLASH_FastRead(blk_addr * 4096, blk_len * 4096, buf);
    return 0;
}

int8_t STORAGE_Write(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len)
{
    SPIFLASH_SectorErase(blk_addr * 4096);
    SPIFLASH_Write(blk_addr * 4096, blk_len * 4096, buf);
    return 0;
}

int8_t STORAGE_GetMaxLun(void)
{
    return (STORAGE_LUN_NBR - 1);
}

