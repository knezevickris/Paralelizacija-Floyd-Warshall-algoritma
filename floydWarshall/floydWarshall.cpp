#define _CRT_SECURE_NO_WARNINGS

#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


void popuniMatricu(int* matrica, int n);
void ispisMatrice(int* matrica, int n);
void sekvencijalniAlgoritam(int* matrica1, int n, int odg);
void paralelniAlgoritam(int* matrica, int n, int odg);


int main() {
	int n, odabir, odgovor; //promjenljive za cuvanje broja cvorova grafa i korisnickih odabira

	while (1)
	{
		//korisnik unosi broj cvorova grafa
		do {
			printf("Unesite broj cvorova grafa: ");
			scanf("%d", &n);

			if (n < 2)
				printf("\nMinimalan broj cvorova povezanog grafa je 2. Ponovite unos.\n");
		} while (n < 2);

		//broj cvorova grafa odredjuje dimenziju kvadratne matrice udaljenosti grafa
		//vrsi se dinamicka alokacija prostora za matricu
		int* matricaUdaljenosti;
		matricaUdaljenosti = (int*)malloc((n * n) * sizeof(int));

		//poziv funkcije za popunjavanje matrice udaljenosti brojnim vrijednostima
		popuniMatricu(matricaUdaljenosti, n);
		//ispisMatrice(matricaUdaljenosti, n);


		printf("\nDa li zelite ispis matrice najkracih udaljenosti (0 - Ne, 1 - Da)?\n>");
		scanf("%d", &odgovor);


		printf("\nIzaberite nacin izvrsavanja:\n - 1 - sekvencijalno izvrsavanje\n - 2 - paralelno izvrsavanje\n>");
		scanf("%d", &odabir);

		switch (odabir)
		{
		case 1:
			//poziv funkcije za sekvencijalno izvrsavanje Floyd - Warshall algoritma
			sekvencijalniAlgoritam(matricaUdaljenosti, n, odgovor);
			break;
		case 2:
			//poziv funkcije za paralelno izvrsavanje Floyd - Warshall algoritma
			paralelniAlgoritam(matricaUdaljenosti, n, odgovor);
			break;
		default:
			break;
		}


		//oslobadjanje zauzete memorije
		free(matricaUdaljenosti);
	}
}

//funkciji popuniMatricu se proslijedjuje pokazivac na matricu udaljenosti grafa i dimenzija matrice
void popuniMatricu(int* matrica, int n) {
	int i, j, vrijednost;
	
	//seed-ovanje pseudoslucajnog niza brojeva
	srand(28);

	for (i = 0; i < n; i++) {
		for (j = 0; j <= i; j++)
		{
			//glavna dijagonala se popunjava nulama jer je udaljenost izmedju nekog cvora i samog sebe 0
			if (i == j)
				matrica[i * n+j] = 0;
			//ostatak matrice se popunjava random pseudoslucajnim pozitivnim vrijednostima
			//pretpostavili smo da je graf potpuno povezan tj. da postoji grana izmedju bilo koja dva cvora grafa i zato nema beskonacnih vrijednosti
			else
			{
				vrijednost = 1 + rand() % n;
				//gornja i donja trougaona matrica su simetricne
				matrica[i * n + j] = vrijednost;
				matrica[j * n + i] = vrijednost;
			}
		}
	}
}

//funkcija ispisMatrice vrsi ispisivanje sadrzaja matrice na konzolu
void ispisMatrice(int* matrica, int n) {
	int i, j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("\t%d", matrica[i * n + j]);
		printf("\n");
	}

}

