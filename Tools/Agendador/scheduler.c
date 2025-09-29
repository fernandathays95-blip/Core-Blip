#include <stdint.h>
#include "kernel_base.h" // Funcoes putc, ui_log_status

// =======================================================
// 1. ESTRUTURAS DE DADOS DO AGENDADOR
// =======================================================

// Define a estrutura que armazena o estado de um processo (PCB)
typedef struct {
    uint32_t esp;       // Endereco do topo da Pilha (Stack Pointer)
    uint32_t pid;       // ID do Processo
    uint32_t state;     // Estado (e.g., RUNNING, READY, BLOCKED)
    uint32_t stack[1024]; // Espaco de pilha dedicado (4KB)
} PCB;

#define MAX_PROCESSES 4
#define STACK_SIZE_WORDS 1024 // 4KB

// Array para armazenar todos os PCBs
static PCB process_table[MAX_PROCESSES];
static int current_pid = 0; // O PID do processo atualmente em execucao

// Funcao externa (em Assembly) para fazer o Context Switch
// Ele salva os registradores na pilha antiga e carrega os da nova.
extern void context_switch(uint32_t new_esp);

// =======================================================
// 2. FUNCOES DE CONTROLE DO AGENDADOR
// =======================================================

/**
 * Rotina que e chamada pela Interrupcao do Timer (IRQ0).
 * Este é o ponto de entrada da multitarefa.
 */
void scheduler_timer_interrupt(uint32_t esp_from_interrupt) {
    
    // 1. Salvar o contexto (estado) do processo atual
    // O esp_from_interrupt e o topo da pilha onde o hardware salvou os registradores
    process_table[current_pid].esp = esp_from_interrupt;

    // 2. Logica de Selecao (Round-Robin)
    // Apenas avanca para o proximo processo na tabela
    do {
        current_pid = (current_pid + 1) % MAX_PROCESSES;
    } while (process_table[current_pid].state != 1); // Garante que o processo esta READY/RUNNING

    // 3. Carregar o contexto (estado) do proximo processo
    uint32_t new_esp = process_table[current_pid].esp;
    
    // 4. Efetuar o Salto! (Context Switching)
    // O Assembly ira restaurar os registradores do novo processo e retornar
    // da interrupcao para o codigo do novo processo.
    context_switch(new_esp);
}

/**
 * Funcao para criar um novo processo e adiciona-lo ao Agendador.
 */
void create_process(void (*entry_point)()) {
    
    // Encontra o proximo slot PID disponivel
    int new_pid = 0;
    for (new_pid = 0; new_pid < MAX_PROCESSES; new_pid++) {
        if (process_table[new_pid].state == 0) break; // 0 = FREE
    }
    
    if (new_pid == MAX_PROCESSES) {
        ui_log_status("SCHEDULER ERRO: Tabela de processos cheia.", 0x0C);
        return;
    }

    // 1. Inicializar o PCB
    PCB *new_pcb = &process_table[new_pid];
    new_pcb->pid = new_pid;
    new_pcb->state = 1; // 1 = READY (Pronto para rodar)

    // 2. Configurar a Pilha (Simular um estado de interrupcao limpo)
    // O ponto de entrada da pilha e onde o Context Switch ira "retornar".
    uint32_t *stack_ptr = (uint32_t*) (new_pcb->stack + STACK_SIZE_WORDS);
    
    // Configura o topo da pilha para simular uma interrupcao que acabou de acontecer.
    // O Assembly de Context Switch espera uma pilha com EIP (endereco de retorno) no topo.
    *(--stack_ptr) = (uint32_t)entry_point; // Onde o processo vai comecar a rodar!

    // Salva o novo topo da pilha (ESP)
    new_pcb->esp = (uint32_t)stack_ptr;
    
    ui_log_status("Novo processo criado e agendado.", 0x0A);
}

/**
 * Funcao de inicializacao do Agendador.
 */
void init_scheduler() {
    // A rotina de init_timer_driver() (nao mostrada) configuraria o timer para 10ms.
    
    // Exemplo de criacao de processos (App Loader e Diagnostico)
    // create_process(action_run_app); // Funcao para rodar um aplicativo
    // create_process(action_diagnostics); // Funcao para rodar o monitor CPU
    
    // Inicializa o processo 0 (o Kernel Idle Loop)
    process_table[0].pid = 0;
    process_table[0].state = 1; 
    
    ui_log_status("Agendador Ativo. Pronto para multitarefa.", 0x0F);
}
