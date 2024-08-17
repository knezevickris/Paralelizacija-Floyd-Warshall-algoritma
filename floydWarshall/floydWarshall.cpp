#include<omp.h>
#include<stdio.h>

#define N 10

int main() {
	int dimenzijaMatrice;
	/*
	* Neophodno je da korisnik unese broj čvorova grafa. Broj čvorova određuje dimenziju kvadratne matrice susjedsta čvorova.
	*/
	printf("Unesite broj čvorova grafa: ");
	scanf("%d", &dimenzijaMatrice);
	printf("dimenzija matrice je %d", dimenzijaMatrice);
	int matricaUdaljenosti[N][N] = {};
}