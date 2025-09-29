// blue_selector_cursor.c - Implementacao do cursor de selecao azul (Highlight).

// Presume-se que 'putc' esta disponivel
extern void putc(char c, int row, int col, char color);

// Cores usadas:
#define DEFAULT_COLOR 0x07 // Fundo Preto (0), Texto Branco (7)
#define SELECT_COLOR  0x1F // Fundo Azul (1), Texto Branco (F)

// Variaveis globais do cursor de selecao
static int selector_row = 10;
static int selector_col = 0;

/**
 * Funcao para aplicar o highlight azul na posicao atual do cursor.
 */
void highlight_cursor() {
    // 1. Le o caractere que esta na posicao do cursor
    unsigned char* video_memory = (unsigned char*)0xb8000;
    int offset = (selector_row * 80 + selector_col) * 2;
    char current_char = video_memory[offset];

    // 2. Desenha o caractere com o destaque (Fundo Azul, Texto Branco)
    putc(current_char, selector_row, selector_col, SELECT_COLOR);
}

/**
 * Funcao para remover o highlight e restaurar a cor padrao.
 */
void unhighlight_cursor() {
    // 1. Le o caractere que esta na posicao do cursor
    unsigned char* video_memory = (unsigned char*)0xb8000;
    int offset = (selector_row * 80 + selector_col) * 2;
    char current_char = video_memory[offset];

    // 2. Restaura a cor padrao (Fundo Preto, Texto Branco)
    putc(current_char, selector_row, selector_col, DEFAULT_COLOR);
}

/**
 * Logica central: Move o cursor e atualiza o destaque.
 */
void move_selector(int delta_col, int delta_row) {
    // 1. Remove o destaque da posicao antiga
    unhighlight_cursor();
    
    // 2. Calcula e valida a nova posicao
    selector_col += delta_col;
    selector_row += delta_row;

    // Limites da tela (80 colunas, 25 linhas)
    if (selector_col < 0) selector_col = 0;
    if (selector_col >= 80) selector_col = 79;
    if (selector_row < 0) selector_row = 0;
    if (selector_row >= 25) selector_row = 24;

    // 3. Aplica o destaque na nova posicao
    highlight_cursor();
    
    // Se estivesse integrado ao TalkBack, chamariamos aqui:
    // update_cursor_and_talk(selector_row, selector_col);
}

// Funcao de inicializacao
void init_blue_selector() {
    const char *title = "Seletor Azul Ativo (Highlight)";
    for (int i = 0; title[i] != '\0'; i++) {
        putc(title[i], 16, i, 0x0F); 
    }
    
    // Desenha alguns itens de menu de exemplo para selecionar
    putc('A', 10, 5, DEFAULT_COLOR);
    putc('R', 10, 6, DEFAULT_COLOR);
    putc('T', 10, 7, DEFAULT_COLOR);
    
    putc('N', 11, 5, DEFAULT_COLOR);
    putc('E', 11, 6, DEFAULT_COLOR);
    putc('W', 11, 7, DEFAULT_COLOR);

    // Inicializa na posicao (10, 5) e aplica o destaque
    selector_row = 10;
    selector_col = 5;
    highlight_cursor();

    // EXEMPLO: Em um loop de Kernel futuro, isso moveria a selecao
    // move_selector(0, 1); // Moveria para a linha 11
}
