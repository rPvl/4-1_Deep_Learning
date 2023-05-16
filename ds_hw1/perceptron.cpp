#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>//��������
#include<ctime>//����ð����ؽõ����
#pragma warning(disable:4996)
using namespace std;

int iteration = 1000; //�ݺ� Ƚ��
int trainNum = 0;//train data ����
int testNum = 0;//test data ����
int class1Num = 0; //class1 �� ����
int TESTcorrect0 = 0; //test_class0���� �� �з��� ����
int TESTcorrect1 = 0; //test_class1�� �� �з��� ����
int TRAINfaile = 0; //train_class �߸� �з��� ����
// wx - bias = 0
double weigth[4] = { 0 }; //w, random initialize�ؾ���. bias ������ �� ���� �� �ø�.

double arr[10000][5] = { 0 }; //x, train data                 �迭 �Ȱ����ž�����, �Լ� ������ ������ ũ�� �÷��� ��.
double test[10000][5] = { 0 }; //test data

//int classification1[10000] = { 0 };//input : index, ������ ���� Ȯ���� �Ÿ� �ֱ�
//int classification0[10000] = { 0 };

void perceptron();
void predict();
void EvaluationBeforetrain1();

void inpu1(ifstream& train1, ifstream& test1) {//���� �б�
	char str;
	for (int i = 0; i < trainNum; i++) {
		for (int j = 0; j < 3; j++) {
			train1 >> arr[i][j] >> str;
		}
		arr[i][3] = 1;
		train1 >> arr[i][4];
	}
	
	for (int i = 0; i < testNum; i++) {
		for (int j = 0; j < 3; j++) {
			test1 >> test[i][j] >> str;
			//if (j == 3 && test[i][j] == 1) class1Num++;
		}
		test[i][3] = 1;
		test1 >> test[i][4];
		if (test[i][4] == 1) class1Num++;
	
	}
}

void inpu2(ifstream& train2, ifstream& test2) {//���� �б�
	char str[105];
	char str2;
	double a;
	string str1;
	train2 >> str1;// ó�� ���� ����
	for (int i = 0; i < trainNum; i++) {
		train2.getline(str, 100, ',');//number
		//cout << "�Է� " << str << endl;
		train2.getline(str, 100, ',');//date
		train2 >> arr[i][0];//temperature
		train2 >> str2;
		train2 >> a >> str2;
		train2 >> arr[i][1];//light
		train2 >> str2;
		train2 >> arr[i][2];//co2
		train2 >> str2;
		train2 >> a >> str2;
		arr[i][3] = 1;//bias ������ ���� �ϳ� �ø�
		train2 >> arr[i][4];//class

		/*
		for (int j = 0; j < 5; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
		*/
	}

	test2 >> str1>>str2;// ó�� ���� ����
	for (int i = 0; i < testNum; i++) {
		test2.getline(str, 100, ',');//number
		//cout << "�Է� " << str << endl;
		test2.getline(str, 100, ',');//date
		test2 >> test[i][0];//temperature
		test2 >> str2;
		test2 >> a >> str2;
		test2 >> test[i][1];//light
		test2 >> str2;
		test2 >> test[i][2];//co2
		test2 >> str2;
		test2 >> a >> str2;
		test[i][3] = 1;//bias ������ ���� �ϳ� �ø�
		test2 >> test[i][4];//class
		if (test[i][4] == 1) class1Num++;
		/*
		for (int j = 0; j < 5; j++) {
			cout << test[i][j] << " ";
		}
		cout << endl;
		*/
	}
}

