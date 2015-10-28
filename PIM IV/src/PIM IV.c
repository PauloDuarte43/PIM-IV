/*
 ============================================================================
 Name        : PIM.c
 Author      : Paulo Duarte
 Version     :
 Copyright   : © 2015 Paulo Duarte All Rights Reserved
 Description : PIM IV in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int menuSize = 2;
int num_cities = 6;
float valueOfKM = 0.2625;
char *cities[] = { "Porto Alegre", "Canoas", "Esteio", "Sapucaia do Sul",
		"São Leopoldo", "Novo Hamburgo" };
int citiesKM[] = { 0, 8, 17, 24, 38, 40 };
char bus[4][9];
char busTmp[4][9];
void waitingForUser(void);
void clearScreen(void);
void printCities(void);
int printMenu();
struct tm* getLocalTime();
void printBusAccents(void);
void printBusAccentsTemp(void);

int main(void) {
	int i;
	int j;
	int rowSelected;
	int columnSelected;
	int option;
	int start;
	int end;
	int oldMan;
	float subTotal;
	unsigned int totalKM;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 9; j++) {
			bus[i][j] = '0';
			busTmp[i][j] = '0';
		}
	}

	while (1) {
		option = printMenu();

		switch (option) {
		case 1:
			clearScreen();
			printf("Venda de passagens\n");

			printf("\nEscolha o ponto de partida\n\n");
			printCities();

			printf("\nSelecione uma opção: ");
			scanf("%d", &start);
			start -= 1;
			clearScreen();
			printf("\nPartida: %s\n", cities[start]);

			printf("\nEscolha o ponto de destino\n");
			printCities();
			printf("\nSelecione uma opção: ");
			scanf("%d", &end);
			end -= 1;
			clearScreen();

			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("**************************\n");

			if (citiesKM[start] < citiesKM[end]) {
				totalKM = citiesKM[end] - citiesKM[start];
			} else {
				totalKM = citiesKM[start] - citiesKM[end];
			}
			subTotal = valueOfKM * totalKM;
			printf("SubTotal: R$%.2f\n", subTotal);
			printf("\n");
			printBusAccents();

			while (1) {
				printf("\nSelecione a fila (de 1 a 4): ");
				scanf("%d", &rowSelected);

				printf("\nSelecione a coluna (de 1 a 9): ");
				scanf("%d", &columnSelected);

				printf("Fila: %d, Coluna: %d\n\n", rowSelected, columnSelected);
				if (bus[rowSelected - 1][columnSelected - 1] == '0') {
					bus[rowSelected - 1][columnSelected - 1] = 'x';
					busTmp[rowSelected - 1][columnSelected - 1] = 'x';
					break;
				} else {
					printf("Poltrona ocupada, selecione outro assento.");
				}
			}

			clearScreen();
			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("*Poltrona: %d:%d\n", rowSelected, columnSelected);
			printBusAccentsTemp();
			printf("**************************\n");
			printf("SubTotal: R$%.2f\n", subTotal);
			printf("\n");
			busTmp[rowSelected - 1][columnSelected - 1] = '0';
			break;
		case 2:
			clearScreen();
			printf("Relatório de vendas\n\n");
			printBusAccents();
			break;
		case 0:
			clearScreen();
			printf("Até mais!\n");
			return EXIT_SUCCESS;
		}
		waitingForUser();
	}
}

void waitingForUser(void) {
	char prev = 0;
	printf("\nPrecione [Enter] para continuar.\n");
	while (1) {
		char c = getchar();
		if (c == '\n' && prev == c) {
			break;
		}
		prev = c;
	}
}

void clearScreen(void) {
	if (__unix__) {
		system("clear");
	} else {
		system("cls");
	}
}

void printCities(void) {
	int i;
	for (i = 0; i < num_cities; i++) {
		printf("%d: %s\n", i + 1, cities[i]);
	}
}

int printMenu() {
	int option;
	WRITEMENU: clearScreen();
	printf("\nNOME DA EMPRESA DE ONIBUS\n\n");
	printf("1 - Vender passagem\n");
	printf("2 - Relatório de vendas\n");
	printf("0 - Sair do sistema\n\n");
	printf("Selecione uma opção: ");
	scanf("%d", &option);

	if (option < 0 || option > menuSize) {
		goto WRITEMENU;
	}
	return option;
}

struct tm* getLocalTime() {
	time_t epoch_time;
	struct tm* tm_p;
	epoch_time = time(NULL);
	tm_p = localtime(&epoch_time);
	/*struct tm* tm_p = getLocalTime();
	 tm_p->tm_sec += 2400;
	 mktime(tm_p);
	 printf("The time is %.2d:%.2d\n", tm_p->tm_hour, tm_p->tm_min);
	 return 0;*/
	return tm_p;
}

void printBusAccents(void) {
	int i, j;
	printf(" 123456789\n");
	for (i = 0; i < 4; i++) {
		printf("%d", i + 1);
		for (j = 0; j < 9; j++) {
			printf("%c", bus[i][j]);
		}
		printf("\n");
	}
}

void printBusAccentsTemp(void) {
	int i, j;
	printf(" 123456789\n");
	for (i = 0; i < 4; i++) {
		printf("%d", i + 1);
		for (j = 0; j < 9; j++) {
			printf("%c", busTmp[i][j]);
		}
		printf("\n");
	}
}
