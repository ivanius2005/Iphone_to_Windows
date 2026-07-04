# IVN_05 · Mirror

```
  ██╗██╗   ██╗███╗   ██╗    ██████╗ ███████╗
  ██║██║   ██║████╗  ██║   ██╔═████╗██╔════╝
  ██║██║   ██║██╔██╗ ██║   ██║██╔██║███████╗
  ██║╚██╗ ██╔╝██║╚██╗██║   ████╔╝██║╚════██║
  ██║ ╚████╔╝ ██║ ╚████║██╗╚██████╔╝███████║
  ╚═╝  ╚═══╝  ╚═╝  ╚═══╝╚═╝ ╚═════╝ ╚══════╝
              M I R R O R
```

**Mirror your iPhone screen to Windows 11 — no commercial software, no jailbreak, no apps on the phone.**

AirPlay receiver based on [UxPlay](https://github.com/FDH2/UxPlay), compiled from source with MSYS2, plus a native C launcher so you can start it with a double click.

---

## What it does

- The PC announces itself on the local network as an AirPlay receiver (`Iphone_to_Windows`)
- From the iPhone: **Control Center → Screen Mirroring → Iphone_to_Windows**
- The iPhone screen appears in a window on the PC, audio included
- Fully local. Nothing leaves your network.

## Requirements

- Windows 11 (tested on 11 26H2)
- iPhone with iOS 12+ on the **same WiFi network** as the PC
- ~3 GB of disk space for MSYS2 and dependencies

> ⚠️ **Smart App Control**: if enabled, Windows will block GStreamer's DLLs.
> Disable it at *Windows Security → App & browser control → Smart App Control*.

---

## Installation

### 1 · Install MSYS2

Download the installer from [msys2.org](https://www.msys2.org/) and install it at the default path (`C:\msys64`).

Open the **MSYS2 MINGW64** terminal (important: MINGW64, not UCRT or MSYS) and update the system:

```bash
pacman -Syu
```

If the terminal closes on its own, reopen it and run the command again until it says everything is up to date.

### 2 · Install dependencies

```bash
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make \
  mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gst-plugins-base \
  mingw-w64-x86_64-gst-plugins-good mingw-w64-x86_64-gst-plugins-bad \
  mingw-w64-x86_64-gst-libav mingw-w64-x86_64-openssl \
  mingw-w64-x86_64-libplist git
```

### 3 · Build UxPlay

```bash
cd ~
git clone https://github.com/FDH2/UxPlay.git
cd UxPlay
mkdir build && cd build
cmake ..
cmake --build .
```

If everything goes well, the last line will be:

```
[40/40] Linking CXX executable uxplay.exe
```

### 4 · Test it

```bash
./uxplay -n "Iphone_to_Windows"
```

On the iPhone: **Control Center → Screen Mirroring → Iphone_to_Windows**.

> 💡 Windows Firewall will ask for permission the first time — accept it.
> If the dialog doesn't appear, create the rules manually (cmd as administrator):
> ```
> netsh advfirewall firewall add rule name="mDNS" dir=in action=allow protocol=UDP localport=5353
> netsh advfirewall firewall add rule name="AirPlay" dir=in action=allow protocol=TCP localport=7000
> ```

### 5 · Build the launcher (double-click to start)

The launcher (`launcher/ivn_mirror_launcher.c`) sets up the MinGW64 DLL path
and starts UxPlay, so you don't have to open the MSYS2 terminal every time.

Before building, open the file and edit the two marked values:

```c
// 1. Replace YOUR_USERNAME with your Windows username
const char *cmd =
    "C:\\msys64\\home\\YOUR_USERNAME\\UxPlay\\build\\uxplay.exe "
    "-n \"Iphone_to_Windows\"";   // 2. Change the name if you want
```

Then compile from the MSYS2 MINGW64 terminal:

```bash
gcc ~/ivn_mirror_launcher.c -o "/c/Users/YOUR_USERNAME/Desktop/IVN_Mirror.exe"
```

Double-click `IVN_Mirror.exe` → receiver starts. Close the window to stop it.

---

## Customizing the receiver name

The name that appears on the iPhone when tapping "Screen Mirroring" can be
changed at any time. There are two places to do it:

**Direct use (without launcher):**
```bash
./uxplay -n "Whatever name you want"
```

**In the launcher** (`launcher/ivn_mirror_launcher.c`), find this line and edit the name:
```c
"-n \"Iphone_to_Windows\"";
```

Replace `Iphone_to_Windows` with any name, recompile, and you're done.

---

## Known issues

| Symptom | Cause | Fix |
|---|---|---|
| iPhone doesn't see the receiver | Firewall blocking mDNS | Add rules from step 4 |
| `DLL load failed` / apps blocked | Smart App Control enabled | Disable it (see Requirements) |
| iPhone sees it as a speaker only | Audio-only receiver (e.g. airplay2-receiver) | Use UxPlay, which implements full mirroring |
| Black window / no video | Incomplete GStreamer plugins | Reinstall `gst-plugins-*` packages from step 2 |

---

## Roadmap

- [x] **Phase 1** — Local network mirror (this repository)
- [ ] **Phase 2** — Remote access via self-hosted server (HLS/WebRTC stream from
      the receiver to a browser, accessible from anywhere)
- [ ] **Reverse** — View the PC screen from the iPhone (desktop capture →
      web stream, no apps on the phone)

---

## Credits & licenses

- [**UxPlay**](https://github.com/FDH2/UxPlay) — the actual AirPlay receiver,
  by F. Duncanh and contributors, licensed **GPL-3.0**. This repository
  does **not** redistribute its code or binaries; it only documents how to build it.
- [**GStreamer**](https://gstreamer.freedesktop.org/) — multimedia framework (LGPL).
- The launcher (`launcher/`) is original work, licensed **MIT** (see `LICENSE`).

---

<sub>Part of the **IVN_05** ecosystem · `#0A0A0A` · `#00FF47` · BMW M</sub>
