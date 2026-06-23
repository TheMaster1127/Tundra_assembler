# Tundra_assembler

Tundra_assembler is a minimal, dependency-free assembler that produces highly optimized, Section-Header-Free ELF64 executable binaries for Linux x86_64 (AMD64 / Intel 64). 

### ⚠️ Important Note: Not a Universal Assembler
**This is not meant to be a universal or fully-fledged assembler.** 

---

## Building the Assembler

Tundra_assembler is written in **HTVM** (a custom language which is itself written in HTVM). However, HTVM transpiles directly to C++, meaning you do not need any special toolchains to build the assembler. 

You simply compile the generated C++ source file using any standard C++ compiler:

```bash
g++ Tundra_assembler.cpp -o Tundra_assembler
```

---

## Usage

Using the assembler is strictly straightforward. There are no command-line flags, options, or `-o` output arguments.

```bash
./Tundra_assembler your_program.s
```

**Requirements:**
1. The input file **must** have a `.s` extension.
2. The compiled executable will be automatically generated in the same directory, stripped of the `.s` extension (e.g., `your_program.s` outputs a binary named `your_program`).

---

## Syntax & FASM Differences

The syntax is almost identical to FASM for the instructions it supports. However, because it is simplified, there are strict limitations you must be aware of if you try to write assembly for it by hand.

### Unsupported FASM Features
* **No Macros:** The assembler does not support macros of any kind.
* **Limited Instruction Set:** It only knows how to encode the specific x86_64 instructions shown below. Unrecognized instructions will simply fail to assemble.
* **No Complex Equates:** Mathematical expressions are generally limited. You cannot do complex arithmetic in declarations.

### Directive Limitations
Tundra_assembler supports `db`, `dq`, `rb`, and `rq`, but with highly stripped-down parsing rules:

* **`db` (Define Byte):** Only supports **up to one comma**. 
  * ✅ Supported: `db "hello world", 10` (String + single terminator)
  * ❌ **Not Supported:** `db "hello", 10, 0` (Multiple commas will break the parser).
* **`$-label` Size Calculation:** Supported exclusively in the format `len = $-label` for calculating string lengths.

### Directives Reference
The assembler expects a standard FASM-like file structure. The following directives are recognized:

* `format ELF64 executable 3` (Required format header)
* `entry <label>` (Defines the executable entry point)
* `segment readable writeable` (Denotes the data/BSS section)
* `segment readable executable` (Denotes the code section)

### Data Declarations
* `db`: Declare strings or 8-bit integers (max 1 comma).
* `dq`: Declare 64-bit integers.
* `rb`: Reserve bytes (BSS).
* `rq`: Reserve quadwords (BSS). 

