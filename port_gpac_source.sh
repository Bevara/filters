#!/bin/bash
# port_gpac_source.sh <filter> <file.c>
#
# Takes a vendored gpac source from third_parties/gpac and applies the
# two adaptations required by the side modules:
#   - EMSCRIPTEN_KEEPALIVE on the entry points, to survive dead code
#     elimination
#   - the final filter auto-registration block
#
# The block is REGENERATED from the (name, function) pairs read in the
# current file, rather than being copied: extracting a block by anchoring on the include was
# fragile, as the include sometimes appears at the top of the file (dec_vorbis.c line 31 out of
# 331) and the extraction would pull in the entire body.
#
# Functions renamed upstream are detected and reported.
#
# To be used only on files marked "to update" (a reprendre) by
# update_gpac_sources.sh: those marked "TO SORT" (A TRIER) contain local patches.
set -e
f="$1"; c="$2"
[ -n "$f" ] && [ -n "$c" ] || { echo "usage: $0 <filtre> <fichier.c>" >&2; exit 1; }
root="$(cd "$(dirname "$0")" && pwd)"
G="$root/third_parties/gpac"
dst="$root/$f/$c"
src=$(find "$G/src" -name "$c" | head -1)
[ -n "$src" ] || { echo "$c introuvable dans gpac" >&2; exit 1; }
[ -f "$dst" ] || { echo "$dst introuvable" >&2; exit 1; }

keep=$(grep -oE "EMSCRIPTEN_KEEPALIVE [a-z0-9_]+" "$dst" | awk '{print $2}' | sort -u)
pairs=$(grep -oE 'gf_filter_auto_register\("[a-z0-9_]+", *[a-z0-9_]+\)' "$dst" \
        | sed 's/gf_filter_auto_register("//;s/", */|/;s/)//')
ctor=$(grep -oE 'void register_[a-z0-9_]+\(void\)' "$dst" | head -1 | sed 's/void //;s/(void)//')

cp "$src" "$dst"

for fn in $keep; do
    perl -0pi -e "s/const GF_FilterRegister \*\Q$fn\E\(/const GF_FilterRegister * EMSCRIPTEN_KEEPALIVE $fn(/g" "$dst"
done

if [ -n "$pairs" ]; then
    {
        printf '\n/*Bevara: side modules register their own filters at load time.*/\n'
        printf '#include "filter_register.h"\n__attribute__((constructor))\nvoid %s(void) {\n' "${ctor:-register_$(echo "$c" | sed 's/\.c$//')}"
        echo "$pairs" | while IFS='|' read -r name fn; do
            if grep -q "\b$fn\b" "$dst"; then
                printf '    gf_filter_auto_register("%s", %s);\n' "$name" "$fn"
            else
                alt=$(grep -oE "const GF_FilterRegister \*[A-Za-z_ ]*\b[a-z0-9_]+_register\(" "$dst" | grep -oE "[a-z0-9_]+_register" | sort -u | head -1)
                echo "  RENOMME dans $f/$c : $fn -> ${alt:-INTROUVABLE}" >&2
                [ -n "$alt" ] && printf '    gf_filter_auto_register("%s", %s);\n' "$name" "$alt"
            fi
        done
        printf '}\n'
    } >> "$dst"
fi
echo "  $f/$c repris ($(echo "$keep" | wc -w | tr -d ' ') point(s) d'entree, $(echo "$pairs" | grep -c . || true) filtre(s) enregistre(s))"
