# 📁 Source_File — Core Peripheral Access Layer

This directory contains the **authoritative, actively maintained source code** for the PSoC4SPlus Peripheral Access Layer.

The design and structure of this codebase intentionally follow **CMSIS-style conventions**, ensuring that the software representation mirrors the underlying silicon accurately, predictably, and safely.

---

## 🎯 Design Philosophy

The core objective of this directory is to provide a **clear, register-accurate hardware abstraction**, without hiding how the hardware actually works.

This layer is **not a high-level driver framework**.  
Instead, it serves as a **precise bridge between silicon and software**.

---

## 🧠 Key Concepts Followed

### 1️⃣ Hardware-to-Software Mapping

#### Memory-Mapped I/O (MMIO)
Microcontroller peripherals are accessed through **fixed memory addresses**.  
Each peripheral register corresponds to a specific address in the MCU’s memory map.

This project explicitly models this behavior using C constructs, rather than opaque APIs.

#### Register Structuring Using C `struct`s
- Hardware registers are represented using **C structures**
- The structure layout **exactly matches the register layout defined in the TRM**
- This allows direct and readable access to peripheral registers

### 2️⃣ Read-Modify-Write (RMW) Access Pattern

Peripheral control registers often contain **multiple unrelated control bits** within the same register.

Writing a full 32-bit value directly can unintentionally overwrite:
- Configuration bits set by other code
- Reserved bits
- Hardware-controlled status bits

To avoid this, all register updates follow the **Read-Modify-Write (RMW)** pattern:

1. Read the current register value  
2. Modify only the required bit fields  
3. Write the updated value back to the register  

This ensures:
- Safe bit-level updates
- No unintended side effects
- Correct behavior when multiple features share a register

The RMW pattern is used consistently across all peripheral implementations in this directory.
