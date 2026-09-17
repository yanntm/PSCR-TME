# TME4 — Files de tâches et pipeline

Le programme divise par deux la largeur et la hauteur des images d'un dossier.
L'énoncé donne les questions ; `answers.md` conserve vos commandes, hypothèses,
observations et conclusions. Toutes les commandes ci-dessous partent du dossier du TME.

## Préparer et compiler

Dépendances : compilateur C++20, CMake, Qt6 et un shell (Linux, macOS ou WSL).
Pour Qt6 : `sudo apt install qt6-base-dev` sur Ubuntu/Debian,
`sudo dnf install qt6-qtbase-devel` sur Fedora, `brew install qt` sur macOS.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 2
./download_images.sh
```

Le téléchargement se fait une seule fois. On peut aussi utiliser ses propres images,
placées directement dans `input_images` (pas dans des sous-dossiers).
Pour le débogage, utiliser un autre dossier :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug -j 2
```

Mesurer en Release. Recompiler après chaque modification.

## Commencer par des exécutions directes (Q1–Q6)

```sh
rm -rf output_reference
./build/TME4 -m resize -i input_images -o output_reference
rm -rf output_images
./build/TME4 -m pipe -i input_images -o output_images
./check.sh output_reference output_images
```

`resize` traite les images dans main ; `pipe` utilise main et un consommateur.
Le programme affiche le temps écoulé, les temps CPU et la mémoire du processus.
`check.sh` compare seulement le nombre de fichiers à la référence séquentielle.
Vérifier aussi l'absence d'erreur et la terminaison normale.
Garder `output_reference` pour les comparaisons. Réutiliser `output_images` pour chaque
essai, en le supprimant avant de lancer le programme : celui-ci recrée le dossier.
Cela évite de compter des fichiers d'un essai précédent. Conserver les mesures dans
`answers.md` ; les images du dernier essai peuvent être supprimées après vérification.
Les erreurs de chargement/sauvegarde sont affichées.

En Q2, utiliser au moins deux images et une file de capacité 1. Pour borner une
exécution volontairement bloquée sur Linux/WSL :

```sh
rm -rf output_images
timeout 10 ./build/TME4 -m pipe -q 1 -i input_images -o output_images
```

Le code de retour 124 signifie que `timeout` a interrompu la commande.
Sans cet utilitaire (notamment sur macOS), interrompre avec Ctrl-C après avoir
constaté le blocage. Adapter le délai si le traitement correct dure déjà plus de 10 s.
Restaurer le code avant chaque permutation et à la fin de la question.

Après implantation de Q3 :

```sh
rm -rf output_images
./build/TME4 -m pipe_mt -n 1 -i input_images -o output_images
./check.sh output_reference output_images
rm -rf output_images
./build/TME4 -m pipe_mt -n 4 -i input_images -o output_images
./check.sh output_reference output_images
```

Après Q5, puis Q6 :

```sh
rm -rf output_images
./build/TME4 -m mt_pipeline -i input_images -o output_images
./check.sh output_reference output_images
rm -rf output_images
./build/TME4 -m mt_pipeline -r 4 -s 4 -w 4 -i input_images -o output_images
./check.sh output_reference output_images
```

Les champs d'options et leur parsing sont fournis. Les modes `pipe_mt` et
`mt_pipeline` du squelette signalent qu'ils restent à implanter (code de retour 2).
Les trois fonctions d'étape ont leurs signatures et chronomètres, avec des corps à compléter.

## Comparer plusieurs configurations (à partir de Q7)

Le lanceur exécute chaque configuration et conserve ses mesures et ses traces.

```sh
./measure.sh reference
./measure.sh pipe_mt 1 2 4 8
./measure.sh pipeline 1/1/1 2/1/1 1/2/1 1/1/2
```

- `reference` lance le mode séquentiel et retient le nombre de fichiers produits.
- Après `pipe_mt`, chaque entier est un nombre de consommateurs.
- Après `pipeline`, chaque triplet donne readers/resizers/savers.

Le tableau rapporte temps écoulé (ms), CPU cumulé (ms), pic RAM (octets), nombre de
fichiers, code de retour et contrôle. `OK` signifie sortie normale avec le nombre
attendu de fichiers ; ce contrôle ne compare pas les pixels.
Chaque invocation crée un dossier daté dans `results`, avec `results.csv` et, pour
chaque configuration, `arguments.txt` (un argument par ligne) et `output.log`.
Les temps CPU par étape sont dans `output.log`. Les images produites sont comptées,
puis supprimées après chaque mesure, y compris en cas d’échec. Le dossier temporaire
est aussi nettoyé lorsque le script est interrompu avec Ctrl-C.
Un programme qui se bloque reste bloqué : l'interrompre, puis expliquer le problème
avant de poursuivre les mesures.

Le contrôle s'applique au même jeu d'entrée : refaire `reference` si ce jeu change.
Conserver également la machine utilisée et les modifications de code dans le journal.
Les résultats conservés sont uniquement les commandes, les traces et les rapports.

Options facultatives (après les configurations) :

```sh
./measure.sh reference --input mes_images --exe ./build/TME4
./measure.sh pipe_mt 1 4 --input mes_images --exe ./build/TME4
./measure.sh pipeline 4/1/1 --queue-size 1
./measure.sh pipeline 4/1/1 --queue-size 100
```

`--results autre_dossier` permet de séparer les contrôles et expériences de jeux différents.

Les scripts utilisent le shell et les utilitaires courants (`find`, `wc`, `awk`,
`mktemp`). Aucune dépendance Python.
