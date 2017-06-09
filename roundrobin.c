#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "operations.h"

//	Functie pentru tipul 3 de abordare al proceselor:
void roundrobin(FILE *input, FILE *output) {

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

	int cuanta;
	//	Pentru acest tip de abordare avem nevoie de cuanta de timp:
	fscanf(input, "%d\n", &cuanta);
	int cnt_cuanta = cuanta;

	//	Cat timp avem ce citi din fisier...
	while(fgets(buffer, 100, input)) {

		//	Initializez tick pentru a sti daca il aplicam
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
				//	si decrementam si contorul pentru cuanta de timp
				cnt_cuanta--;
			}
			//	Daca se termina timpul necesar rularii procesului
			//	sau daca a fost deja mutat procesul din running...
			if( running -> t <= 0 ) {
				// scoatem procesul si introducem altul (daca exista)
				running -> t = -1;
				cnt_cuanta = cuanta; // reinitializam contorul
				// 	Daca exista proces in coada ready, il adaug pe primul
				if( ready_list -> next != NULL ) {

					memcpy(running, ready_list -> next, sizeof(List));

					List *p = ready_list -> next;
					ready_list -> next = p -> next;
					free(p);

					//	Ma asigur ca ready_end este pus corect:
					List *tmp = ready_list;
					while( tmp -> next != NULL ) tmp = tmp -> next;
					ready_end = tmp;
				}
			}
			//	Daca se termina cuanta de timp oferita standard procesului,
			//	scoatem procesul si il inlocuim cu urmatorul din lista:
			if( cnt_cuanta == 0 ){
				cnt_cuanta = cuanta; //	Reinitializam contorul
				
				// 	si copiem primul proces din coada ready. Nu verificam daca
				// 	exista elemente in ea pentru ca va fi cel putin procesul
				//	proaspat scos din running si adaugat in coada ready
				List *p = malloc(sizeof(List));
				memcpy(p, running, sizeof(List));
				//	Adaug elementul la sfarsitul cozii:
				ready_end -> next = p;
				ready_end = ready_end -> next;
				ready_end -> next = NULL;

				//	Mut primul element din ready in running:
				running = ready_list -> next;
				ready_list -> next = ready_list -> next -> next;
				//	Ma asigur ca ready_end este pus corect:
				List *tmp = ready_list;
				while( tmp -> next != NULL ) tmp = tmp -> next;
				ready_end = tmp;
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
