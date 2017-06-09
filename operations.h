/*
	Acest header contine operatiile de baza folosite indiferent
	de abordarea aleasa: fcfs, sjf, roundrobin sau pp.

	add - adaugarea unui proces in coada ready
	wait - mutarea unui proces din running in coada waiting
	event - mutarea unui proces din coada waiting in coada ready
	show - afisarea numelui procesului si al timpului ramas pana la finalizare
*/

#ifndef _OPERATIONS_H
	#define _OPERATIONS_H
	void add(List **ready_end, char *buffer);
	void wait(List **waiting_end, List **running);
	void event(char *buffer, List **waiting_list, List **ready_end);
	void show(List *running, FILE *output);
#endif