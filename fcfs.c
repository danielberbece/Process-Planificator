#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "operations.h"

//	Functie pentru tipul 1 de abordare al proceselor:
void fcfs(FILE *input, FILE *output) {

	//	Trec la linia urmatoare din fisierul de intrare
	fscanf(input, "\n");

	//	Initializez capurile de lista folosite:
	//	running
	List *running = malloc(sizeof(List));
	running -> next = NULL;
	running -> t = -1;

	//	ready_list si ready_end
	List *ready_list = (List *)malloc(sizeof(List));
	List *ready_end = ready_list;
	ready_list -> next = NULL;

	//	waiting_list si waiting_end
	List *waiting_list = (List *)malloc(sizeof(List));
	List *waiting_end = waiting_list;
	waiting_list -> next = NULL;

	//	Initializez si bufferul din care voi prelua
	//	datele despre procesele adaugate si comenzile:
	//	(w)ait, (t)ick, (e)vent si (s)how
	char *buffer = (char*)malloc(100*sizeof(char));

	//	Cat timp avem ce citi din fisier...
	while(fgets(buffer, 100, input)) {

		//	initializez tick pentru a sti daca il aplicam
		// 	sau nu la sfarsit 
		int tick = 0;

		//	In functie de prima litera din buffer, apelam functia corespunzatoare:
		//	adaugare - multipla sau simpla:
		if( buffer[0] == 'm' || buffer[0] == 'a' ) {
			add(&ready_end, buffer);
			// tick
			tick = 1;
		}
		//	tick:
		if( buffer[0] == 't') {
			tick ++;
		}
		//	wait:
		if( buffer[0] == 'w' ) {
			wait(&waiting_end, &running);
			// tick
			tick ++;
		}
		//	event:
		if( buffer[0] == 'e' ) {

			event(buffer, &waiting_list, &ready_end);

			List *tmp = waiting_list;
			while( tmp -> next != NULL ) tmp = tmp -> next;
			waiting_end = tmp;
			//tick
			tick ++;
		}
		//	show:
		if( buffer[0] == 's' ) {
			show(running, output);
		}
		//	bataie de ceas:
		while( tick ) {

			//	Daca avem proces valabil in running ii decrementam timpul:
			if( running -> t > 0 ) {
				running -> t --;
			}
			//	Daca se termina timpul necesar rularii procesului
			//	sau daca a fost deja mutat procesul din running...
			if( running -> t <= 0 ) {
				// scoatem procesul si introducem altul (daca exista)
				running -> t = -1;
				if( ready_list -> next != NULL ) {
					
					//	Copiem urmatorul proces din coada ready in running:
					List *p = ready_list -> next;
					ready_list -> next = p -> next;
					running = p;

					//	Si verificam ca ready_end sa fie corect:
					List *tmp = ready_list;
					while( tmp -> next != NULL ) tmp = tmp -> next;
					ready_end = tmp;
				}
			}

			tick--;
		}	
	}

	//	La sfarsit, eliberam memoria heap folosita:
	List *nod;
	
	nod = ready_list;
	while(nod != NULL) {
		List *tmp = nod;
		nod = nod -> next;
		free(tmp);
	}

	nod = waiting_list;
	while( nod != NULL ) {
		List *tmp = nod;
		nod = nod -> next;
		free(tmp);
	}

	free(running);
	free(buffer);
}
