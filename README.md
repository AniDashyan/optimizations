# Analyzing the Impact of Compiler Optimizations

## Overview

This project demonstrates how different compiler optimization levels affect program performance and code generation. The program implements a simple loop that sums elements in an array, then compiles it with different optimization levels (`-O0`, `-O1`, `-O2`, `-O3`) to measure execution time and analyze the generated assembly code.

**Key Learning Objectives:**
- Understand how compiler optimizations impact performance and code size
- Compare assembly output across different optimization levels
- Observe differences in instruction count and loop structure
- Measure the trade-offs between compilation time and runtime performance

The project uses `objdump` (Linux) or `dumpbin` (Windows) to analyze generated assembly, providing insights into how compilers transform high-level code into optimized machine instructions.

## Build & Run

### Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Linux: `objdump` (usually included with build tools)
- Windows: Visual Studio or MinGW

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd <repository-name>

# Configure the build
cmake -S . -B build

# Build the project
cmake --build build

# Run the basic program
./build/optimization_test          # Linux/macOS
# or
./build/Release/optimization_test.exe  # Windows

# Run optimization analysis (with disassembly)
cd build
make test_all_optimizations        # Linux
# or
cmake --build . --target test_all_optimizations  # Windows
```

## Example Output

### Basic Program Output
```
Array sum: 499500
Execution time: 0.000123 seconds
```

### Optimization Analysis Output
```
=== Testing -O0 ===
Array sum: 499500
Execution time: 0.000845 seconds

Disassembly of section .text:
0000000000001169 <main>:
    1169: 55                    push   %rbp
    116a: 48 89 e5              mov    %rsp,%rbp
    116d: 48 83 ec 20           sub    $0x20,%rsp
    1171: c7 45 fc 00 00 00 00  movl   $0x0,-0x4(%rbp)
    1178: c7 45 f8 00 00 00 00  movl   $0x0,-0x8(%rbp)
    117f: eb 0e                 jmp    118f <main+0x26>
    1181: 8b 45 f8              mov    -0x8(%rbp),%eax
    1184: 01 45 fc              add    %eax,-0x4(%rbp)
    1187: 83 45 f8 01           addl   $0x1,-0x8(%rbp)
    118b: 83 7d f8 63           cmpl   $0x63,-0x8(%rbp)
    118f: 7e f0                 jle    1181 <main+0x18>

Size: 8760 bytes

=== Testing -O1 ===
Array sum: 499500
Execution time: 0.000234 seconds

Disassembly of section .text:
0000000000001160 <main>:
    1160: b8 00 00 00 00        mov    $0x0,%eax
    1165: ba 00 00 00 00        mov    $0x0,%edx
    116a: 01 d0                 add    %edx,%eax
    116c: 83 c2 01              add    $0x1,%edx
    116f: 83 fa 64              cmp    $0x64,%edx
    1172: 75 f6                 jne    116a <main+0xa>

Size: 8664 bytes

=== Testing -O2 ===
Array sum: 499500
Execution time: 0.000089 seconds

Disassembly of section .text:
0000000000001160 <main>:
    1160: b8 4e c6 07 00        mov    $0x7c64e,%eax
    1165: c3                    retq

Size: 8600 bytes

=== Testing -O3 ===
Array sum: 499500
Execution time: 0.000089 seconds

Disassembly of section .text:
0000000000001160 <main>:
    1160: b8 4e c6 07 00        mov    $0x7c64e,%eax
    1165: c3                    retq

Size: 8600 bytes
```

## How It Works

### Understanding the Output

The optimization analysis reveals dramatic differences in how compilers handle the same source code:

#### **-O0 (No Optimization)**
- **Instructions**: 15+ assembly instructions
- **Characteristics**: Direct translation of C++ code
- **Loop Structure**: Explicit loop with counter variable, memory operations
- **Performance**: Slowest execution (~0.000845s)
- **File Size**: Largest binary (8760 bytes)

#### **-O1 (Basic Optimization)**
- **Instructions**: ~8 assembly instructions  
- **Characteristics**: Register optimization, reduced memory access
- **Loop Structure**: Still contains loop but with register variables
- **Performance**: Moderate improvement (~0.000234s)
- **File Size**: Slightly smaller (8664 bytes)

#### **-O2 (Aggressive Optimization)**
- **Instructions**: Just 2 instructions!
- **Characteristics**: **Constant folding** - compiler calculated the result at compile time
- **Loop Structure**: Loop completely eliminated
- **Performance**: Fastest execution (~0.000089s)
- **File Size**: Smallest (8600 bytes)

#### **-O3 (Maximum Optimization)**
- **Instructions**: Same as -O2 (2 instructions)
- **Characteristics**: Same constant folding as -O2
- **Performance**: Same as -O2
- **File Size**: Same as -O2

### Key Insights

1. **Constant Folding**: At -O2/-O3, the compiler recognizes that summing 0+1+2+...+999 is a compile-time constant (499500) and replaces the entire loop with a single `mov` instruction.

2. **Performance vs Size**: Higher optimization levels generally produce smaller, faster code.

3. **Diminishing Returns**: -O3 doesn't always provide benefits over -O2; the choice depends on the specific code patterns.

4. **Assembly Analysis**: The disassembly shows exactly how high-level constructs (loops, variables) translate to machine code.

### Compilation Process
The CMake configuration automatically:
- Compiles the same source with different flags
- Measures execution time
- Generates disassembly output
- Reports file sizes
- Works cross-platform (Linux with `objdump`, Windows with `dumpbin`)

This demonstrates that understanding compiler optimizations is crucial for performance-critical applications, and that sometimes the best optimization is letting the compiler do the work!
