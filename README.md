Paralelizacija Floyd-Warshall algoritma je tema projekta urađenog u sklopu predmeta Paralelni računarski sistemi. 
Ovaj projekat implementira Floyd-Warshall algoritam za pronalaženje najkraćih putanja između svih parova čvorova u grafu. Algoritam je paralelizovan koristeći tehniku višenitnosti uz OpenMP biblioteku.

U pitanju je konzolna aplikacija. Po pokretanju, od korisnika se zahtjeva unos broja čvorova grafa, neophodno je unijeti pozitivan cijeli broj veći od 2, jer je to minimalan broj čvorova povezanog grafa. U slučaju pogrešnog unosa, omogućen je ponovan unos.
Potom korisnik ima mogućnost izbora da li želi ispis konačne matrice najkraćih udaljenosti na konzolu i mogućnost izbora sekvencijalnog ili paralelnog načina izvršenja.

Floyd-Warshall algoritam u svom serijskom obliku  ima vremensku složenost n^3 što ga čini računski intenzivnim i vremenski zahtjevnim za grafove sa velikim brojem čvorova. 
Paralelizacija ovog algoritma predstavlja pokušaj ka povećanju efikasnosti i smanjenju vremena izvršavanja. 
Cilj ovog projekta je bio pokazati kako se performanse algoritma mogu značajno poboljšati korištenjem paralelizacije. To su dokazali rezultati testiranja na dva različita uređaja. 
Glavni zaključak istraživanja je da je paralelizacija Floyd-Warshall algoritma korisna za  guste grafove sa velikim brojem čvorova. Paralelizacija može biti nepoželjna ili čak kontraproduktivna kada graf ima mali broj čvorova, jer se koristi relativno malo računskih resursa. 
