#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#undef main

typedef struct egyenlegelem //				Láncolt lista deklarálása
{
	int ev;
	int bev;
	int kiad;
	int egyenleg;
	struct egyenlegelem* next;
}egyenlegelem;

typedef struct kamatelem
{
	int ev;
	double kamat_plusz;
	double kamat_minusz;
	struct kamatelem* next;
}kamatelem;

egyenlegelem* beszur_e(egyenlegelem* uj, egyenlegelem* kezdet)
{
	egyenlegelem* kereso = kezdet;
	if (kezdet == NULL) //ha még nincs a listának egy eleme sem akkor teljesül
	{
		return uj;
	}
	if (kereso->ev > uj->ev) //legelsõ elem elé való beszúrás
	{
		kezdet = uj;
		uj->next = kereso;
		return uj;
	}
	while ((kereso->next) != NULL && (kereso->next->ev) < (uj->ev)) //a lépteteés feltétele
	{
		kereso = kereso->next; // léptetés
	}
	uj->next = kereso->next; //befûzés a listába
	kereso->next = uj;		//befûzés a listába
	return kezdet;
}

kamatelem* beszur_k(kamatelem* uj, kamatelem* kezdet)
{
	kamatelem* kereso = kezdet;
	if (kezdet == NULL) //ha még nincs a listának egy eleme sem akkor teljesül
	{
		kezdet = uj;
		return kezdet;
	}
	if (kereso->ev > uj->ev) //legelsõ elem elé való beszúrás
	{
		kezdet = uj;
		uj->next = kereso;
	}
	while ((kereso->next != NULL) &&(kereso->next->ev) < (uj->ev)) //a lépteteés feltétele
	{
		kereso = kereso->next; // léptetés
	}
	uj->next = kereso->next; //befûzés a listába
	kereso->next = uj;		//befûzés a listába
	return kezdet;
}

egyenlegelem* beolvbevkiad(void)
{
	int status;
	egyenlegelem* kezdet = NULL;
	FILE *fp;

	egyenlegelem* uj = (egyenlegelem*)malloc(sizeof(egyenlegelem));
	if (uj == NULL) return NULL;
	uj->next = NULL; // a beszur_e függvény miatt kell

	fp = fopen("bevkiad.txt", "r");
	if (fp == NULL) return NULL;

	while (fscanf(fp, "%d", &(uj->ev)) > 0)
	{
		fscanf(fp, "%d", &(uj->bev));
		fscanf(fp, "%d", &(uj->kiad));
		uj->egyenleg = ((uj->bev) - (uj->kiad));
		kezdet = beszur_e(uj, kezdet);
		uj = (egyenlegelem*)malloc(sizeof(egyenlegelem)); //következõ beolvasásnak elõkészítés
		if (uj == NULL) return NULL;
		uj->next = NULL; // a beszur_e függvény miatt kell
	}
	free(uj); //a következõnek volt elõkészítve de nincs következõ
	status = fclose(fp);
	if (status != 0) return NULL;
	return kezdet;
}

kamatelem* beolvkamat(void)
{
	double kamat_plusz, kamat_minusz;
	int status;
	kamatelem* kezdet = NULL;
	kamatelem* uj = (kamatelem*)malloc(sizeof(kamatelem));
	FILE *fp;

	if (uj == NULL) return NULL;
	uj->next = NULL; // a beszur_k függvény miatt kell

	fp = fopen("kamat.txt", "r");
	if (fp == NULL) return NULL;

	while (fscanf(fp, "%d", &(uj->ev)) > 0)
	{
		fscanf(fp, "%lf", &kamat_plusz);
		fscanf(fp, "%lf", &kamat_minusz);
		uj->kamat_plusz = (kamat_plusz/100) + 1; //százalékból szorzóvá alakítás
		uj->kamat_minusz = (kamat_minusz/100) + 1; //százalékból szorzóvá alakítás
		kezdet = beszur_k(uj, kezdet);
		uj = (kamatelem*)malloc(sizeof(kamatelem)); //elõkészítés a következõ beolvasásnak
		if (uj == NULL) return NULL;
		uj->next = NULL; // a beszur_e függvény miatt kell
	}
	free(uj); //a következõnek volt elõkészítve de nincs következõ
	status = fclose(fp);
	if (status != 0) return NULL;
	return kezdet;
}

