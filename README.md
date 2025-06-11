# Minishell

Minishell é um projeto da 42 School que consiste em implementar um shell minimalista, capaz de interpretar comandos, executar programas, gerenciar variáveis de ambiente, redirecionamentos, pipes e alguns builtins.

---

## Integrantes

- André Lucas
- Emerson Schuma

---

## Como compilar

```bash
make
```

## Como executar

```bash
./minishell
```

## Funcionalidades

- Execução de comandos do sistema
- Suporte a múltiplos comandos com pipes (`|`)
- Redirecionamento de entrada e saída (`<`, `>`, `>>`, `<<`)
- Variáveis de ambiente e expansão de variáveis (`$VAR`, `$?`)
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Histórico de comandos (usando readline)
- Tratamento de sinais (`Ctrl+C`, `Ctrl+D`, etc.)

## Estrutura de Pastas

```
src/
  ├── builtins/
  ├── executor/
  ├── parser/
  ├── include/
  └── ...
libs/
  └── libft/
```

## Dependências

- [readline](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [libft](https://github.com/42Paris/libft) (inclusa em `libs/libft`)

## Observações

- O projeto segue as normas da 42 (Norminette).
- Para rodar, é necessário ter o readline instalado:
  ```bash
  sudo apt-get install libreadline-dev
  ```

---

<p align="center">
    <b>Projeto desenvolvido para a 42 School</b>
</p>