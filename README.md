# Qt-Chess – Netzwerkfähiges Schach in C++

**Qt-Chess** ist eine plattformunabhängige Schach-Applikation in modernem C++ mit Qt-GUI und Client-/Server-Netzwerkarchitektur. Spiele gegen Freunde im LAN oder Internet – mit responsiver Oberfläche, animierten Figuren und vollständiger Spielregellogik.

---

## 🧩 Features

| Kategorie      | Beschreibung |
|---------------|--------------|
| ♟ Regeln       | Komplettes Regelwerk: Rochade, En Passant, Umwandlung, Schach/Matt, Patt, 50-Züge-Regel |
| 🖼 GUI         | Qt Widgets + .ui-Dateien, Drag & Drop, Sounds, Icons |
| 🌐 Netzwerk    | TCP-Socket-Kommunikation (Client/Server), Spectator-Modus |
| 🧠 Architektur | Saubere Trennung: GUI / Logik / Netzwerk |
| 📦 Ressourcen  | `.qrc`-Einbindung aller Bilder & Sounds |
| 🛠 Plattformen | Linux, macOS, Linux (getestet) |

---

## 🗂 Projektstruktur

```
├── GUI/
│   ├── Pics/           # Figurenbilder & UI-Grafiken
│   ├── Sounds/         # Soundeffekte
│   ├── *.ui            # Qt Designer Files
│   ├── gamewindow.cpp  # Schachbrett-Fenster
│   └── mainwindow.cpp  # Menüfenster
│
├── Logik/
│   ├── SpielController.*   # Spielregeln & Ablauf
│   ├── Spielfigur(en).*    # Verschiedene Figuren
│   ├── Spielfeld.*         # Datenmodell des Boards
│   └── Spieler.*           # Spielerobjekte
│
├── Netzwerk/
│   ├── ChessSocket*.*      # Server, Client, Kommunikation
│   └── GameHandler.*       # Vermittlung GUI <-> Netzwerk
│
├── README.md
└── *.pro / CMakeLists.txt
```

---

## ⚙️ Voraussetzungen

| Komponente     | Version       |
|----------------|---------------|
| Qt             | ≥ 5.15        |
| C++            | ≥ C++17       |
| Compiler       | GCC, Clang, MSVC |

---

## 🚀 Build-Anleitung

### Variante 1: Qt Creator (empfohlen)
```bash
git clone https://github.com/deinname/qt-chess.git
qtcreator GUI/first_trial_chess.pro
```

### Variante 2: CMake
```bash
git clone https://github.com/deinname/qt-chess.git
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/qt-chess
```

---

## ▶️ Spielstart

### Server starten
```bash
./qt-chess --server --port 12345
```

### Client beitreten
```bash
./qt-chess --client --host <IP> --port 12345
```

---

## ⌨️ Steuerung

| Aktion           | Tastenkombination |
|------------------|-------------------|
| Ziehen           | Drag & Drop       |
| Remis anbieten   | Strg + R          |
| Aufgeben         | Strg + Q          |
| Rückgängig       | Strg + Z          |
| Chat öffnen      | Enter             |

---

## 🧪 Tests

```bash
cmake -DENABLE_TESTS=ON .
make tests
./tests/qt_chess_tests
```

---

## 🔮 Roadmap

- [ ] KI-Gegner (Minimax)
- [ ] PGN-Import/Export
- [ ] Mobile-Version (Qt Quick)
- [ ] Datenbankanbindung für Partienspeicherung

---

## 📄 Lizenz

MIT License – siehe `LICENSE`

---

## 🙏 Danksagung

- Qt Doku & Community
- [chess-icons.com](https://chess-icons.com) – SVG-Figuren
- Alle Mitwirkenden & Tester
