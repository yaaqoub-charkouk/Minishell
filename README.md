# 🐚 Minishell

> **A production-grade Unix shell implementation** demonstrating systems programming mastery through C.  
> Built with low-level process management, sophisticated parsing algorithms, and rigorous software engineering practices.

[![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square&logoColor=white)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen?style=flat-square)]()
[![Standard](https://img.shields.io/badge/Standard-POSIX-orange?style=flat-square&logo=linux&logoColor=white)](https://pubs.opengroup.org/onlinepubs/9699919799/)
[![Code Quality](https://img.shields.io/badge/Quality-Production-blueviolet?style=flat-square)]()

---

## 📋 Table of Contents

- [Overview](#overview)
- [Core Features](#core-features)
- [Architecture](#architecture)
- [Quick Start](#quick-start)
- [Usage Guide](#usage-guide)
- [Built-in Commands](#built-in-commands)
- [Technical Deep Dive](#technical-deep-dive)
- [Project Structure](#project-structure)
- [Highlights & Learning Outcomes](#highlights--learning-outcomes)
- [Advanced Topics](#advanced-topics)

---

## 🎯 Overview

**Minishell** is a comprehensive implementation of a POSIX-compliant Unix shell in C. Developed at **1337 Coding School** (42 Network), this project demonstrates expertise in:

| Aspect | Coverage |
|--------|----------|
| **Systems Programming** | Process management, IPC, signals, file descriptors |
| **Parsing & Compilation** | Tokenization, AST construction, operator precedence |
| **Software Engineering** | Modular architecture, memory safety, error handling |
| **Low-Level APIs** | `fork()`, `execve()`, `pipe()`, `dup2()`, `sigaction()` |

The implementation mimics **core Bash functionality** while maintaining clean, production-quality C code with comprehensive memory management.

**Developed with:** 🤝 **Adnane Kharkhour** (Pair Programming)

---

## ✨ Core Features

### 🔷 Essential Shell Operations

```bash
✅ Command Execution      │ Run external programs with arguments
✅ Pipes                  │ Chain commands with | operator
✅ Redirections           │ <, >, >>, << file I/O operations  
✅ Logical Operators      │ && and || conditional execution
✅ Built-in Commands      │ cd, pwd, echo, export, env, exit, unset
✅ Signal Handling        │ SIGINT (Ctrl+C), SIGQUIT, SIGTERM
✅ Environment Variables  │ $VAR, $?, $$, $0 expansion
```

### 🔷 Advanced Capabilities

| Feature | Description | Example |
|---------|-------------|---------|
| **Quote-Aware Parsing** | Handles single/double quotes and escapes | `echo "hello 'world'"` |
| **Operator Precedence** | Shunting Yard algorithm for correct evaluation | `cmd1 \|\| cmd2 && cmd3` |
| **Variable Expansion** | Dynamic substitution of environment variables | `echo $HOME` |
| **Command Substitution** | Execute commands within arguments | `echo $(pwd)` |
| **Wildcard Expansion** | Glob pattern matching | `echo *.c` |
| **Heredoc Support** | Multi-line input with `<<` operator | `cat << EOF` |
| **Memory Safety** | Complete leak-free execution | Custom allocation tracking |

---

## 🏗️ Architecture

### 🔹 Execution Pipeline

The shell follows a **classic interpreter pattern** with 6 distinct phases:

```
┌────────────────────────────────────────────────────────────┐
│ 1️⃣  INPUT PHASE: Read user command with readline()        │
└──────────────────────┬───────────────────────────────────┘
                       ▼
┌────────────────────────────────────────────────────────────┐
│ 2️⃣  TOKENIZATION: Lexical analysis with quote tracking    │
│    • Break input into tokens                              │
│    • Preserve quoted strings                              │
│    • Identify multi-char operators (&&, ||, >>, <<)       │
└──────────────────────┬───────────────────────────────────┘
                       ▼
┌────────────────────────────────────────────────────────────┐
│ 3️⃣  SYNTAX VALIDATION: Check for parse errors            │
│    • Invalid operator sequences                           │
│    • Unclosed quotes                                      │
│    • Unbalanced parentheses                               │
└──────────────────────┬───────────────────────────────────┘
                       ▼
┌────────────────────────────────────────────────────────────┐
│ 4️⃣  OPERATOR PRECEDENCE: Shunting Yard Algorithm          │
│    • Convert infix → postfix notation                     │
│    • Maintain operator stack                              │
│    • Generate output queue with correct order             │
└──────────────────────┬───────────────────────────────────┘
                       ▼
┌────────────────────────────────────────────────────────────┐
│ 5️⃣  AST CONSTRUCTION: Build Binary Execution Tree         │
│    • Convert queue to hierarchical tree                   │
│    • Leaf nodes = commands                                │
│    • Internal nodes = operators                           │
└──────────────────────┬───────────────────────────────────┘
                       ▼
┌────────────────────────────────────────────────────────────┐
│ 6️⃣  EXECUTION: Recursive tree traversal                   │
│    • File descriptor setup (redirections, pipes)          │
│    • Process creation (fork)                              │
│    • Command execution (execve)                           │
│    • Status handling                                      │
└────────────────────────────────────────────────────────────┘
```

### 🔹 Operator Precedence (Shunting Yard)

```
PRECEDENCE ORDER (Lowest → Highest):
  4 │ Logical OR  (||)      └─ Evaluated last
  3 │ Logical AND (&&)      │
  2 │ Pipe       (|)        │  
  1 │ Redirections (<,>,>>,<<) └─ Evaluated first
```

**Example Tree: `echo hello | cat && grep test < file.txt`**

```
          &&
         /  \
        |    <
       / \   /\
    echo  cat  grep  file.txt
     |     |
   hello  test
```

### 🔹 Module Breakdown

| Module | Responsibility | Key Files |
|--------|-----------------|-----------|
| **Parsing** | Tokenization, syntax validation, AST construction | `tokenize.c`, `build_queue.c`, `build_tree.c` |
| **Execution** | Process management, command resolution, I/O | `execute.c`, `pipes.c`, `redirections.c` |
| **Built-ins** | Internal command implementations | `cd.c`, `export.c`, `echo.c`, `env.c` |
| **Expansion** | Variable & glob substitution | `expand.c` |
| **Signals** | Async signal handling | `signals.c` |
| **Utilities** | Memory management, data structures | `memory.c`, `struct.h` |

---

## 🚀 Quick Start

### ⚙️ Prerequisites

```bash
# System requirements
- GCC 7.0+ or Clang
- GNU Make 4.0+
- Linux/Unix/macOS with POSIX compliance
- Readline library (usually pre-installed)
```

### 📥 Installation

```bash
# Clone repository
git clone https://github.com/yaaqoub-charkouk/Minishell.git
cd Minishell

# Compile
make                    # Standard build
make bonus             # With bonus features
make clean             # Remove objects
make fclean            # Full clean
make re                # Rebuild

# Run
./minishell
```

### ✅ Verify Installation

```bash
$ ./minishell
minishell-4.2$ echo "Welcome to Minishell!"
Welcome to Minishell!
minishell-4.2$ exit
```

---

## 💻 Usage Guide

### 🔸 Basic Command Execution

```bash
minishell> ls -la /tmp
minishell> pwd
minishell> whoami
minishell> date
```

### 🔸 Pipelines (Process Chaining)

```bash
# Chain multiple commands
minishell> cat file.txt | grep "pattern" | sort | uniq

# Complex pipeline
minishell> ps aux | grep bash | wc -l

# Multiple pipes
minishell> find . -name "*.c" | head -20 | xargs wc -l
```

### 🔸 Output Redirection

```bash
# Truncate file (overwrite)
minishell> echo "Hello" > output.txt

# Append to file
minishell> echo "World" >> output.txt

# Redirect stderr (note: minishell uses stdout only)
minishell> ls /nonexistent > error.txt

# Combined input and output
minishell> cat < input.txt > output.txt
```

### 🔸 Input Redirection

```bash
# Read from file
minishell> sort < unsorted.txt

# Heredoc (multi-line input)
minishell> cat << EOF
> Line 1
> Line 2
> EOF
```

### 🔸 Logical Operators

```bash
# AND - Execute second only if first succeeds
minishell> mkdir test && cd test && pwd

# OR - Execute second only if first fails
minishell> ls /tmp || echo "Directory not found"

# Complex chains
minishell> cd /tmp && ls && echo "Success" || echo "Failed"
```

### 🔸 Environment Variables

```bash
# Set variable
minishell> export MY_VAR="Hello World"

# Use variable
minishell> echo $MY_VAR
Hello World

# Exit status
minishell> ls /nonexistent
minishell> echo $?
2

# View all variables
minishell> env
```

### 🔸 Built-in Commands

```bash
# Navigation
minishell> cd /home
minishell> pwd

# Environment
minishell> export SHELL_VERSION="4.2"
minishell> unset SHELL_VERSION

# Print
minishell> echo "Hello" -n
Hello

# Exit shell
minishell> exit 0
```

---

## 🔧 Built-in Commands Reference

| Command | Syntax | Purpose | Examples |
|---------|--------|---------|----------|
| **cd** | `cd [path]` | Change directory | `cd /tmp` · `cd ~` · `cd -` |
| **pwd** | `pwd` | Print working directory | `pwd` |
| **echo** | `echo [-n] [text]` | Output text | `echo "Hello"` · `echo -n "No newline"` |
| **export** | `export VAR=value` | Set environment variable | `export PATH=$PATH:/usr/bin` |
| **unset** | `unset VAR` | Remove variable | `unset TEMP_VAR` |
| **env** | `env` | Show all environment variables | `env` |
| **exit** | `exit [code]` | Exit shell | `exit 0` · `exit 42` |

---

## 🔍 Technical Deep Dive

### 🔹 Phase 1: Tokenization (Lexical Analysis)

The **tokenizer** is the first stage of the parsing pipeline:

```c
Input:    "echo hello | grep o && pwd"
           
Process:  • Scan character by character
          • Track quote context (single/double)
          • Recognize special operators
          • Build token list

Output:   [echo hello] | [grep o] && [pwd]
```

**Quote Handling:**
```bash
"hello world"     → single token: hello world
'$VAR'            → single token: $VAR (literal)
hello\ world      → single token: hello world (escaped space)
```

### 🔹 Phase 2: Shunting Yard Algorithm

The **Shunting Yard algorithm** ensures correct operator precedence:

```c
// Example: "cmd1 | cmd2 && cmd3 < file"
//
// Precedence:
// - Redirections (lowest)
// - Pipe
// - Logical AND/OR (highest)

Input Queue:    [cmd1] [|] [cmd2] [&&] [cmd3] [<] [file]
Operator Stack: [empty]

Step 1: cmd1 → Output
Step 2: | → Stack
Step 3: cmd2 → Output
Step 4: && has higher precedence than |
        Pop | → Output
        Push && → Stack
Step 5: cmd3 → Output
Step 6: < → Stack (highest precedence)
Step 7: file → Output
Step 8: Pop all remaining operators

Output Queue: [cmd1] [cmd2] [|] [cmd3] [file] [<] [&&]
              
This is postfix notation (RPN) - ready for execution!
```

### 🔹 Phase 3: Binary Tree AST

The postfix queue is converted to a **binary expression tree**:

```c
Postfix:  [cmd1] [cmd2] [|] [cmd3] [<] [&&]

Tree Building (Stack-based):
  1. cmd1 → Push
  2. cmd2 → Push
  3. | → Pop 2, create node with left=cmd1, right=cmd2
  4. cmd3 → Push
  5. < → Pop 2, create node...
  
Final Tree:
          &&
         /  \
        |    REDIR_IN
       / \    /  \
    cmd1 cmd2 cmd3 file
```

### 🔹 Phase 4: Pre-Execution Phase

All **file descriptors** are resolved before forking:

```c
// For each redirection node:
- Input (<):   Open file for reading
- Output (>):  Create/truncate file for writing
- Append (>>): Open file for appending
- Heredoc (<<): Create temp file with input content

// For each pipe:
- Create pipe with pipe()
- Store read/write FDs in node

// For each command:
- Attach FDs to node metadata
```

### 🔹 Phase 5: Recursive Execution

The tree is **recursively executed** with proper process management:

```c
int execute(t_tree *node, t_data *shell) {
    
    if (is_command(node)) {
        // Built-in or external command
        return cmd_execution(node, shell);
    }
    
    else if (is_pipe(node)) {
        // Fork left and right, connect with pipe
        pid_left = fork();
        if (pid_left == 0)
            execute(node->left, shell);  // Child: left side
        
        pid_right = fork();
        if (pid_right == 0)
            execute(node->right, shell); // Child: right side
        
        wait all children...
    }
    
    else if (is_and(node)) {
        // Execute right only if left succeeded
        int status = execute(node->left, shell);
        if (status == 0)
            return execute(node->right, shell);
    }
    
    else if (is_or(node)) {
        // Execute right only if left failed
        int status = execute(node->left, shell);
        if (status != 0)
            return execute(node->right, shell);
    }
}
```

### 🔹 Process Model

```
Parent (minishell)
    │
    ├─ cmd1 (forked)
    ├─ cmd2 (forked, connected to cmd1 via pipe)
    └─ cmd3 (forked, waits for cmd2)

File Descriptor Inheritance:
- Parent's stdin/stdout remain intact
- Children inherit and close appropriate FDs
- Pipe FDs automatically closed by parent
```

### 🔹 Signal Handling

```c
Interactive Signals:
- SIGINT (Ctrl+C)   → Display newline, show prompt
- SIGQUIT (Ctrl+\)  → Ignored in shell, default in children
- SIGTERM (kill)    → Graceful termination

Child Processes:
- Reset to default handlers
- Allows natural interrupt behavior
```

---

## 📁 Project Structure

```
Minishell/
│
├── 📄 main.c                  ⭐ Entry point and main loop
├── 📄 main_utils.c            Build utilities, tokenization start
├── 📄 minishell.h             Central header with structures
├── Makefile                   Build configuration
│
├── 📁 parsing/                ⭐ Lexical analysis & AST construction
│   ├── tokenize.c             Tokenization with quote handling
│   ├── build_queue.c          Shunting Yard algorithm
│   ├── build_tree.c           Binary tree construction
│   ├── syntax.c               Syntax validation & error checking
│   └── parsing.h              Function declarations
│
├── 📁 execution/              ⭐ Command execution engine
│   ├── execute.c              Recursive tree traversal
│   ├── execute_utils.c        PATH resolution, built-in detection
│   ├── pipes.c                Pipe creation & connection
│   ├── redirections.c         File I/O and heredoc handling
│   └── execution.h            Declarations
│
├── 📁 built_ins/              ⭐ Built-in command implementations
│   ├── cd.c                   Change directory
│   ├── export.c               Environment variable management
│   ├── echo.c                 Echo command
│   ├── env.c                  Environment display
│   ├── pwd.c                  Print working directory
│   ├── exit.c                 Shell termination
│   ├── unset.c                Variable removal
│   └── built_ins.h            Declarations
│
├── 📁 expand/                 ⭐ Variable & wildcard expansion
│   ├── expand.c               Main expansion logic
│   └── expand.h               Declarations
│
├── 📁 signals/                ⭐ Signal handling
│   ├── signals.c              SIGINT, SIGQUIT, SIGTERM handlers
│   └── signals.h              Declarations
│
├── 📁 utils/                  ⭐ Utilities & memory management
│   ├── struct.h               Core data structure definitions
│   ├── utils.h                Utility function declarations
│   ├── memory.c               Memory allocation/cleanup
│   └── utils.c                Helper functions
│
├── 📁 libft/                  Custom C library (utility functions)
│   ├── libft.h                Library header
│   ├── ft_*.c                 Implementation files
│   └── Makefile               Sub-build
│
└── 📁 bonus/                  Bonus implementations
    ├── parsing/               Mirrored parsing modules
    ├── execution/             Mirrored execution modules
    └── ...                    Other bonus features
```

---

## 🎓 Highlights & Learning Outcomes

### 💡 Why This Project Matters

Minishell bridges the gap between **abstract concepts** and **real-world systems programming**:

```
Academic Knowledge          Practical Implementation
─────────────────────────────────────────────────────
Process management   →  fork(), execve(), waitpid(), signals
File descriptors     →  open(), close(), dup2(), pipe()
Operator precedence  →  Shunting Yard algorithm
Syntax analysis      →  Tokenization, AST construction
I/O operations       →  Redirections, heredocs
Memory management    →  Custom allocation, leak prevention
```

### 🎯 Core Competencies Demonstrated

#### 🔴 Systems Programming (60% of project)
- **Process Model**: Understanding fork-exec paradigm, process groups, wait mechanisms
- **File Descriptors**: Low-level I/O, duplication, multiplexing
- **Signals**: Asynchronous event handling, signal safety
- **Memory**: Stack/heap management, pointer arithmetic, leak detection

#### 🟡 Parsing & Compiler Design (25% of project)
- **Tokenization**: Lexical analysis with context awareness
- **Operator Precedence**: Shunting Yard algorithm for expression parsing
- **Abstract Syntax Trees**: Hierarchical representation of commands
- **Error Recovery**: Syntax validation and user-friendly error messages

#### 🟢 Software Engineering (15% of project)
- **Modular Architecture**: Clear module boundaries and responsibilities
- **Error Handling**: Comprehensive error checking throughout
- **Code Quality**: Adherence to 42 School code standards
- **Testing**: Manual validation against bash behavior

---

## 🛠️ Advanced Topics

### 📊 Performance Characteristics

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Tokenization | O(n) | Single pass through input |
| Shunting Yard | O(n) | Linear with token count |
| Tree Building | O(n) | Proportional to operators |
| Execution | O(p) | p = number of processes created |

### 🔐 Memory Safety Features

```c
✅ Custom malloc/free wrapper with tracking
✅ Leak detection at shutdown
✅ Automatic cleanup on all exit paths
✅ Buffer overflow protection via fixed-size tokens
✅ NULL pointer validation throughout
```

### 🐛 Known Limitations (By Design)

```
• No job control (bg/fg, jobs, etc.)
• Limited globbing (*, no [abc] patterns)
• No arithmetic expansion
• No function definitions
• No history/editing persistence
```

These are intentionally simplified for learning purposes.

---

## 📚 Advanced Usage

### 🔹 Debugging

```bash
# Compile with debug symbols
CFLAGS="-g -O0" make clean && make

# Run with gdb
gdb ./minishell
(gdb) break execute
(gdb) run
(gdb) step
```

### 🔹 Testing Harness

```bash
# Create test script
cat > test.sh << 'EOF'
echo "Test 1: Basic pipe"
echo "hello" | cat

echo "Test 2: Logical operators"
ls /tmp && echo "Success"

echo "Test 3: Redirections"
echo "test" > /tmp/test.txt
cat < /tmp/test.txt
EOF

# Compare with bash
echo "=== BASH ===" && bash test.sh
echo "=== MINISHELL ===" && ./minishell < test.sh
```

### 🔹 Valgrind Memory Check

```bash
# Run with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./minishell << EOF
echo "test"
exit
EOF
```

---

## 🚦 Development Path

This project exemplifies the journey of a **future software engineer**:

```
Foundation (Shell Basics)
    ↓
Parsing & AST (Compiler Concepts)
    ↓
Process Management (Systems Programming)
    ↓
Integration & Optimization (Software Engineering)
    ↓
Production-Ready Code (Professional Practice)
```

### Next Steps for Learning

1. **Extend the shell**: Add more built-ins (history, alias, test)
2. **Optimize parsing**: Implement proper error recovery
3. **Add debugging**: Implement shell tracing (`set -x`)
4. **Advanced I/O**: Non-blocking operations, select/poll
5. **Distribute**: Package for multiple POSIX systems

---

## 📖 References & Resources

### Official Documentation
- 🔗 [POSIX Shell Specification](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- 🔗 [Linux man pages](https://man7.org/)
- 🔗 [GNU Bash Manual](https://www.gnu.org/software/bash/manual/)

### Algorithms
- 📖 [Shunting Yard Algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) - Wikipedia
- 📖 [Operator Precedence](https://en.wikipedia.org/wiki/Operator_precedence) - Wikipedia

### Books
- 📚 *The Linux Programming Interface* - Michael Kerrisk
- 📚 *Operating Systems: Three Easy Pieces* - Remzi H. Arpaci-Dusseau
- 📚 *Advanced Bash-Scripting Guide* - Mendel Cooper
- 📚 *Compilers: Principles, Techniques & Tools* - Aho, Lam, Sethi, Ullman

### Learning Resources
- 🎓 [42 School Official](https://42.fr/)
- 🎓 [42 Network](https://42network.org/)
- 📝 [Shell Scripting Tutorial](https://www.shellscript.sh/)

---

## 📄 License

This project is open source under the **MIT License**.

```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

---

## 👨‍💻 About the Developer

### Yaaqoub Charkouk
**Future Software Engineer | Systems Programming Enthusiast**

Currently pursuing a dual path:
- 🎓 **1337 Coding School** (42 Network) - Advanced systems programming
- 🎓 **University** - Mathematics & Computer Science degree

### Technical Focus
| Area | Expertise |
|------|-----------|
| **Systems Programming** | OS concepts, process management, signals, IPC |
| **Backend Development** | Server architecture, API design, databases |
| **Networking** | Protocols, distributed systems, socket programming |
| **Languages** | C, Java, Python, Shell scripting |
| **Algorithms** | Data structures, complexity analysis, optimization |

### Philosophy
> *"I believe in understanding how software works at every abstraction layer. By mastering low-level systems programming, I build the mental models necessary for designing robust, efficient, and scalable systems."*

### Current Interests
🖥️ Operating systems internals  
🔧 Compiler design and language implementation  
🌐 Distributed systems and microservices  
🤖 Systems-level machine learning  
📊 Large-scale data processing  

---

## 🤝 Contributing & Feedback

While this is a learning project from 42 School, I welcome:

- **Code reviews** and architectural suggestions
- **Bug reports** with reproduction steps
- **Performance optimization** ideas
- **Documentation improvements**

Feel free to open issues or reach out with questions!

---

## 📬 Connect

| Platform | Link |
|----------|------|
| **GitHub** | [@yaaqoub-charkouk](https://github.com/yaaqoub-charkouk) |
| **School** | 1337 Coding School (42 Network) |

---

<div align="center">

### 🌟 Minishell: Where Systems Programming Meets Software Engineering Excellence

**Built at 1337 Coding School** • **Crafted with ❤️** • **Deployed with Pride**

```
     ╔═══════════════════════════════════════╗
     ║  $ ./minishell                        ║
     ║  minishell-4.2$ echo "The journey    ║
     ║  > of a software engineer begins      ║
     ║  > with understanding systems at      ║
     ║  > their core level"                 ║
     ║  The journey of a software engineer   ║
     ║  begins with understanding systems at ║
     ║  their core level                     ║
     ║  minishell-4.2$ exit 0                ║
     ╚═══════════════════════════════════════╝
```

**⭐ If this project helped you understand shells, systems programming, or parsing, please star it!**

*"First, do no harm. Then, do something useful. Finally, make it elegant." - 42 Philosophy*

</div>

---

**Last Updated:** June 2026 | **Status:** Production Ready | **Maintained by:** Yaaqoub Charkouk
