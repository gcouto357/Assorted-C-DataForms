// Gabriel Couto
// COP 3502 Spring 2020
// ga350657

// including the libraries needed
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "KindredSpirits.h"

// copy of the create a node function from testcases
node *createNode(int data)
{
  node *n = malloc(sizeof(node));

  n->data = data;
  n->left = n->right = NULL;

  return n;
}

// counts nodes using recursion
int NodeCounter(node *tree)
{
  if (tree == NULL)
    return 0;

  return (NodeCounter(tree->left) + 1 + NodeCounter(tree->right));
}

// check for reflection
int isReflection(node *a, node *b)
{
  // check if both are NULL
  if (a == NULL && b == NULL)
    return 1;

  // check if either are NULL
  else if (a == NULL || b == NULL)
    return 0;

  // check if data dont match
  else if (a->data != b->data)
    return 0;

  // call the left and right children but on opposite sides of the tree
  else
    return isReflection(a->right, b->left) && isReflection(a->left, b->right);
}

// recursive function using preorder to create a refelction of the tree passed
node *traversalCreate(node *root)
{

  node *tmp;

  // base case
  if (root == NULL)
    return root;

  // creates the node in the tree
  tmp = createNode(root->data);

  // calls for the right and left child making the node in the opposite side
  tmp->left = traversalCreate(root->right);
  tmp->right = traversalCreate(root->left);

  return tmp;
}

// the gateway function for the traversal create
node *makeReflection(node *root)
{
  node *temproot;

  temproot = traversalCreate(root);

  return temproot;
}

// PreOrder traversal using array pointers to store data
void PreOrder(node *root, int* count, int* array)
{
  if (root == NULL)
    return;

  array[*count] = root->data;
  ++*count;
  PreOrder(root->left, count, array);
  PreOrder(root->right, count, array);
}

// PostOrder traversal using array pointers to store data
void PostOrder(node *root, int* count, int* array)
{
  if (root == NULL)
    return;

  PostOrder(root->left, count, array);
  PostOrder(root->right, count, array);
  array[*count] = root->data;
  ++*count;
}

// check for kindredSpirits
int kindredSpirits(node *a, node *b)
{
  // variable declaration
  int bad = 1, i = 0, *array1 = NULL, *array2 = NULL, count1 = 0, len1, len2, *array3 = NULL, *array4 = NULL;

  // checks for both null or if one is null
  if (a == NULL && b == NULL)
    return 1;

  if (a == NULL || b == NULL)
    return 0;

  // finding if the trees have the same amount of nodes
  len1 = NodeCounter(a);
  len2 = NodeCounter(b);

  if (len1 != len2)
    return 0;

  // allocate 2 arrays with the size of the trees(nodes)
  array1 = malloc(sizeof(int) * len1);
  array2 = malloc(sizeof(int) * len1);

  // recusively call and retrieve the data from the 2 trees
  PreOrder(a, &count1, array1);
  count1 = 0;
  PostOrder(b, &count1, array2);

  // compare the data to check
  while (i != len1)
  {
    if (array1[i] != array2[i])
      bad = 0;

    i++;
  }

  // if the data didnt match try the other way around for the trees
  if (bad == 0)
  {
    // grab the data post and pre order but for the opposiste trees
    count1 = 0;
    PreOrder(b, &count1, array1);
    count1 = 0;
    PostOrder(a, &count1, array2);
    i = 0;

    // compare data again
    while (i != len1)
    {
      // if the data doesnt match again free thed arrays and return 0
      if (array1[i] != array2[i])
      {
        free(array1);
        free(array2);
        return 0;
      }

      i++;
    }
  }

  // if the data passes the first case return 1 and free the arrays
  free(array1);
  free(array2);
  return 1;
}

double difficultyRating(void)
{
  return 3;
}

double hoursSpent(void)
{
  return 13;
}
