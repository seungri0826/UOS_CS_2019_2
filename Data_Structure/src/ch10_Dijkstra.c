#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100
#define INF 1000000	/* ���Ѵ� (������ ���� ���) */

typedef struct GraphType {
	int n;	// ������ ����
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES];
int found[MAX_VERTICES];
int order[MAX_VERTICES];	/* �ִܰ���� ���� ���� ��� */


int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void print_status(GraphType* g)
{
	static int step = 1;
	printf("> STEP %d\n", step++);
	printf(" distance: ");
	for (int i = 0; i < g->n; i++) {
		if (distance[i] == INF)
			printf(" * ");
		else
			printf("%2d ", distance[i]);
	}
	printf("\n");
	printf(" found: ");
	for (int i = 0; i < g->n; i++)
		printf("%2d ", found[i]);
	printf("\n\n");
}

void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	for (i = 0; i < g->n; i++)	/* �ʱ�ȭ */
	{
		distance[i] = g->weight[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE;
	distance[start] = 0;
	order[0] = start;	/* �ִܰ�� ������ ù���ŷ� ���� ������ ���� */
	for (i = 0; i < g->n - 1; i++) {
		print_status(g);
		u = choose(distance, g->n, found);
		found[u] = TRUE;
		order[i + 1] = u; /* �ִܰ�� ������ ���� ������ ���� */
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w])
					distance[w] = distance[u] + g->weight[u][w];
	}
	for (int j = 0; j < g->n; j++) {
		if(found[j] == FALSE)
			order[i] = j; /* �ִܰ�� ������ found[] ���� FALSE�� ����, �� ���� ������ ���� */
	}
}

int main(void)
{
	int i;
	printf("================================\n");
	printf("   Dijkstra �ִܰ�� �˰���\n");
	printf("================================\n\n");

	GraphType g = {8,
	{{ 0, INF, INF, INF, INF, INF, INF, INF },
	{ 300, 0, INF, INF, INF, INF, INF, INF },
	{ 1000, 800, 0, INF, INF, INF, INF, INF },
	{ INF, INF, 1200, 0, INF, INF, INF, INF },
	{ INF, INF, INF, 1500, 0, 250, INF, INF },
	{ INF, INF, INF, 1000, INF, 0, 900, 1400 },
	{ INF, INF, INF, INF, INF, INF, 0, 1000 },
	{ 1700, INF, INF, INF, INF, INF, INF, 0 }} 
	};
	char* name[MAX_VERTICES] = {
		"�ν���������", "�������ý���", "����", "��ī��", "������", "����", "���ֹ̾�", "���ø�����"
	};	/* ������ �̸� */

	shortest_path(&g, 4); /* �����Ͽ��� ��� */

	printf(">> ORDER\n ");	/* �ִܰ�� ������� ���� ��� */
	for (i = 0; i < g.n - 1; i++) {
		printf("%s -> ", name[order[i]]);
	}
	printf("%s\n\n", name[order[i]]);
	return 0;
}