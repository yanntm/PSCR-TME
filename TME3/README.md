# TME 3 — countword en concurrence

Le sujet PDF contient les questions. Travaillez dans `src/main.cpp`, puis dans les
conteneurs pour les approfondissements. Consignez vos réponses et traces dans
`answers.md` ; conservez chaque mode pour comparer vos expériences.

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

Depuis ce dossier, avec un compilateur C++20 et CMake :

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

Debug facilite le débogage ; utilisez Release pour mesurer les performances.

## Prise en main

Depuis le dossier du TME (le sous-shell garde votre terminal dans ce dossier) :

```sh
(
  cd build-release || exit 1
  ./countword ../WarAndPeace.txt freqstd &&
  ./countword ../WarAndPeace.txt partition 4 &&
  ../check.sh freqstd.freq partition.freq
)
```

Syntaxe : `./countword [fichier] [mode] [nombre_de_partitions]`.
Les valeurs par défaut sont `../WarAndPeace.txt`, `freqstd` et `4`.
Le dernier argument doit être strictement positif ; il servira de nombre de
threads dans les modes que vous ajouterez.

Modes fournis (tous séquentiels) :

- `freqstd` : lecture du fichier avec une `std::unordered_map`.
- `freqstdf` : même table, lecture via `processRange` sur le fichier entier.
- `partition` : même table, appels successifs à `processRange` sur les plages.
- `freq` : table personnalisée `HashMap`, pour la partie 3.

Chaque mode écrit `mode.freq` dans le dossier courant : total des mots, nombre de
mots différents, puis fréquences triées (fréquence décroissante, mot croissant en
cas d'égalité). Le temps affiché comprend la lecture, le comptage, le tri et
l'écriture. `check.sh` compare les fichiers qui lui sont passés ; comparez chaque
nouveau mode à `freqstd.freq`.

## Mesurer et vérifier

Une fois un mode correct implanté, choisissez-le dans `mode` ci-dessous.
La boucle mesure 1, 4, 8 et 16 threads, trois fois chacun, et compare chaque
résultat à la référence. Elle s'arrête au premier échec (programme ou comparaison).
Ne comparez pas un fichier laissé par une exécution précédente après un crash.

Depuis le dossier du TME :

```sh
(
  cd build-release || exit 1
  mode=mt_mutex  # À remplacer par le mode que vous avez implanté.
  ./countword ../WarAndPeace.txt freqstd || exit 1
  for n in 1 4 8 16; do
    for essai in 1 2 3; do
      printf '\nMode=%s threads=%s essai=%s\n' "$mode" "$n" "$essai"
      ./countword ../WarAndPeace.txt "$mode" "$n" &&
        ../check.sh freqstd.freq "$mode.freq" || exit 1
    done
  done
)
```

Pour les approfondissements, prolongez la liste par `32 64` et comparez aussi
la version lock-free à compteurs par thread (`mt_lfna`) à la version shardée.

Pour essayer la boucle avec le code fourni, choisissez `mode=partition` :
`n` règle alors le nombre de plages, toujours traitées séquentiellement.
Gardez les traces de comparaison aussi pour les modes volontairement incorrects
qui se terminent : ne pas planter ne suffit pas à obtenir les bons résultats.

## Parcours

Les parties 0 et 1 du sujet conduisent à une première solution multi-thread avec
les conteneurs standard. La partie 2 explore les tables locales et la fusion.
Les parties 3 et 4 approfondissent les conteneurs concurrents, puis le lock-free.
Ces approfondissements sont à poursuivre selon votre avancement, pendant ou
après la séance. `HashMap.h` et `ListLF.h` sont fournis pour ces parties.

Conservez vos modifications et vos réponses dans votre dépôt, puis faites un
push en fin de séance.
