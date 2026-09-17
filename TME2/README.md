# TME 2 — countword

Le programme lit un livre et calcule progressivement ses fréquences de mots.
Le sujet PDF contient les questions. Travaillez dans `src/main.cpp` et
`src/FreqMap.h` ; gardez vos réponses et traces dans `answers.md`.

## Décrire la machine

Depuis le dossier du TME :

```sh
./machine-info.sh
```

Copiez le bloc affiché au début de `answers.md`. Complétez le contexte :
Debug/Release, machine native ou VM/WSL/conteneur, autres programmes actifs.
Le script indique l'OS, le CPU, les cœurs, les processeurs logiques, la RAM et la
fréquence maximale annoncée lorsqu'elle est accessible. Sous Windows, utilisez
WSL ou renseignez ces informations manuellement. La fréquence réelle varie
(turbo, température, économie d'énergie) ; les temps dépendent de la plateforme.
Le compilateur affiché est celui de `c++` ; précisez le vôtre si CMake en utilise un autre.

## Compiler

Depuis le dossier du TME, avec un compilateur C++20 et CMake :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Debug facilite le débogage ; Release sert aux mesures de performances.
Après chaque modification du code, relancez `cmake --build build-debug` ou
`cmake --build build-release` selon la configuration utilisée.

## Prise en main

Les commandes suivantes se lancent depuis le dossier du TME :

```sh
./build-debug/countword WarAndPeace.txt count
./build-release/countword WarAndPeace.txt count
```

Syntaxe : `countword [fichier] [mode] [nombre_de_buckets]`.
Le mode `count` est fourni. Les modes `unique`, `freq`, `freqstd` et `freqhash`
sont à compléter au fil des questions. Le dernier argument ne sert qu'au mode
`freqhash`.

Si vous lancez le programme sans arguments depuis un dossier de build, il utilise
`../WarAndPeace.txt` et le mode `count`. Les commandes ci-dessus donnent le chemin
explicitement pour rester dans le dossier du TME.

## Mesures

Une fois le mode `freqstd` implanté, répétez une mesure depuis le dossier du TME :

```sh
for essai in 1 2 3; do
  printf '\nEssai %s\n' "$essai"
  ./build-release/countword WarAndPeace.txt freqstd || break
done
```

Vérifiez les résultats avant de comparer les temps : même nombre de mots, mêmes
fréquences et même top 10 selon la question. Précisez aussi si l'affichage d'un
mot toutes les 100 lectures est activé. Une seule mesure ne suffit pas à conclure
sur un petit écart ; évitez les autres tâches lourdes pendant les essais.

## Garder une trace

Dans `answers.md`, remplacez l'exemple de la question 1 par votre exécution.
Pour chaque question, copiez les commandes et leurs résultats entre triples
backquotes ; vous pouvez couper le milieu des longues sorties, en conservant
les dernières lignes et le temps d'exécution. Ajoutez vos explications lorsque
la question en demande.

Conservez votre code et vos réponses dans votre dépôt, puis faites un push en
fin de séance.
