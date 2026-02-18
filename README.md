# CHIP-8 Emulator in C

A simple **CHIP-8 emulator** written in C using SDL2.  
Run classic CHIP-8 games and demos like Pong, Tetris, and Space Invaders.

---

## What is CHIP-8?

CHIP-8 is a **virtual machine** from the 1970s designed for simple games.  
It features:

- A **64×32 monochrome display**
- **16 8-bit registers** (V0–VF)
- A **stack** for subroutine calls
- Simple **opcodes** for graphics, input, and timers

Programs (ROMs) are interpreted and drawn on your screen via this emulator.

---

## Build

Make sure you have **SDL2** installed. Then build with:

```bash
make
```

---

## Usage

Run a CHIP-8 ROM:

```bash
./chip8 path/to/rom
```

Example with a provided ROM:

```bash
./chip8 roms/PONG.ch8
```
