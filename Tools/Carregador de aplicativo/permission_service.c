// permission_service.c - Implementa a logica de Permissoes em Tempo de Execucao do Core-Blip.

// Presume que funcoes de UI e escrita na tela estao disponiveis
extern void putc(char c, int row, int col, char color);
extern void move_selector(int delta_col, int delta_row); // Usado para a escolha

// Define o recurso de exemplo que o aplicativo quer acessar
#define RESOURCE_ID_DISK_IO 1 
#define RESOURCE_ID_NETWORK 2 

// Variaveis de estado de permissao
static int permission_granted = 0;
static int current_selection_row = 15; // Linha da opcao 'Permitir'

/**
 * Funcao central que verifica o manifesto e aciona o pop-up de permissao.
 * O App Loader chamaria esta funcao antes de saltar para o codigo do App.
 */
int check_and_request_permission(const char* app_name, int resource_id) {
    
    // Se ja foi concedida (simulacao de cache)
    if (permission_granted) return 1;

    // 1. Exibe a Notificacao de Seguranca do Kernel (Pop-up)
    const char *prompt_title = "== SOLICITACAO DE PERMISSAO ==";
    const char *prompt_app = "Aplicativo: ";
    const char *prompt_resource = "Recurso:";
    const char *resource_name = (resource_id == RESOURCE_ID_DISK_IO) ? "ESCRITA EM DISCO" : "REDE";

    // Desenha o titulo (Fundo Preto, Texto Amarelo - 0x0E)
    int row = 13;
    for (int i = 0; prompt_title[i] != '\0'; i++) {
        putc(prompt_title[i], row, 15 + i, 0x0E);
    }
    
    // Desenha a mensagem e o recurso solicitado (Linha 14)
    // (Simplificado, em um OS real usariamos uma funcao de escrita de string)
    putc('>', 14, 15, 0x07); 
    putc(' ', 14, 16, 0x07);
    putc('P', 14, 17, 0x0F);
    putc('r', 14, 18, 0x0F);
    putc('o', 14, 19, 0x0F);
    putc('g', 14, 20, 0x0F);
    putc('r', 14, 21, 0x0F);
    putc('a', 14, 22, 0x0F);
    putc('m', 14, 23, 0x0F);
    putc('a', 14, 24, 0x0F);
    putc(' ', 14, 25, 0x0F);
    putc(' ', 14, 26, 0x0F);
    putc(resource_name[0], 14, 27, 0x0C); // Vermelho para recurso perigoso

    // 2. Opcoes de Escolha (Permitir/Ignorar)
    putc('P', 15, 20, 0x1F); // Opcao 'Permitir' com destaque AZUL (0x1F)
    putc('e', 15, 21, 0x1F); 
    putc('r', 15, 22, 0x1F); 
    putc('m', 15, 23, 0x1F); 
    putc('i', 15, 24, 0x1F); 
    putc('t', 15, 25, 0x1F); 
    putc('i', 15, 26, 0x1F); 
    putc('r', 15, 27, 0x1F); 

    putc('I', 16, 20, 0x07); // Opcao 'Ignorar' (Cor Padrao)
    putc('g', 16, 21, 0x07); 
    putc('n', 16, 22, 0x07); 
    putc('o', 16, 23, 0x07); 
    putc('r', 16, 24, 0x07); 
    putc('a', 16, 25, 0x07); 
    putc('r', 16, 26, 0x07); 

    // 3. Esperar pela entrada do usuario (Simplificacao)
    // Em um Kernel real, o sistema entraria em um loop de interrupcao de teclado aqui.
    
    // SIMULACAO: Assumimos que o usuario seleciona 'Permitir'
    // Na sua proxima versao, o 'handle_key_event' do seu servico de acessibilidade
    // faria o 'move_selector' para mudar a linha (15 ou 16)
    
    permission_granted = 1; // Permissao concedida para o exemplo

    // Limpar o pop-up apos a decisao (simulacao)
    for(int r = 13; r <= 16; r++) {
        for(int c = 15; c < 50; c++) putc(' ', r, c, 0x00);
    }

    return permission_granted;
}
