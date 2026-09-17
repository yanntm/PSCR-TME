#!/usr/bin/env bash
# Résumé à copier au début de answers.md ; aucune donnée d'identification personnelle.
set -eu
export LC_ALL=C

printf '## Machine de mesure\n\n```text\n'
printf 'Date : %s\n' "$(date -u '+%Y-%m-%d %H:%M UTC')"
printf 'Système : %s\n' "$(uname -srm)"
if [ -r /etc/os-release ]; then
    awk -F= '$1 == "PRETTY_NAME" {gsub(/"/, "", $2); print "Distribution : " $2}' /etc/os-release
fi
if command -v lscpu >/dev/null 2>&1; then
    lscpu | awk -F: '
        {value=$2; sub(/^[ \t]+/, "", value)}
        /^Model name:/ {print "CPU : " value}
        /^CPU\(s\):/ {print "CPU logiques (système) : " value}
        /^Thread\(s\) per core:/ {print "Threads par cœur : " value}
        /^Core\(s\) per socket:/ {print "Cœurs par socket : " value}
        /^Socket\(s\):/ {print "Sockets : " value}
        /^CPU max MHz:/ {print "Fréquence maximale annoncée (MHz) : " value; freq=1}
        END {if (!freq) print "Fréquence maximale : non exposée par le système"}'
    if command -v nproc >/dev/null 2>&1; then
        printf 'CPU logiques disponibles pour ce processus : %s\n' "$(nproc)"
    fi
    if [ -r /proc/meminfo ]; then
        awk '/^MemTotal:/ {printf "RAM visible : %.1f GiB\n", $2/1048576}' /proc/meminfo
    fi
elif [ "$(uname -s)" = Darwin ]; then
    printf 'macOS : %s\n' "$(sw_vers -productVersion)"
    printf 'CPU : %s\n' "$(sysctl -n machdep.cpu.brand_string 2>/dev/null || printf 'non disponible')"
    printf 'Cœurs physiques : %s\n' "$(sysctl -n hw.physicalcpu)"
    printf 'CPU logiques : %s\n' "$(sysctl -n hw.logicalcpu)"
    hz=$(sysctl -n hw.cpufrequency_max 2>/dev/null || true)
    if [ -n "$hz" ]; then
        awk -v hz="$hz" 'BEGIN {printf "Fréquence maximale annoncée : %.2f GHz\n", hz/1e9}'
    else
        printf 'Fréquence maximale : non exposée par le système\n'
    fi
else
    printf 'CPU et fréquence : à compléter manuellement\n'
fi
if command -v c++ >/dev/null 2>&1; then
    c++ --version | sed -n '1s/^/Compilateur par défaut : /p'
fi
printf '```\n\n'
printf 'À compléter : Debug/Release, machine native/VM/WSL/conteneur, charge concurrente.\n'
printf 'La fréquence réelle varie avec la charge, le turbo et la température.\n'
