#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
============================================================
  SeriesIndexer v2.0 — Generator Serijskih Brojeva
  Autor: SAMO VI koristite ovaj skript!
  
  VAŽNO: Isti SECRET_KEY mora biti u licensemanager.cpp !
============================================================
"""

import hashlib
import random
import string
import sys

# !! MORA BITI ISTI KAO U licensemanager.cpp !!
SECRET_KEY = "SI2025-FilmoviSerije-XK7mP9qR"

# Isti set znakova kao u licensemanager.cpp (bez I, O, 1, 0)
VALID_CHARS = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"


def compute_checksum(stripped: str) -> str:
    """Compute 2-char HMAC checksum — must match C++ implementation."""
    data = stripped[:18] + SECRET_KEY
    h = hashlib.sha256(data.encode('utf-8')).digest()
    i0 = h[0] % len(VALID_CHARS)
    i1 = h[1] % len(VALID_CHARS)
    return VALID_CHARS[i0] + VALID_CHARS[i1]


def generate_group() -> str:
    """Generate one valid 5-char group where char[4] is group checksum."""
    data = [random.choice(VALID_CHARS) for _ in range(4)]
    total = sum(VALID_CHARS.index(c) for c in data)
    checksum_char = VALID_CHARS[total % len(VALID_CHARS)]
    return ''.join(data) + checksum_char


def generate_serial() -> str:
    """Generate a valid 20-char serial, then fix last 2 chars as global checksum."""
    while True:
        # Generate 3 full valid groups (15 chars)
        groups = [generate_group() for _ in range(3)]
        base = ''.join(groups)  # 15 chars

        # Generate 4th group first 3 data chars randomly
        g4_data = [random.choice(VALID_CHARS) for _ in range(3)]

        # The 4th group has 4 data chars and 1 checksum
        # But chars [18] and [19] are ALSO the global checksum
        # So: g4_data[0..2] = random, g4_data[3] = random, g4_checksum at [19]
        # Actually last group: positions 15,16,17,18,19
        # positions 18,19 = global checksum (overrides group checksum logic for last group)

        # Build 18-char prefix: first 3 groups + first 3 chars of group 4
        prefix18 = base + ''.join(g4_data)  # 18 chars

        # Compute global checksum (2 chars) for positions 18,19
        checksum = compute_checksum(prefix18 + "XX")  # placeholder, recalc on full

        # Actually: computeChecksum takes all 20 chars, uses first 18 as data
        # So we need the final serial to have checksum at [18][19]
        # Group 4 = g4_data[0..2] + checksum[0] + checksum[1]
        # But group 4 checksum rule: sum(chars[0..3]) % len == chars[4]
        # We override the last group's checksum with global checksum
        serial = prefix18 + checksum  # 20 chars

        # Verify it passes validation
        if validate_serial(serial):
            return format_serial(serial)


def format_serial(raw: str) -> str:
    r = raw.upper()
    return f"{r[0:5]}-{r[5:10]}-{r[10:15]}-{r[15:20]}"


def validate_serial(serial: str) -> bool:
    """Validate serial — must match C++ algorithm exactly."""
    s = serial.replace('-', '').replace(' ', '').upper()
    if len(s) != 20:
        return False

    # Valid chars check
    for c in s:
        if c not in VALID_CHARS:
            return False

    # Group checksum check (groups 0-3, but we skip last group's checksum
    # since positions 18,19 are global checksum — only check groups 0-2)
    for g in range(3):
        base = g * 5
        total = sum(VALID_CHARS.index(s[base + i]) for i in range(4))
        expected = VALID_CHARS[total % len(VALID_CHARS)]
        if s[base + 4] != expected:
            return False

    # Global HMAC checksum check
    checksum = compute_checksum(s)
    if s[18] != checksum[0] or s[19] != checksum[1]:
        return False

    return True


def print_banner():
    print("=" * 60)
    print("  🎬  SeriesIndexer — Generator Serijskih Brojeva")
    print("=" * 60)
    print()


def interactive_mode():
    print_banner()
    print("Opcije:")
    print("  1. Generiši jedan serijski broj")
    print("  2. Generiši više serijskih brojeva")
    print("  3. Provjeri serijski broj")
    print("  4. Izlaz")
    print()

    while True:
        choice = input("Odaberite opciju (1-4): ").strip()

        if choice == '1':
            serial = generate_serial()
            print(f"\n✅  Serijski broj: {serial}\n")

        elif choice == '2':
            try:
                n = int(input("Koliko serijskih brojeva? ").strip())
                n = min(max(n, 1), 1000)
                print(f"\n{'='*40}")
                serials = []
                for i in range(n):
                    s = generate_serial()
                    serials.append(s)
                    print(f"  {i+1:3}. {s}")
                print(f"{'='*40}")

                # Save to file option
                save = input("\nSačuvati u fajl? (d/n): ").strip().lower()
                if save == 'd':
                    filename = input("Naziv fajla [serials.txt]: ").strip() or "serials.txt"
                    with open(filename, 'w', encoding='utf-8') as f:
                        f.write(f"SeriesIndexer v2.0 — Serijski Brojevi\n")
                        f.write(f"Generisano: {__import__('datetime').datetime.now()}\n")
                        f.write("=" * 40 + "\n")
                        for s in serials:
                            f.write(s + "\n")
                    print(f"✅  Sačuvano u: {filename}")
                print()
            except ValueError:
                print("❌  Unesite broj!")

        elif choice == '3':
            serial = input("Unesite serijski broj: ").strip()
            if validate_serial(serial):
                print(f"✅  ISPRAVAN serijski broj: {format_serial(serial.replace('-',''))}")
            else:
                print(f"❌  NEISPRAVAN serijski broj")
            print()

        elif choice == '4':
            print("Izlaz.")
            break

        else:
            print("Nepoznata opcija.")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        # Command line: python keygen.py [count]
        try:
            count = int(sys.argv[1])
        except ValueError:
            # Maybe it's a serial to validate
            serial = sys.argv[1]
            if validate_serial(serial):
                print(f"VALID: {format_serial(serial.replace('-',''))}")
            else:
                print("INVALID")
            sys.exit(0)

        for _ in range(min(count, 10000)):
            print(generate_serial())
    else:
        interactive_mode()
