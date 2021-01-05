#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <map>
#include <utility>
#include <Windows.h>

#define INPUT_SIZE 2	// input�� ���� = 2
#define LEARNING_RATE 0.2f	// �н���
#define TOLERANCE 0.05f // ��� (|t - o| <= TOLERANCE)�� �� �н� ����

using namespace std;

// AND, OR, XOR gate �н� ������
vector<vector<float>> train_gate_x = {
	{0,0},{0,1},{1,0},{1,1}
};
vector<float> train_gate_AND_y = { 0,0,0,1 };
vector<float> train_gate_OR_y = { 0,1,1,1 };
vector<float> train_gate_XOR_y = { 0,1,1,0 };

// ����� ��� ������
vector<vector<float>> train_DOUGHNUT_x = {
	{0,0}, {0,1}, {1,0}, {1,1}, {0.5,1}, {1, 0.5}, {0, 0.5}, {0.5, 0}, {0.5, 0.5} 
};
vector<float> train_DOUGHNUT_y = { 0,0,0,0,0,0,0,0,1 };

// Sigmoid �Լ�
float Sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}

// Sigmoid �Լ��� ���Լ�
float Sigmoid_D(float x)
{
	float y = Sigmoid(x);
	return y * (1 - y);
}

class Neuron
{
private:
	vector<float> input_this; // �ش� Neuron�� input
	vector<float> weight_this; // �ش� Neuron�� weight
	vector<float> weight_next; // Back Propagation �������� �ʿ��� ������ next layer�� weight
	float bias;
	float net;
	float delta_this; // �ش� Neuron�� delta

	// Initial Value(�ʱⰪ) ����
	void setValue()
	{
		bias = -1;	// bias�� �ʱⰪ�� -1�� ����
		
		random_device rd;
		mt19937 random(rd());	// ���� ����
		uniform_real_distribution<float> distr(-1.0, 1.0);	// �����Ǵ� ������ -1.0 �̻� 1.0 �̸��� �Ǽ�

		for (size_t i = 0; i < weight_this.size(); i++)
		{
			weight_this[i] = distr(random);	// weight vector�� ���� ����
		}
	}

public:
	// ������
	Neuron(size_t input_size)
	{
		weight_this.resize(input_size); // weight.size() == INPUT_SIZE
		input_this.resize(input_size);
		setValue();
	}

	// Sigmoid �Լ��� ���� ����� output�� ��ȯ�ϴ� �Լ�
	float Forward(const vector<float> & input)	
	{
		float wx = 0.0;

		for (size_t i = 0; i < weight_this.size(); i++)
		{
			wx += weight_this[i] * input[i];	// wx�� ��
		}

		input_this = input;
		net = bias + wx;
		return Sigmoid(net); // = output
	}

	// Back Propagation ���������� ������ ��Ÿ�� �Լ�
	void Backward(float c, float error, const vector<float> & input_this) 
	{
		delta_this = Sigmoid_D(net) * error; // delta�� ����

		for (size_t i = 0; i < input_this.size(); i++)
		{
			weight_this[i] += c * delta_this * input_this[i]; // weight �� ����
		}
		bias += c * delta_this * 1; // bias �� ����
	}

	// vector w�� �μ��� �޾� weight_next�� �����ϴ� �Լ�
	void setWeight_next(vector<float> w)
	{
		weight_next.clear();
		for (size_t i = 0; i < w.size(); i++)
		{
			weight_next.push_back(w[i]);
		}
	}

	vector<float> & getWeight_next()
	{
		return weight_next;
	}

	int getInputSize()
	{
		return weight_this.size();
	}

	float getNet()
	{
		return net;
	}

	vector<float> & getInput()
	{
		return input_this;
	}

	float getDelta()
	{
		return delta_this;
	}

	vector<float> & getWeight_this()
	{
		return weight_this;
	}

	float & getBias()
	{
		return bias;
	}
};

