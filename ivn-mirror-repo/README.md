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

**Duplicar la pantalla del iPhone en Windows 11, sin software comercial, sin jailbreak y sin apps en el móvil.**

Receptor AirPlay basado en [UxPlay](https://github.com/FDH2/UxPlay), compilado desde el código fuente con MSYS2, más un lanzador nativo en C para arrancarlo con doble clic.

---

## ¿Qué hace?

- El PC se anuncia en la red local como receptor AirPlay (`Iphone_to_Windows`)
- Desde el iPhone: **Centro de Control → Duplicar pantalla → Iphone_to_Windows**
- La pantalla del iPhone aparece en una ventana del PC, con audio incluido
- Todo en red local. Nada sale de tu casa.

## Requisitos

- Windows 11 (probado en 11 26H2)
- iPhone con iOS 12+ en la **misma red WiFi** que el PC
- ~3 GB de disco para MSYS2 y dependencias

> ⚠️ **Smart App Control**: si está activado, Windows bloqueará las DLLs
> de GStreamer. Desactívalo en *Windows Security → Control de aplicaciones
> y explorador → Smart App Control*.

---

## Instalación

### 1 · Instalar MSYS2

Descarga el instalador desde [msys2.org](https://www.msys2.org/) e instálalo en la ruta por defecto (`C:\msys64`).

Abre la terminal **MSYS2 MINGW64** (importante: la MINGW64, no la UCRT ni la MSYS) y actualiza el sistema:

```bash
pacman -Syu
```

Si la terminal se cierra sola, ábrela de nuevo y repite el comando hasta que diga que todo está al día.

### 2 · Instalar dependencias

```bash
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make \
  mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gst-plugins-base \
  mingw-w64-x86_64-gst-plugins-good mingw-w64-x86_64-gst-plugins-bad \
  mingw-w64-x86_64-gst-libav mingw-w64-x86_64-openssl \
  mingw-w64-x86_64-libplist git
```

### 3 · Compilar UxPlay

```bash
cd ~
git clone https://github.com/FDH2/UxPlay.git
cd UxPlay
mkdir build && cd build
cmake ..
cmake --build .
```

Si todo va bien, la última línea será:

```
[40/40] Linking CXX executable uxplay.exe
```

### 4 · Probar

```bash
./uxplay -n "Iphone_to_Windows"
```

En el iPhone: **Centro de Control → Duplicar pantalla → Iphone_to_Windows**.

> 💡 Windows Firewall pedirá permiso la primera vez — acéptalo.
> Si no aparece el diálogo, crea las reglas manualmente (cmd como administrador):
> ```
> netsh advfirewall firewall add rule name="mDNS" dir=in action=allow protocol=UDP localport=5353
> netsh advfirewall firewall add rule name="AirPlay" dir=in action=allow protocol=TCP localport=7000
> ```

### 5 · Compilar el lanzador (doble clic)

El lanzador (`launcher/ivn_mirror_launcher.c`) configura el PATH de las DLLs
de MinGW64 y arranca UxPlay, para no tener que abrir MSYS2 cada vez.

Antes de compilar, abre el archivo y edita las dos cosas marcadas:

```c
// 1. Sustituye TU_USUARIO por tu nombre de usuario de Windows
const char *cmd =
    "C:\\msys64\\home\\TU_USUARIO\\UxPlay\\build\\uxplay.exe "
    "-n \"Iphone_to_Windows\"";   // 2. Cambia el nombre si quieres
```

Luego compila desde la terminal MSYS2 MINGW64:

```bash
gcc ~/ivn_mirror_launcher.c -o "/c/Users/TU_USUARIO/Desktop/IVN_Mirror.exe"
```

Doble clic en `IVN_Mirror.exe` → receptor en marcha. Cerrar la ventana lo detiene.

---

## Personalizar el nombre del receptor

El nombre que aparece en el iPhone al pulsar "Duplicar pantalla" se puede cambiar
en cualquier momento. Hay dos sitios donde modificarlo:

**Uso directo (sin lanzador):**
```bash
./uxplay -n "El nombre que quieras"
```

**En el lanzador** (`launcher/ivn_mirror_launcher.c`), busca esta línea y edita el nombre:
```c
"-n \"Iphone_to_Windows\"";
```

Cambia `Iphone_to_Windows` por cualquier nombre, recompila y listo.

---

## Problemas conocidos

| Síntoma | Causa | Solución |
|---|---|---|
| El iPhone no ve el receptor | Firewall bloqueando mDNS | Reglas del paso 4 |
| `DLL load failed` / apps bloqueadas | Smart App Control | Desactivarlo (ver Requisitos) |
| El iPhone lo ve solo como altavoz | Receptor solo-audio (p. ej. airplay2-receiver) | Usar UxPlay, que sí implementa mirroring |
| Ventana negra / sin vídeo | Plugins GStreamer incompletos | Reinstalar los paquetes `gst-plugins-*` del paso 2 |

---

## Roadmap

- [x] **Fase 1** — Mirror en red local (este repositorio)
- [ ] **Fase 2** — Acceso remoto vía servidor propio (stream HLS/WebRTC desde
      el receptor hacia un navegador, accesible desde fuera de casa)
- [ ] **Inversa** — Ver la pantalla del PC desde el iPhone (captura de
      escritorio → stream web, sin apps en el móvil)

---

## Créditos y licencias

- [**UxPlay**](https://github.com/FDH2/UxPlay) — el receptor AirPlay real,
  de F. Duncanh y colaboradores, licencia **GPL-3.0**. Este repositorio
  **no** redistribuye su código ni sus binarios; solo documenta cómo compilarlo.
- [**GStreamer**](https://gstreamer.freedesktop.org/) — framework multimedia (LGPL).
- El lanzador (`launcher/`) es obra propia, licencia **MIT** (ver `LICENSE`).

---

<sub>Parte del ecosistema **IVN_05** · `#0A0A0A` · `#00FF47` · BMW M</sub>