int kiiregyenleg(egyenlegelem* egyenleg_p, egyenlegelem* eltero_p)
{
	int status;
	FILE *fp = NULL;
	if (egyenleg_p == NULL) return 1;
	fp = fopen("kimenet.txt", "w");
	if (fp == NULL) return 1; // ha nem sikerült megnyitni return 1
	while (egyenleg_p != NULL)
	{
		fprintf(fp, "%d\t", egyenleg_p->ev);
		fprintf(fp, "%d\n", egyenleg_p->egyenleg);
		egyenleg_p = egyenleg_p->next;
	}
	fprintf(fp, "Legnagyobb eltérés éve: %d, egyenlege: %d", eltero_p->ev, eltero_p->egyenleg);
	status = fclose(fp);
	if (status != 0) return 1;
	return 0;
}

void kiircmd_e(egyenlegelem* egyenleg_p) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	if (egyenleg_p == NULL) return;
	printf("[ev]\t[bev]\t[kiad]\t[valtozas]\n\n");
	while (egyenleg_p != NULL)
	{
		printf("%d\t", egyenleg_p->ev);
		printf("%d\t", egyenleg_p->bev);
		printf("%d\t", egyenleg_p->kiad);
		printf("%d\n", egyenleg_p->egyenleg);
		egyenleg_p = egyenleg_p->next;
	}
	printf("\n");
	return;
}

void kiircmd_k(kamatelem* kamat_p) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	if (kamat_p == NULL) return;
	printf("[ev]\t[kamat+]\t[kamat-]\n");
	while (kamat_p != NULL)
	{
		printf("%d\t", kamat_p->ev);
		printf("%5f\t", ((kamat_p->kamat_plusz)-1)*100);
		printf("%5f\n", ((kamat_p->kamat_minusz)-1)*100);
		kamat_p = kamat_p->next;
	}
	printf("\n");
	return;
}

egyenlegelem* hozzaadcmd_e(egyenlegelem* egyenleg_p)
{
	egyenlegelem* uj = NULL;
	int check;
	uj = (egyenlegelem*)malloc(sizeof(egyenlegelem));
	printf("Szintaxis: [ev] [tab] [bevetel] [tab] [kiadas]: ");
	check = scanf_s("%d\t%d\t%d", &(uj->ev), &(uj->bev), &(uj->kiad));
	if (check != 3) 
	{
		free(uj);
		return NULL;
	}
	uj->egyenleg = ((uj->bev)-(uj->kiad));
	return beszur_e(uj, egyenleg_p);
}

kamatelem* hozzaadcmd_k(kamatelem* kamat_p)
{
	kamatelem* uj = NULL;
	int check;
	uj = (kamatelem*)malloc(sizeof(kamatelem));
	printf("Szintaxis: [ev] [megtak.] [hitel]\n");
	check = scanf_s("%d\t%lf\t%lf", &(uj->ev), &(uj->kamat_plusz), &(uj->kamat_minusz));
	if (check != 3)
	{
		free(uj);
		return NULL;
	}
	return beszur_k(uj, kamat_p);
}

double abs_(double n)
{
	if (n < 0) return (n * -1);
	else return n;
}

double egyenlegszamitas(egyenlegelem* egyenleg_p, kamatelem* kamat_p) //visszatérési értéke az átlag
{
	int hibahely = 0; //hanyadik elemnél van hiba az összehasonlításban
	int elozoev = 0;
	int sum = 0, db = 0;
	//if (egyenleg_p == NULL || kamat_p == NULL) return -1; //hibakezelés
	while (egyenleg_p != NULL && kamat_p != NULL)
	{
		if (egyenleg_p->ev != kamat_p->ev) return hibahely;
		if (elozoev + (egyenleg_p->egyenleg) < 0)
			egyenleg_p->egyenleg = (elozoev + (egyenleg_p->egyenleg)) * kamat_p->kamat_minusz; //kerekítés egészre

		else
			egyenleg_p->egyenleg = (elozoev + (egyenleg_p->egyenleg)) * kamat_p->kamat_plusz; //kerekítés egészre

		elozoev = egyenleg_p->egyenleg;
		hibahely++;
		sum += egyenleg_p->egyenleg;
		db++;
		egyenleg_p = egyenleg_p->next; //léptetés
		kamat_p = kamat_p->next; //léptetés
	}
	return sum / db; //átlag
}

