#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * input ������ n�� 1-layer perceptron�� �����ϴ� ���α׷�
 * ��ǻ�Ͱ��к� 2018920031 ���¸�
 *
 * x[n]: int
 * w[n]: float (�ʱ� ��: 0.0 <= w[n] < 100.0)
 * theta: float (�ʱ� ��: 10.0 <= theta < 50.0)
 */

void calculation(int xRow, int xCol, float * w, int ** x, int * out, float theta); 
void showResult(int xRow, int xCol, float * w, int ** x, int * out, float theta, int * pw); 
void modifyValue(int xCol, float * w, float * pt);

int main()
{
	int input; // input�� ���� (n)
	printf("*******************************************************\n");
	printf("*******  n-input AND gate (1-layer perceptron)  *******\n");
	printf("*******************************************************\n");
	printf("\n\n  > the number of inputs : ");
	scanf("%d", &input); // ����ڷκ��� input�� ���� �Է� �޾� ����
	printf("\n");
	
	int i, j;
	int xRow = 1 << input; // == 2^n
	int xCol = input; // == n

	int ** x; // input�� x[2^n][n], int�� 2���� �迭
	float * w; // weight�� w[n], float�� 1���� �迭
	int * out; // output�� out[2^n], int�� 1���� �迭

	int num;
	int wrong = 0; // Ʋ�� output�� ����
	int * pw = &wrong;
	int cnt = 0; // �´� output�� ���� ������ ���� ������ Ƚ�� 

	srand(time(NULL)); // �ߺ� ���� ���� ������ ���� ���� �ð��� seed�� ����
	float theta = (float)(rand() % 400) / 10 + 10; // �������� �����Ǵ� �ʱ� theta ��: 10.0 <= theta < 50.0
	float * pt = &theta;

	w = (float *)calloc(input, sizeof(float)); // w[n]�� �����ϱ� ���� ���� �Ҵ� (calloc)
	for (i = 0; i < input; i++)
	{
		w[i] = (float)(rand() % 1000) / 10; // �������� �����Ǵ� �ʱ� w[n] ��: 0.0 <= w[n] < 100.0
	}
		
	out = (int *)calloc(xRow, sizeof(int)); // out[2^n]�� �����ϱ� ���� ���� �Ҵ� (calloc)

	x = (int **)calloc(xRow, sizeof(int*)); // x[2^n][n]�� �����ϱ� ���� ���� �Ҵ� (calloc)
	for (i = 0; i < xRow; ++i)
	{
		x[i] = (int *)calloc(xCol, sizeof(int));
	}

	// 10���� -> 2���� ��ȯ�� �̿��Ͽ� x[2^n][n]�� input ���� ����
	for (i = 0; i < xRow; i++)
	{
		num = i;
		for (j = 0; j < xCol; j++)
		{
			x[i][j] = num % 2;
			num = num / 2;

			if (num == 0)
				break;
		}
	}

	// �´� output�� ���� ������ ���α׷� ��� ���� �ݺ�
	while (1)
	{
		calculation(xRow, xCol, w, x, out, theta);
		showResult(xRow, xCol, w, x, out, theta, pw);

		if (wrong == 0) // �� input �� output�� ��� ������ ���α׷� ����, �� �õ� Ƚ�� ���
		{
			printf("\n\n\n***********************************************\n");
			printf("*******  We've got a correct AND gate!  *******\n");
			printf("***********************************************\n");
			printf("\n  > the number of attempts : %d\n\n\n", cnt);
			break;
		}
		else
			modifyValue(xCol, w, pt); // w[n], theta �� ����

		cnt++; // �´� output�� ���� ������ ���� ������ Ƚ�� 1 ����
		wrong = 0; // Ʋ�� output�� ���� �ʱ�ȭ
	}
	
	// ���� �Ҵ��� �޸� ��ȯ
	for (i = 0; i < xRow; i++)
		free(x[i]);
	free(x);
	free(w);
	free(out);

	return 0;
}

