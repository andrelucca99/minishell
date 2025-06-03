Com base no conteúdo do PDF do projeto **Minishell** (versão 8.3), aqui está um **cronograma de desenvolvimento** organizado para duas pessoas, com divisão equilibrada de tarefas entre:

- **Pessoa A**: foco principal em **Lexing, Parsing e Expansão**
- **Pessoa B**: foco principal em **Execução, Redirecionamentos, Builtins e Pipes**

---

## 📆 Cronograma de Desenvolvimento (2 semanas)

> **Meta:** concluir a parte obrigatória em até 14 dias  
> **Obs:** Ambos devem revisar e testar tudo juntos ao final de cada grande etapa

---

### FAZER DEPOIS

 - Tratar status code ao sair do minishell
 - Armazenar o status code
 - Armazenar num arquivo temporario
 - Colocar expandir var HEREDOC

### 🔹 Semana 1 — Construção do núcleo

| Dia | Pessoa A (Lexing/Parsing)                         | Pessoa B (Execução)                                  |
|-----|---------------------------------------------------|------------------------------------------------------|
| 1   | Estudo geral do projeto / montar estrutura de pastas | Idem                                                 |
| 2   | Implementar prompt com `readline()`               | Configurar estrutura de execução / builtins básicos  |
| 3   | Tokenização básica (separar palavras e símbolos)  | Execução com `execve`, PATH, fork, wait              |
| 4   | Parsing com tratamento de aspas `'` e `"`         | Implementar `cd`, `pwd`, `echo`                      |
| 5   | Implementar expansão de variáveis `$VAR`, `$?`    | Implementar `env`, `export`, `unset`, `exit`         |
| 6   | Integrar parsing + expansão                       | Implementar redirecionamentos: `<`, `>`             |
| 7   | Implementar here_doc `<<` e `>>`                  | Começar suporte a `pipe` com `pipe()` e `dup2`       |

---

### 🔸 Semana 2 — Integração e finalização

| Dia | Pessoa A (Lexing/Parsing)                         | Pessoa B (Execução)                                  |
|-----|---------------------------------------------------|------------------------------------------------------|
| 8   | Reforçar parsing para múltiplos comandos/pipes    | Finalizar execução de comandos encadeados com pipes |
| 9   | Lidar com erros de sintaxe                        | Implementar tratamento de sinais: `Ctrl+C`, `Ctrl+D`, `Ctrl+\` |
| 10  | Criar módulo de histórico com `add_history()`     | Testar builtins combinados com redirecionamento e pipes |
| 11  | Testes integrados: parsing + execução             | Testes integrados: execução + signals                |
| 12  | Revisar liberações de memória (evitar leaks)      | Idem                                                 |
| 13  | Refatoração geral, norminette, modularização      | Idem                                                 |
| 14  | Testes finais (interativos e automáticos)         | Preparação para avaliação (Makefile, documentação)   |

---

## 📂 Divisão de Diretórios Sugerida

```bash
minishell/
├── src/
│   ├── lexer/          # Pessoa A
│   ├── parser/         # Pessoa A
│   ├── expander/       # Pessoa A
│   ├── executor/       # Pessoa B
│   ├── builtins/       # Pessoa B
│   ├── signals/        # Pessoa B
│   ├── main.c
├── include/
│   └── minishell.h
├── libft/
├── Makefile
```

---

## ✅ Tarefas obrigatórias (resumo do PDF)

- [x] Prompt com `readline`
- [x] Histórico de comandos
- [x] Execução com base em PATH
- [x] Redirecionamentos `<`, `>`, `>>`, `<<`
- [x] Pipes (`|`)
- [x] Expansão de variáveis `$VAR`, `$?`
- [x] Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- [x] Tratamento de sinais (Ctrl-C, Ctrl-D, Ctrl-\)
- [x] Nenhum leak de memória (exceto os causados pela `readline`)

---

Se quiser, posso gerar um **Kanban** visual com essas tarefas (em texto ou imagem), ou até mesmo um modelo de `minishell.h` inicial para ambos começarem. Quer isso?