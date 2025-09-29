// Dentro de app_loader.c, a funcao de lancamento seria:

extern int check_and_request_permission(const char* app_name, int resource_id);

void launch_application(const char* app_name) {
    
    // PASSO NOVO E CRUCIAL: Verificar a permissao antes de carregar!
    if (check_and_request_permission(app_name, RESOURCE_ID_DISK_IO) == 0) {
        // Permissao negada! Exibe erro e retorna ao Kernel
        putc('D', 20, 0, 0x0C); 
        putc('E', 20, 1, 0x0C);
        putc('N', 20, 2, 0x0C);
        putc('I', 20, 3, 0x0C);
        putc('E', 20, 4, 0x0C);
        putc('D', 20, 5, 0x0C);
        return; 
    }
    
    // ... RESTANTE DA LOGICA DE CARREGAMENTO DE ARQUIVO (read_from_disk) ...

    // Se a permissao foi concedida (retornou 1):
    // 1. Carrega o arquivo
    // 2. Salta para o codigo do App na memoria
}