class Network
{
private:
	vector<vector<Neuron>> layers; // ��� layer�� ���� vector
	vector<float> output_this; // �� layer�� output�� ���� vector
	vector<float> delta_next; // �� layer�� ���� layer�� delta�� ���� vector
	vector<size_t> setting_all; // �������� �μ��� ���� vector
	size_t layer_num; // layer ����
	float error_o, error_h; // error_o = (train_y) - (Forward �� output)

public:
	// ������, �μ��� �־��� setting vector�� ���� �Ű���� ������
	Network(const vector<size_t> & setting) 
	{
		layer_num = setting.size() - 1; 
		for (size_t i = 0; i < setting.size(); i++)
		{
			setting_all.push_back(setting[i]);
		}

		for (size_t i = 1; i <= layer_num; i++)
		{
			vector<Neuron> layer;
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				layer.push_back(Neuron(setting_all[i - 1]));
			}
			layers.push_back(layer);
		}
	}

	// input layer ~ output layer ���� �� �� �����ϴ� �Լ�
	vector<float> Forward(const vector<float> & input_this)
	{
		vector<float> input_tmp;
		for (size_t i = 0; i < input_this.size(); i++)
		{
			input_tmp.push_back(input_this[i]);
		}

		for (size_t i = 1; i <= layer_num; i++) /// layer ������ŭ �ݺ�
		{
			output_this.clear();
			for (size_t j = 0; j < setting_all[i]; j++) /// setting_all[i] = this layer�� neuron ����
			{
				output_this.push_back(layers[i - 1][j].Forward(input_tmp)); /// layers[i - 1] = this layer
			}

			input_tmp.clear();
			for (size_t j = 0; j < output_this.size(); j++)
			{
				input_tmp.push_back(output_this[j]);
			}
		}

		// weight_next�� ������
		for (size_t i = layer_num - 1; i >= 1; i--) /// 2��° layer���� (i=2)
		{
			// next layer�� �� neuron ������ weight�� weight_next�� �ӽ� ����
			vector<vector<float>> weight_next;
			weight_next.clear();
			for (size_t j = 0; j < setting_all[i + 1]; j++) /// setting_all[i + 1] = next layer�� neuron ����
			{
				weight_next.push_back(layers[i][j].getWeight_this()); /// layers[i] = next layer
			}

			// weight_next�� transpose�Ͽ� this layer�� �� neuron�� weight_next�� ����
			for (size_t k = 0; k < setting_all[i]; k++)	/// setting_all[i] = this layer�� neuron ����
			{
				vector<float> weight_next_trans;
				for (size_t j = 0; j < setting_all[i + 1]; j++) /// setting_all[i + 1] = next layer�� neuron ����
				{
					weight_next_trans.push_back(weight_next[j][k]);
				}
				layers[i - 1][k].setWeight_next(weight_next_trans); /// layers[i - 1] = this layer
			}
		}

		return output_this;
	}

	// ��ü input �����Ϳ� ���ؼ� �� �� Back Propagation �����ϴ� �Լ�
	void Backward(float c, const vector<vector<float>> & train_x, const vector<float> & train_y)
	{
		// ��ü input �����Ϳ� ���� ����
		for (size_t n = 0; n < train_x.size(); n++)
		{
			// output layer�� ����
			float o = Forward(train_x[n])[0]; // Forward ����
			error_o = train_y[n] - o; // output layer�� error
			layers[layer_num - 1][0].Backward(c, error_o, layers[layer_num - 1][0].getInput()); 
			/// layers[layer_num - 1] = this layer = output layer

			// hidden layer & input layer�� ����
			for (size_t i = layer_num - 1; i >= 1; i--) /// layers[layer_num - 1] = this layer = output layer
			{
				delta_next.clear();
				for (size_t j = 0; j < layers[i].size(); j++) /// layers[i] = next layer
				{
					delta_next.push_back(layers[i][j].getDelta()); /// layers[i] = next layer
				}

				for (size_t j = 0; j < layers[i - 1].size(); j++) /// layers[i - 1] = this layer
				{
					error_h = 0.0f; // hidden layer & input layer�� error
					for (size_t k = 0; k < layers[i].size(); k++) /// layers[i] = next layer
					{
						error_h += delta_next[k] * layers[i - 1][j].getWeight_next()[k]; /// layers[i - 1] = this layer
					}
					layers[i - 1][j].Backward(c, error_h, layers[i - 1][j].getInput()); /// layers[i - 1] = this layer
				}
			}
		}
	}

	vector<float> & getOutput_this()
	{
		return output_this;
	}

	size_t getLayer_num()
	{
		return layer_num;
	}

	vector <size_t> getSetting_all()
	{
		return setting_all;
	}

	vector<vector<Neuron>> getLayers()
	{
		return layers;
	}
};

