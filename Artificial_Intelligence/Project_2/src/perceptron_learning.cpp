#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <map>

#define INPUT_SIZE 2		// input�� ���� = 2
#define EPOCH_MAX 100000	// �н� Ƚ��
#define LEARNING_RATE 0.1	// �н���

using namespace std;

// �н� ������ : { {input ����}, target } ����
// AND gate �н� ������ ����
vector<pair<vector<double>, double>> target_AND = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 0 },
{ { 1, 0 }, 0 },
{ { 1, 1 }, 1 }
};
// OR gate �н� ������ ����
vector<pair<vector<double>, double>> target_OR = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 1 },
{ { 1, 0 }, 1 },
{ { 1, 1 }, 1 }
};
// XOR gate �н� ������ ����
vector<pair<vector<double>, double>> target_XOR = {
	{ { 0, 0 }, 0 },
{ { 0, 1 }, 1 },
{ { 1, 0 }, 1 },
{ { 1, 1 }, 0 }
};

// Sigmoid �Լ�
double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

class Neuron
{
private:
	vector<double> weight;
	double bias;

	// Initial Value(�ʱⰪ) ����
	void setValue()
	{
		bias = -1;	// bias�� �ʱⰪ�� ���ǻ� -1�� ����
		
		random_device rd;
		mt19937 random(rd());	// ���� ����
		uniform_real_distribution<double> distr(-1.0, 1.0);	// �����Ǵ� ������ -1.0 �̻� 1.0 �̸��� �Ǽ�

		for (size_t i = 0; i < weight.size(); i++)
		{
			weight[i] = distr(random);	// weight vector�� ���� ����
		}
		
		 //weight[0] = -0.577339;	// ���� �ۼ� �� �ӽ÷� ������ ��
		 //weight[1] = 0.744271;	// ���� �ۼ� �� �ӽ÷� ������ ��
	}

public:
	// ������
	Neuron(int input_size)
	{
		weight.resize(input_size); // weight.size() == INPUT_SIZE
		setValue();
	}

	// Sigmoid �Լ��� ���� ����� output�� ��ȯ�ϴ� �Լ�
	double Calculate(const vector<double> & input)
	{
		double output, wx = 0.0;

		for (size_t i = 0; i < weight.size(); i++)
		{
			wx += weight[i] * input[i];	// wx�� ��
		}
		
		output = Sigmoid(bias + wx);
		return output;
	}

	// output�� target�� ���Ͽ� �н���Ű�� �Լ�
	// c�� �н���(LEARNING_RATE), target�� �н� �����Ϳ��� input �������� ������ ���� vector
	void Learn(double c, const vector<pair<vector<double>, double>> & target)
	{
		int input_size = target[0].first.size();

		for (size_t i = 0; i < target.size(); i++)
		{
			double o = Calculate(target[i].first); // ���� ���� output
			double t = target[i].second; // �н� �����Ϳ� ������ target

			for (int j = 0; j < input_size; j++)
			{
				weight[j] += c * (t - o) * target[i].first[j];	// weight ���� ����
			}

			bias += c * (t - o) * 1;	// bias ���� ����
		}
	}

	// private ����� weight�� getter
	vector<double> & getWeight()
	{
		return weight;
	}

	// private ����� bias�� getter
	double getBias()
	{
		return bias;
	}
};

// Neuron�� �н���Ű�� �� ����� ����ϴ� �Լ�
void showResult(Neuron neuron, const vector<pair<vector<double>, double>> & target, string gateName)
{
	int i, j, k;
	double gradient, intercept; // ����, y ����
	double error, total_error = 0.0; // error = 0.5 * pow((t - o), 2.0), total_error = error�� ��

	cout << "\n----------------------------------------------------------------\n";
	cout << "=========================== " << gateName << " gate ===========================\n";
	cout << "----------------------------------------------------------------\n";

	// Initial Value ���
	cout << "[Initial Value] ";
	for (i = 0; i < INPUT_SIZE; i++)
	{
		cout << "W" << i + 1 << " = " << neuron.getWeight()[i] << ", ";
	}
	cout << "bias = " << neuron.getBias() << '\n';

	// (INPUT_SIZE == 2)�� ��, ������ ���� �� y ���� ���
	gradient = -(neuron.getWeight()[0] / neuron.getWeight()[1]);
	intercept = -(neuron.getBias() / neuron.getWeight()[1]);
	cout << "  => gradient = " << gradient << ", intercept = " << intercept << '\n';

	// ��� ���
	for (j = 0; j < pow(2.0, INPUT_SIZE); j++)
	{
		for (k = 0; k < INPUT_SIZE - 1; k++)
		{
			cout << target[j].first[k] << " " << gateName << " ";
		}
		cout << target[j].first[k] << " => " << neuron.Calculate(target[j].first);
		error = 0.5 * pow((target[j].second - neuron.Calculate(target[j].first)), 2.0);
		total_error += error;
		cout << "        // error = " << error << '\n';
	}
	cout << "\t\t\t // total error = " << total_error << '\n';
	cout << "----------------------------------------------------------------\n";

	// �н�
	for (i = 1; i <= EPOCH_MAX; i++)
	{
		error = 0.0;
		total_error = 0.0;
		neuron.Learn(LEARNING_RATE, target);

		if (i == 5 || i == 10 || i == 20 || i == 35 || i == 50 || i == 100 || i == 500 || i == 1000 || i == 5000 || i == 10000 || (i % 50000) == 0)
		{
			cout << "[Epoch " << i << "] ";
			for (j = 0; j < INPUT_SIZE; j++)
			{
				cout << "W" << j + 1 << " = " << neuron.getWeight()[j] << ", ";
			}
			cout << "bias = " << neuron.getBias() << '\n';

			// (INPUT_SIZE == 2)�� ��, ������ ���� �� y ���� ���
			gradient = -(neuron.getWeight()[0] / neuron.getWeight()[1]);
			intercept = -(neuron.getBias() / neuron.getWeight()[1]);
			cout << "  => gradient = " << gradient << ", intercept = " << intercept << '\n';

			// ��� ���
			for (j = 0; j < pow(2.0, INPUT_SIZE); j++)
			{
				for (k = 0; k < INPUT_SIZE - 1; k++)
				{
					cout << target[j].first[k] << " " << gateName << " ";
				}
				cout << target[j].first[k] << " => " << neuron.Calculate(target[j].first);
				error = 0.5*pow((target[j].second - neuron.Calculate(target[j].first)), 2.0);
				cout << "        // error = " << error << '\n';
				total_error += error;
			}
			cout << "\t\t\t // total error = " << total_error << '\n';
			cout << "----------------------------------------------------------------\n";
		}
	}
}

int main(void)
{
	//AND gate
	Neuron neuron_AND(INPUT_SIZE);
	showResult(neuron_AND, target_AND, "AND");

	//OR gate
	Neuron neuron_OR(INPUT_SIZE);
	showResult(neuron_OR, target_OR, "OR");

	//XOR gate
	Neuron neuron_XOR(INPUT_SIZE);
	showResult(neuron_XOR, target_XOR, "XOR");

	return 0;
}