int free_egyenleg(egyenlegelem* egyenleg_p)
{
	egyenlegelem* lemarado = egyenleg_p;
	while (lemarado != NULL)
	{
		egyenleg_p = egyenleg_p -> next;
		free(lemarado);
		lemarado = egyenleg_p;
	}
	return 0;
}

int free_kamat(kamatelem* kamat_p)
{
	kamatelem* lemarado = kamat_p;
	while (lemarado != NULL)
	{
		kamat_p = kamat_p->next;
		free(lemarado);
		lemarado = kamat_p;
	}
	return 0;
}

egyenlegelem* elter(egyenlegelem* egyenleg_p, kamatelem* kamat_p)
{
	double atlag = egyenlegszamitas(egyenleg_p, kamat_p);
	double max = egyenleg_p->egyenleg;
	egyenlegelem* p = egyenleg_p;
	while (egyenleg_p != NULL)
	{
		if (abs_(egyenleg_p->egyenleg - atlag) > max)
		{
			max = abs_(egyenleg_p->egyenleg - atlag);
			p = egyenleg_p;
		}
	egyenleg_p = egyenleg_p->next;
	}
	return p;
}

void welcome(void)
{
	printf("######################################################################\n");
	printf("## Banki Egyenleg Kalkulator 2017  ###################################\n");
	printf("## Keszitette: Szabo Daniel        ###################################\n");
	printf("######################################################################\n\n");
}

int main(void)
{
	egyenlegelem* egyenleg_p = beolvbevkiad();
	kamatelem* kamat_p = beolvkamat();
	egyenlegelem* cmdad_e = NULL;
	kamatelem* cmdad_k = NULL;
	egyenlegelem* eltero_p = NULL;
	welcome();
	if (egyenleg_p != NULL) printf("bevkiad.txt sikeresen beolvasva\n");
	if (kamat_p != NULL) printf("kamat.txt sikeresen beolvasva\n\n");
	int be;
	while (1)
	{
		printf("Mit szeretne tenni?\n");
		printf("[1] Beolvasott adatok megtekintese\n");
		printf("[2] Uj sor hozzadasa bevkiad.txt-hez\n");
		printf("[3] Uj sor hozzaadasa kamat.txt-hez\n");
		printf("[4] Egyenleg kiszamitasa es kiirasa kimenet.txt-be\n");
		printf("[9] Kilepes\n\n");
		if (scanf_s("%d", &be) < 1)
		{
			printf("Hibas bevitel!\nProbalja ujra!\n");
			system("pause");
			system("cls");
			welcome();
			continue;
		}
		switch (be)
		{
		case 1:
			kiircmd_e(egyenleg_p);
			kiircmd_k(kamat_p);
			system("pause");
			system("cls");
			welcome();
			continue;
		case 2:
			cmdad_e = hozzaadcmd_e(egyenleg_p);
			if (cmdad_e != NULL) 
			{
				egyenleg_p = cmdad_e;
				printf("Sikeres hozzadas\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
			else
			{
				printf("Sikertelen beolvasas\nProbalja ujra!\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
		case 3:
			cmdad_k = hozzaadcmd_k(kamat_p);
			if (cmdad_k != NULL)
			{
				kamat_p = cmdad_k;
				printf("Sikeres hozzadas\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
			else
			{
				printf("Sikertelen beolvasas\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
		case 4:
			eltero_p = elter(egyenleg_p, kamat_p);
			if (kiiregyenleg(egyenleg_p, eltero_p) != 0)
			{
				printf("Hiba a kiirasnal!\nProbalja ujra\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
			else
			{
				printf("Sikeres kiiras\n");
				system("pause");
				system("cls");
				welcome();
				continue;
			}
		case 9:
			printf("A program kilep!\n");
			system("pause");
			system("cls");
			free_egyenleg(egyenleg_p); //eltero_p is itt szabadul fel
			free_kamat(kamat_p);
			break;
		}
		break;

	}
	return 0;
}
