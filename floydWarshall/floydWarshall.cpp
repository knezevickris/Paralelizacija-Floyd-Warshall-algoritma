#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 69

void popuniMatricu(int* matrica, int n);
void ispisMatrice(int* matrica, int n);
void sekvencijalniAlgoritam(int* matrica1, int n);

int main(int argc, char** argv) {
	int n, brojNiti;

	brojNiti = omp_get_max_threads();

	printf("Unesite broj cvorova grafa: ");
	scanf("%d", &n);

	
	int *matricaUdaljenosti;
	matricaUdaljenosti = (int*)malloc((n * n) * sizeof(int));

	popuniMatricu(matricaUdaljenosti, n);

	//ispisMatrice(matricaUdaljenosti, n);

	sekvencijalniAlgoritam(matricaUdaljenosti, n);

	free(matricaUdaljenosti);
}

void popuniMatricu(int* matrica, int n) {
	int i, j, vrijednost;
	srand(28);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
		{
			if (i == j)
				matrica[i * n+j] = 0;
			else
			{
				vrijednost = 3+ rand() % MAX;
				matrica[i*n+j] = vrijednost;
			}
		}
	}
}

void ispisMatrice(int* matrica, int n) {
	int i, j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("\t%d", matrica[i * n + j]);
		printf("\n");
	}

}

void sekvencijalniAlgoritam(int* matrica1, int n)
{
	int i, j, k;
	time_t pocetak, kraj;

	time(&pocetak);

	for(k=0; k<n; k++)
		for(i=0; i<n; i++)
			for (j = 0; j < n; j++) {
				if (i == j)
					continue;
				else if (matrica1[i * n + k] + matrica1[k * n + j] < matrica1[i * n + j]) {
					matrica1[i * n + j] = matrica1[i * n + k] + matrica1[k * n + j];
				}
				else;
			}


	printf("\n\nMatrica najkracih udaljenosti:\n\n");
	ispisMatrice(matrica1, n);
	time(&kraj);


	printf("\n\nVrijeme izvrsenja algoritma: %.4f sekundi\n", difftime(kraj, pocetak));

}