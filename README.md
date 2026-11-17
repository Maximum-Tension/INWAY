# INWAY

**INWAY** is a tiny peer-to-peer remote desktop for Windows PCs on the same local network: one EXE, no account, no cloud, nothing to install.

I made INWAY for the people around me who never used a PC in their lives. When they need help, they should not have to read a 9 digit ID over the phone or click through ten windows. They open INWAY, read two short things from a box (`2.147` and `942 853`), and that's it. The tools everyone knows send everything through the internet. Inside one building there is no reason to: a LAN is faster, and nothing leaves it.

* **One EXE, both sides.** The same `INWAY.exe` shares this PC and controls other PCs. Copy it and run it.
* **Made for people who never used a PC.** The address is only the last two numbers (`2.253`), the code is split in two (`942 853`), every screen is a coloured box, and it speaks English and Turkish.
* **Local network only.** No account, no server, no internet needed. The firewall rule I add only lets in PCs from the same network.
* **Works where others stop.** Installed as a service, it works on the lock screen, the login screen, in Task Manager and in "Do you want to allow this app?" popups, and it can send **`CTRL` + `ALT` + `DEL`**.
* **Copy here, paste there.** Text, files and folders, both ways, with plain `CTRL` + `C` and `CTRL` + `V`.
* **Smooth when it matters.** Moving parts of the screen are sent as JPEG, then sent again sharp a moment after they stop.
* **Updates itself.** It checks the GitHub releases on start and checks every download with the SHA-256 GitHub gives.

<details>
  <summary><strong>Table of Contents</strong></summary>

