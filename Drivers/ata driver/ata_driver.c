#include <stdint.h>

// Endereços de I/O para o Drive Primário (Master) do Barramento ATA
#define ATA_PORT_DATA       0x1F0 // Porta de Dados
#define ATA_PORT_ERROR      0x1F1 // Porta de Erro/Feature
#define ATA_PORT_SECTOR_CNT 0x1F2 // Contador de Setores
#define ATA_PORT_LBA_LOW    0x1F3 // LBA Endereço (Bytes 0-7)
#define ATA_PORT_LBA_MID    0x1F4 // LBA Endereço (Bytes 8-15)
#define ATA_PORT_LBA_HIGH   0x1F5 // LBA Endereço (Bytes 16-23)
#define ATA_PORT_DRIVE_SEL  0x1F6 // Seleção de Drive/LBA Mode
#define ATA_PORT_COMMAND    0x1F7 // Porta de Comando/Status

// Comando ATA para Leitura de Setor (Read Sectors with Retry)
#define ATA_CMD_READ_PIO    0x20

// Tamanho padrao de um setor
#define SECTOR_SIZE         512

// Presume funcoes outb/inb/insw para I/O de baixo nivel (em Assembly)
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);
extern void insw(uint16_t port, void* addr, uint32_t count);
extern void ui_log_status(const char *status_msg, char color_byte);

/**
 * Funcao para esperar que o disco termine de processar um comando.
 * Essencial para sincronizacao de I/O.
 */
void ata_wait_ready() {
    // 1. Espera ate que o bit BSY (Busy) caia (o drive nao esta mais ocupado)
    while (inb(ATA_PORT_COMMAND) & 0x80) { /* loop */ }
    // 2. Espera ate que o bit DRQ (Data Request) suba (pronto para transferir dados)
    while (!(inb(ATA_PORT_COMMAND) & 0x08)) { /* loop */ }
}

/**
 * Le um unico setor do disco (512 bytes) e coloca os dados no buffer.
 * @param lba_address O endereço lógico do setor a ser lido (LBA).
 * @param buffer O ponteiro para onde os 512 bytes devem ser salvos.
 * @return 0 em caso de sucesso, -1 em caso de falha.
 */
int ata_read_sector(uint32_t lba_address, uint8_t* buffer) {
    // 1. Esperar o drive ficar pronto
    ata_wait_ready();

    // 2. Enviar Endereço LBA e Contador de Setores (LBA28 Mode)
    outb(ATA_PORT_DRIVE_SEL, 0xE0 | ((lba_address >> 24) & 0x0F)); // 0xE0: Master Drive, LBA Mode
    outb(ATA_PORT_SECTOR_CNT, 1);                              // Ler 1 setor
    outb(ATA_PORT_LBA_LOW, (uint8_t)(lba_address & 0xFF));
    outb(ATA_PORT_LBA_MID, (uint8_t)((lba_address >> 8) & 0xFF));
    outb(ATA_PORT_LBA_HIGH, (uint8_t)((lba_address >> 16) & 0xFF));

    // 3. Enviar Comando de Leitura (READ PIO)
    outb(ATA_PORT_COMMAND, ATA_CMD_READ_PIO);

    // 4. Esperar que os dados estejam prontos para leitura
    ata_wait_ready();

    // 5. Ler 256 palavras (512 bytes) da porta de dados para o buffer
    // 'insw' (Input String Word) e uma instrucao de Assembly crucial.
    insw(ATA_PORT_DATA, buffer, SECTOR_SIZE / 2); // 512 bytes / 2 bytes por palavra = 256 palavras

    // 6. Checar status de erro (simplificado)
    if (inb(ATA_PORT_COMMAND) & 0x01) {
        ui_log_status("ATA ERRO: Falha na leitura do setor.", 0x0C); // Vermelho
        return -1;
    }

    return 0; // Sucesso
}

// Buffer de teste para armazenar o primeiro setor
static uint8_t boot_sector_data[SECTOR_SIZE];

/**
 * Funcao de inicializacao do Driver ATA.
 */
void init_ata_driver() {
    ui_draw_string("Driver ATA: Lendo Setor de Boot (LBA 0)...", 30, 0, 0x07);

    if (ata_read_sector(0, boot_sector_data) == 0) {
        // Se a leitura foi bem-sucedida, loga os primeiros bytes do setor de boot
        ui_log_status("ATA OK. Setor de Boot lido com sucesso.", 0x0A); // Verde
        
        // Em um OS real, voce checaria a assinatura MBR (0xAA55) aqui.
        if (boot_sector_data[510] == 0x55 && boot_sector_data[511] == 0xAA) {
             ui_log_status("Assinatura MBR encontrada (0xAA55). Disco valido.", 0x0F);
        }

    } else {
        ui_log_status("ATA ERRO: Falha ao inicializar o disco.", 0x0C);
    }
}
