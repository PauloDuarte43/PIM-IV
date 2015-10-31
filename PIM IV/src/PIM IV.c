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
#include <string.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#ifndef __unix__
#define __unix__ 0
#endif

//Global variables
int idVenda = 0;
int menuSize = 2;
int num_cities = 6;
float valueOfKM = 0.2625;
int timeBetweenBus = 2100;
int busBegin = 5;
int busEnd = 23;
char *cities[] = { "Porto Alegre", "Canoas", "Esteio", "Sapucaia do Sul",
		"Sao Leopoldo", "Novo Hamburgo" };
int citiesKM[] = { 0, 8, 17, 24, 38, 40 };
char bus[4][9];
char busTmp[4][9];
char hoursBus[35][12];

//0 - partida
//1 - destino
//2 - fila
//3 - coluna
//4 - idoso
//5 - horario
int sales[1296][6];

void waitingForUser(void);
void clearScreen(void);
void printCities(void);
int printMenu();
struct tm* getLocalTime();
void printBusAccents(char printBus[4][9]);
void initializeBusHours(void);
int printBusHours(void);
void initializeBusSeats(void);
float calcTotal(int start, int end);
int clean_stdin();

int main(void) {
	int rowSelected;
	int columnSelected;
	int start;
	int end;
	int busHour;
	//int oldMan;
	float subTotal;
	char c;

	initializeBusSeats();
	initializeBusHours();

	while (1) {
		switch (printMenu()) {
		case 1:
			clearScreen();
			printf("Venda de passagens\n");

			while (1) {
				printf("\nEscolha o ponto de partida\n\n");
				printCities();

				printf("\nSelecione uma opcao: ");
				while (((scanf("%d%c", &start, &c) != 2 || c != '\n')
						&& clean_stdin()) || start < 1 || start > 4) {
					printf("\nPonto de partida invalido.\n");
					printf("Selecione uma opcao: ");
				}
				start -= 1;
				clearScreen();
				printf("\nPartida: %s\n", cities[start]);

				printf("\nEscolha o ponto de destino\n");
				printCities();
				printf("\nSelecione uma opcao: ");
				while (((scanf("%d%c", &end, &c) != 2 || c != '\n')
						&& clean_stdin()) || end < 1 || end > 9) {
					printf("\nPonto de destino invalido.\n");
					printf("Selecione uma opcao: ");
				}
				end -= 1;
				clearScreen();

				if (start == end) {
					printf(
							"O ponto de partida e destino devem ser diferentes.\n");
				} else {
					break;
				}
			}

			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("**************************\n");

			subTotal = calcTotal(start, end);
			printf("SubTotal: R$%.2f\n", subTotal);
			printf("\n");

			printf("\nSelecione a fila (de 1 a 4): ");
			while (((scanf("%d%c", &rowSelected, &c) != 2 || c != '\n')
					&& clean_stdin()) || rowSelected < 1 || rowSelected > 4) {
				printf("\nFila invalida. ");
				printf("\nSelecione a fila (de 1 a 4): ");
			}
			rowSelected -= 1;

			printf("\nSelecione a coluna (de 1 a 9): ");
			while (((scanf("%d%c", &columnSelected, &c) != 2 || c != '\n')
					&& clean_stdin()) || columnSelected < 1
					|| columnSelected > 9) {
				printf("\nColuna invalida. ");
				printf("\nSelecione a coluna (de 1 a 9): ");
			}
			columnSelected -= 1;
			printf("Fila: %d, Coluna: %d\n\n", rowSelected + 1,
					columnSelected + 1);

			int totalHours = printBusHours();
			printf("\nSelecione o horario de partida do onibus (de 1 a %d)\n", totalHours);
			while (((scanf("%d%c", &busHour, &c) != 2 || c != '\n')
					&& clean_stdin()) || busHour < 1 || busHour > totalHours) {
				printf("\nHorario invalido.\n");
				printBusHours();
				printf("\nSelecione o horario de partida do onibus (de 1 a %d)\n", totalHours);
			}
			busHour -= 1;

			clearScreen();
			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("*Horario de saida: %s\n", hoursBus[busHour]);
			printf("*Poltrona: %d:%d\n", rowSelected, columnSelected);
			printBusAccents(busTmp);
			printf("**************************\n");
			printf("SubTotal: R$%.2f\n", subTotal);
			printf("\n");

			busTmp[rowSelected - 1][columnSelected - 1] = '0';
			sales[idVenda][0] = start;
			sales[idVenda][1] = end;
			sales[idVenda][2] = rowSelected;
			sales[idVenda][3] = columnSelected;
			sales[idVenda][4] = 0;
			sales[idVenda][5] = busHour;
			idVenda++;
			break;
		case 2:
			clearScreen();
			printf("Relatorio de vendas\n\n");
			printf("\n");
			int i = 0;
			while (i < idVenda) {
				printf("\n**************************\n");
				printf("*Numero da passagem: %d\n", i + 1);
				printf("*Partida: %s\n", cities[sales[i][0]]);
				printf("*Destino: %s\n", cities[sales[i][1]]);
				printf("*Horario de saida: %s\n", hoursBus[sales[i][5]]);
				printf("*Poltrona: %d:%d\n", sales[i][2] + 1, sales[i][3] + 1);
				printf("**************************\n");
				printf("SubTotal: R$%.2f\n", subTotal);
				printf("\n");
				i++;
			}
			break;
		case 0:
			clearScreen();
			printf("Ate mais!\n");
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
	printf("2 - Relatorio de vendas\n");
	printf("0 - Sair do sistema\n\n");
	printf("Selecione uma opcao: ");
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

	return tm_p;
}

void initializeBusSeats(void) {
	int i;
	int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 9; j++) {
			bus[i][j] = '0';
			busTmp[i][j] = '0';
		}
	}
}

