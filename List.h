/*
	Definirea structurii pentru formarea listelor
	folosite in abordarea temei: coada ready, coada waiting
	si lista statica de un element, running.
	Fiecare element al listei contine:
		t - timp necesar finalizarii procesului
		pr - prioritatea procesului
		cnt_promote - contor pentru cresterea prioritatii procesului
		cnt_demote - contor pentru scaderea prioritatii procesului
		name - numele procesului
		next - adresa catre urmatorul element al listei
*/

#ifndef _LIST_H
	#define _LIST_H
	typedef struct list {

		int t;
		int pr;
		int cnt_promote;
		int cnt_demote;
		char name[21];
		struct list *next;
	} List;
#endif
