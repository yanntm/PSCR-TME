# TME1 : TestListe

Programme `TestList` à corriger : le code fourni contient des fautes de compilation, de link, d'exécution et de gestion mémoire (voir l'énoncé).

## Compiler

Depuis ce dossier, avec un compilateur C++20 et CMake :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/TestList
```

Sur Linux, la première commande génère une fois pour toutes un Makefile classique dans `build-debug` ; la seconde l'invoque (équivalent portable de `make -C build-debug`). Après chaque modification du code, relancez `cmake --build build-debug`.

Debug (`-O0 -g -Wall -Wextra -pedantic`) : pas d'optimisation, symboles pour le debugger, warnings. Pour un binaire optimisé (`-O3`), configurez une seconde fois dans un autre dossier :

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Dans VS Code avec l'extension CMake Tools, ouvrez ce dossier et acceptez la configuration proposée ; la barre d'état donne le choix Debug/Release, Build, Run et Debug. Le binaire est alors dans `build/`.

## Valgrind

Sur le binaire Debug :

```sh
valgrind --leak-check=full ./build-debug/TestList
```
