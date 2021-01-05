#include <stdio.h>
#define ROWS 6
#define COLS 6
#define MAX_TERMS 36
/*
* ��� ��� ǥ�� ��� #2�� �̿��� �� �迭�� ���� ���ϴ� ���α׷�
* ��ǻ�Ͱ��к� 2018920031 ���¸�
*
* main(void): �� �迭�� ������ �Է��ϰ� �Լ� ����
* addSparseMatrix(SparseMatrix a, SparseMatrix b): ��� ��� ǥ�� ��� #2�� �̿��Ͽ� �� �迭 a, b�� ���� ���ϴ� �Լ�
* showResult(SparseMatrix m): ��� �迭 m�� 2���� matrix �������� ����ϴ� �Լ�
*/

typedef struct {
	int row;
	int col;
	int value;
} element;

typedef struct {
	element data[MAX_TERMS];
	int rows;	// ���� ����
	int cols;	// ���� ����
	int terms;	// ���� ����
} SparseMatrix;

// ��� ��� ǥ�� ��� #2�� �̿��Ͽ� �� �迭 a, b�� ���� ���ϴ� �Լ�
// c = a + b
SparseMatrix addSparseMatrix(SparseMatrix a, SparseMatrix b)
{
	SparseMatrix c;
	int ca = 0, cb = 0, cc = 0;	// �� �迭�� �׸��� ����Ű�� index

	// �迭 a�� �迭 b�� ũ�Ⱑ ������ Ȯ��
	if (a.rows != b.rows || a.cols != b.cols)
	{
		printf("�� �������� ũ�Ⱑ ���� �ʽ��ϴ�!\n");
		exit(1);
	}

	c.rows = a.rows;
	c.cols = a.cols;
	c.terms = 0;

	// �迭 a�� �迭 b�� ���� �������� index�� ���� ��
	while (ca < a.terms && cb < b.terms)
	{
		// �� �׸��� �������� ��ȣ�� ���
		int inda = a.data[ca].row*a.cols + a.data[ca].col;
		int indb = b.data[cb].row*b.cols + b.data[cb].col;

		// �迭 a �׸��� �迭 b �׸񺸴� �տ� �ִ� ���
		if (inda < indb)
		{
			c.data[cc++] = a.data[ca++];
		}
		// �迭 a �׸�� �迭 b �׸��� ���� ��ġ�� �ִ� ���
		else if (inda == indb)
		{
			// �迭 a �׸�� �迭 b �׸��� ���� ���� 0�� �ƴ� ���
			if ((a.data[ca].value + b.data[cb].value) != 0)
			{
				c.data[cc].row = a.data[ca].row;
				c.data[cc].col = a.data[ca].col;
				c.data[cc++].value = a.data[ca++].value + b.data[cb++].value;
			}
			else
			{
				ca++;
				cb++;
			}
		}
		// �迭 b �׸��� �迭 a �׸񺸴� �տ� �ִ� ���
		else
			c.data[cc++] = b.data[cb++];
	}

	// �迭 a�� �迭 b�� ���� �ִ� �׵��� �迭 c�� ����
	for (; ca < a.terms;)
		c.data[cc++] = a.data[ca++];
	for (; cb < b.terms; )
		c.data[cc++] = b.data[cb++];
	c.terms = cc;

	return c;
}

// ��� �迭 m�� 2���� matrix �������� ����ϴ� �Լ�
void showResult(SparseMatrix m)
{
	int indm = 0;
	int i, j, k;

	printf(" > �� Matrix�� ���� ������ �����ϴ�.\n");
	printf("  �� ");
	for (k = 0; k < m.cols; k++)
		printf("   ");
	printf("��\n");

	for (i = 0; i < m.rows; i++)
	{
		for (j = 0; j < m.cols; j++)
		{
			if (i != 0 || i != m.rows - 1)
			{
				if (j == 0)
					printf("  �� ");
			}
			
			if (i == m.data[indm].row && j == m.data[indm].col)
			{
				printf("%2d ", m.data[indm++].value);
			}
			else
				printf(" 0 ");
		}
		printf("��\n");
	}

	printf("  �� ");
	for (k = 0; k < m.cols; k++)
		printf("   ");
	printf("��\n");
}

// main �Լ�
int main(void)
{
	// �� �迭�� �� �Է�
	SparseMatrix m1 = { {{0,0,1}, {1,1,1}, {2,1,1}, {3,2,1}, {4,3,1}, {5,1,2}, {5,3,-1}}, 6,6,7 };
	SparseMatrix m2 = { {{0,0,1}, {1,0,1}, {2,1,1}, {3,2,1}, {4,0,1}, {5,0,1}}, 6,6,6 };

	SparseMatrix m3;
	m3 = addSparseMatrix(m1, m2);
	showResult(m3);

	return 0;
}