// Console â�� ����� ����ϴ� �Լ�
// : Epoch ��, input & output ǥ, Total Error ��, �� layer�� neuron �� weight & bias ǥ
void printResult(Network net, vector<vector<float>> & train_x, vector<float> & train_y, int epoch, float error_total)
{
	size_t input_size = train_x[0].size();
	size_t train_size = train_x.size();
	size_t layer_num = net.getLayer_num();
	vector<size_t> setting_all = net.getSetting_all();
	vector<vector<float>> input_output_layer; // output layer�� input�� ������

	cout << " [Epoch " << epoch << "]" << "\n\n"; // ���� Epoch �� ���

	// input & output ǥ
	cout << " ";
	for (size_t i = 0; i < input_size; i++)
	{
		cout << "  X" << i + 1 << " |";
	}
	cout << "   OUTPUT   " << endl;
	cout << " ";
	for (size_t i = 0; i < input_size; i++)
	{
		cout << "-----|";
	}
	cout << "------------" << endl;
	for (size_t i = 0; i < train_size; i++)
	{
		cout << " ";
		for (size_t j = 0; j < input_size; j++)
		{
			cout.width(4);
			cout << train_x[i][j] << " |";
		}
		cout << " ";
		cout.width(9);
		cout << net.Forward(train_x[i])[0] << endl;
		input_output_layer.push_back((net.getLayers()[layer_num - 1][0]).getInput()); // output layer�� input�� ������
	}

	// Total Error �� ���
	cout << "\n => Total Error = " << error_total << "\n\n";

	// �� layer�� neuron �� weight & bias ǥ
	for (size_t i = 0; i < layer_num; i++)
	{
		cout << "   (Layer #" << i + 1 << ")" << endl;
		cout << "               ";
		for (size_t j = 0; j < setting_all[i]; j++)
		{
			cout << "|     W" << j + 1 << "     ";
		}
		cout << "|    Bias    ";
		cout << "\n    -----------";
		for (size_t j = 0; j < setting_all[i] + 1; j++)
		{
			cout << "|------------";
		}
		cout << endl;

		for (size_t j = 0; j < setting_all[i + 1]; j++)
		{
			cout << "     Neuron #" << j + 1 << " |";
			for (size_t k = 0; k < setting_all[i]; k++)
			{
				cout.width(11);
				cout << net.getLayers()[i][j].getWeight_this()[k] << " |";
			}
			cout.width(11);
			cout << net.getLayers()[i][j].getBias() << endl;
		}
		
		// output layer�� ���, train_x�� ���� ������ ������ input ���� ���� ��� (�׷��� �׸��� ����)
		if (i == layer_num - 1)
		{
			cout << "\n               |";
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				cout<< "     X" << j+1 << "     |";
			}
			cout << "\n               |";
			for (size_t j = 0; j < setting_all[i]; j++)
			{
				cout << "------------|";
			}
			cout << "\n";
			for (size_t j = 0; j < train_size; j++)
			{
				cout << "               |";
				for (size_t k = 0; k < setting_all[i]; k++)
				{
					cout.width(11);
					cout << input_output_layer[j][k] << " |";
				}
				cout << "\n";
			}
		}
		cout << endl;
	}
	cout << "----------------------------------------------------------------" << endl;
}

