/*
 ============================================================================
 Name        : PIM.c
 Author      : Paulo Duarte
 Version     :
 Copyright   : © 2015 Paulo Duarte All Rights Reserved
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define menuSize 3

void waitingForUser(void) {
	char prev = 0;
	printf("Precione [Enter] para continuar.\n");
	while (1) {
		char c = getchar();
		if (c == '\n' && prev == c) {
			break;
		}
		prev = c;
	}
}

int main(void) {
	char bus[9][4];
	char *cities[] = { "Porto Alegre", "Canoas", "Esteio", "Sapucaia do Sul",
			"São Leopoldo", "Novo Hamburgo" };
	int num_states = 6;
	int i;
	int j;
	int option;
	int start;
	int end;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 4; j++) {
			bus[i][j] = '0';
		}
	}

	while (1) {
		READMENU: printf("\nNOME DA EMPRESA DE ONIBUS\n\n");
		printf("1 - Vender passagem\n");
		printf("2 - Relatório de vendas\n");
		printf("0 - Sair do sistema\n\n");
		printf("Selecione uma opção: ");
		scanf("%d", &option);

		if (option < 0 || option > menuSize) {
			printf("Opção inválida\n");
			goto READMENU;
		}

		switch (option) {
		case 0:
			printf("Até mais!");
			return EXIT_SUCCESS;
		case 1:
			printf("Venda de passagens\n\n");
			for (i = 0; i < num_states; i++) {
				printf("%d: %s\n", i, cities[i]);
			}
			break;
		case 2:
			printf("Relatório de vendas\n\n");
			break;
		}
		waitingForUser();
	}
}

