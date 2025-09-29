// accessibility_talkback_logic.c - Simula o Servico de Leitor de Tela (TalkBack)

// Presume que o Kernel tem acesso direto a memoria de video
#define VIDEO_MEMORY_START 0xb8000

// Buffer onde o texto a ser 'falado' sera armazenado.
// Este e o nosso "fila de fala" simulada.
static char speech_buffer[100];
static int buffer_index = 0;

// Funcao que 'fala' o conteudo lido (na verdade, apenas registra o que seria falado)
void speak_buffer() {
    // Esta funcao seria o ponto de injecao do Driver de Audio e do TTS.
    
    // Por enquanto, apenas esvazia o buffer e imprime um status
    if (buffer_index > 0) {
        // Encerra a string
        speech_buffer[buffer_index] = '\0'; 
        
        // Simula a fala imprimindo o texto em uma area de log do Kernel (linha 22)
        const char *log_prefix = "FALA: ";
        int log_col = 0;
        
        // Limpa a linha de log (simulacao)
        for(int i = 0; i < 80; i++) putc(' ', 22, i, 0x00);
        
        // Imprime o prefixo de log
        for (int i = 0; log_prefix[i] != '\0'; i++) {
             putc(log_prefix[i], 22, log_col + i, 0x0F);
        }
        log_col += 6;
        
        // Imprime o conteudo do buffer de fala
        for (int i = 0; speech_buffer[i] != '\0'; i++) {
            putc(speech_buffer[i], 22, log_col + i, 0x0F); 
        }

        buffer_index = 0; // Reseta o buffer apos a 'fala'
    }
}

// Funcao central: Le o caractere na posicao do cursor e o coloca no buffer de fala
void read_and_speak_cursor_content(int row, int col) {
    // Reseta o buffer antes de ler a nova informacao
    buffer_index = 0; 

    // 1. Le o caractere na memoria de video (na posicao do cursor)
    unsigned char* video_memory = (unsigned char*)VIDEO_MEMORY_START;
    int offset = (row * 80 + col) * 2;
    char current_char = video_memory[offset];

    // 2. Coloca o caractere lido no buffer
    if (current_char != ' ' && current_char != '\0') {
        speech_buffer[buffer_index++] = ' '; // Fala "caractere"
        speech_buffer[buffer_index++] = current_char; 
        
        // 3. Simula um texto de contexto (ex: "X na coluna 0")
        const char *context = " na tela.";
        for (int i = 0; context[i] != '\0'; i++) {
            if (buffer_index < 99) {
                speech_buffer[buffer_index++] = context[i];
            }
        }
        
        // 4. Executa a 'fala' (imprime o log)
        speak_buffer();
    }
}

// Funcao de logica: Atualiza o cursor e fala o conteudo
// Pode ser chamada apos a funcao 'move_cursor'
void update_cursor_and_talk(int new_row, int new_col) {
    // Esta funcao faria:
    // 1. clear_cursor(); (Limpar o antigo)
    // 2. cursor_row = new_row;
    // 3. cursor_col = new_col;
    // 4. draw_cursor(); (Desenhar o novo)
    
    // 5. O novo passo de acessibilidade:
    read_and_speak_cursor_content(new_row, new_col);
}

// Funcao de inicializacao que o Kernel chamaria
void init_talkback_logic() {
    const char *title = "Logica TalkBack Ativa (Checando linha 10, coluna 0)";
    for (int i = 0; title[i] != '\0'; i++) {
        putc(title[i], 14, i, 0x0C); // Vermelho Claro
    }

    // Exemplo: Simula que o usuario moveu o cursor para a posicao (10, 0)
    update_cursor_and_talk(10, 0); 
}