void initializeBusHours(void) {
	int i = 0;
	char hours[3];
	char minutes[3];
	char seconds[3];
	char busHour[12];
	int begin = (busBegin * (60 * 60));
	int end = (busEnd * (60 * 60));
	while (begin <= end) {
		int forHours = begin / 3600, remainder = begin % 3600, forMinutes =
				remainder / 60, forSeconds = remainder % 60;
		sprintf(hours, "%d", forHours);
		if (forHours < 10) {
			char temp[3];
			strcpy(temp, "0");
			strcat(temp, hours);
			strcpy(hours, temp);
		}
		sprintf(minutes, "%d", forMinutes);
		if (forMinutes < 10) {
			char temp[3];
			strcpy(temp, "0");
			strcat(temp, minutes);
			strcpy(minutes, temp);

		}
		sprintf(seconds, "%d", forSeconds);
		if (forSeconds < 10) {
			char temp[3];
			strcpy(temp, "0");
			strcat(temp, seconds);
			strcpy(seconds, temp);
		}

		strcpy(busHour, hours);
		strcat(busHour, ":");
		strcat(busHour, minutes);
		strcat(busHour, ":");
		strcat(busHour, seconds);

		strcpy(hoursBus[i], busHour);
		begin += timeBetweenBus;
		i++;
	}
}

void printBusAccents(char printBus[4][9]) {
	int i, j;
	printf(" 123456789\n");
	for (i = 0; i < 4; i++) {
		printf("%d", i + 1);
		for (j = 0; j < 9; j++) {
			printf("%c", printBus[i][j]);
		}
		printf("\n");
	}
}

int printBusHours(void) {
	int j = 0, i = 0;
	while (i < (sizeof(hoursBus) / sizeof(char[12]))) {
		if (strlen(hoursBus[i]) > 0) {
			j++;
			if (i + 1 < 10) {
				printf("  %d - %s | ", i + 1, hoursBus[i]);
			} else {
				printf(" %d - %s | ", i + 1, hoursBus[i]);
			}
			if ((i + 1) % 5 == 0)
				printf("\n");
		}
		i++;
	}
	return j;
}

float calcTotal(int start, int end) {
	float subTotal;
	unsigned int totalKM;
	if (citiesKM[start] < citiesKM[end]) {
		totalKM = citiesKM[end] - citiesKM[start];
	} else {
		totalKM = citiesKM[start] - citiesKM[end];
	}
	subTotal = valueOfKM * totalKM;
	return subTotal;
}

int clean_stdin(void) {
	while (getchar() != '\n')
		;
	return 1;
}