int main() {
	srand((unsigned int)time(NULL)); //seed ������ ����ð� �ο�

	cout << "-----------------------------Synthetic dataset-----------------------------" << "\n";
	//file �Է�
	ifstream train1,test1;
	train1.open("synthetic_data_train.txt");//���ϸ� ����.
	test1.open("synthetic_data_test.txt");
	trainNum = 2000;//������ ������ �°� �Է�
	testNum = 400;//������ ������ �°� �Է�
	inpu1(train1, test1);//�����б�

	//w�� ���ؼ� random initialize
	weigth[0] = rand() % 100;
	weigth[1] = rand() % 100;
	weigth[2] = rand() % 100;
	weigth[3] = rand() % 100;
	weigth[3] *= -1;

	//for (int i = 0; i < 4; i++)cout <<"weigthr : "<< weigth[i] << " ";
	//cout << endl;

	//train���� test data�� ���� ���� Ȯ��
	predict();
	cout << "train���� test data�� ���� ���� Ȯ��" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0�ε� class1�� �� �� �з��� ��� : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1�ε� class0�� �� �� �з��� ��� : " << class1Num - TESTcorrect1 << endl << endl;

	//���� �ʱ�ȭ
	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	perceptron();//train
	predict();//test


	//Weight vector w�� random initialize�Ǳ� ������, ������ ������ ����� �ٲ�Ƿ� 10ȸ ������ ��հ��� ����� ��
	cout << "\n" << "train ���� test ����" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0<<"  "<<TESTcorrect1<<"\n";//�°� �з��� ����
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0�ε� class1�� �� �� �з��� ��� : " << (testNum - class1Num)-TESTcorrect0 << endl;
	cout<<"class1�ε� class0�� �� �� �з��� ��� : "<< class1Num - TESTcorrect1<<endl;


	train1.close();
	test1.close();
	cout << "\n" << "-----------------------------Real dataset-----------------------------" << "\n";

	//���� �ʱ�ȭ
	trainNum = 8143;//������ ������ �°� �Է�
	testNum = 9752;//������ ������ �°� �Է�

	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	class1Num = 0;

	ifstream train2, test2;
	train2.open("datatraining.txt");//���ϸ� ����.
	test2.open("datatset2.txt");
	inpu2(train2, test2);//�����б�


/*
	for (int i = 0; i < trainNum; i++) {
		for (int j = 0; j < 4; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
	
	for (int i = 0; i < testNum; i++) {
		for (int j = 0; j < 4; j++) {
			cout << test[i][j] << " ";
		}
		cout << endl;
	}*/


	//w�� ���ؼ� random initialize
	weigth[0] = rand() % 100;
	weigth[1] = rand() % 100;
	weigth[2] = rand() % 100;
	weigth[3] = rand() % 100;
	weigth[3] *= -1;

	//for (int i = 0; i < 4; i++)cout <<"weigth : "<< weigth[i] << " ";
	//cout << endl;

	predict();//train���� test data�� ���� ���� Ȯ��

	cout << "train���� test data�� ���� ���� Ȯ��" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";//�°� �з��� ����
	cout << "class1 : " << class1Num << endl;//�� class1����
	cout << "class0 : " << testNum - class1Num << endl;//�� class0 ����
	cout << "class0�ε� class1�� �� �� �з��� ��� : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1�ε� class0�� �� �� �з��� ��� : " << class1Num - TESTcorrect1 << endl << endl;

	//���� �ʱ�ȭ
	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	perceptron();//train
	predict();//test


	//Weight vector w�� random initialize�Ǳ� ������, ������ ������ ����� �ٲ�Ƿ� 10ȸ ������ ��հ��� ����� ��
	cout << "\n" << "train ���� test ����" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";//�°� �з��� ����
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0�ε� class1�� �� �� �з��� ��� : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1�ε� class0�� �� �� �з��� ��� : " << class1Num - TESTcorrect1 << endl;


	train2.close();
	test2.close();

	return 0;
}

//perceptron function
void perceptron() {
	EvaluationBeforetrain1(); // �Ʒ��� ���� Ȯ��
	double unitABS = 0;
	double x = 0;
	TRAINfaile = 0;
	for (int iter = 0; iter < iteration; iter++) {
		TRAINfaile = 0;
		for (int i = 0; i < trainNum; i++) {
			x = weigth[0] * arr[i][0] + weigth[1] * arr[i][1] + weigth[2] * arr[i][2] + weigth[3] * arr[i][3];
			if (x<=0 && arr[i][4]==1) { //class�� 1������ �� �� �з��� ���
				unitABS = sqrt(arr[i][0] * arr[i][0] + arr[i][1] * arr[i][1] + arr[i][2] * arr[i][2] + arr[i][3] * arr[i][3]);//�������� ����� ����.
				weigth[0] = weigth[0] + (arr[i][0] / unitABS);
				weigth[1] = weigth[1] + (arr[i][1] / unitABS);
				weigth[2] = weigth[2] + (arr[i][2] / unitABS);
				weigth[3] = weigth[3] + (arr[i][3] / unitABS);
				TRAINfaile++;
			}
			else if (x>0 && arr[i][4]==0 ) { //class�� 0������ �� �� �з��� ���
				unitABS = sqrt(arr[i][0] * arr[i][0] + arr[i][1] * arr[i][1] + arr[i][2] * arr[i][2] + arr[i][3] * arr[i][3]);
				weigth[0] = weigth[0] - (arr[i][0] / unitABS);
				weigth[1] = weigth[1] - (arr[i][1] / unitABS);
				weigth[2] = weigth[2] - (arr[i][2] / unitABS);
				weigth[3] = weigth[3] - (arr[i][3] / unitABS);
				TRAINfaile++;
			}
		}
		if (iter < 30) {//0���ͽ���~29����
			cout << "iter " << iter+1 << " traindata �߸��з����� : " << TRAINfaile << "\n";
		}
	}
}

void predict() {
	double x = 0;
	for (int i = 0; i < testNum; i++) {
		x = weigth[0] * test[i][0] + weigth[1] * test[i][1] + weigth[2] * test[i][2] + weigth[3] * test[i][3];
		if (x <= 0) {
			if (test[i][4] == 0) TESTcorrect0++; 
		}
		else {
			if (test[i][4] == 1) TESTcorrect1++;
		}
	}
}

void EvaluationBeforetrain1() { // �Ʒ��� ���� Ȯ��
	TRAINfaile = 0;
	double x = 0;
	for (int i = 0; i < trainNum; i++) {
		x = weigth[0] * arr[i][0] + weigth[1] * arr[i][1] + weigth[2] * arr[i][2] + weigth[3] * arr[i][3];
		if (x <= 0 && arr[i][4] == 1) { //class�� 1������ class0���� �� �� �з��� ���
			TRAINfaile++;
		}
		else if (x > 0 && arr[i][4] == 0) { //class�� 0������ class1�� �� �� �з��� ���
			TRAINfaile++;
		}
	}
	cout << "Iteration 0 traindata �߸��з����� : " << TRAINfaile << "\n";
}