// ��� (|t - o| <= TOLERANCE)�� ������ �н��� �ݺ��ϰ�, ������ Epoch���� Console â�� ����� ����ϴ� �Լ�
void trainResult(Network net, vector<vector<float>> & train_x, vector<float> & train_y, string dataName)
{
	size_t input_size = train_x[0].size(); // == INPUT_SIZE
	size_t train_size = train_x.size(); // == train_y.size()
	int epoch = 0;
	vector<float> error; // |t - o| ���� �����ϴ� vector
	float error_total;
	bool STOP;

	size_t layer_num = net.getLayer_num(); // layer ����
	vector<size_t> setting_all = net.getSetting_all(); // Network �������� �μ��� �Է� ���� setting vector

	cout << "\n----------------------------------------------------------------\n";
	cout << "=========================== " << dataName << " ===========================\n";
	cout << "----------------------------------------------------------------\n";

	do 
	{
		STOP = true;
		error_total = 0.0f;
		error.clear();
		net.Backward(LEARNING_RATE, train_x, train_y); // �н� 1ȸ ����
		
		for (size_t i = 0; i < train_size; i++)
		{
			error.push_back(abs(train_y[i] - net.Forward(train_x[i])[0])); // |t - o| ���� error vector�� ����
			STOP = STOP && (error[i] <= TOLERANCE); 
			// ��� error ��(|train_y[i] - net.Forward(train_x[i])[0]|)�� TOLERANCE ���� �۰ų� ���ƾ� �н� ����
			error_total += 0.5f * pow(error[i], 2); // MSE ���� ��, MSE = 0.5 * error[i] ^ 2
		}

		// Console â ���
		if (epoch <= 1000)
		{
			if (epoch == 0 || epoch == 5 || epoch == 10 || epoch == 20 || epoch == 35 || epoch == 50 ||
				epoch == 100 || (epoch % 200) == 0)
			{
				printResult(net, train_x, train_y, epoch, error_total);
				cout << endl;
			}
		}
		else if ((epoch < 10000) && (epoch % 2000) == 0)
		{
			printResult(net, train_x, train_y, epoch, error_total);
			cout << endl;
		}
		else if ((epoch % 10000) == 0)
		{
			printResult(net, train_x, train_y, epoch, error_total);
			cout << endl;
		}

		epoch++; // epoch 1 ����
	} while (!STOP);

	// �н� �Ϸ� �� Console â ���
	printResult(net, train_x, train_y, epoch, error_total);
	cout << "*** �н��� �Ϸ�Ǿ����ϴ�. ***\n\n";
}

int main(void)
{
	size_t data_num; // ���õ� �������� ��ȣ
	size_t layer_num; // ������ layer�� ����
	vector<size_t> setting_user; // ������ setting vector

	// ����ڷκ��� �н��� ����ϴ� �������� ����, layer�� ����, �� layer�� neuron ������ �Է� ����
	cout << "\n > �н��� ����ϴ� �н� �������� ������ �����ϼ���. (input�� ũ��� " << INPUT_SIZE << "�Դϴ�.)" << endl;
	cout << "   (1) AND gate\n" << "   (2) OR gate\n" << "   (3) XOR gate\n" << "   (4) Doughnut\n" << "   => ";
	cin >> data_num;
	cout << "\n > Layer�� ������ �����ϼ���.\n   => ";
	cin >> layer_num;
	setting_user.push_back(INPUT_SIZE);
	cout << "\n ********* �����ϴ� �� Layer �� Neuron ���� *********" << endl;
	cout << "  - Layer 3���� ���: {3, 4, 1} �Ǵ� {4, 2, 1}" << endl;
	cout << "  - Layer 4���� ���: {3, 4, 2, 1}" << endl;
	cout << " ****************************************************" << endl;
	for (size_t i = 1; i < layer_num; i++)
	{
		size_t tmp;
		cout << "\n > " << i << " ��° Layer�� Neuron ������ �����ϼ���.\n   => ";
		cin >> tmp;
		setting_user.push_back(tmp);
	}
	cout << "\n > " << layer_num << " ��° Layer(Output Layer)�� Neuron ������ 1�� �����մϴ�.\n\n"; // output�� 1������ �ϱ� ������
	setting_user.push_back(1);
	Sleep(2000); // delay �ο�
	
	// ����ڰ� ������ �н� �������� ������ ���� �н� ��� ���
	if (data_num == 1)
	{
		Network net_AND(setting_user);
		trainResult(net_AND, train_gate_x, train_gate_AND_y, "AND gate");
	}
	else if (data_num == 2)
	{
		Network net_OR(setting_user);
		trainResult(net_OR, train_gate_x, train_gate_OR_y, "OR gate ");
	}
	else if (data_num == 3)
	{
		Network net_XOR(setting_user);
		trainResult(net_XOR, train_gate_x, train_gate_XOR_y, "XOR gate");
	}
	else if (data_num == 4)
	{
		Network net_DOUGHNUT(setting_user);
		trainResult(net_DOUGHNUT, train_DOUGHNUT_x, train_DOUGHNUT_y, "DOUGHNUT");
	}
	
	return 0;
}