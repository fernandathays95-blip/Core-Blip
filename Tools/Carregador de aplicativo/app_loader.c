// app_loader.c - Rotina para carregar e iniciar um novo programa.

// Define onde o programa sera carregado na memoria (Endereco de carga).
// Um Kernel real usaria gerenciamento de memoria virtual para isso.
#define APP_LOAD_ADDRESS 0x200000 
// Tamanho maximo do aplicativo em bytes
#define MAX_APP_SIZE     4096 


// Simula a funcao de leitura de disco
// No seu Core-Blip, esta rotina usaria a interrupcao INT 0x13 da BIOS.
// Aqui, apenas copiamos dados simulados.
int read_from_disk(const char* filename, char* buffer, int max_size) {
    if (filename[0] == 'A' && filename[1] == 'P' && filename[2] == 'P') {
        // Simula o codigo do aplicativo sendo lido do disco
        const char *simulated_app_code = "APP_START: Iniciado o utilitario de teste! APP_END.";
        int len = 0;
        
        while (simulated_app_code[len] != '\0' && len < max_size - 1) {
            buffer[len] = simulated_app_code[len];
            len++;
        }
        buffer[len] = '\0'; // Garantir que e' uma string terminada em null
        return len;
    }
    return -1; // Arquivo nao encontrado
}

/**
 * Funcao principal: Carrega um programa e "salta" para ele.
 * O Kernel chamaria esta funcao ao executar um novo comando.
 */
void launch_application(const char* app_name) {
    
    char *load_target = (char*)APP_LOAD_ADDRESS;
    
    // 1. Logica de leitura de disco
    int size = read_from_disk(app_name, load_target, MAX_APP_SIZE);
    
    if (size > 0) {
        // Log de sucesso (usa a funcao de impressao do Kernel)
        putc('L', 20, 0, 0x0A); // 'L' Verde para "Launched"
        putc('O', 20, 1, 0x0A);
        putc('A', 20, 2, 0x0A);
        putc('D', 20, 3, 0x0A);
        
        const char *success_msg = "App carregado. Tamanho: ";
        // (Em um Kernel real, voce formataria 'size' em string aqui)
        
        // 2. Transfere o Controle (O Ponto Chave)
        
        // Em um Kernel real, voce usaria Assembly para:
        // a) Configurar a pilha do novo aplicativo.
        // b) Salvar o estado do Kernel.
        // c) Fazer um 'jmp' (salto) para o endereco APP_LOAD_ADDRESS.
        
        // SIMULACAO: Mostra o conteudo carregado e termina.
        int col = 0;
        char *app_data = (char*)APP_LOAD_ADDRESS;
        
        while(*app_data != '\0' && col < 50) {
            putc(*app_data, 21, col++, 0x0F);
            app_data++;
        }
        
    } else {
        putc('E', 20, 0, 0x0C); // 'E' Vermelho para Erro
        putc('R', 20, 1, 0x0C);
        putc('R', 20, 2, 0x0C);
        putc('O', 20, 3, 0x0C);
    }
}

// Funcao de inicializacao que o Kernel chamaria (Exemplo de uso)
void init_app_loader() {
    launch_application("APP_TEST");
}
