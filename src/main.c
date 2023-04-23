#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for sleep

#include "../libraries/bstree.h"
#include "../libraries/hashtab.h"
#include "../libraries/header.h"

struct listnode *hashtab_KR[Hashtab_Size];
struct listnode *hashtab_Add[Hashtab_Size];
struct listnode *hashtab[Hashtab_Size];
char words[Max_Keys][Max_Key_Length];

int main() {

  struct listnode *node_KR, *node_Add;
  double time_KR = 0.0, time_Add = 0.0;
  ;
  double begin, end;

  //---------Read-Words---------//
  FILE *file = fopen("keys.txt", "r");
  for (int i = 0; i < Max_Keys; i++) {
    fgets(words[i], Max_Key_Length, file);
    words[i][strlen(words[i]) - 2] = '\0';
  }
  fclose(file);
  //---------Read-Words---------//

  printf("\tExperiment #6 --> Hashtab_Lookup_Add vs Hashtab_Lookup_KP "
                 "<--\n");

  print_cap(73, '-');
  printf("\t  time_Add\t time_KR\t Add_Collis\t  KP_Collis\t\n");
  print_cap(73, '-');

  hashtab_init(hashtab_KR);
  hashtab_init(hashtab_Add);

  for (int i = 0; i < Max_Keys; i++) {
    hashtab_add(hashtab_KR, words[i], i);
    hashtab_add_Add(hashtab_Add, words[i], i);

    if ((i + 1) % 10000 == 0) {
      char *word = words[getrand(0, i)];
      //---------KR---------//
      begin = wtime();
      node_KR = hashtab_lookup(hashtab_KR, word);
      end = wtime();
      time_KR = end - begin;
      //---------JR---------//

      //---------DJB---------//
      begin = wtime();
      node_Add = hashtab_lookup_Add(hashtab_Add, word);
      end = wtime();
      time_Add = end - begin;
      //---------DJB---------//
      int collisions_KP = get_collisions_v2(hashtab_KR, i + 1);
      int collisions_Add = get_collisions_v2(hashtab_Add, i + 1);

      printf("%d\t %.12f\t %.12f\t %d\t\t %d\t\t\n",
             i + 1, time_Add, time_KR, collisions_Add, collisions_KP);
    }
  }
  print_cap(73, '-');
  free_table(hashtab_KR);
  free_table(hashtab_Add);
  struct bstree *tree, *node_tree;
  struct listnode *hash, *node_hash;
  double time_tree = 0.0, time_hash = 0.0;

  printf("\t\t\tExperiment #1 --> Bstree_Lookup vs Hashtab_Lookup <--"
                 "<--\n");

  print_cap(121, '-');
  printf("\t  time_hash\t time_tree\t key_hash\t key_tree\t\n");
  print_cap(121, '-');

  hashtab_init(hashtab);
  hashtab_add(hashtab, words[0], 0);
  tree = bstree_create(words[0], 0);

  for (int i = 1; i < Max_Keys; i++) {
    bstree_add(tree, words[i], i);
    hashtab_add(hashtab, words[i], i);

    if ((i + 1) % 10000 == 0) {
      char *rand_key = words[getrand(0, i)];
      //---------Good---------//
      begin = wtime();
      node_tree = bstree_lookup(tree, rand_key);
      end = wtime();
      time_tree = end - begin;
      //---------Good---------//

      //---------Bad---------//
      begin = wtime();
      node_hash = hashtab_lookup(hashtab, rand_key);
      end = wtime();
      time_hash = end - begin;
      //---------Bad---------//

      printf("%d\t %.12f\t %.12f\t %35.35s\t %35.35s\t\n",
             i + 1, time_hash, time_tree, node_hash->key, node_tree->key);
    }
  }
  print_cap(121, '-');
  bstree_free(tree);
  free_table(hashtab);
  printf("\t\t\tExperiment #2 --> Bstree_add vs Hashtab_add <--"
                 "<--\n");

  print_cap(121, '-');
  printf("\t time_hash\t time_tree\t\n");
  print_cap(121, '-');

  hashtab_init(hashtab);
  hashtab_add(hashtab, words[0], 0);
  tree = bstree_create(words[0], 0);

  for (int i = 1; i < Max_Keys; i++) {
    if ((i + 1) % 10000 == 0) {
      //---------Good---------//
      begin = wtime();
      bstree_add(tree, words[i], i);
      end = wtime();
      time_tree = end - begin;
      //---------Good---------//

      //---------Bad---------//
      begin = wtime();
      hashtab_add(hashtab, words[i], i);
      end = wtime();
      time_hash = end - begin;
      //---------Bad---------//

      printf("%d\t %.12f\t %.12f\t\n",
             i + 1, time_hash, time_tree);
    }
    bstree_add(tree, words[i], i);
    hashtab_add(hashtab, words[i], i);
  }
  print_cap(121, '-');
  bstree_free(tree);
  free_table(hashtab);
  return 0;
}
