#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "operations.h"
//	Functie pentru Planificare cu Prioritati Dinamice:
void bonus(FILE *input, FILE *output) {

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

	int cuanta; int proritate_max;
	//	Pentru acest tip de abordare avem nevoie de cuanta de timp
	//	si de prioritatea maxima a proceselor primite:
	fscanf(input, "%d %d\n", &cuanta, &proritate_max);
	int cnt_cuanta = cuanta;	//	initializam contorul pentru cuanta
	//	Variabila added este folosita la verificarea daca a fost
	//	adaugat un proces cu prioritate mai mare in coada ready
	int added = 0;

	//	Cat timp avem ce citi din fisier...
	while(fgets(buffer, 100, input)) {

		//	Initializez tick pentru a sti daca il aplicam
		// 	sau nu la sfarsit 
		int tick = 0;

		//	In functie de prima litera din buffer, apelam functia corespunzatoare:
		//	adaugare - multipla sau simpla:
		if( buffer[0] == 'm' || buffer[0] == 'a' ) {
			add(&ready_end, buffer);
			//	S-a adaugat un proces in coada ready:
			added = 1;
			// tick
			tick = 1;
		}
		// 	tick:
		if( buffer[0] == 't') {
			tick ++;
		}
		//	wait:
		if( buffer[0] == 'w' ) {
			//	daca un proces trece in wait, incrementam contorul pentru promote:
			running -> cnt_promote ++;
			//	iar daca contorul atinge val. 2, atunci il resetam si marim
			//	prioritatea procesului:
			if( running -> cnt_promote == 2 ){
				running -> cnt_promote = 0;
				running -> pr ++;
			}
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
			//	S-a adaugat un proces in ready, marcam variabila:
			added = 1;
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
				cnt_cuanta = cuanta;	//	reinitializam contorul
				//	Daca exista proces(e) in coada ready, il adaug pe cel
				//	cu prioritatea cel mai mare. Facem o cautare
				//	de maxim pe prioritate:
				if( ready_list -> next != NULL ) {
					//	Initializam pointerii folositi la parcrugere, iar
					//	pointerii _back sunt folositi la reformarea legaturilor
					List *p = ready_list -> next;
					List *p_back = ready_list;
					List *maxim = ready_list -> next;
					List *maxim_back = ready_list;

					while( p != NULL ) {
						//	Daca gasesc unul cu prioritate mai mare..
						if( (p -> pr) > (maxim -> pr) ) {
							//	schimb maxim si maxim_back
							maxim = p;
							maxim_back = p_back;
						}
						p = p -> next;
						p_back = p_back -> next;
					}
					//	Dupa ce am gasit procesul cu prioritate maxima,
					//	il mut in running si rearanjez legaturile din coada
					maxim_back -> next = maxim -> next;
					running = maxim;

					//	Ma asigur ca ready_end este pus corect:
					List *tmp = ready_list;
					while( tmp -> next != NULL ) tmp = tmp -> next;
					ready_end = tmp;
					//	Daca nu mai exista procese, marcam added cu 0
					if( ready_list == ready_end ) added = 0;
				}
			}
			//	Daca se termina cuanta de timp oferita standard procesului,
			//	scoatem procesul si il inlocuim cu unul din lista, care
			//	are prioritatea cea mai mare dintre toate; de asemenea,
			//	penalizam procesul daca a fost preemtat pentru a doua oara
			//	din cauza scurgerii cuantei de timp alocate:
			if( cnt_cuanta == 0 ){
				running -> cnt_demote ++;	//	Incrementam contorul pentru demote
				cnt_cuanta = cuanta;	//	Reintializam contorul pentru cuanta
				//	Verificam daca este necesara penalizarea procesului:
				if( running -> cnt_demote == 2 ){
					running -> cnt_demote = 0;
					running -> pr --;
				}

				//	La fel, facem o cautare de maxim pe prioritate:
				//	Dar mai intai, mutam procesul actual din running
				//	in coada ready, astfel incat sa se poata intoarce in 
				//	running daca el are prioritatea cea mai mare
				List *p = malloc(sizeof(List));
				memcpy(p, running, sizeof(List));
				//	Adaug procesul la sfarsitul cozii:
				ready_end -> next = p;
				ready_end = ready_end -> next;
				ready_end -> next = NULL;

				//	Pointerii _back sunt folositi la reformarea legaturilor
				List *q = ready_list -> next;
				List *q_back = ready_list;
				List *maxim = ready_list -> next;
				List *maxim_back = ready_list;
				//	Cat timp parcurg lista...
				while(q != NULL){
					//	Daca gasesc prioritate mai mare decat cel din maxim
					if( (q -> pr) > (maxim -> pr) ) {
						//	Schimb maximul cu acel proces:
						maxim = q;
						maxim_back = q_back;
					}
					q = q -> next;
					q_back = q_back -> next;
				}
				//	Mut in running maximul:
				running = maxim;
				//	Si refac legaturile din coada ready:
				maxim_back -> next = maxim -> next;
				
				List *tmp = ready_list;
				while( tmp -> next != NULL ) tmp = tmp -> next;
				ready_end = tmp;
			}
			//	Daca avem procese proaspat adaugate in ready, verificam
			//	daca unul din procesele adaugate are prioritate mai
			//	mare de cat cel din running pentru a il preemta:
			if( added == 1 ){
				//	Initializez pointari pentru a parcurge lista si
				//	depista procesul cu maxima prioritate:
				List *p = ready_list -> next;
				List *p_back = ready_list;
				List *maxim = ready_list -> next;
				List *maxim_back = ready_list;
				//	Cat timp parcurg lista nenula:
				while(p != NULL){
					if( (p -> pr) > (maxim -> pr) ) {
						maxim = p;
						maxim_back = p_back;
					}
					p = p -> next;
					p_back = p_back -> next;
				}
				//	Daca s-a gasit proces maxim, cu prioritate mai mare decat
				//	procesul din running, il mut in running:
				if( maxim != NULL && ((maxim -> pr) > (running -> pr)) ) {
					List *p = malloc(sizeof(List));
					memcpy(p, running, sizeof(List));

					ready_end -> next = p;
					ready_end = ready_end -> next;
					ready_end -> next = NULL;

					maxim_back -> next = maxim -> next;
				
					memcpy(running, maxim, sizeof(List));
					cnt_cuanta = cuanta;
					free(maxim);
				}
				//	Modificam variabila pentru verificare:
				added = 0;
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
