# TME 5 — ray tracer

Le programme dessine une scène de sphères colorées et éclairées dans une image BMP.
Le sujet PDF contient les questions. Travaillez dans `src/Renderer.h`, `src/Queue.h`,
`src/Pool.h` et `src/main.cpp` ; gardez vos réponses et vos temps dans `answers.md`.

## Compiler

Depuis le dossier du TME, avec un compilateur C++20 et CMake, aucune dépendance :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Debug facilite le débogage ; Release sert aux mesures. Après chaque modification,
relancez `cmake --build build-release`.

## Lancer

Depuis le dossier du TME :

```sh
./build-release/TME5
./build-release/TME5 -m sequential -W 100 -H 100
```

Le programme écrit `spheres.bmp`, par défaut en 2000x2000 avec 250 sphères.

Options :

- `-o,--output` : fichier BMP produit (défaut : spheres.bmp)
- `-W,--width`, `-H,--height` : résolution (défaut : 2000x2000)
- `-s,--spheres` : nombre de sphères (défaut : 250)
- `-m,--mode` : `sequential` (fourni), puis `ThreadPerPixel`, `ThreadPerRow`, `ThreadManual`,
  `PoolPixel`, `PoolRow`, `PoolFunctionalRow` au fil des questions
- `-n,--nbthread` : nombre de threads pour les modes qui en prennent (défaut : 4)

Les branches des modes sont prêtes, en commentaire, dans le `main` : décommentez chacune
quand vous implantez la méthode correspondante dans `Renderer`.

## Garder une trace

Dans `answers.md`, commencez par décrire votre machine (cœurs, fréquence, RAM, compilateur,
OS), puis pour chaque question copiez la commande et la ligne `Total time` obtenue.
Mesurez en Release, à résolution fixée, et comparez les modes entre eux.

Conservez votre code et vos réponses dans votre dépôt, puis faites un push en fin de séance.
