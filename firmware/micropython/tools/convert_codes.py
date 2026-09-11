#!/usr/bin/env python3
"""Convert the actual C++ region tables, including aliases and ordering.

Derived from Mitch Altman, Limor Fried, Ken Shirriff and BenBE's TV-B-Gone
sources. This port and generated data are licensed CC BY-SA 4.0.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT.parent / "TV-B-Gone_kit_V2" / "WORLDcodes.cpp"


def parse_source(source):
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", source, flags=re.S)
    arrays = {}
    for name, body in re.findall(r"uint(?:8|32)_t\s+(\w+)\[\]\s*=\s*\{(.*?)\};", text, re.S):
        arrays[name] = [int(v.strip()) for v in body.split(",") if v.strip()]
    codes = {}
    for name, body in re.findall(r"struct\s+IrCode\s+(\w+)\s*=\s*\{(.*?)\};", text, re.S):
        freq, count, pairs_name, sequence_name = [v.strip() for v in body.split(",") if v.strip()]
        pairs, sequence = arrays[pairs_name], arrays[sequence_name]
        if len(pairs) % 2 or len(sequence) != int(count) or not sequence:
            raise ValueError("Invalid pair count: " + name)
        if any(i < 0 or i >= len(pairs) // 2 for i in sequence):
            raise ValueError("Invalid pair index: " + name)
        if int(freq) < 0 or any(v < 0 for v in pairs):
            raise ValueError("Negative timing: " + name)
        codes[name] = [name, int(freq), pairs, sequence]
    regions = {}
    for region in ("NA", "EU"):
        match = re.search(region + r"powerCodes\[\]\s*=\s*\{(.*?)\};", text, re.S)
        if match is None:
            raise ValueError("Missing region: " + region)
        names = re.findall(r"&\s*(\w+)", match[1])
        if not names:
            raise ValueError("Empty region: " + region)
        regions[region] = [codes[name] for name in names]
    return regions


def generate(source=SOURCE):
    raw = source.read_bytes()
    regions = parse_source(raw.decode())
    files = {}
    for region, codes in regions.items():
        files["codes_" + region.lower() + ".jsonl"] = "".join(
            json.dumps(code, separators=(",", ":")) + "\n" for code in codes
        )
    files["codes_manifest.json"] = json.dumps({
        "source": "../TV-B-Gone_kit_V2/WORLDcodes.cpp",
        "sha256": hashlib.sha256(raw).hexdigest(),
        "license": "CC-BY-SA-4.0",
        "counts": {region: len(codes) for region, codes in regions.items()},
    }, indent=2) + "\n"
    return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if checked-in data is stale")
    args = parser.parse_args()
    for name, content in generate().items():
        path = ROOT / name
        if args.check:
            if not path.exists() or path.read_text() != content:
                raise SystemExit("Stale generated file: " + str(path))
        else:
            path.write_text(content)
        print(name, len(content.encode()), "bytes")


if __name__ == "__main__":
    main()