*(Note: Uninitialized `rb`/`rq` memory is automatically merged with the data segment at assemble-time to reduce the binary ELF header footprint, mirroring FASM's optimization).*

## Supported Registers

Tundra_assembler supports the full standard set of x86_64 general-purpose registers across all access widths, including the high-byte registers.

| Width | Registers |
| :--- | :--- |
| **64-bit** | `rax`, `rcx`, `rdx`, `rbx`, `rsp`, `rbp`, `rsi`, `rdi`, `r8`, `r9`, `r10`, `r11`, `r12`, `r13`, `r14`, `r15` |
| **32-bit** | `eax`, `ecx`, `edx`, `ebx`, `esp`, `ebp`, `esi`, `edi`, `r8d`, `r9d`, `r10d`, `r11d`, `r12d`, `r13d`, `r14d`, `r15d` |
| **16-bit** | `ax`, `cx`, `dx`, `bx`, `sp`, `bp`, `si`, `di`, `r8w`, `r9w`, `r10w`, `r11w`, `r12w`, `r13w`, `r14w`, `r15w` |
| **8-bit** | `al`, `cl`, `dl`, `bl`, `spl`, `bpl`, `sil`, `dil`, `r8b`, `r9b`, `r10b`, `r11b`, `r12b`, `r13b`, `r14b`, `r15b` |
| **High 8-bit** | `ah`, `ch`, `dh`, `bh` |

---

## Supported Instruction Set

Below is the complete list of instructions that Tundra_assembler is capable of encoding. These are organized by their logical operations.

### 1. Data Movement & Memory
* **`mov`**: Move data between registers, registers and memory, or load immediate values.
* **`movzx`**: Move with zero-extend (supports byte and word source widths).
* **`movsx`**: Move with sign-extend (supports byte and word source widths).
* **`movsxd`**: Move with sign-extend doubleword to quadword.
* **`lea`**: Load effective address (supports RIP-relative, base, SIB, scale, and index scaling).

### 2. Arithmetic & Logical Operations
The standard two-operand arithmetic and logical instructions support register-to-register, register-to-memory, memory-to-register, and register-to-immediate forms:
* **`add`**: Integer addition.
* **`sub`**: Integer subtraction.
* **`adc`**: Add with carry.
* **`sbb`**: Subtract with borrow.
* **`and`**: Logical AND.
* **`or`**: Logical OR.
* **`xor`**: Logical Exclusive OR.
* **`cmp`**: Compare two operands (sets flags).
* **`test`**: Logical compare (AND mask without modifying destination).

### 3. Single-Operand Arithmetic & Multiplication/Division
* **`inc`**: Increment operand by 1.
* **`dec`**: Decrement operand by 1.
* **`neg`**: Two's complement negation.
* **`not`**: One's complement logical negation.
* **`mul`**: Unsigned multiplication (AX/DX destination).
* **`imul`**: Signed multiplication. Supports:
  * Single-operand form (`imul reg`).
  * Two-operand form (`imul dest, src`).
  * Three-operand immediate form (`imul dest, src, imm`).
* **`div`**: Unsigned division.
* **`idiv`**: Signed division.

### 4. Stack Operations
* **`push`**: Push operand onto the stack. Supports 64-bit and 16-bit registers, and immediate values (both 8-bit and 32-bit sign-extended).
* **`pop`**: Pop value from stack into 64-bit or 16-bit register, or memory.

### 5. Control Flow
* **`call`**: Call procedure (supports direct relative address, register-indirect, and memory-indirect).
* **`ret`**: Return from procedure (supports bare `ret` and `ret imm` for stack cleanup).
* **`jmp`**: Unconditional jump (supports relative, short, register-indirect, and memory-indirect).
* **Conditional Jumps (`jcc`)**: Supports short and near relative conditional jumps:
  * `je` / `jz` (Jump if equal / zero)
  * `jne` / `jnz` (Jump if not equal / not zero)
  * `jg` / `jnle` (Jump if greater)
  * `jge` / `jnl` (Jump if greater or equal)
  * `jl` / `jnge` (Jump if less)
  * `jle` / `jng` (Jump if less or equal)
  * `ja` / `jnbe` (Jump if above / unsigned greater)
  * `jae` / `jnb` / `jnc` (Jump if above or equal / carry clear)
  * `jb` / `jnae` / `jc` (Jump if below / carry set)
  * `jbe` / `jna` (Jump if below or equal)
  * `js` (Jump if sign / negative)
  * `jns` (Jump if not sign / positive)
  * `jo` (Jump if overflow)
  * `jno` (Jump if not overflow)
  * `jp` / `jpe` (Jump if parity / parity even)
  * `jnp` / `jpo` (Jump if not parity / parity odd)

### 6. Bitwise Shifts & Rotates
Supports shifting by `1`, by the `cl` register, or by an 8-bit immediate value:
* **`shl`** / **`sal`**: Shift logical/arithmetic left.
* **`shr`**: Shift logical right.
* **`sar`**: Shift arithmetic right (preserves sign bit).
* **`rol`**: Rotate left.
* **`ror`**: Rotate right.
* **`rcl`**: Rotate left through carry.
* **`rcr`**: Rotate right through carry.

### 7. System & String Instructions
* **`syscall`**: Fast system call (encoded as `0f 05`).
* **`int`**: Software interrupt. `int 3` is encoded compactly as `cc`. Other interrupts are encoded as `cd imm8`.
* **`rep movsq`**: Repeat move string quadword (encoded as `f3 48 a5`). Used in the compiler's array/struct copying operations.

---

## License

This project is licensed under the [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html).
