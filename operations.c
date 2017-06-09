#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

//	Functie de adaugarea a unui proces sau a mai multor procese
//	in coada ready:
void add(List **ready_end, char *buffer) {
	//	Mut i pe prima pozitie a numelui:
	int i = 2; // 2 pentru single add
	if( buffer[0] == 'm' ) i = 3; // 3 pentru multiple add

	//	Cat timp nu am ajuns la sfarsitul bufferului...
	while( i != strlen(buffer) ){
		
		//	Aloc memorie pentru un nou proces (tip List)
		List *p = (List *)malloc(sizeof(List));
		
		//	Copiez numele caracter cu caracter:
		int k = 0;
		while( buffer[i] != ' ' ) {
			p -> name[k++] = buffer[i++];
		}
		
		i++;	//	Incrementez pentru a trece de spatiu

		//	Copiez timpul necesar procesarii (initial)
		p -> t = 0;
		while( buffer[i] != ' ' ) {
			p -> t *= 10;
			p -> t += (buffer[i++] - '0');
		}

		i++;	//	Incrementez pentru a trece de spatiu

		//	Copiez prioritatea procesului
		p -> pr = 0;
		while( buffer[i] != ' ' && buffer[i] != '\n' ) {
			p -> pr *= 10;
			p -> pr += (buffer[i] - '0');
			i++;
		}
		i++;

		//	Leg urmatorul element ca NULL
		p -> next = NULL;
		//	Initializez contorul pentru promote si demote
		//	folosit la dinamizarea prioritatilor
		p -> cnt_promote = 0;
		p -> cnt_demote = 0;

		//	Si adaug procesul citit la sfarsitul listei
		//	ca intr-o coada:
		(*ready_end) -> next = p;
		(*ready_end) = p;
	}
}

//	Functie de mutare a unui proces din running in waiting:
void wait(List **waiting_end, List **running) {

	//	Copiez procesul intr-o memorie noua:
	List *p = malloc(sizeof(List));
	memcpy(p, *running, sizeof(List));
	//	Leg procesul la sfarsitul cozii:
	(*waiting_end) -> next = p;
	*waiting_end = p;
	(*waiting_end) -> next = NULL;
	//	Si "anulez" procesul din running:
	(*running) -> t = -1;
}

//	Functie de mutare a unui proces din wating in ready:
void event(char *buffer, List **waiting_list, List **ready_end) {

	//	Incrementez bufferul a.i. sa ramana doar numele procesului in el
	//	si copiez intr-o variabila locala, pentru a nu modifica bufferul:
	buffer += 2;
	char tmp[21];
	strcpy(tmp, buffer);
	//	Marchez sfarsitul stringului:
	tmp[strlen(tmp) - 1] = 0;

	//	Incep sa caut elementul din coada waiting cu numele primit si copiat:
	List *p = *waiting_list;	//	Pointer necesar pentru modificarea legaturilor
	List *q = (*waiting_list) -> next;
	//	Daca nu face matching pe nume, merg la elementul urmator
	while( strcmp(q->name, tmp) != 0 ) {
		q = q -> next;
		p = p -> next;
	}
	//	Dupa ce am gasit pozitia in lista, modific legaturile din waiting
	//	si mut procesul la sfarsitul cozii ready:
	p -> next = q -> next;
	(*ready_end) -> next = q;
	q -> next = NULL;
	*ready_end = q;
}

//	Functie de afisare a numelui si timpului procesului aflat in running:
void show(List *running, FILE *output) {

	//	Daca exista proces in running, afisez datele despre el:
	if( running -> t != -1 ) fprintf(output, "%s %d\n", running->name, running->t);
		else fprintf(output, "\n");
		//	altfel afisez linie noua..
}