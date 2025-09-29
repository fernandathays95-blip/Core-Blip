// ui_control.c - Servico centralizado para todo o desenho de interface.

#include <stdint.h>

// Endereco de memoria de video (VGA Text Mode)
#define VIDEO_MEMORY_START 0xb8000

// =======================================================
// Funcoes de Controle de Baixo Nivel
// =======================================================

/**
 * Funcao de baixo nivel: Escreve um caractere na posicao exata.
 * ESTA E A UNICA FUNCAO QUE TOCA DIRETAMENTE NA MEMORIA DE VIDEO.
 */
void put_char(char c, int row, int col, char color_byte) {
    unsigned char* video_memory = (unsigned char*)VIDEO_MEMORY_START;
    int offset = (row * 80 + col) * 2;
    
    video_memory[offset] = c;
    video_memory[offset + 1] = color_byte;
}

/**
 * Funcao de controle: Limpa a tela.
 */
void ui_clear_screen() {
    for (int row = 0; row < 25; row++) {
        for (int col = 0; col < 80; col++) {
            put_char(' ', row, col, 0x00); // Fundo Preto
        }
    }
}

// =======================================================
// Funcoes de Desenho de Alto Nivel (Usadas pelos Modulos)
// =======================================================

/**
 * Funcao de desenho: Desenha uma string em uma linha/coluna.
 */
void ui_draw_string(const char *str, int row, int col, char color_byte) {
    int i = 0;
    while (str[i] != '\0') {
        put_char(str[i], row, col + i, color_byte);
        i++;
    }
}

/**
 * Funcao de utilidade: Desenha uma linha de separacao/borda.
 */
void ui_draw_separator(int row, char color_byte) {
    for (int col = 0; col < 80; col++) {
        put_char('-', row, col, color_byte);
    }
}

/**
 * Funcao de Log: Exibe uma mensagem de status/erro do sistema (Linha 24).
 */
void ui_log_status(const char *status_msg, char color_byte) {
    // 1. Limpa a linha de log
    for(int c = 0; c < 80; c++) put_char(' ', 24, c, 0x00);
    
    // 2. Escreve a nova mensagem
    ui_draw_string("[STATUS] ", 24, 0, 0x07); // Prefixo cinza
    ui_draw_string(status_msg, 24, 9, color_byte);
}
