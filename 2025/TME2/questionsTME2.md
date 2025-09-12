# Questions - TME 2 : Conteneurs, Map, Lib Standard

Instructions : copiez vos réponses dans ce fichier (sous la question correspondante). A la fin de la séance, commitez vos réponses.

1) Exécutez le programme (mode `count`) sur le fichier fourni. Combien y a-t-il de mots dans le livre ?

Réponse (collez une trace ici) :

```
trace
```


2) Comment le temps d'exécution est-il impacté par les variations suivantes ?

- exécuter en mode Debug vs Release
- garder l'affichage d'un mot toutes les 100 lectures vs supprimer cette trace
- placer le fichier WarAndPeace dans `/tmp/` (si vous êtes à la ppti, par défaut vous travaillez sur NFS, /tmp est un disque local).

Réponse :

Runtime debug avec trace :

Runtime debug sans trace :

Runtime release avec trace :

Runtime release sans trace :

(mesures nfs vs tmp si à la ppti)

3) Implémentez un nouveau mode "unique", en vous aidant du squelette fourni. 
Modifiez le programme pour compter le nombre de mots différents que contient le texte. Pour cela on propose dans un premier temps de stocker tous les mots rencontrés dans un vecteur, et de traverser ce vecteur à chaque nouveau mot rencontré pour vérifier s'il est nouveau ou pas.
Exécutez le programme sur le fichier WarAndPeace.txt fourni. Combien y a-t-il de mots différents ? Combien de temps prend l'execution ?

Réponse (collez une trace ici) :

```
trace
```

4) Modifiez le programme pour introduire le mode "freq" qui calcule le nombre d'occurrences de chaque mot (fréquence). 
Pour cela, on adaptera le code précédent pour utiliser un vecteur qui stocke des `pair<string,int>` au lieu de stocker juste des string. 
Afficher le nombre d'occurrences des mots "war", "peace" et "toto".

Réponse :


5) Trier ce vecteur de paires par nombre d'occurrences décroissantes à l'aide de `std::sort` puis afficher les dix mots les plus fréquents. 

`std::sort` prend les itérateurs de début et fin de la zone à trier, et un prédicat binaire. Voir l'exemple suivant.

```CPP
#include <vector>
#include <string>
#include <algorithm>

class Etu {
	public :
	std::string nom;
	int note;
};

int main_sort () {
	std::vector<Etu> etus ;
	// plein de push_back de nouveaux étudiants dans le désordre

	// par ordre alphabétique de noms croissants
	std::sort(etus.begin(), etus.end(), [] (const Etu & a, const Etu & b) { return a.nom < b.nom ;});
	// par notes décroissantes
	std::sort(etus.begin(), etus.end(), [] (const Etu & a, const Etu & b) { return a.note > b.note ;});
	return 0;
}
```

Réponse :


6) Quelle est la complexité de ce code en temps et mémoire ? Donnez une trace avec temps d'exécution en mode release.


Réponse :



7) Implantez une table de hash simple en partant du squelette fourni HashMap.h:
 * Définir un struct Entry, muni de champs key et value et d'un constructeur. La clé peut être const. 
 * La table de hash stocke un `vector` de `forward_list` de `Entry` et le nombre d'entrées qu'elle contient. Le vecteur "buckets" est alloué à la construction à la taille demandée, en plaçant une liste vide dans chaque bucket.
 * Implantez le constructeur de la classe et l'opération size (remplissage actuel)
 * Implantez les deux méthodes principales, à la sémantique proche de Java.

 ```
 // Return pointer to value associated with key, or nullptr if not found.
 V* get(const K& key);

 // Insert or update (key,value).
 // Returns true if an existing entry was updated, false if a new entry was inserted.
 bool put(const K& key, const V& value);
```    

Réponse : Dans le fichier HashMap.h

8) En appui sur une table de hash \texttt{HashMap<string,int>} associant des entiers (le nombre d'occurrence) aux mots, et reprendre les questions où l'on calculait de nombre d'occurrences des mots avec cette nouvelle structure de donnée. Ce sera le nouveau mode "freqhash". Pensez à déclarer le nouveau fichier dans `CMakeLists.txt`. Combien de temps prend le calcul ? Testez quelques valeurs pour la taille initiale de la table, e.g. 100, 1024, 10000 vu que la table ne grossit jamais actuellement. Gardez un setting qui fonctionne bien.

Réponse : tracer les temps

9) On souhaite comme dans la version précédente afficher les 10 mots les plus fréquents.
Ajoutez dans la table de hash une fonction `std::vector<std::pair<K,V>> toKeyValuePairs() const` qui convertit les entrées de la table en un vecteur de paires. Pour celà on parcourt chaque liste de chaque bucket. Contrôlez les résultats par rapport à la version "freq" simple.

Réponse : une trace

10) Ecrire un nouveau mode "freqstd" qui s'appuie sur la classe du standard `std::unordered_map` pour faire la même chose que "freqhash". Pour la partie extraction des entrées vers un vecteur pour les trier, on peut simplement itérer la table (même s'il y a d'autres méthodes comme `std::copy`)

```
unordered_map<string,int> map;
for (const pair<string,int> & entry : map) {
    cout << "Key:" << entry.first << " Value " << entry.second << "\n";
}
```

Remesurer les performances avec cette version.

Réponse :

11) Conclure sur la qualité de notre structure de données maison.

12) BONUS: Si la taille actuelle est supérieure ou égale à 80\% du nombre de buckets, la table est considérée surchargée :
 la plupart des accès vont nécessiter d'itérer des listes. On souhaite dans ce cas doubler la taille d'allocation (nombre de buckets).  Ecrivez une fonction membre \texttt{void grow()} qui agrandit (double le nombre de buckets) d'une table contenant déjà des éléments.  Quelle est la complexité de cette réindexation ?

 Indices : créez une table vide mais allouée à la bonne taille, itérer nos entrées actuelles et les insérer une par une dans la cible, écraser notre table avec celle de l'autre (voire utiliser un `std::move`).

