#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<stdlib.h>

int pauzirano = 0;
long long zadnji_prosti = 2;

void zavrsi() {
	printf("zadnji prosti broj = %llu\n", zadnji_prosti);
	exit(0);
}

void promijeni_pause() {
	if(!pauzirano) {
		printf("pauziram ...\n");
	} else {
		printf("nastavljam ...\n");
	}
	pauzirano = 1 - pauzirano;
}

int je_prost(int n) {
	int i;
	for(i = 2; n % i != 0; i++);
	if(i == n) {
		return 1;
	}
	return 0;
}

void sljedeci_prosti() {
	int i;
	for(i = zadnji_prosti + 1; ; ++i) {
		if (je_prost(i)) {
			zadnji_prosti = i;
			break;
		}
	}
}

void periodicki_posao() {
	printf("zadnji prosti broj = %llu\n", zadnji_prosti);
}

int main(void) {
	struct itimerval t;

	sigset(SIGINT, promijeni_pause);
	sigset(SIGTERM, zavrsi);	
	sigset(SIGALRM, periodicki_posao);

	t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 10;

	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 1000000;
	
	setitimer ( ITIMER_REAL, &t, NULL );

	while(1) {
		if (pauzirano){
			pause();
		} else {
			sljedeci_prosti();
		}
	}
	
	return 0;
}