#include <stdint.h>
#include "kernel_base.h" // Presume que putc() e kernel_main() estao definidos em algum lugar

// Funcao para ler o Time-Stamp Counter (TSC) da CPU.
// O TSC e um registrador de 64 bits que conta os ciclos da CPU desde o boot.
// Esta e uma forma de interagir diretamente com o hardware.
uint64_t read_tsc() {
    uint32_t high, low;
    
    // Codigo Assembly Inserido (inline assembly) para a instrucao RDTSC
    // 'rdtsc' (Read Time-Stamp Counter) coloca o valor em EDX:EAX
    __asm__ __volatile__ (
        "rdtsc" : "=a"(low), "=d"(high)
    );
    
    // Combina os 32 bits altos e baixos em um uint64_t
    return ((uint64_t)high << 32) | low;
}

// Funcao que formata e exibe o valor do TSC
void display_cpu_cycles(int row, int col) {
    uint64_t cycles = read_tsc();
    
    // Buffer para armazenar a string formatada
    char buffer[20]; 
    int i = 19;
    
    // Converte o numero (cycles) para uma string (decimal)
    buffer[i--] = '\0';
    do {
        buffer[i--] = (cycles % 10) + '0';
        cycles /= 10;
    } while (cycles > 0);

    // Salta o ponteiro de volta para o inicio da string no buffer
    const char *str = &buffer[i + 1];

    // Exibe a string de diagnostico
    putc('C', row, col, 0x0E); // 'C' (Amarelo) para "Cycles"
    putc('P', row, col + 1, 0x0E);
    putc('U', row, col + 2, 0x0E);
    putc(':', row, col + 4, 0x0A); // ':' (Verde)
    
    int j = 0;
    while (str[j] != '\0') {
        putc(str[j], row, col + 6 + j, 0x0F); // Numero (Branco)
        j++;
    }
}

// Esta funcao seria chamada dentro do seu kernel_main()
void init_cpu_monitor() {
    const char *title = "Monitor de Diagnostico CPU v1.0";
    for (int i = 0; title[i] != '\0'; i++) {
        putc(title[i], 5, i, 0x0B); // Azul claro
    }
    
    // Exibe a leitura da CPU na linha 7
    display_cpu_cycles(7, 0); 
}
