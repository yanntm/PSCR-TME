#!/bin/sh
# Une exécution par configuration : traces brutes, mesures, sortie et nombre de fichiers.
usage() {
    echo "Usage : ./measure.sh reference"
    echo "        ./measure.sh pipe_mt 1 2 4 8"
    echo "        ./measure.sh pipeline 1/1/1 2/1/1 1/2/1"
    echo "Options : --exe chemin --input dossier --queue-size N"
    echo "          --results dossier"
}
fail() { echo "Erreur : $*" >&2; exit 1; }
positive() { case "$1" in ''|*[!0-9]*|0) return 1;; esac; }
[ "$#" -gt 0 ] || { usage; exit 1; }
case "$1" in -h|--help) usage; exit 0;; esac
mode=$1
shift
case "$mode" in reference|pipe_mt|pipeline) ;; *) usage; exit 1;; esac
exe=./build/TME4
input=input_images
results=results
queue=10
configs=
while [ "$#" -gt 0 ]; do
    case "$1" in
        --exe|--input|--results|--queue-size)
            [ "$#" -ge 2 ] || fail "Valeur manquante pour $1"
            case "$1" in
                --exe) exe=$2;; --input) input=$2;; --results) results=$2;;
                --queue-size) queue=$2;;
            esac
            shift 2;;
        --*) fail "Option inconnue : $1";;
        *)
            # Les configurations ne contiennent que des entiers et des barres obliques.
            case "$1" in ''|*[!0-9/]*) fail "Configuration invalide : $1";; esac
            configs="$configs $1"
            shift;;
    esac
done
positive "$queue" || fail "Capacités strictement positives requises."
[ -x "$exe" ] || fail "Exécutable absent : $exe"
[ -d "$input" ] || fail "Dossier d'entrée absent : $input"
mkdir -p "$results" || exit 1
if [ "$mode" = reference ]; then
    [ -z "$configs" ] || fail "reference ne prend pas de configuration."
    configs=reference
else
    [ -n "$configs" ] || fail "Indiquer au moins une configuration."
    [ -f "$results/reference.count" ] || fail "Lancer d'abord ./measure.sh reference."
    expected=$(cat "$results/reference.count")
    positive "$expected" || fail "Référence invalide."
fi
# Vérifier les arguments avant de lancer la première mesure.
for config in $configs; do
    case "$mode" in
        pipe_mt) positive "$config" || fail "Effectif invalide : $config";;
        pipeline)
            r=${config%%/*}; rest=${config#*/}; s=${rest%%/*}; w=${rest#*/}
            [ "$r/$s/$w" = "$config" ] && positive "$r" && positive "$s" && positive "$w" \
                || fail "Attendu : readers/resizers/savers, par exemple 2/1/1.";;
    esac
done
run=$(mktemp -d "$results/$(date +%Y%m%d-%H%M%S)-$mode-XXXXXX") || exit 1
# Un seul dossier temporaire d'images, vidé après chaque mesure.
images=$run/images
cleanup() { rm -rf -- "$images"; }
trap cleanup 0
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM
printf 'configuration,elapsed_ms,cpu_ms,peak_rss_bytes,files,exit_code,check\n' > "$run/results.csv"
printf '%-14s %12s %12s %16s %8s %6s %s\n' config elapsed_ms cpu_ms peak_rss_bytes files exit check
failed=0
index=0
for config in $configs; do
    index=$((index + 1))
    folder=$run/$index
    mkdir -p "$folder" "$images" || exit 1
    case "$mode" in
        reference) set -- -m resize;;
        pipe_mt) set -- -m pipe_mt -n "$config";;
        pipeline)
            r=${config%%/*}; rest=${config#*/}; s=${rest%%/*}; w=${rest#*/}
            set -- -m mt_pipeline -r "$r" -s "$s" -w "$w";;
    esac
    set -- "$exe" "$@" -i "$input" -o "$images" -q "$queue"
    # Un argument par ligne pour garder les chemins avec espaces lisibles.
    printf '%s\n' "$@" > "$folder/arguments.txt"
    "$@" > "$folder/output.log" 2>&1
    code=$?
    count=$(find "$images" -type f | wc -l | tr -d ' ')
    cleanup || fail "Impossible de supprimer les images temporaires."
    elapsed=$(awk '/^Total runtime \(wall clock\):/{print $(NF-1)}' "$folder/output.log")
    cpu=$(awk '/^Total CPU time across all threads:/{print $(NF-1)}' "$folder/output.log")
    ram=$(awk '/^Peak RSS \(bytes\):/{print $NF}' "$folder/output.log")
    status=OK
    if [ "$code" -ne 0 ]; then
        status=EXIT_ERROR
    elif [ "$count" -eq 0 ]; then
        status=EMPTY
    elif [ "$mode" = reference ]; then
        printf '%s\n' "$count" > "$results/reference.count"
        printf '%s\n' "$run" > "$results/reference.run"
        status=REFERENCE
    elif [ "$count" -ne "$expected" ]; then
        status=COUNT_ERROR
    fi
    case "$status" in OK|REFERENCE) ;; *) failed=1;; esac
    printf '%s,%s,%s,%s,%s,%s,%s\n' "$config" "$elapsed" "$cpu" "$ram" "$count" "$code" "$status" >> "$run/results.csv"
    printf '%-14s %12s %12s %16s %8s %6s %s\n' "$config" "$elapsed" "$cpu" "$ram" "$count" "$code" "$status"
done
echo "Traces, arguments et tableau : $run"
exit "$failed"
