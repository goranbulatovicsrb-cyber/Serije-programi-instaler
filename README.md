# 🎬 SeriesIndexer v2.0

[![Build](https://github.com/YOUR_USERNAME/SeriesIndexer/actions/workflows/build.yml/badge.svg)](https://github.com/YOUR_USERNAME/SeriesIndexer/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/YOUR_USERNAME/SeriesIndexer?color=6c63ff)](https://github.com/YOUR_USERNAME/SeriesIndexer/releases/latest)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue)](#)
[![Qt](https://img.shields.io/badge/Qt-5.15%2B-41CD52)](https://www.qt.io/)

Moderni C++ Qt5/Qt6 program za indeksiranje, pretragu i upravljanje kolekcijom serija.  
Podržava uvoz/izvoz Excel fajlova i automatsko preuzimanje IMDB ocjena.

---

## 📥 Brza instalacija (Windows)

1. Idite na [**Releases**](../../releases/latest)
2. Preuzmite `SeriesIndexer_Windows_x64.zip`
3. Raspakovajte negdje npr. `C:\SeriesIndexer\`
4. Pokrenite **`SeriesIndexer.exe`**

> ✅ Sve Qt DLL-ove su već uključene u ZIP — nema potrebe instalirati ništa!

---

## ✨ Funkcije

| Funkcija | |
|----------|-|
| 📂 Uvoz `.xlsx` fajla | Automatski prepoznaje domaće/strane serije |
| 🔄 Smart Update | Uvezi ažurirani Excel, samo izmjene se upisuju |
| 📤 Izvoz u Excel | Sa IMDB ocjenama, žanrovima, formatiranjem |
| ⭐ IMDB ocjene | Automatsko preuzimanje (ocjena, žanr, opis, glumci) |
| ⭐ IMDB Batch | Preuzmi ocjene za cijelu kolekciju odjednom |
| 🔍 Napredna pretraga | Po nazivu, žanru, godini, hard disku |
| 🎛️ Filteri | Kategorija, HDD, raspon ocjena, gledano/negledano |
| ➕ Ručno dodavanje | Dodaj, uredi, obriši serije |
| 🌙 Dark UI | Moderan profesionalan izgled |

---

## 🛠️ Build iz koda

### Windows (Qt Creator)
1. Instaliraj [Qt 5.15+](https://www.qt.io/download-open-source) — odaberi `MSVC 2019 64-bit` ili `MinGW 64-bit`
2. Otvori `CMakeLists.txt` u Qt Creatoru
3. Odaberi `Release` konfiguraciju → **Build** → **Run**

### Windows (Command line — MSVC)
```bat
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cd build\Release
windeployqt.exe SeriesIndexer.exe
SeriesIndexer.exe
```

### Linux (Ubuntu/Debian)
```bash
sudo apt install cmake qtbase5-dev libqt5sql5-sqlite python3-openpyxl
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./SeriesIndexer
```

---

## 🔄 GitHub Actions — automatski build

Svaki push na `main` granu automatski builda exe za Windows i binarku za Linux.

### Kako dobiti release sa exe-om:

```bash
git clone https://github.com/YOUR_USERNAME/SeriesIndexer.git
cd SeriesIndexer

# Napravi tag za release
git tag v2.0
git push origin v2.0
```

→ GitHub Actions automatski builda i pravi **Release** sa downloadable `.exe`!

---

## ⭐ IMDB API ključ

Za automatsko preuzimanje ocjena potreban je **besplatan** OMDb API ključ:

1. Registrujte se na: https://www.omdbapi.com/apikey.aspx
2. Dobijate 1000 besplatnih zahtjeva/dan
3. U aplikaciji: **⚙ Podešavanja** → unesite ključ → **Sačuvaj**
4. Kliknite **⭐ IMDB Sve** za preuzimanje ocjena za cijelu kolekciju

---

## 📂 Struktura Excel fajla

| SERIJA | VELIČINA | GODINA | HARD DISK |
|--------|----------|--------|-----------|
| Breaking Bad | 20.9 GB | (2008-2013) | WD 5 TB |
| Senke nad Balkanom | 21.6 GB | (2017-) | Hitachi serije |

- Sheet `DOMAĆE SERIJE` → automatski kategorija: Domaće
- Sheet `STRANE SERIJE` → automatski kategorija: Strane

---

## ⌨️ Prečice

| Prečica | Akcija |
|---------|--------|
| `Ctrl+N` | Dodaj seriju |
| `F2` | Uredi odabranu |
| `Delete` | Obriši odabranu |
| `Space` | Toggle gledano/negledano |
| `Ctrl+O` | Uvezi Excel |
| `Ctrl+U` | Update iz Excel |
| `Ctrl+E` | Izvezi u Excel |
| `Ctrl+I` | IMDB za odabranu seriju |
| `Ctrl+Shift+I` | IMDB batch za sve |
| `Ctrl+,` | Podešavanja |

---

## 🗄️ Baza podataka

SQLite baza se automatski kreira pri prvom pokretanju:
- **Windows:** `%APPDATA%\SeriesIndexer\SeriesIndexer\series.db`
- **Linux:** `~/.local/share/SeriesIndexer/SeriesIndexer/series.db`

---

## 📄 Licenca

MIT License — slobodno koristite, modificirajte i distribuirajte.
