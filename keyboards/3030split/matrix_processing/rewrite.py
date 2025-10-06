#!/usr/bin/env python3
"""
clean_and_convert.py

Reads a messy JSON-like input file and writes one cleaned JSON object per line
in the output file, in the requested format:
{"matrix": [row, col], "x": ..., "y": ..., "r": ..., "rx": ..., "ry": ...}
"""

import json
import re
import sys
from pathlib import Path

INPUT = "data.json"    # change to your filename
OUTPUT = "output.jsonl"

def remove_trailing_commas(s: str) -> str:
    """Remove trailing commas before a } or ] (simple structural clean)."""
    # This removes patterns like ",}" or ", ]" (with optional whitespace/newlines)
    # We iterate until nothing changes because nested cleaning may expose more.
    prev = None
    while prev != s:
        prev = s
        s = re.sub(r',\s*([}\]])', r'\1', s, flags=re.M)
    return s

def extract_top_level_objects(text: str):
    """
    Scan the text and extract top-level {...} objects.
    This parser tracks quotes/escapes so braces inside strings don't confuse it.
    Returns a list of object strings (including their outer braces).
    """
    objs = []
    i = 0
    n = len(text)
    depth = 0
    in_string = False
    string_char = None
    escape = False
    current_start = None

    while i < n:
        ch = text[i]

        if in_string:
            if escape:
                escape = False
            elif ch == '\\':
                escape = True
            elif ch == string_char:
                in_string = False
                string_char = None
            # else stay in string
        else:
            # not in string
            if ch == '"' or ch == "'":
                in_string = True
                string_char = ch
            elif ch == '{':
                if depth == 0:
                    current_start = i
                depth += 1
            elif ch == '}':
                if depth > 0:
                    depth -= 1
                    if depth == 0 and current_start is not None:
                        obj_str = text[current_start:i+1]
                        objs.append(obj_str)
                        current_start = None
                # else unmatched brace - ignore
            # ignore other chars
        i += 1

    return objs

def clean_object_text(obj_text: str) -> str:
    """
    Apply cleaning passes to an object text:
    - remove trailing commas before } and ]
    - (optionally) remove JS-style comments (naive)
    """
    # Remove common JS-style comments outside strings (naive approach):
    # First remove block comments /* ... */ and line comments // ...\n
    # These regexes can accidentally remove content inside strings; we already
    # extracted objects tracking strings, so it's less likely — still be cautious.
    obj_text = re.sub(r'/\*.*?\*/', '', obj_text, flags=re.S)
    obj_text = re.sub(r'//.*?(?=\n|$)', '', obj_text, flags=re.M)

    # Remove trailing commas before } or ]
    obj_text = remove_trailing_commas(obj_text)

    return obj_text

def convert_objects_to_output(objects, out_path):
    with open(out_path, "w", encoding="utf-8") as outf:
        for idx, obj_text in enumerate(objects):
            cleaned = clean_object_text(obj_text)
            try:
                data = json.loads(cleaned)
            except json.JSONDecodeError as e:
                # If a single object fails to parse, show a helpful error and continue
                print(f"[Warning] JSON parse failed for object #{idx}: {e}")
                print("Snippet (first 300 chars):")
                print(cleaned[:300])
                continue

            # Pull values (with fallbacks)
            row = data.get("row")
            col = data.get("col")
            state = data.get("state", {})

            # If row/col absent, try to pull from nested keys or continue
            if row is None or col is None:
                print(f"[Warning] object #{idx} missing row/col; skipping.")
                continue

            out_obj = {
                "matrix": [row, col],
                "x": state.get("x", 0),
                "y": state.get("y", 0),
                "r": state.get("r", 0),
                "rx": state.get("rx", 0),
                "ry": state.get("ry", 0),
            }
            outf.write(json.dumps(out_obj, ensure_ascii=False) + "\n")

def main(input_path=INPUT, output_path=OUTPUT):
    p = Path(input_path)
    if not p.exists():
        print(f"Input file not found: {input_path}", file=sys.stderr)
        return 1

    raw = p.read_text(encoding="utf-8")

    # Quick heuristic: if the file uses outer { { ... }, { ... } } style,
    # try to strip the outermost braces and treat the inside as text to extract.
    text = raw.strip()
    # If it starts with "{" and then whitespace and then "{" we likely have an extra wrapper.
    # But extraction below doesn't care; it finds every top-level {...} block.
    objects = extract_top_level_objects(text)
    if not objects:
        print("No JSON objects found in the file.", file=sys.stderr)
        return 1

    print(f"Found {len(objects)} top-level object(s). Cleaning and writing to {output_path} ...")
    convert_objects_to_output(objects, output_path)
    print("Done.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
