/*
	Tema 1 - Structuri de Date - Planificator de Procese
	Student: Daniel Berbece
	Grupa: 311CC
	Mai multe detalii despre abordarea temei se gasesc in README.
*/

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

//	Declaratiile functiilor principale - dezvoltarea lor
//	se gaseste in fisierele cu acelasi nume

void fcfs(FILE *input, FILE *output);
void sjf(FILE *input, FILE *output);
void roundrobin(FILE *input, FILE *output);
void pp(FILE *input, FILE *output);
void bonus(FILE *input, FILE *output);


int main(int argc, char *argv[]) {

	// 	Fisierele de intrare, respectiv iesire sunt primite
	// 	ca parametrii la rularea programului

	FILE *input = fopen(argv[1], "r");
	FILE *output = fopen(argv[2], "w");

	//	In functie de numarul citit la inceputul fisierului
	//	alegem abordarea de planificare a proceselor:

	int type;
	fscanf(input, "%d", &type);

	switch( type ) {
		case 1:	//	First Came, First Served
			fcfs(input, output);
			break;
		case 2: //	Shortest Job First
			sjf(input, output);
			break;
		case 3: //	Round Robin
			roundrobin(input, output);
			break;
		case 4: //	Planificare cu Prioritati
			pp(input, output);
			break;
		case 5: //	Planificare cu Prioritati Dinamice
			bonus(input, output);
			break;
		default: break;
	}

	//	Finalizarea programului
	
	fclose(input);
	fclose(output);
	return 0;
}
