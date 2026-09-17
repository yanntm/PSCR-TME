#!/bin/sh
# Contrôle simple : même nombre de fichiers que la version séquentielle.
if [ "$#" -ne 2 ] || [ ! -d "$1" ] || [ ! -d "$2" ]; then
    echo "Usage : ./check.sh dossier_reference dossier_sortie" >&2
    exit 1
fi
reference=$(find "$1" -type f | wc -l | tr -d ' ')
actual=$(find "$2" -type f | wc -l | tr -d ' ')
if [ "$reference" -eq 0 ]; then
    echo "ERREUR : référence vide." >&2
    exit 1
fi
if [ "$reference" -ne "$actual" ]; then
    echo "ERREUR : $actual fichiers produits, $reference attendus."
    exit 1
fi
echo "OK : $actual fichiers dans chaque dossier."