- [Download](#download)
- [How to use](#how-to-use)
  - [Share this PC](#share-this-pc)
  - [Connect to another PC](#connect-to-another-pc)
  - [Inside the remote window](#inside-the-remote-window)
  - [Command line](#command-line)
  - [Shortcut .bat files](#shortcut-bat-files)
  - [Servers: always on](#servers-always-on)
  - [Moving PCs from CONNECT to INWAY](#moving-pcs-from-connect-to-inway)
- [Updates](#updates)
  - [Making a release](#making-a-release)
- [How to compile](#how-to-compile)
  - [Files](#files)
- [Languages](#languages)
- [How it works](#how-it-works)
- [Security](#security)
- [Extras](#extras)

</details>

---

# Download

> **The newest INWAY.exe is always here:**
> 
> **[Latest release][__RELEASES__]**

Put the same `INWAY.exe` on both PCs. When it opens, Windows asks for admin rights: click **Yes**. With them, INWAY can also control admin windows like Task Manager. The first time, Windows also asks if INWAY may use the network: click **Allow access**.

---

# How to use

## Share this PC

Open `INWAY.exe`. Sharing starts by itself. The yellow box shows two things:

```
  ┌─ LET SOMEONE CONTROL THIS PC ──────────────┐
  │                                            │
  │  Address   2.147                           │
  │  Code      942 853                         │
  │                                            │
  │  Give them the code, or just click Accept  │
  │  when the window pops up.                  │
  └────────────────────────────────────────────┘
```

Tell them the **Address**. Then either tell them the **Code** too, or don't, and click **Accept** when the INWAY window pops up on your screen. While someone is connected, a small window in the corner says who it is and has a **Disconnect** button.

## Connect to another PC

Open `INWAY.exe` and type the **Address** shown on the other PC, then press Enter.

* `2.253` means `192.168.2.253`: the first two numbers come from your own PC. A full address works too.
* Then type the **Code**, or just press Enter: the other PC gets an **Accept / Cancel** window.

The other screen opens in a new window. Close that window to stop.

## Inside the remote window

| What                   | How                                                                     |
| ---------------------- | ----------------------------------------------------------------------- |
| Mouse, keyboard        | Just use them, they go to the other PC                                  |
| Windows key            | Works too, also `WIN` + `R`, `WIN` + `E` and so on                      |
| `CTRL` + `ALT` + `DEL` | The menu at the top, or press **`CTRL` + `ALT` + `END`**                |
| Copy and paste         | `CTRL` + `C` on one PC, `CTRL` + `V` on the other. Text, files, folders |
| Stop                   | Close the window, or press `CTRL` + `C` in the black window             |

Copied files go over right away, into a temp folder on the other PC, and are put on its clipboard when the last byte arrives. Big copies take a moment: wait for the "ready to paste" line before you paste. One copy can be up to 4 GB.

## Command line

| Command                        | What it does                                                          |
| ------------------------------ | --------------------------------------------------------------------- |
| `INWAY.exe`                    | Share this PC and ask for another one to control                      |
| `INWAY.exe -p 938572`          | Use this code instead of a random one                                 |
| `INWAY.exe -c 2.253 948248`    | Connect to `2.253` with that code. INWAY closes when the session ends |
| `INWAY.exe -c 2.253`           | Same, but they click Accept                                           |
| `INWAY.exe -install -p 938572` | Share this PC all the time, also on the lock screen (servers)         |
| `INWAY.exe -uninstall`         | Remove that again                                                     |
| `INWAY.exe -removeold`         | Remove the old CONNECT tool from this PC                              |
| `INWAY.exe -lang tr`           | Language: `en` or `tr` (normally it follows Windows)                  |
| `INWAY.exe -noupdate`          | Don't look for a new version this time                                |
| `INWAY.exe -h`                 | Show these                                                            |

A code can be written with a space too: `-p 938 572`.

## Shortcut .bat files

`-c` connects once and closes INWAY when you close the remote window, when the other PC hangs up, or when you press `CTRL` + `C`. So a shortcut is one line:

```bat
@echo off
cd /d "%~dp0"
INWAY.exe -c 2.253 938572
```

Don't put `pause` at the end, or the black window stays open.

## Servers: always on

On a PC that stays on all the time, run this **once**:

```bat
INWAY.exe -install -p 938572
```

Windows asks for permission: click **Yes**. INWAY copies itself to `C:\Program Files\INWAY`, installs a Windows service called **INWAY**, and opens the firewall for the local network only. On an account without admin rights, the admin part runs hidden and the result shows in the normal window, so a remote session never gets stuck behind an admin window. From now on that PC is shared all the time:

* on the lock screen and the login screen, also right after a restart,
* in Task Manager and other admin windows,
* in "Do you want to allow this app?" popups,
* and **`CTRL` + `ALT` + `DEL`** works from the menu.

A server only lets people in with its code: nobody sits there to click Accept. After 5 wrong codes in a row it waits 30 seconds before it takes the next one.

To change the code, run `-install` again with the new one. To take it out: `INWAY.exe -uninstall`.

**Servers update themselves.** The service asks GitHub for a new release every 6 hours. The first check comes 3 to 15 minutes after it starts, and the times are spread a little, so many servers behind one internet connection don't all ask at once. A new `INWAY.exe` is downloaded and checked against GitHub's SHA-256, then it **waits until nobody is connected**: it never drops a session. Then it stops the service, takes the place of the old EXE and starts the service again. The code, the port and the firewall rule stay the same. What happens is written to `C:\Program Files\INWAY\INWAY.log`.

To keep a server on manual updates, install it like this:

```bat
INWAY.exe -install -p 938572 -noupdate
```

> **Installing from far away:** over a normal INWAY session, the Windows **Yes** prompt can't be clicked remotely: someone at that PC has to click it. Over the background service (or the old CONNECT service) you can click it yourself. That's why moving PCs over works through CONNECT.

> **Why a service?** Windows only lets programs with system rights see the lock screen and admin windows, and only a service may press `CTRL` + `ALT` + `DEL`. INWAY already runs as administrator when it can, which covers Task Manager and other admin windows, but not the lock screen.

## Moving PCs from CONNECT to INWAY

INWAY uses its own port (`50506`), service name and folder, so it can run next to the old CONNECT tool. That's how I move a PC over from far away without losing it:

1. Connect to the PC with the old **CONNECT**.
2. On that PC, download `INWAY.exe` from the [latest release][__RELEASES__] and run `INWAY.exe -install -p 938572`.
3. Close CONNECT and connect again, now with **INWAY**.
4. Through INWAY, run `INWAY.exe -removeold` on that PC. It removes the old service, its firewall rule and `C:\Program Files\CONNECT`.
5. Done. Next PC.

---

# Updates

Every time INWAY starts, it asks GitHub for the latest release and reads the **SHA-256** GitHub gives for its `INWAY.exe` (the `digest` you see under every release file). Then it hashes **its own EXE**. **Different hash = update.** So two releases on the same day (`20.26.11.17-Mk1145`, then `20.26.11.17-Mk1151`) work too.

Two rules keep it safe:

* **Never backwards.** A release whose date is older than the running EXE's version is never installed.
* **Never in the source folder.** An `INWAY.exe` that sits next to `src\inway.h` is a test build, so it never replaces itself. Copy it somewhere else to test the updater.

When an update is due:

1. It downloads `INWAY.exe` of that release into `%TMP%` and checks its SHA-256 against the same `digest` again. Wrong or missing: no update.
2. It starts the new EXE and leaves it a small note in `%TMP%` that says where the old EXE is. The old one closes.
3. The new one reads the note, puts itself in the old one's place, deletes the note and goes on with the same command line.

No network or a GitHub rate limit: INWAY just starts as it is. `-noupdate` skips the check.

The copy in `C:\Program Files\INWAY` belongs to the background service, and the service updates it by itself (see [Servers: always on](#servers-always-on)).

## Making a release

1. Set the new version in [`src/version.h`](src/version.h). It is the date: `YY.YY.MM.DD`, so 2026/11/17 is `20.26.11.17`.
2. Build with `MAKE.bat`.
3. Make a GitHub release. The tag **starts with the date**, anything after it is yours: **`20.26.11.17-Mk1151`**. Attach **`INWAY.exe`** with exactly that name.

That's all. GitHub makes the SHA-256 by itself, so there is no `.sha256` file to upload.

> **Packing and signing:** do it in this order: **UPX first, then sign, then upload.** UPX breaks a signature, so signing comes last. GitHub's SHA-256 is made from the file you upload, so the updater checks exactly the packed and signed EXE.

---

# How to compile

You need **Windows 10 or 11** and **MinGW-w64** (`gcc` and `windres`). I use the one from [WinLibs][__WINLIBS__]; [MSYS2][__MSYS2__] works too. Put its `bin` folder in `PATH`, then:

```bat
MAKE.bat
```

`INWAY.exe` appears next to `MAKE.bat`. `MAKE.bat clean` removes the build files.

Without `MAKE.bat` it is two lines:

```sh
windres --codepage=65001 -I src -I res -i res\inway.rc -O coff -o inway.res.o
gcc -std=c99 -O2 -Isrc src\*.c inway.res.o -o INWAY.exe -static -s -lws2_32 -lgdi32 -luser32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lurlmon -lwinhttp -lbcrypt -lwindowscodecs
```

## Files

| File                                 | What it is                                                               |
| ------------------------------------ | ------------------------------------------------------------------------ |
| [`src/main.c`](src/main.c)           | Command line, main screen, `-c`, `-install`                              |
| [`src/inway.h`](src/inway.h)         | The one header: port, protocol, all modules                              |
| [`src/version.h`](src/version.h)     | The version, used by the code and the resources                          |
| [`src/host.c`](src/host.c)           | Sharing side: screen, mouse, keyboard, code check                        |
| [`src/viewer.c`](src/viewer.c)       | Controlling side: the remote window, Windows key, `CTRL` + `ALT` + `DEL` |
| [`src/codec.c`](src/codec.c)         | Lossless tile packer and JPEG through Windows Imaging Component          |
| [`src/clipboard.c`](src/clipboard.c) | Copy and paste of text, files and folders                                |
| [`src/htmlui.c`](src/htmlui.c)       | The styled popups (MSHTML, like HTA files)                               |
| [`src/service.c`](src/service.c)     | Service, lock screen, `-install`, `-uninstall`, `-removeold`             |
| [`src/update.c`](src/update.c)       | GitHub release auto updater                                              |
| [`src/net.c`](src/net.c)             | Sockets, short addresses, codes                                          |
| [`src/console.c`](src/console.c)     | Colours and boxes in the black window                                    |
| [`src/lang.c`](src/lang.c)           | Every sentence, in English and Turkish                                   |
| [`src/text.c`](src/text.c)           | Growing text buffer, UTF-8 helpers                                       |
| [`res/inway.rc`](res/inway.rc)       | Icon, manifest, popups, version details                                  |
| [`res/accept.htm`](res/accept.htm)   | "2.253 wants to control this PC"                                         |
| [`res/banner.htm`](res/banner.htm)   | "2.253 is controlling this PC"                                           |

---

# Languages

INWAY picks Turkish when Windows is in Turkish, English otherwise. `-lang en` or `-lang tr` overrides it.

Every sentence is in [`src/lang.c`](src/lang.c), one table per language. To add one, copy the English table, translate it, and pick it in `LANG_INIT()`. The boxes wrap long lines by themselves, so a longer translation never breaks them. A missing sentence falls back to English.

---

# How it works

* **The screen** is cut into 64 x 64 tiles. Only tiles that changed are sent, packed with a tiny lossless coder in the spirit of QOI.
* **When a lot changes at once** (a window opens, a video plays), the changed rows go as a few JPEG pictures instead, many times smaller. A moment after they stop changing, the same tiles are sent again lossless, so text is always sharp in the end. Up to 60 frames a second while things move.
* **The viewer** redraws only the part that changed, the fast way while moving and the smooth way once still.
* **Mouse, keys and clipboard** go the other way on the same connection. Everything is a small message: a type, a size, the data.
* **Copied files** are streamed as name, size and bytes. Names that try to leave the temp folder (`..`, `C:`, `\`) are refused.

---

# Security

> **INWAY is for a network you trust.**

* **The connection is not encrypted.** Someone who can watch the traffic on your network can see the screen and the keys, also a password typed on the lock screen. Use it at home or in your office, not on a public network.
* **Who gets in:** only someone with the code, or someone you click **Accept** for. A wrong code costs 2 seconds, 5 in a row cost 30 seconds.
* **The firewall rule** made by `-install` only accepts PCs from the local network.
* **A server's code** is stored in its service settings, so an admin of that PC can read it.

---

# Extras

* Found a bug or have an idea? [Open an issue][__ISSUES__].
* INWAY is open source under the **[GPL-3.0](LICENSE)** license.

© 2026 Teoman Deniz - [maximum-tension.com][__SITE__]

[__RELEASES__]: https://github.com/Maximum-Tension/INWAY/releases/latest
[__ISSUES__]: https://github.com/Maximum-Tension/INWAY/issues
[__WINLIBS__]: https://winlibs.com/
[__MSYS2__]: https://www.msys2.org/
[__SITE__]: https://maximum-tension.com
