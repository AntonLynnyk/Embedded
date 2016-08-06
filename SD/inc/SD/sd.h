#define SD_BLOCK_SIZE 512

#define CMD0 GO_IDLE_STATE
#define CMD1 SEND_OP_COND

#define CMD0_GO_IDLE_STATE
#define CMD1_SEND_OP_COND
#define CMD9_SEND_CSD
#define CMD10_SEND_CID
#define CMD12_STOP_TRANSMISSION
#define CMD13_SEND_STATUS
#define CMD16_SET_BLOCKLEN
#define CMD17_READ_SINGLE_BLOCK
#define CMD18_READ_MULTIPLE_BLOCK
#define CMD23_SET_BLOCK_COUNT
#define CMD24_WRITE_BLOCK
#define CMD25_WRITE_MULTIPLE_BLOCK
#define CMD27_PROGRAM_CSD
#define CMD58_READ_OCR

void SD_Initialize(void);
void SD_ReadCIDRegister(uint8_t dataBlock[16]);
void SD_WriteDataBlock(const uint8_t writeBlock[], uint32_t sector, uint32_t writeBytesCount);
int SD_ReadDataBlock(uint8_t readBlock[], uint32_t sector, uint16_t offset, uint16_t readBytesCount);

enum SD_R1
{
    In_Idle_State        = 1 << 0,
    Erase_Reset          = 1 << 1,
    Illegal_Command      = 1 << 2,
    CRC_Error            = 1 << 3,
    Erase_Sequence_Error = 1 << 4,
    Address_Error        = 1 << 5,
    Parameter_Error      = 1 << 6,
    Start_Bit            = 1 << 7 // Always zero
};
