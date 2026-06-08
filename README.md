# 🐚 Minishell

> A robust Unix shell implementation in C with support for pipes, redirections, built-ins, and signal handling

[![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen?style=flat-square)]()
[![Standard](https://img.shields.io/badge/Standard-POSIX-orange?style=flat-square)](https://pubs.opengroup.org/onlinepubs/9699919799/)

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Implementation Details](#implementation-details)
- [Project Structure](#project-structure)
- [Technical Highlights](#technical-highlights)
- [Learning Outcomes](#learning-outcomes)

---

## 🎯 Overview

**Minishell** is a systems programming project that implements a functional POSIX-compliant shell in C. Developed as part of the 42 Network curriculum at 1337 Coding School, this project demonstrates deep understanding of:

- **Process management** (fork, exec, wait)
- **Inter-process communication** (pipes, file descriptors)
- **Signal handling** (SIGINT, SIGQUIT, SIGTERM)
- **File I/O and redirections** (stdin, stdout, stderr)
- **Lexical parsing and tokenization** with quote-aware handling
- **Operator precedence** using the Shunting Yard algorithm
- **Binary tree AST** for command representation

The implementation mimics core Bash functionality while maintaining clean, modular C code with proper memory management and error handling.

**Validated with teammate:** 🤝 **Adnane Kharkhour**

---

## ✨ Features

### Core Functionality

- ✅ **Command Execution** - Execute external commands with arguments
- ✅ **Pipes** - Chain commands with `|` operator
  ```bash
  cat file.txt | grep "pattern" | wc -l
  ```
- ✅ **Redirections** - Input/Output file redirection
  ```bash
  command < input.txt > output.txt
  command >> append.txt
  ```
- ✅ **Logical Operators** - AND (`&&`) and OR (`||`) operators for conditional execution
  ```bash
  ls && echo "Success" || echo "Failed"
  ```
- ✅ **Built-in Commands** - Essential shell built-ins
  - `cd` - Change directory
  - `pwd` - Print working directory
  - `export` - Set environment variables
  - `unset` - Unset environment variables
  - `env` - Display environment
  - `exit` - Exit the shell
  - `echo` - Print arguments (with `-n` flag)

### Advanced Features

- 🔄 **Signal Handling** - Proper handling of SIGINT, SIGQUIT, SIGTERM
- 🔤 **Variable Expansion** - Support for `$VAR` and `$?` (exit status)
- 🗂️ **Environment Management** - Full environment variable support
- 📝 **Error Handling** - Comprehensive error messages and status codes
- 🧹 **Memory Management** - Proper cleanup and leak prevention
- 📋 **Quote-Aware Tokenization** - Strict input validation with proper quote handling
- 🔀 **Operator Precedence** - Correct handling with Shunting Yard algorithm
- 🌳 **Binary Tree AST** - Abstract syntax tree for command representation

---

## 🏗️ Architecture

### Parsing Pipeline: Shunting Yard Algorithm

The shell uses the **Shunting Yard algorithm** to parse commands with proper operator precedence:

```
Operator Precedence (lowest to highest):
1. Logical OR (||)
2. Logical AND (&&)
3. Pipes (|)
4. Redirections (>, >>, <, <<)
```

### Component Overview

```
┌─────────────────────────────────────────────┐
│         Main Loop (main.c)                  │
│    Read → Parse → Execute → Repeat          │
└────────────────┬────────────────────────────┘
                 │
        ┌────────┼────────┐
        ▼        ▼        ▼
   ┌─────────┐ ┌────────────────┐ ┌──────────┐
   │Tokenize │ │Shunting Yard   │ │Binary    │
   │         │ │Algorithm       │ │Tree AST  │
   │•Quotes  │ │•Precedence     │ │•Commands │
   │•Escape  │ │•Stack-based    │ │•Operators│
   └─────────┘ │•Output Queue   │ └──────────┘
               └────────────────┘
                      │
        ┌─────────────┼──────────────┐
        ▼             ▼              ▼
   ┌─────────┐ ┌───────────┐ ┌──────────────┐
   │Expansion│ │Pre-Exec   │ │Recursive     │
   │         │ │Phase      │ │Execution     │
   │•Variable│ │•FD Setup  │ │•Pipes        │
   │•History │ │•Heredoc   │ │•Redirections │
   └─────────┘ └───────────┘ └──────────────┘
        │             │              │
        └─────────────┼──────────────┘
                 │
        ┌────────▼────────┐
        │  Built-ins &    │
        │  Process Mgmt   │
        └─────────────────┘
```

### Design Principles

1. **Tokenization** - Input is tokenized with awareness of quotes and escape sequences
2. **Shunting Yard** - Operators are processed based on precedence using a stack-based algorithm
3. **AST Construction** - The output queue is converted to a binary tree for recursive execution
4. **Pre-Execution Phase** - All file descriptors are resolved before process execution
5. **Clean Execution** - Binary tree is recursively executed with proper process management

---

## 🚀 Getting Started

### Prerequisites

- **GCC** or **Clang** compiler
- **GNU Make** build tool
- **POSIX-compliant** Unix/Linux system (Linux, macOS)
- **Readline library** (for line editing capabilities)

### Installation

```bash
# Clone the repository
git clone https://github.com/yaaqoub-charkouk/Minishell.git
cd Minishell

# Compile the project
make

# Run the shell
./minishell
```

### Build Targets

```bash
make          # Compile the project
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Rebuild from scratch
make bonus    # Build bonus features
```

---

## 💻 Usage

### Basic Commands

```bash
$ ./minishell
minishell-4.2$ ls -la
minishell-4.2$ pwd
minishell-4.2$ cd /home
minishell-4.2$ pwd
minishell-4.2$ echo "Hello, World!"
```

### Pipes and Redirections

```bash
# Piping commands
minishell> cat file.txt | grep "search_term" | sort | uniq

# Output redirection
minishell> echo "Hello" > output.txt
minishell> echo "World" >> output.txt

# Input redirection
minishell> sort < unsorted.txt

# Combined operations
minishell> grep "pattern" < input.txt > output.txt
```

### Logical Operators

```bash
# AND operator (execute second command only if first succeeds)
minishell> ls /tmp && echo "Directory exists"

# OR operator (execute second command only if first fails)
minishell> ls /nonexistent || echo "Directory not found"

# Chaining operators
minishell> cd /tmp && ls && pwd || echo "Failed"
```

### Environment Variables

```bash
# Set environment variable
minishell> export MY_VAR="Hello"

# Use environment variable
minishell> echo $MY_VAR
Hello

# Display all environment variables
minishell> env

# Check exit status
minishell> ls /nonexistent
ls: cannot access '/nonexistent': No such file or directory
minishell> echo $?
2
```

### Exit the Shell

```bash
minishell> exit 0
```

---

## 🔧 Built-in Commands

| Command | Syntax | Description | Example |
|---------|--------|-------------|---------|
| `cd` | `cd [path]` | Change working directory | `cd /tmp` |
| `pwd` | `pwd` | Print working directory | `pwd` |
| `export` | `export VAR=value` | Set environment variable | `export PATH=$PATH:/usr/bin` |
| `unset` | `unset VAR` | Unset environment variable | `unset TEMP_VAR` |
| `env` | `env` | Display all environment variables | `env` |
| `echo` | `echo [-n] [text]` | Print text to stdout | `echo "Hello" -n` |
| `exit` | `exit [code]` | Exit the shell | `exit 42` |

---

## 🔍 Implementation Details

### Tokenization Phase

1. **Lexical Analysis** - Input string is scanned character by character
2. **Quote Handling** - Single quotes (`'`) and double quotes (`"`) are tracked
3. **Operator Recognition** - Multi-character operators (`&&`, `||`, `>>`, `<<`) are identified
4. **Token Creation** - Tokens are created as linked list nodes with type information

```c
// Example: "echo hello | grep hello"
// Tokens: [echo hello] [|] [grep hello]
```

### Shunting Yard Algorithm

The algorithm converts infix notation to postfix (RPN) for proper operator precedence:

```c
Precedence Levels (build_queue.c):
- Redirections:  3 (highest)
- Pipes:         2
- OR (||):       1
- AND (&&):      1 (lowest)
```

### Binary Tree Construction

The postfix queue is converted to a binary tree where:
- **Leaf nodes** = Commands (`CMD`)
- **Internal nodes** = Operators (`PIPE`, `AND`, `OR`, `REDIRECTION_*`)

```
Example: cmd1 | cmd2 && cmd3

         &&
        /  \
       |    cmd3
      / \
   cmd1  cmd2
```

### Pre-Execution Phase

Before execution, all file descriptors are resolved:
- **Input files** - Opened and linked to nodes
- **Output files** - Created/truncated/appended as needed
- **Heredocs** - Temporary files created for here-document input
- **Pipes** - File descriptor pairs created

### Recursive Execution

The tree is executed recursively with proper process management:

```c
int execution(t_tree *node, t_data *data, int is_pipe)
{
    if (node is PIPE)
        return execute_pipe(node, data);
    else if (node is AND)
        return execute_and(node, data);
    else if (node is OR)
        return execute_or(node, data);
    else if (node is CMD)
        return execute_cmd(node, data, is_pipe);
}
```

---

## 📁 Project Structure

```
Minishell/
├── main.c                 # Entry point and main loop
├── main_utils.c           # Build & execute, tokenization
├── minishell.h            # Main header file
├── Makefile               # Build configuration
│
├── parsing/               # Lexical analysis and parsing
│   ├── tokenize.c        # Tokenization with quote handling
│   ├── build_queue.c     # Shunting Yard algorithm
│   ├── build_tree.c      # AST construction
│   ├── syntax.c          # Syntax validation
│   └── parsing.h         # Parsing declarations
│
├── execution/             # Command execution engine
│   ├── execute.c         # Recursive execution logic
│   ├── execute_utils.c   # Command resolution & PATH
│   ├── pipes.c           # Pipe handling and setup
│   ├── redirections.c    # I/O redirection & file ops
│   └── execution.h       # Execution declarations
│
├── built_ins/             # Built-in command implementations
│   ├── cd.c              # Change directory
│   ├── export.c          # Export environment variables
│   ├── echo.c            # Echo command
│   ├── env.c             # Display environment
│   └── built_ins.h       # Built-in declarations
│
├── signals/               # Signal handling
│   └── signals.c         # SIGINT, SIGQUIT, SIGTERM handlers
│
├── expand/                # Variable and glob expansion
│   ├── expand.c          # Main expansion logic
│   └── expand.h          # Expansion declarations
│
├── utils/                 # Utility functions
│   ├── struct.h          # Data structure definitions
│   ├── utils.h           # Utility declarations
│   └── memory.c          # Memory management
│
├── libft/                 # Custom C library
│   ├── libft.h          # Library header
│   ├── ft_*.c           # Library functions
│   └── Makefile         # Library build
│
└── bonus/                 # Bonus features (if implemented)
    └── ...               # Bonus implementations
```

---

## 🎓 Technical Highlights

### Core Algorithms

- **Shunting Yard Algorithm** - Converting infix to postfix notation with precedence
- **Binary Tree AST** - Hierarchical representation of command structure
- **Recursive Descent Execution** - Processing AST nodes recursively
- **File Descriptor Management** - Pre-execution resolution and setup

### Systems Programming

- **Process Management**: `fork()`, `execve()`, `waitpid()`, process groups
- **File Descriptors**: `open()`, `close()`, `dup2()`, `pipe()`, multiplexing
- **Signal Handling**: `signal()`, `sigaction()`, safe signal programming
- **Environment**: Variable scope, inheritance, PATH resolution

### Memory & Safety

- **Allocation Tracking** - Custom wrappers for `malloc()/free()`
- **Leak Prevention** - Structured cleanup at all exit paths
- **Quote-Aware Parsing** - Correct handling of escaped characters
- **Bounds Checking** - Safe string operations throughout

### Code Quality

```
Language Breakdown:
├── C                98.8%  (Core implementation)
└── Makefile          1.2%  (Build configuration)

Total LOC: ~1000 lines of well-structured C code
```

---

## 🎯 Learning Outcomes

This project reinforced and demonstrated proficiency in:

### Low-Level Systems Programming
- **Process Creation & Management** - fork, exec, signal coordination
- **Inter-Process Communication** - Pipes, file descriptors, inheritance
- **Signal Safety** - Asynchronous signal handling in shell context
- **File I/O** - Redirections, heredocs, file descriptor juggling

### Parsing & Compiler Design
- **Tokenization** - Lexical analysis with quote and escape awareness
- **Operator Precedence** - Shunting Yard algorithm implementation
- **AST Construction** - Building hierarchical command structures
- **Syntax Validation** - Error checking and reporting

### Software Engineering
- **Modular Architecture** - Clear separation of concerns
- **Error Handling** - Comprehensive error checking
- **Memory Management** - Leak-free code with proper cleanup
- **Code Organization** - Logical grouping and reusable components

### Problem-Solving
- **Complex Parsing** - Handling nested quotes and escapes
- **Process Coordination** - Parent-child communication
- **Edge Case Management** - Empty commands, multiple redirections
- **Signal Safety** - Handling asynchronous events safely

---

## 🛠️ Advanced Usage

### Debugging

```bash
# Compile with debug symbols
CFLAGS="-g -O0" make clean && make

# Run with debugger
gdb ./minishell
(gdb) break main
(gdb) run
(gdb) break execution
(gdb) continue
```

### Testing

```bash
# Test pipes and redirections
./minishell << EOF
echo "test" | cat > output.txt
cat output.txt
exit
EOF

# Compare with bash
bash < test_script.sh
./minishell < test_script.sh
```

---

## 🤝 Contributing

While this is a learning project, feedback and suggestions are welcome! Feel free to:

- Open an issue for bugs or feature requests
- Suggest improvements to the architecture
- Share optimizations or edge cases

---

## 📚 References

- **POSIX Shell Specification**: [pubs.opengroup.org](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- **Shunting Yard Algorithm**: [Wikipedia](https://en.wikipedia.org/wiki/Shunting_yard_algorithm)
- **Advanced Bash-Scripting Guide**: [tldp.org](https://tldp.org/LDP/abs/html/)
- **The Linux Programming Interface**: Michael Kerrisk
- **Operating Systems: Three Easy Pieces**: Remzi H. Arpaci-Dusseau
- **42 School Norm**: Clean code standards and best practices

---

## 📝 License

This project is open source and available under the MIT License.

---

## 👨‍💻 About the Developer

**Yaaqoub Charkouk** - Software Engineer in Training

Currently balancing Software Engineering studies at **1337 Coding School (42 Network)** with a **Mathematics & Computer Science degree**. 

### Focus Areas
- 🖥️ Systems programming and OS-level concepts
- 🔧 Backend development and architecture
- 🌐 Networking protocols and distributed systems
- ☕ Java-based applications and frameworks
- 🤖 Artificial intelligence and machine learning
- 📈 Large-scale software systems design

### Philosophy
> I believe in understanding how software works beneath the abstraction layers, combining theoretical knowledge with practical implementation to build robust, efficient systems.

---

## 📬 Connect

- **GitHub**: [@yaaqoub-charkouk](https://github.com/yaaqoub-charkouk)
- **School**: 1337 Coding School (42 Network)

---

<div align="center">

**Made with ❤️ at 1337 Coding School**

🌟 This shell demonstrates mastery of systems programming, parsing algorithms, and process management

⭐ If this project helped you understand shell implementation or systems design, please consider giving it a star!

</div>
