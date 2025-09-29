// accessibility_service.c - Servico que integra todas as ferramentas de acessibilidade.

// Declaracoes externas para as funcoes que vao gerenciar o cursor e a fala.
// Estas funcoes estao em blue_selector_cursor.c e accessibility_talkback_logic.c.
extern void init_blue_selector();
extern void move_selector(int delta_col, int delta_row);
extern void update_cursor_and_talk(int new_row, int new_col);
extern void init_talkback_logic();
extern void putc(char c, int row, int col, char color);


// Variaveis globais de estado do servico
static int current_selection_row = 10;
static int current_selection_col = 5;

/**
 * Funcao principal para iniciar o servico de acessibilidade.
 * O kernel_main() chamaria esta funcao.
 */
void start_accessibility_service() {
    
    // 1. Inicializa o modulo de TalkBack (Logica de Buffer de Fala)
    init_talkback_logic(); 
    
    // 2. Inicializa o modulo de selecao visual (cria os itens e o cursor inicial)
    init_blue_selector();
    
    const char *status_msg = "Servico de Acessibilidade CORE ATIVO";
    for (int i = 0; status_msg[i] != '\0'; i++) {
        putc(status_msg[i], 18, i, 0x0A); // Verde claro na linha 18
    }

    // 3. Informa a logica de fala (TalkBack) qual conteudo foi selecionado inicialmente.
    // O (10, 5) e a posicao onde o cursor azul foi desenhado primeiro.
    update_cursor_and_talk(current_selection_row, current_selection_col);
}

/**
 * Funcao de manipulacao de evento (Simula o pressionar de uma tecla).
 * Esta funcao seria chamada quando o driver de teclado estivesse pronto.
 */
void handle_key_event(int key_code) {
    // Simula a tecla de seta para baixo
    if (key_code == 400) { // Codigo de exemplo para "DOWN"
        
        // 1. Move a selecao visual (highlight azul)
        move_selector(0, 1); 
        
        // Atualiza o estado interno
        current_selection_row += 1;

        // 2. Fala o novo conteudo na nova posicao
        update_cursor_and_talk(current_selection_row, current_selection_col);
    
    } 
    // Outras teclas como UP, LEFT, RIGHT seriam implementadas aqui...
}