//funckija sekvencijalniAlgoritam vrsi pronalazak najkracih putanja u matrici udaljenosti ciji se pokazivac proslijedjuje kao parametar, a n je dimenzija matrice
//od vrijednosti parametra odg zavisi da li ce se matrica ispisivati na konzoli
void sekvencijalniAlgoritam(int* matrica, int n, int odg)
{
	int i, j, k; //promjenljive za iteraciju kroz matricu
	double pocetak, kraj; //promjenljive za mjerenje pocetnog i krajnjeg vremena
		
	//biljezenje pocetnog vremena
	pocetak = omp_get_wtime();

	//sekvencijalna implementacija Floyd-Warshall algoritma
	for(k=0; k<n; k++)
		for(i=0; i<n; i++)
			for (j = 0; j < n; j++) {
				if (i == j) //preskace elemente na glavnoj dijagonali (udaljenost od cvora do samog sebe je 0)
					continue;
				//azurira vrijednost matrice ako se pronadje kraci put između cvorova i i j preko cvora k
				else if (matrica[i * n + k] + matrica[k * n + j] < matrica[i * n + j]) {
					matrica[i * n + j] = matrica[i * n + k] + matrica[k * n + j];
				}
				else; //ako nema kraceg puta, nista se ne mijenja.
			}

	//ako je korisnik odabrao da ispise matricu, poziva funkciju koja prikazuje matricu najkracih udaljenosti
	if (odg == 1)
	{
		printf("\n\nMatrica najkracih udaljenosti:\n\n");
		ispisMatrice(matrica, n);
	}

	//biljezenje zavrsnog vremena
	kraj = omp_get_wtime();

	//racunanje vremena izvrsenja algoritma i ispis rezultata u milisekundama
	printf("\n\nVrijeme izvrsenja algoritma: %.5f sekundi.\n\n", kraj - pocetak);

}

void paralelniAlgoritam(int* matrica, int n, int odg) {
	int i, j, k; 
	int brojNiti = omp_get_max_threads(); //inicijalizacija promjenljive u kojoj se cuva ukupan broj niti procesora
	double pocetnoVrijeme, krajnjeVrijeme; 

	printf("\nImate %d niti na raspolaganju.\n", brojNiti);
	
	//dinamicka alokacija niza u kom ce se cuvati vrijeme aktivnosti pojedinacnih niti
	double* vrijemeNiti = (double*)malloc(brojNiti * sizeof(double));
	//inicijalizacija niza
	for (i = 0; i < brojNiti; i++) {
		vrijemeNiti[i] = 0.0;
	}
	
	omp_set_num_threads(brojNiti); //postavljanje broja niti koje ce se koristiti za sljedeci paralelni region

	pocetnoVrijeme = omp_get_wtime(); //pocetak mjerenja vremena rada algoritma

#pragma omp parallel private(i,j,k) //promjenljive su privatne za svaku nit tj. svaka nit ima svoju kopiju ovih promenljivih
{
	int idNiti = omp_get_thread_num();
	double pocetakRadaNiti, zavrstakRadaNiti;

	for (k = 0; k < n; k++) 
	{
#pragma omp for schedule(static) //zadatak se dijeli izmedju niti koristeci staticko rasporedjivanje
		for (i = 0; i < n; i++) 
		{
			pocetakRadaNiti = omp_get_wtime(); //biljezi se vrijeme kad nit pocinje sa radom sa i-tim redom
			for (j = 0; j < n; j++) 
			{
				if (i == j)
					continue;
				else if (matrica[i * n + k] + matrica[k * n + j] < matrica[i * n + j]) 
				{
					matrica[i * n + j] = matrica[i * n + k] + matrica[k * n + j]; //ako je pronadjen kraci put izmedju cvorova i i j preko cvora k, azurira vrijednost u matrici
				}
				else;
			}
			zavrstakRadaNiti = omp_get_wtime(); //biljezi se vrijeme zavrsetka rada niti sa i-tim redom
			vrijemeNiti[idNiti] += zavrstakRadaNiti - pocetakRadaNiti; //izracunato vrijeme koje je nit aktivno provela dodaje se ukupnom vremenu rada te niti.
		}
	}
}
	//ako je korisnik odabrao da ispise matricu, poziva funkciju koja prikazuje matricu najkracih udaljenosti
	if (odg == 1)
	{
		printf("\n\nMatrica najkracih udaljenosti:\n\n");
		ispisMatrice(matrica, n);
	}

	krajnjeVrijeme = omp_get_wtime(); //biljezenje zavrsetka izvrsavanja algoritma
	
	printf("\nVrijeme paralelnog izvrsenja algoritma: %.5f sekundi.\n", krajnjeVrijeme-pocetnoVrijeme);

	for (i = 0; i < brojNiti; i++) {
		printf("Nit %d je bila aktivna %.5f sekundi -> (%.2f%%) ukupnog vremena.\n", i+1, vrijemeNiti[i], (vrijemeNiti[i] / (krajnjeVrijeme-pocetnoVrijeme)) * 100.0);
	}

	free(vrijemeNiti);
}