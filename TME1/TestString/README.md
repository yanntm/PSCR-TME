# TME1 : TestString (bonus)

Implémentez la classe `pr::String` et ses fonctions utilitaires dans `src/String.cpp` et `src/strutil.cpp`, en décommentant progressivement les tests de `src/main.cpp` jusqu'à ce que tous passent sans fuite mémoire.

## Compiler

Depuis ce dossier, avec un compilateur C++20 et CMake :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/TestString
```

Après chaque modification du code, relancez `cmake --build build-debug`. Dans VS Code avec l'extension CMake Tools, ouvrez ce dossier et acceptez la configuration proposée ; le binaire est alors dans `build/`.

## Progression

Décommentez un test dans `main.cpp` (et l'appel correspondant dans `runAllTests`), implémentez ce qu'il exige, vérifiez, passez au suivant.

1. `strutil.cpp` : `length`, `newcopy`, `compare`.
2. `String.cpp` : constructeur et destructeur (avec traces sur `std::cout`), `operator<<`, constructeur par copie, `operator=` par copie, `operator==`, `operator<`, `newcat` et `operator+`.
3. Déplacement : `String(String&&)` et `operator=(String&&)`. Observez les traces : quels appels deviennent des déplacements ?

## Valgrind

À chaque étape, sur le binaire Debug :

```sh
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build-debug/TestString
```
