/* 
 * This file contains code that will generate the dataset for 
 * Floyd Warhsall parallel algorithm testing 
 *
 * Compiling:
 *   gcc -o main main.c
 *
 * Running:
 *  ./main [NUM_OF_VERTICES_TO_GENERATE]
 * 
 * Output: A file with test_data.dat will be generated everytime
 * you execute this program
 * 
 * File: floyd_warshal_omp.c                Author: Inteasar Haider
 * Date: January 01, 2017                   Version: V1.0
 * 
 */

#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define DATA_FILE_NAME_OUTPUT "test_data.dat"

int **distances;

void write_file (int num_nodes);
void read_file (void);
void print_distance_adjacency_matrix (int **distances, int num_nodes);

int main (int argc, char *argv[])
{
    
  if( argc == 2 ) {
    write_file (atoi(argv[1]));
  }
 
  //read_file ();

  return 0;
}


void read_file () {
  
  int num_nodes;
  FILE *data_file;
  int temp = 0, mem_size = 0;

  data_file = fopen (DATA_FILE_NAME_OUTPUT, "r");

  // The first line will be the number of vertices
  fscanf (data_file, "%d", &num_nodes);
  
  /* Calculating memory needed for adjacency matrix */
  mem_size = (num_nodes * sizeof(int*)) + 
          (num_nodes * num_nodes * sizeof(int));

  /* Dynamically allocating memory for the adjacency matrix */
  distances = malloc (mem_size);

  /* Set the row indexes as pointers to the columns */
  for (int i = 0; i < num_nodes; ++i)
  {
    distances[i]  = (int*)(distances + num_nodes + 1) + 
            (i * num_nodes);
  }
  
  printf("n: %d\n", num_nodes);

  for (int i = 0; i < num_nodes; i++)
  {
    for (int j = 0; j < num_nodes; j++)
    {
      if (fscanf (data_file, "%d", &temp) == EOF)
      {
	break;
      } 
      else
      { 
	  if (i == j) 
	  { 
	    /* distance between same node should be set zero */
	    distances[i][j] = 0;
	  } 
	  else 
	  {
	    /* Generating random numbers between 0 - 10 */
	    distances[i][j]= temp;
	  }
      }
//      printf("(%d,%d) = %d\n", i, j, temp);
    }
  }
  print_distance_adjacency_matrix (distances, num_nodes);
  
  fclose (data_file);
  free (distances);
}


void write_file (int num_nodes)
{
  FILE *data_file;

  data_file = fopen (DATA_FILE_NAME_OUTPUT, "w");
  
  fprintf (data_file, "%d\x0D\x0A", num_nodes);

  for (int i = 0; i < num_nodes; i++)
  {
    for (int j = 0; j < num_nodes; j++)
    {
      if (i == j) 
      { 
	/* distance between same node should be set zero */
	fprintf (data_file, "%d ",  0);
      } 
      else 
      {
	/* Generating random numbers between 0 - 10 */
	fprintf (data_file, "%d ",  (( ((int) rand()) % 15 ) + 1));
      }
    }
    fprintf (data_file, "\x0D\x0A");
  }

  fclose (data_file);
}


void print_distance_adjacency_matrix (int **distances, int num_nodes) 
{
      int i, j;
      printf("    ");
      
      for (i = 0; i < num_nodes; ++i) 
      {
          printf("%4c", 'A' + i);
      }
      
      printf("\n");
      
      for (i = 0; i < num_nodes; ++i) 
      {
          printf("%4c", 'A' + i);
          for (j = 0; j < num_nodes; ++j)
          {
              printf("%4d", distances[i][j]);
          }
          printf("\n");
      }
      
      printf("\n");
}
