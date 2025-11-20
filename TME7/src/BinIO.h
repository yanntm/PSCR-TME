// BinIO.h
#pragma once

#include "Graph.h"
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

struct NodeHeader {
  size_t id;
  size_t child_count;
};


/**
 * Sérialise un noeud et le sous graphe qu'il permet d'atteindre dans le fichier.
 * Arguments :
 * - fd : descripteur de fichier ouvert en écriture
 * - node : pointeur vers le Vertex à sérialiser
 * - ptr_to_offset : table de hash associant aux pointeurs de Vertex les offsets dans le fichier où ils ont été sérialisés
 * Retourne l'offset dans le fichier où le noeud a été sérialisé.
 */
off_t serialize(int fd, const Vertex *node,
                std::unordered_map<const Vertex *, off_t> &ptr_to_offset) {
  // 1. Tester le map; si présent rendre la valeur.

  // 2. Sinon, on doit créer un nouveau noeud, en fin du fichier.
  // Seek à l'offset de fin, noter cette position : c'est celle du nouveau noeud.
  // L'ajouter au map ptr->offset (immediatement, avant toute récursion).

  // 3. Au bon offset écrire le header (id, child_count)

  // 4. s'assurer de faire grandir le fichier suffisamment pour loger le nouveau noeud mais pas trop.
  // Plusieurs options ici : ftruncate, ou write de zéros (puis seek), ou lseek au delà de la fin et write...

  // 5. en boucle sur child_count (lseek/write ou pwrite recommandé),
  // itérer sur les enfants, appeler récursivement serialize pour chaque enfant. (Attention décale le curseur de fd!)
  // Récupérer l'offset retourné, et l'écrire à la bonne position dans le fichier (après le header, dans le tableau d'offsets).

  // 6. rendre l'offset du noeud nouvellement sérialisé.
  return 0;
}

/**
 * Désérialise un noeud à partir du fichier.
 * Arguments :
 * - fd : descripteur de fichier ouvert en lecture
 * - offset : position dans le fichier du noeud à désérialiser
 * - offset_to_vertex : table de hash associant aux offsets des pointeurs vers les Vertex déjà
 * désérialisés
 * - graph : le graphe dans lequel insérer les Vertex désérialisés
 * Retourne un pointeur vers le Vertex désérialisé.
 */
Vertex *deserialize(int fd, off_t offset, std::unordered_map<off_t, Vertex *> &offset_to_vertex,
                    Graph &graph) {

  // 1. Tester le map; si présent rendre la valeur.

  // 2. Sinon, on doit créer/mettre à jour le noeud dans Graph.
  // Seek à l'offset demandé; lire un header (id, child_count).

  // 3. demander à Graph le Vertex correspondant à id (findNode).
  // 4. Mettre à jour le map offset->Vertex (avant toute récursion).

  // 5. en boucle sur child_count,
  // itérer et lire les offsets des enfants (pread recommandé).
  // Pour chaque offset, appeler récursivement deserialize (Attention ça va faire bouger le curseur de fd!)
  // Ajouter les pointeurs vers enfant qui reviennent de la récursion au Vertex en construction.

  // 6. rendre le Vertex construit.                    

  Vertex *v = nullptr;

  return v;
}

void writeBin(const Graph &graph, const std::string &filename) {
  int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd == -1) {
    perror("open output");
    exit(1);
  }

  // Skip header: N (size_t)
  // but write a placeholder for now/leave some space
  // TODO

  // Serialize from node 0 after the header
  // preparation du hash : associe aux pointeurs de Vertex les offsets dans le fichier.
  std::unordered_map<const Vertex *, off_t> ptr_to_offset;
  // Serialize récursif par simplicité. On serialise tout ce qui est atteignable depuis 0.
  serialize(fd, graph.findNode(0), ptr_to_offset);

  // N is ptr_to_offset.size() : nombre de noeuds sérialisés
  size_t N = ptr_to_offset.size();

  // Write header at file start
  // TODO

  close(fd);
}

Graph parseBin(const std::string &filename) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    perror("open input");
    exit(1);
  }

  // Read header: N nombre de noeuds du graphe (offset 0)
  size_t N;

  // TODO

  // Create graph; on prealloue.
  Graph graph(N);

  // Offset de la racine, juste après le N
  off_t root_offset = -1; // TODO

  // préparation du hash : associe aux offset des pointeurs de Vertex.
  std::unordered_map<off_t, Vertex *> offset_to_vertex;
  // Deserialize, récursif par simplicité.
  deserialize(fd, root_offset, offset_to_vertex, graph);

  // ok Graph loaded !

  close(fd);

  return graph;
}