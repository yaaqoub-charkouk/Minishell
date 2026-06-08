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
- **Lexical parsing and tokenization**
- **Environment variable expansion**

The implementation mimics core Bash functionality while maintaining clean, modular C code with proper memory management and error handling.

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

---

## 🏗️ Architecture

### Component Overview

```
┌─────────────────────────────────────────────┐
│         Main Loop (main.c)                  │
│    Read → Parse → Execute → Repeat          │
└────────────────┬────────────────────────────┘
                 │
        ┌────────┼────────┐
        ▼        ▼        ▼
   ┌────────┐ ┌─────────┐ ┌──────────┐
   │Parsing │ │Execution│ │Built-ins │
   │        │ │         │ │          │
   │•Tokens │ │•Pipes   │ │•cd       │
   │•Quotes │ │•Redirect│ │•export   │
   │•Expand │ │•Process │ │•unset    │
   └────────┘ └─────────┘ └──────────┘
        │        │            │
        └────────┼────────────┘
                 │
        ┌────────▼────────┐
        │   Utilities     │
        │   & Signals     │
        └─────────────────┘
```

### Design Principles

1. **Modularity** - Separated concerns into distinct directories
2. **Reusability** - Custom `libft` library for common operations
3. **Robustness** - Comprehensive error handling and edge case management
4. **Efficiency** - Minimal system calls and optimized data structures

---

## 🚀 Getting Started

### Prerequisites

- **GCC** or **Clang** compiler
- **GNU Make** build tool
- **POSIX-compliant** Unix/Linux system (Linux, macOS)
- **Readline library** (optional, for enhanced line editing)

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
make bonus    # Build bonus features (if implemented)
```

---

## 💻 Usage

### Basic Commands

```bash
$ ./minishell
minishell> ls -la
minishell> pwd
minishell> cd /home
minishell> pwd
minishell> echo "Hello, World!"
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

# Combined redirections
minishell> grep "error" < input.log > errors.txt 2>&1
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

### Parsing Pipeline

1. **Lexical Analysis** - Tokenize input into meaningful units
2. **Quote Handling** - Process single/double quotes and escape sequences
3. **Variable Expansion** - Expand `$VAR` and special variables
4. **AST Construction** - Build Abstract Syntax Tree for execution

### Execution Engine

1. **Command Resolution** - Locate command in PATH or execute absolute path
2. **File Descriptor Management** - Setup redirections before execution
3. **Process Creation** - Fork child process and execute command
4. **Wait & Cleanup** - Collect child process status and manage resources

### Signal Management

```c
// Handle Ctrl+C gracefully
signal(SIGINT, signal_handler);

// Handle Ctrl+\ for core dump (disabled in interactive shell)
signal(SIGQUIT, signal_handler);

// Handle child process termination
signal(SIGCHLD, signal_handler);
```

---

## 📁 Project Structure

```
Minishell/
├── main.c                 # Entry point and main loop
├── main_utils.c           # Main loop utilities
├── minishell.h            # Header file with data structures
├── Makefile               # Build configuration
│
├── parsing/               # Lexical analysis and parsing
│   ├── lexer.c           # Tokenization
│   ├── parser.c          # AST construction
│   └── expand.c          # Variable expansion
│
├── execution/             # Command execution engine
│   ├── execute.c         # Main execution logic
│   ├── pipes.c           # Pipe handling
│   └── redirections.c    # I/O redirection
│
├── built_ins/             # Built-in command implementations
│   ├── cd.c              # Change directory
│   ├── export.c          # Environment variable export
│   ├── echo.c            # Echo command
│   └── utils.c           # Built-in utilities
│
├── signals/               # Signal handling
│   └── signals.c         # Signal handlers and setup
│
├── expand/                # Variable and glob expansion
│   ├── expansion.c       # Main expansion logic
│   └── variable.c        # Variable substitution
│
├── utils/                 # Utility functions
│   ├── memory.c          # Memory allocation wrappers
│   ├── string.c          # String utilities
│   └── error.c           # Error handling
│
├── libft/                 # Custom C library
│   ├── libft.h          # Library header
│   ├── ft_*.c           # Library functions
│   └── Makefile         # Library build
│
└── bonus/                 # Bonus features
    └── features.c        # Additional enhancements
```

---

## 🎓 Technical Highlights

### Systems Programming Concepts

- **Process Management**: Deep dive into `fork()`, `execve()`, `waitpid()`
- **File Descriptors**: Understanding `open()`, `close()`, `dup2()`, `pipe()`
- **Signal Handling**: Synchronous vs asynchronous signal handling
- **Environment**: Understanding environment variable scope and inheritance

### Memory Safety

- **Allocation Tracking** - Custom wrappers for `malloc()/free()`
- **Leak Prevention** - Structured cleanup at exit
- **Bounds Checking** - Safe string operations with bounds

### Code Quality

```
Language Breakdown:
├── C                98.8%  (Core implementation)
└── Makefile          1.2%  (Build configuration)
```

---

## 📊 Learning Outcomes

This project reinforced and demonstrated proficiency in:

### Low-Level Systems Programming
- Process creation, management, and synchronization
- File descriptor manipulation and I/O redirection
- Signal handling and safe signal programming

### Software Engineering Practices
- **Modular Architecture** - Clear separation of concerns
- **Error Handling** - Comprehensive error management
- **Memory Management** - Leak-free C code
- **Documentation** - Self-documenting code with comments

### Problem-Solving
- Complex parsing and tokenization
- Managing parent-child process communication
- Handling edge cases and race conditions

---

## 🛠️ Advanced Usage

### Debugging

```bash
# Compile with debug symbols
CFLAGS="-g -O0" make clean make

# Run with debugger
gdb ./minishell
(gdb) run
(gdb) break main
(gdb) continue
```

### Testing

```bash
# Test built-in commands
./minishell << EOF
export TEST="value"
echo $TEST
cd /tmp
pwd
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
- Create a pull request with improvements
- Suggest better approaches or optimizations

---

## 📚 References

- **POSIX Shell Specification**: [pubs.opengroup.org](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- **Advanced Bash-Scripting Guide**: [tldp.org](https://tldp.org/LDP/abs/html/)
- **The Linux Programming Interface**: Michael Kerrisk
- **Operating Systems: Three Easy Pieces**: Remzi H. Arpaci-Dusseau

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

⭐ If this project helped you understand shell implementation, please consider giving it a star!

</div>
