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

int menuSize = 3;
int maxCapacity = 35;
int num_cities = 6;
float valueOfKM = 0.2625;
float studentDiscountPercentage = 0.5;
float oldManDiscountPercentage = 1;
int timeBetweenBus = 2100;
int busBegin = 5;
int busEnd = 23;
char *cities[] = { "Porto Alegre", "Canoas", "Esteio", "Sapucaia do Sul",
		"Sao Leopoldo", "Novo Hamburgo" };
int citiesKM[] = { 0, 8, 17, 24, 38, 40 };
char bus[4][9];
char busTmp[4][9];
char hoursBus[35][12];

//0-partida 1-destino 2-fila 3-coluna 4-idoso 5-horario 6-estudante
int countSales = 0;
int sales[1296][7];

int countSalesByHour;
int salesByHour[1296][7];

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
int cleanStdin();
void findSaleByHour(int idHour);
int validAssent(int row, int column);
int busNumber(char busHour[12]);
int validOldMan(void);

int main(void) {
	int rowSelected;
	int columnSelected;
	int start;
	int end;
	int busHour;
	float subTotal;
	float discount;
	float total;
	char c;
	int totalHours;
	char oldMan;
	char student;
	char confirmSale;
	double totalGeral;
	int countStudent;
	int countOldMan;

	//initializeBusSeats();
	initializeBusHours();

	while (1) {
		switch (printMenu()) {
		case 1:
			clearScreen();
			START: printf("Venda de passagens\n");

			while (1) {
				totalHours = printBusHours();
				printf(
						"\nSelecione o horario de partida do onibus (de 1 a %d)\n",
						totalHours);
				while (((scanf("%d%c", &busHour, &c) != 2 || c != '\n')
						&& cleanStdin()) || busHour < 1 || busHour > totalHours) {
					clearScreen();
					printf("\nHorario invalido.\n");
					printBusHours();
					printf(
							"\nSelecione o horario de partida do onibus (de 1 a %d)\n",
							totalHours);
				}
				busHour -= 1;
				findSaleByHour(busHour);
				if (countSalesByHour <= maxCapacity) {
					break;
				} else {
					printf("\nOnibus lotado escolha outro horario.\n");
				}
			}

			clearScreen();
			printf("*Horario de saida: %s\n", hoursBus[busHour]);

			while (1) {
				printf("\nEscolha o ponto de partida\n\n");
				printCities();

				printf("\nSelecione uma opcao: ");
				while (((scanf("%d%c", &start, &c) != 2 || c != '\n')
						&& cleanStdin()) || start < 1 || start > 4) {
					clearScreen();
					printCities();
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
						&& cleanStdin()) || end < 1 || end > 9) {
					clearScreen();
					printCities();
					printf("\nPonto de destino invalido.\n");
					printf("Selecione uma opcao: ");
				}
				end -= 1;

				if (start == end) {
					printf(
							"O ponto de partida e destino devem ser diferentes.\n");
				} else {
					break;
				}
			}

			clearScreen();
			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("**************************\n");

			while (1) {
				printf("\nSelecione a fila (de 1 a 4): ");
				while (((scanf("%d%c", &rowSelected, &c) != 2 || c != '\n')
						&& cleanStdin()) || rowSelected < 1 || rowSelected > 4) {
					clearScreen();
					printf("\nFila invalida. ");
					printf("\nSelecione a fila (de 1 a 4): ");
				}
				rowSelected -= 1;

				printf("\nSelecione a coluna (de 1 a 9): ");
				while (((scanf("%d%c", &columnSelected, &c) != 2 || c != '\n')
						&& cleanStdin()) || columnSelected < 1
						|| columnSelected > 9) {
					clearScreen();
					printf("\nColuna invalida. ");
					printf("\nSelecione a coluna (de 1 a 9): ");
				}
				columnSelected -= 1;
				if (!validAssent(rowSelected, columnSelected)) {
					printf("Assento ocupado\n");
				} else {
					break;
				}
			}

			clearScreen();
			printf("Fila: %d, Coluna: %d\n\n", rowSelected + 1,
					columnSelected + 1);

			subTotal = calcTotal(start, end);

			printf("\nDesconto para idoso? (S-Sim ou N-Nao) ");
			while (((scanf("%s%c", &oldMan, &c) != 2 || c != '\n')
					&& cleanStdin())
					|| (oldMan != 'n' && oldMan != 's' && oldMan != 'N'
							&& oldMan != 'S')) {
				printf("\nOpcao invalida.");
				printf("\nDesconto para idoso? (S-Sim ou N-Nao) ");
			}

			discount = 0;
			if (oldMan == 's' || oldMan == 'S') {
				if (!validOldMan()) {
					printf(
							"Este horario ja atingiu o limite de idosos, selecione outro horario.");
					waitingForUser();
					goto START;
				}
				discount = subTotal * oldManDiscountPercentage;
			} else {
				printf("\nDesconto para estudante? (S-Sim ou N-Nao) ");
				while (((scanf("%s%c", &student, &c) != 2 || c != '\n')
						&& cleanStdin())
						|| (student != 'n' && student != 's' && student != 'N'
								&& student != 'S')) {
					printf("\nOpcao invalida.");
					printf("\nDesconto para estudante? (S-Sim ou N-Nao) ");
				}
				if (student == 's' || student == 'S') {
					discount = subTotal * studentDiscountPercentage;
				}
			}
			total = subTotal - discount;
			/*printf("SubTotal: R$%.2f\n", subTotal);
			 printf("\n");*/

			clearScreen();
			printf("\n**************************\n");
			printf("*Partida: %s\n", cities[start]);
			printf("*Destino: %s\n", cities[end]);
			printf("*Horario de saida: %s\n", hoursBus[busHour]);
			printf("*Numero do onibus: %d\n", busNumber(hoursBus[busHour]));
			printf("*Poltrona: %d:%d\n", rowSelected + 1, columnSelected + 1);
			printf("**************************\n");
			printf("SubTotal: R$%.2f\n", subTotal);
			printf("Desconto: R$%.2f\n", discount);
			printf("Total: R$%.2f\n", total);
			printf("\n");

			printf("\nConfirmar venda? (S-Sim ou N-Nao) ");
			while (((scanf("%s%c", &confirmSale, &c) != 2 || c != '\n')
					&& cleanStdin())
					|| (confirmSale != 'n' && confirmSale != 's'
							&& confirmSale != 'N' && confirmSale != 'S')) {
				printf("\nOpcao invalida.");
				printf("\nConfirmar venda? (S-Sim ou N-Nao) ");
			}
			if (confirmSale == 's' || confirmSale == 'S') {
				sales[countSales][0] = start;
				sales[countSales][1] = end;
				sales[countSales][2] = rowSelected;
				sales[countSales][3] = columnSelected;
				sales[countSales][4] = oldMan == 's' || oldMan == 'S';
				sales[countSales][5] = busHour;
				sales[countSales][6] = student == 's' || student == 'S';
				countSales++;
			} else {
				printf("\nVenda cancelada.");
			}
			waitingForUser();
			break;
		case 2:
			clearScreen();
			printf("Relatorio de vendas\n\n");
			printf("\n");
			int i = 0;
			totalGeral = 0;
			countStudent = 0;
			countOldMan = 0;

			while (i < countSales) {
				printf("\n**************************\n");
				printf("*Numero da passagem: %d\n", i + 1);
				printf("*Partida: %s\n", cities[sales[i][0]]);
				printf("*Destino: %s\n", cities[sales[i][1]]);
				printf("*Horario de saida: %s\n", hoursBus[sales[i][5]]);
				printf("*Numero do onibus: %d\n",
						busNumber(hoursBus[sales[i][5]]));
				printf("*Poltrona: %d:%d\n", sales[i][2] + 1, sales[i][3] + 1);
				printf("**************************\n");
				subTotal = calcTotal(sales[i][0], sales[i][1]);
				discount = 0;
				if (sales[i][4] == 1) {
					countOldMan++;
					discount = subTotal * oldManDiscountPercentage;
				} else {
					if (sales[i][6] == 1) {
						countStudent++;
						discount = subTotal * studentDiscountPercentage;
					}
				}
				total = subTotal - discount;
				printf("SubTotal: R$%.2f\n", subTotal);
				printf("Desconto: R$%.2f\n", discount);
				printf("Total: R$%.2f\n", total);
				printf("\n");
				totalGeral += total;
				i++;
			}
			printf("\n**************************\n");
			printf("*Total de passagens vendidas: %d\n", countSales);
			printf(
					"*Total de passagens vendidas com desconto de estudante: %d\n",
					countStudent);
			printf("*Total de passagens vendidas com desconto de idoso: %d\n",
					countOldMan);
			printf("Total de vendas do dia: R$%.2f\n", totalGeral);
			printf("**************************\n");

			waitingForUser();
			break;
		case 3:
			printf("Relatorio de vendas por horario\n\n");
			int h = printBusHours();
			printf("\nSelecione o horario de partida do onibus (de 1 a %d)\n",
					totalHours);
			totalGeral = 0;
			while (((scanf("%d%c", &busHour, &c) != 2 || c != '\n')
					&& cleanStdin()) || busHour < 1 || busHour > h) {
				printf("\nHorario invalido.\n");
				printBusHours();
				printf(
						"\nSelecione o horario de partida do onibus (de 1 a %d)\n",
						h);
			}
			busHour -= 1;
			findSaleByHour(busHour);
			i = 0;
			clearScreen();
			while (i < countSalesByHour) {
				printf("\n**************************\n");
				printf("*Numero da passagem: %d\n", i + 1);
				printf("*Partida: %s\n", cities[salesByHour[i][0]]);
				printf("*Destino: %s\n", cities[salesByHour[i][1]]);
				printf("*Horario de saida: %s\n", hoursBus[salesByHour[i][5]]);
				printf("*Numero do onibus: %d\n",
						busNumber(hoursBus[salesByHour[i][5]]));
				printf("*Poltrona: %d:%d\n", salesByHour[i][2] + 1,
						salesByHour[i][3] + 1);
				printf("**************************\n");
				subTotal = calcTotal(salesByHour[i][0], salesByHour[i][1]);
				if (sales[i][4] == 1) {
					discount = subTotal * oldManDiscountPercentage;
				} else {
					if (sales[i][6] == 1) {
						discount = subTotal * studentDiscountPercentage;
					}
				}
				printf("SubTotal: R$%.2f\n", subTotal);
				printf("Desconto: R$%.2f\n", discount);
				printf("Total: R$%.2f\n", total);
				printf("\n");
				totalGeral += total;
				i++;
			}
			waitingForUser();
			break;
		case 0:
			clearScreen();
			printf("Ate mais!\n");
			return EXIT_SUCCESS;
		}
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
	char c;
	clearScreen();
	printf("\nNOME DA EMPRESA DE ONIBUS\n\n");
	printf("1 - Vender passagem\n");
	printf("2 - Relatorio de vendas\n");
	printf("3 - Relatorio de vendas por onibus\n");
	printf("0 - Sair do sistema\n\n");
	printf("Selecione uma opcao: ");
	while (((scanf("%d%c", &option, &c) != 2 || c != '\n') && cleanStdin())
			|| option < 0 || option > menuSize) {
		clearScreen();
		printf("Opcao invalida.\n");
		printf("\nNOME DA EMPRESA DE ONIBUS\n\n");
		printf("1 - Vender passagem\n");
		printf("2 - Relatorio de vendas\n");
		printf("3 - Relatorio de vendas por onibus\n");
		printf("0 - Sair do sistema\n\n");
		printf("Selecione uma opcao: ");
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

void findSaleByHour(int idHour) {
	int i = 0;
	int a;
	countSalesByHour = 0;
	while (i < countSales) {
		if (sales[i][5] == idHour) {
			for (a = 0; a < 6; a++) {
				salesByHour[countSalesByHour][a] = sales[i][a];
			}
			countSalesByHour++;
		}
		i++;
	}
}

int validAssent(int row, int column) {
	int i = 0;
	while (i < countSalesByHour) {
		if (salesByHour[i][2] == row && salesByHour[i][3] == column) {
			return 0;
		}
		i++;
	}
	return 1;
}

int validOldMan(void) {
	int i = 0;
	int count = 0;
	while (i < countSalesByHour) {
		if (salesByHour[i][4] == 1) {
			count++;
		}
		i++;
	}
	if (count < 2) {
		return 1;
	} else {
		return 0;
	}
}

int busNumber(char busHour[12]) {
	char *n = malloc(sizeof(char) * 5);
	n[0] = busHour[4];
	n[1] = busHour[3];
	n[2] = busHour[1];
	n[3] = busHour[0];
	int a = (int) strtol(n, NULL, 10);
	return a;
}

int cleanStdin(void) {
	while (getchar() != '\n')
		;
	return 1;
}
