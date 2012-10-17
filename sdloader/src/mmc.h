#define MMC_BLOCK_SIZE 512

int mmc_init(void);
int mmc_read_block(uint16_t block_number, uint8_t* block_address);
void print_block(uint8_t *block_address, uint16_t length);
int mmc_if_init(void);
