#include <stdint.h>
#include "kernel_base.h" // Presume putc() e outras funcoes de kernel

// Enderecos de hardware (Portas de I/O) para o Controlador de Teclado (i8042)
#define KBD_DATA_PORT   0x60 // Onde o codigo de varredura e lido
#define KBD_STATUS_PORT 0x64 // Onde o status do teclado e verificado

// Funcoes externas para a logica de permissao/cursor
extern void handle_key_event(int key_code); // Do accessibility_service.c

/**
 * Funcao de baixo nivel para ler a porta de I/O de dados do teclado.
 * Esta funcao usa Assembly Inserido para instrucoes de I/O.
 */
uint8_t read_scan_code() {
    uint8_t data;
    
    // Codigo Assembly Inserido para a instrucao 'in'
    // 'in al, dx' (Le um byte da porta de I/O especificada por DX e coloca em AL)
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a" (data)      // Saida: 'data' recebe o valor de AL
        : "Nd" (KBD_DATA_PORT) // Entrada: Porta 0x60
    );
    return data;
}

/**
 * Rotina que e chamada pelo Kernel quando uma interrupcao de teclado (IRQ1) ocorre.
 * O Kernel de verdade teria que configurar uma tabela de interrupcao (IDT) para isso.
 */
void keyboard_interrupt_handler() {
    
    // 1. Le o codigo de varredura do hardware
    uint8_t scan_code = read_scan_code();
    
    // Codigos de varredura basicos (Exemplos)
    #define SCAN_CODE_ENTER_PRESS 0x1C
    #define SCAN_CODE_UP_PRESS    0x48
    #define SCAN_CODE_DOWN_PRESS  0x50

    int high_level_code = 0; // Codigo de acao para o Servico de Acessibilidade

    // 2. Traducao de Codigo de Varredura para Acao de Alto Nivel
    if (scan_code == SCAN_CODE_ENTER_PRESS) {
        high_level_code = 13; // ENTER
    } else if (scan_code == SCAN_CODE_DOWN_PRESS) {
        high_level_code = 400; // DOWN_ARROW (Codigo personalizado)
    } else {
        // Ignora a maioria das teclas (Shift, Alt, etc.) por enquanto
        return; 
    }
    
    // 3. Entrega a Acao para o Servico de Acessibilidade (Cursor/Permissao)
    if (high_level_code != 0) {
        // Esta chamada faz seu cursor mover e aciona a logica de permissao.
        handle_key_event(high_level_code); 
        
        // Log de acao (para debug)
        putc('K', 24, 0, 0x0E); // 'K' de Key (Amarelo)
        putc(':', 24, 1, 0x0E);
        
        // (Em um OS real, voce formataria o codigo e a acao em string aqui)
    }

    // 4. ENVIA EOI (End Of Interrupt)
    // O hardware precisa saber que voce terminou de processar a interrupcao.
    // Isso e crucial para o proximo evento de teclado.
    // O codigo real e em Assembly e envolve a porta 0x20.
}

// Funcao de inicializacao: O Kernel a chama no inicio.
void init_keyboard_driver() {
    const char *title = "Driver de Teclado Ativo (IRQ1)";
    for (int i = 0; title[i] != '\0'; i++) {
        putc(title[i], 20, 0, 0x0B); // Azul claro
    }
    
    // O Kernel de verdade configuraria a IDT aqui para apontar para keyboard_interrupt_handler()
}
