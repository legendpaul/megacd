# Mega-CD / Sega CD Native Starter (MINNKA)

This repository provides a native (non-SGDK) Mega-CD starter that builds both the Mega Drive main CPU program and the Mega-CD sub CPU program, builds a disc image, and demonstrates mailbox communication, basic ISO reading stubs, palette cycling, and save-data plumbing.

## Quick start (one click)
1. Clone or unzip this repo on Windows 10/11.
2. Double-click `build/ONE_CLICK_BUILD.bat`.
   - The script auto-downloads the required toolchain (`tools/`), builds main and sub CPU binaries, assembles the disc layout, generates `IP.BIN`, and emits `out/game.bin` + `out/game.cue`.

Outputs are placed in `out/`:
- `out/game.bin` / `out/game.cue`
- `out/disc/` with `IP.BIN`, `MAIN.BIN`, `SUB.BIN`, and `DATA/TEST.BIN`
- `out/logs/` build logs

## Running on emulator or hardware
- Use any Sega CD–capable emulator (e.g., Genesis Plus GX, Kega Fusion, PicoDrive). Provide your own legally obtained BIOS; none are included or downloaded.
- Hardware: burn to CD-R or use a Mega EverDrive Pro with Mega-CD; ensure region BIOS matches the target.

## Scenes
- **Title:** shows “MINNKA” and “Press START”.
- **Main Menu:** PLAY / TECH DEMO / SAVE TEST / ABOUT.
- **Play:** simple platformer placeholder (move left/right, jump, pause with START).
- **Tech Demo:** requests `DATA/TEST.BIN` over the mailbox, shows progress text, palette cycling, and PCM start/stop stubs.
- **Save Test:** save API scaffold with slot text and checksum placeholder.
- **About:** build/version and control summary.

## Communication
`src_common/comm.h` defines a mailbox at `0xFFF000` with commands PING, LOAD_FILE, GET_CD_STATUS, PCM_START, and PCM_STOP. Main-side helpers live in `src_main/comm_main.c`; the sub CPU polls the mailbox in `src_sub/main.c` and replies with status codes. Timeouts are handled on the main side.

## Toolchain bootstrap
`build/bootstrap_tools.bat` downloads pinned, SHA-verified binaries to `tools/`:
- m68k-elf GCC toolchain
- mkisofs (cdrtools) for ISO creation
- IP.BIN generator (fallback PowerShell generator is included for offline use)

## Project layout
- `src_ip/` – IP.BIN boot sector loader (fits in the 2 KB Sega CD IP block)
- `src_main/` – main CPU C sources and VDP helpers
- `src_sub/` – sub CPU command loop and ISO stub
- `src_common/` – shared mailbox definitions
- `asm/` – startup vectors for both CPUs
- `build/` – batch scripts, linker scripts
- `disc_template/` – files copied into the disc image (DATA/TEST.BIN sample)
- `out/` – generated outputs (created by the build)
- `tools/` – auto-downloaded toolchain and utilities

## Self-check
To verify the one-click flow:
1. Delete any existing `tools/` and `out/` directories.
2. Double-click `build/ONE_CLICK_BUILD.bat`.
3. Confirm that `out/game.bin` and `out/game.cue` appear with logs under `out/logs/`.

## Notes
- No BIOS files are included or downloaded.
- All downloaded tools are open-source or freely redistributable and validated with SHA256 inside the bootstrap script.
- The IP sector must stay within 2048 bytes; the `src_ip/ip.s` loader mirrors the compact boot stub used by commercial discs (e.g., Road Avenger) and jumps into `MAIN.BIN` placed in Word RAM by the BIOS.
