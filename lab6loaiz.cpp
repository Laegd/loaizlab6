#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h> 
#include <iostream>

using namespace std;

int** createG(int size)
{
	int** G;
	G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++)
	{
		G[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++)
	{
		G[i][i] = 0;
		for (int j = i + 1; j < size; j++)
		{
			G[i][j] = rand() % 2;
			G[j][i] = G[i][j];
		}
	}
	return G;
};

void printG(int** G, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << G[i][j] << " ";
		}
		cout << endl;
	}
};

int** delV(int** G, int size, int V)//удаление ребра
{
	int** Gnew = createG(size - 1);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (j < V && i < V) Gnew[i][j] = G[i][j];
			if (j > V && i < V) Gnew[i][j - 1] = G[i][j];
			if (j < V && i > V) Gnew[i - 1][j] = G[i][j];
			if (j > V && i > V) Gnew[i - 1][j - 1] = G[i][j];
		}
	}
	return Gnew;
};

int** unionV(int** G, int size, int v1, int v2)//отождествление ребра
{
	for (int i = 0; i < size; i++)
	{
		if (G[v2][i] == 1)
		{
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];
		}
	}
	G = delV(G, size, v2);

	return G;
};

int** contrE(int** G, int size, int v1, int v2)//стягивание ребра
{
	G[v1][v2] = 0;
	G[v2][v1] = 0;
	for (int i = 0; i < size; i++)
	{
		if (G[v2][i] == 1)
		{
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];
		}
	}
	G = delV(G, size, v2);

	return G;
}

int** splitV(int** G, int size, int v)//расщипление ребра
{
	int** Gtemp = createG(size + 1);
	for (int i = 0; i < size; i++)
	{
		Gtemp[i][size] = G[i][v];
		Gtemp[size][i] = G[v][i];
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			Gtemp[i][j] = G[i][j];
		}
	}
	Gtemp[v][v] = 0;
	Gtemp[size][size] = 0;
	for (int i = 0; i < size; i++)
	{
		free(G[i]);
	}
	free(G);
	G = NULL;
	return Gtemp;
};

int** unionG(int** G1, int** G2, int size1, int size2)// объединение
{
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;

	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;
	int** Gtemp = createG(sizemax);

	for (int i = 0; i < sizemax; i++)
	{
		for (int j = 0; j < sizemax; j++)
		{
			Gtemp[i][j] = Gmax[i][j];
		}
	}
	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			if (Gmin[i][j] == 1)
			{
				Gtemp[i][j] = Gmin[i][j];
			}

		}
	}
	return Gtemp;
};
int** intersectionG(int** G1, int** G2, int size1, int size2)//Пересечение
{
	int sizemin = (size1 < size2) ? size1 : size2;

	int** Gtemp = createG(sizemin);

	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			Gtemp[i][j] = G1[i][j] & G2[i][j];
		}
	}
	return Gtemp;
};
int** xorG(int** G1, int** G2, int size1, int size2, int* nG5)//Кольцевая сумма
{
	int step;
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;
	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;

	int** Gtemp = createG(sizemax);

	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			Gtemp[i][j] = G1[i][j] ^ G2[i][j];
		}
	}
	for (int i = 0; i < sizemin; i++)
	{
		for (int j = sizemin; j < sizemax; j++)
		{
			Gtemp[i][j] = Gmax[i][j];
		}
	}
	for (int i = sizemin; i < sizemax; i++)
	{
		for (int j = 0; j < sizemax; j++)
		{
			Gtemp[i][j] = Gmax[i][j];
		}
	}
	for (int i = 0; i < sizemax; i++)
	{
		step = 0;
		for (int j = 0; j < sizemax; j++)
		{
			if (Gtemp[i][j] == 1)
			{
				step++;
			}
		}
		if (step == 0)
		{
			Gtemp = delV(Gtemp, sizemax, i);
			sizemax--;
			i--;
		}
	}
	*nG5 = sizemax;
	return Gtemp;
};

int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));
	int nG1, nG2, V, v1, v2, m;
	cout << "Vvod nG1: ";
	cin >> nG1;
	int** G1 = createG(nG1);
	printG(G1, nG1);

	cout << "\nVvod nG2: ";
	cin >> nG2;
	int** G2 = createG(nG2);
	printG(G2, nG2);
	cout << "1. Отождествление вершин" << endl << "2. Cтягивание ребра" << endl << "3. Расщипление ребра" << endl << "4. Объединение графов" << endl << "5. Пересечение графов" << endl << "6. Кольцевая сумма" << endl;
	cout << "Введите номер действия";
	cin >> m;
	switch (m)
	{
	case 1:
	{
		cout << endl << "Vvod otozgdestvlyaemoy vetshini: ";
		cin >> V;
		v2 = V;
		v1 = v2 - 1;
		G1 = unionV(G1, nG1, v1, v2);
		nG1 = nG1 - 1;
		cout << endl << "Otoshdestvlenye vetshini: \n";
		printG(G1, nG1);
		if (G1[v1][v2] != 1)
		{
			cout << "rebra ner";
		}
		break;
	}
	case 2:
	{
		cout << endl << "Styag rebra kakogo: ";
		cin >> V;
		cout << "Tyaga rebra: \n";
		v2 = V;
		v1 = v2 - 1;
		G2 = contrE(G2, nG2, v1, v2);
		printG(G2, nG2 - 1);
		break;
	}
	case 3:
	{
		cout << endl << "kakoe rasship vsrshiny: ";
		cin >> V;
		G1 = splitV(G1, nG1, V);
		printG(G1, nG1 + 1);
		break;
	}
	case 4:
	{
		cout << endl << "Obyedenenie: " << endl;
		int** G3 = unionG(G1, G2, nG1, nG2);
		int nG3 = (nG1 > nG2) ? nG1 : nG2;
		printG(G3, nG3);
		break;
	}
	case 5:
	{
		cout << endl << "Peresechenie: " << endl;
		int** G4 = intersectionG(G1, G2, nG1, nG2);
		int nG4 = (nG1 < nG2) ? nG1 : nG2;
		printG(G4, nG4);
		break;
	}
	case 6:
	{
		cout << endl << "Xor: " << endl;
		int nG5;
		int** G5 = xorG(G1, G2, nG1, nG2, &nG5);
		printG(G5, nG5);
		break;
	}
	}
	return 0;
}