// output = f(net)�� ���ϴ� �Լ�
void calculation(int xRow, int xCol, float * w, int ** x, int * out, float theta)
{
	int i, j;
	float mul, sum = 0; // sum == net
	
	for (i = 0; i < xRow; i++)
	{
		// net �� ����, index�� ���� x�� w�� ���� ������ ����
		for (j = 0; j < xCol; j++)
		{
			mul = w[j] * x[i][j];
			sum += mul;
		}

		// net ���� theta ������ �񱳿� ���� f(net) ���� 
		if (sum > theta)
			out[i] = 1;
		else
			out[i] = 0;
		
		sum = 0;
	}
}

// ���� ��� ǥ, Ʋ�� output ����, ���� w[n], theta�� ���� �����ִ� �Լ�
void showResult(int xRow, int xCol, float * w, int ** x, int * out, float theta, int * pw)
{
	int i, j;
	
	// ����� ǥ �������� ���
	printf("\n-------------------  RESULT  -------------------\n\n");
	for (i = xCol; i > 0; i--)
		printf("   X%d  |", i);
	printf("|  out\n");

	for (i = 0; i < xCol + 1; i++)
		printf("--------");
	printf("\n");

	for (i = 0; i < xRow; ++i)
	{
		for (j = xCol - 1; j >= 0; j--)
			printf("   %d   |", x[i][j]);
		printf("|   %d\n", out[i]);
	}

	// out[xRow-1]�� ������ output�� 0�� �ƴ� ������ Ʋ�� output�� ���� 1 ����
	for (i = 0; i < xRow-1; i++)
	{
		if (out[i] != 0)
			(*pw)++;
	}
	// out[xRow-1]�� 1�� �ƴ� �� Ʋ�� output�� ���� 1 ����
	if (out[xRow - 1] != 1)
		(*pw)++;

	printf("\n  > the number of wrong outputs : %d\n", *pw); // �̹� attempt���� Ʋ�� output�� ���� ���

	// ���� w[n], theta�� ���� �Ҽ��� �Ʒ� ù° �ڸ����� ���
	printf("\n  > the values of variables : \n\n"); 
	for (i = 0; i < xCol; i++)
		printf("    (%d) W%d = %.1f\n", i + 1, i + 1, w[i]);
	printf("    (%d) theta = %.1f\n", i + 1, theta);
}

// �Ǽ� ���� ����ڷκ��� �Է� �޾� w[n], theta ���� �����ϴ� �Լ�
void modifyValue(int xCol, float * w, float * pt)
{
	int i;
	int num;
	float mod; // ����ڷκ��� ������ ���� �Է� �޴� float�� ����

	printf("\n\n\n\n================  MODIFICATION  ================\n\n");
	for (i = 0; i < xCol; i++)
		printf("    (%d) W%d\n", i + 1, i + 1);
	printf("    (%d) theta\n", i + 1);

	// w[n], theta �� � ������ ���� ������ �������� ����ڷκ��� �Է� �޴� ����
	printf("\n  > Choose one variable to modify : ");
	scanf("%d", &num);
	printf("\n  > Enter the modified value (ex. 000.0) : "); // float�� �����̹Ƿ� ������ �ƴ� �Ǽ��� �Է� ����
	scanf("%f", &mod);

	// ����ڰ� theta ���� �����ϴ� ���
	if (num == i + 1)
		*pt = mod;

	// ����ڰ� w[n] ���� �����ϴ� ���
	for (i = 0; i < xCol; i++)
	{
		if (num == i + 1)
			w[i] = mod;
	}

	// ����ڰ� �Ǽ��� num ������ �������� ��� ���� �Է��� ��� �ٽ� �ݺ�
	if (num < 1 || num > xCol + 1)
	{
		printf("\n\n  ** Out of range! Please enter a number among the indexes above (");
		for (i = 1; i <= xCol; i++)
			printf("%d, ", i);
		printf("%d). **\n", i);
		modifyValue(xCol, w, pt);
	}
}