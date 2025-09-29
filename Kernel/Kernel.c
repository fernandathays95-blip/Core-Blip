// kernel.c - O Ponto de Entrada Reutilizavel para todos os seus SOs.

// A unica funcao que interage com o hardware de video.
void putc(char c, int row, int col, char color) {
    unsigned char* video_memory = (unsigned char*)0xb8000;
    int offset = (row * 80 + col) * 2;
    
    video_memory[offset] = c;
    video_memory[offset + 1] = color;
}

// A funcao principal do seu Core.
// Tudo que esta aqui deve ser generico e necessario para *qualquer* SO.
void kernel_main() {
    
    // Imprime a mensagem central do seu framework de boot.
    const char *message = "Core-Blip (Base de SO) Carregado. Pronto para iniciar o Sistema Operacional.";
    int row = 1;
    
    for (int i = 0; message[i] != '\0'; i++) {
        putc(message[i], row, i, 0x07); // 0x07 = cor cinza claro
    }
    
    // Ponto de injeção futuro: A partir daqui, voce chamaria a funcao 
    // especifica do seu SO (ex: 'blipOS_start()', 'minervaOS_start()').
    
    putc('>', 3, 0, 0x0E); // 'K' Amarelo para indicar um prompt.

    // Loop infinito para manter o Core vivo e esperando por interrupcoes.
    while (1) {
        // Nada aqui. A CPU ira aguardar.
    }
}
