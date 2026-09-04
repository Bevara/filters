#!/bin/bash
#
# update_gpac_sources.sh <filtre> [<filtre>...]
#

set -e

source_path="$(cd "$(dirname "$0")" && pwd)"
GPAC="$source_path/third_parties/gpac"
[ -d "$GPAC/include/gpac" ] || { echo "gpac introuvable dans $GPAC" >&2; exit 1; }

# config.h genere : on prend le premier build gpac complet disponible
CONFIG_H=""
for c in "$source_path/../build/filters/third_parties/gpac_release/config.h" \
         "$source_path/../build/filters/third_parties/gpac/config.h"; do
    [ -f "$c" ] && { CONFIG_H="$c"; break; }
done

[ $# -gt 0 ] || { echo "usage: $0 <filtre> [<filtre>...]" >&2; exit 1; }

for f in "$@"; do
    D="$source_path/$f"
    [ -d "$D/include/gpac" ] || { echo "$f : pas d'en-tetes gpac vendores, ignore"; continue; }
    echo "=== $f ==="

    added=0; updated=0; removed=0
    # 1. copie / mise a jour depuis gpac
    while IFS= read -r h; do
        src="$GPAC/include/gpac/$h"; dst="$D/include/gpac/$h"
        mkdir -p "$(dirname "$dst")"
        if [ ! -f "$dst" ]; then cp "$src" "$dst"; added=$((added+1))
        elif ! cmp -s "$src" "$dst"; then cp "$src" "$dst"; updated=$((updated+1)); fi
    done < <(cd "$GPAC/include/gpac" && find . -name '*.h' | sed 's|^\./||')

    # 2. suppression de ce qui n'existe plus en amont, config.h excepte
    while IFS= read -r h; do
        [ "$h" = "config.h" ] && continue
        if [ ! -f "$GPAC/include/gpac/$h" ]; then rm "$D/include/gpac/$h"; removed=$((removed+1)); fi
    done < <(cd "$D/include/gpac" && find . -name '*.h' | sed 's|^\./||')

    # 3. config.h depuis le build
    cfg="inchange"
    if [ -n "$CONFIG_H" ]; then
        cmp -s "$CONFIG_H" "$D/include/gpac/config.h" || { cp "$CONFIG_H" "$D/include/gpac/config.h"; cfg="mis a jour"; }
    else
        cfg="ABSENT du build - lancez ./configure d'abord"
    fi
    echo "  en-tetes : $added ajoutes, $updated mis a jour, $removed supprimes | config.h : $cfg"

    # 4. en-tetes prives copies depuis src/filters (isoffin.h, ff_common.h...)
    #    Ils definissent les structures internes que les sources manipulent :
    #    les oublier laisse des erreurs "no member named ... in ISOMReader".
    priv=0
    while IFS= read -r h; do
        b=$(basename "$h"); [ "$b" = "filter_register.h" ] && continue
        src=$(find "$GPAC/src" -name "$b" 2>/dev/null | head -1)
        [ -n "$src" ] || continue
        cmp -s "$src" "$h" || { cp "$src" "$h"; priv=$((priv+1)); }
    done < <(find "$D" -maxdepth 1 -name '*.h')
    [ "$priv" != "0" ] && echo "  en-tetes prives : $priv mis a jour"

    # 4. rapport sur les sources, sans y toucher
    while IFS= read -r c; do
        src=$(find "$GPAC/src" -name "$c" 2>/dev/null | head -1)
        [ -n "$src" ] || continue
        cmp -s "$D/$c" "$src" && continue
        n=$(diff "$src" "$D/$c" | grep -c "^[<>]" || true)
        # Detection des correctifs locaux : on cherche la marque "Bevara", pas
        # une forme de commentaire. Un comptage de lignes commentees est fragile -
        # il a deja rate un correctif dont les lignes de continuation n'avaient
        # pas de prefixe "*", et le fichier a ete ecrase.
        loc=$(diff "$src" "$D/$c" | grep "^>" | grep "Bevara" | grep -vc "side modules register their own filters" || true)
        # une seule marque suffit : le seuil de 3 datait du comptage de commentaires
        if [ "${loc:-0}" -ge 1 ]; then
            echo "  source A TRIER   : $c ($n lignes d'ecart, $loc de commentaires locaux)"
        else
            echo "  source a reprendre: $c ($n lignes d'ecart, pas de correctif local apparent)"
        fi
    done < <(grep -oE '\$\{CMAKE_CURRENT_SOURCE_DIR\}/[a-zA-Z0-9_]+\.(c|cpp)' "$D/CMakeLists.txt" 2>/dev/null | sed 's|.*/||')
done
