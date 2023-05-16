#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>//난수생성
#include<ctime>//현재시간기준시드생성
#pragma warning(disable:4996)
using namespace std;

int iteration = 1000; //반복 횟수
int trainNum = 0;//train data 개수
int testNum = 0;//test data 개수
int class1Num = 0; //class1 총 개수
int TESTcorrect0 = 0; //test_class0으로 잘 분류된 개수
int TESTcorrect1 = 0; //test_class1로 잘 분류된 개수
int TRAINfaile = 0; //train_class 잘못 분류된 개수
// wx - bias = 0
double weigth[4] = { 0 }; //w, random initialize해야함. bias 때문에 한 차원 더 늘림.

double arr[10000][5] = { 0 }; //x, train data                 배열 똑같은거쓰려면, 함수 같은거 쓰려면 크기 늘려야 함.
double test[10000][5] = { 0 }; //test data

//int classification1[10000] = { 0 };//input : index, 데이터 분포 확인할 거면 넣기
//int classification0[10000] = { 0 };

void perceptron();
void predict();
void EvaluationBeforetrain1();

void inpu1(ifstream& train1, ifstream& test1) {//파일 읽기
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

void inpu2(ifstream& train2, ifstream& test2) {//파일 읽기
	char str[105];
	char str2;
	double a;
	string str1;
	train2 >> str1;// 처음 문장 한줄
	for (int i = 0; i < trainNum; i++) {
		train2.getline(str, 100, ',');//number
		//cout << "입력 " << str << endl;
		train2.getline(str, 100, ',');//date
		train2 >> arr[i][0];//temperature
		train2 >> str2;
		train2 >> a >> str2;
		train2 >> arr[i][1];//light
		train2 >> str2;
		train2 >> arr[i][2];//co2
		train2 >> str2;
		train2 >> a >> str2;
		arr[i][3] = 1;//bias 때문에 차원 하나 늘림
		train2 >> arr[i][4];//class

		/*
		for (int j = 0; j < 5; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
		*/
	}

	test2 >> str1>>str2;// 처음 문장 한줄
	for (int i = 0; i < testNum; i++) {
		test2.getline(str, 100, ',');//number
		//cout << "입력 " << str << endl;
		test2.getline(str, 100, ',');//date
		test2 >> test[i][0];//temperature
		test2 >> str2;
		test2 >> a >> str2;
		test2 >> test[i][1];//light
		test2 >> str2;
		test2 >> test[i][2];//co2
		test2 >> str2;
		test2 >> a >> str2;
		test[i][3] = 1;//bias 때문에 차원 하나 늘림
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
	srand((unsigned int)time(NULL)); //seed 값으로 현재시간 부여

	cout << "-----------------------------Synthetic dataset-----------------------------" << "\n";
	//file 입력
	ifstream train1,test1;
	train1.open("synthetic_data_train.txt");//파일명 주의.
	test1.open("synthetic_data_test.txt");
	trainNum = 2000;//데이터 개수에 맞게 입력
	testNum = 400;//데이터 개수에 맞게 입력
	inpu1(train1, test1);//파일읽기

	//w에 대해서 random initialize
	weigth[0] = rand() % 100;
	weigth[1] = rand() % 100;
	weigth[2] = rand() % 100;
	weigth[3] = rand() % 100;
	weigth[3] *= -1;

	//for (int i = 0; i < 4; i++)cout <<"weigthr : "<< weigth[i] << " ";
	//cout << endl;

	//train전에 test data에 대한 성능 확인
	predict();
	cout << "train전에 test data에 대한 성능 확인" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0인데 class1로 잘 못 분류된 경우 : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1인데 class0로 잘 못 분류된 경우 : " << class1Num - TESTcorrect1 << endl << endl;

	//변수 초기화
	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	perceptron();//train
	predict();//test


	//Weight vector w가 random initialize되기 때문에, 실행할 때마다 결과가 바뀌므로 10회 실행한 평균값을 계산할 것
	cout << "\n" << "train 후의 test 성능" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0<<"  "<<TESTcorrect1<<"\n";//맞게 분류한 개수
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0인데 class1로 잘 못 분류된 경우 : " << (testNum - class1Num)-TESTcorrect0 << endl;
	cout<<"class1인데 class0로 잘 못 분류된 경우 : "<< class1Num - TESTcorrect1<<endl;


	train1.close();
	test1.close();
	cout << "\n" << "-----------------------------Real dataset-----------------------------" << "\n";

	//변수 초기화
	trainNum = 8143;//데이터 개수에 맞게 입력
	testNum = 9752;//데이터 개수에 맞게 입력

	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	class1Num = 0;

	ifstream train2, test2;
	train2.open("datatraining.txt");//파일명 주의.
	test2.open("datatset2.txt");
	inpu2(train2, test2);//파일읽기


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


	//w에 대해서 random initialize
	weigth[0] = rand() % 100;
	weigth[1] = rand() % 100;
	weigth[2] = rand() % 100;
	weigth[3] = rand() % 100;
	weigth[3] *= -1;

	//for (int i = 0; i < 4; i++)cout <<"weigth : "<< weigth[i] << " ";
	//cout << endl;

	predict();//train전에 test data에 대한 성능 확인

	cout << "train전에 test data에 대한 성능 확인" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";//맞게 분류된 개수
	cout << "class1 : " << class1Num << endl;//총 class1개수
	cout << "class0 : " << testNum - class1Num << endl;//총 class0 개수
	cout << "class0인데 class1로 잘 못 분류된 경우 : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1인데 class0로 잘 못 분류된 경우 : " << class1Num - TESTcorrect1 << endl << endl;

	//변수 초기화
	TESTcorrect0 = 0;
	TESTcorrect1 = 0;

	perceptron();//train
	predict();//test


	//Weight vector w가 random initialize되기 때문에, 실행할 때마다 결과가 바뀌므로 10회 실행한 평균값을 계산할 것
	cout << "\n" << "train 후의 test 성능" << "\n";
	cout << "correct0, 1 : " << TESTcorrect0 << "  " << TESTcorrect1 << "\n";//맞게 분류된 개수
	cout << "class1 : " << class1Num << endl;
	cout << "class0 : " << testNum - class1Num << endl;
	cout << "class0인데 class1로 잘 못 분류된 경우 : " << (testNum - class1Num) - TESTcorrect0 << endl;
	cout << "class1인데 class0로 잘 못 분류된 경우 : " << class1Num - TESTcorrect1 << endl;


	train2.close();
	test2.close();

	return 0;
}

//perceptron function
void perceptron() {
	EvaluationBeforetrain1(); // 훈련전 성능 확인
	double unitABS = 0;
	double x = 0;
	TRAINfaile = 0;
	for (int iter = 0; iter < iteration; iter++) {
		TRAINfaile = 0;
		for (int i = 0; i < trainNum; i++) {
			x = weigth[0] * arr[i][0] + weigth[1] * arr[i][1] + weigth[2] * arr[i][2] + weigth[3] * arr[i][3];
			if (x<=0 && arr[i][4]==1) { //class가 1이지만 잘 못 분류된 경우
				unitABS = sqrt(arr[i][0] * arr[i][0] + arr[i][1] * arr[i][1] + arr[i][2] * arr[i][2] + arr[i][3] * arr[i][3]);//단위벡터 사용을 위함.
				weigth[0] = weigth[0] + (arr[i][0] / unitABS);
				weigth[1] = weigth[1] + (arr[i][1] / unitABS);
				weigth[2] = weigth[2] + (arr[i][2] / unitABS);
				weigth[3] = weigth[3] + (arr[i][3] / unitABS);
				TRAINfaile++;
			}
			else if (x>0 && arr[i][4]==0 ) { //class가 0이지만 잘 못 분류된 경우
				unitABS = sqrt(arr[i][0] * arr[i][0] + arr[i][1] * arr[i][1] + arr[i][2] * arr[i][2] + arr[i][3] * arr[i][3]);
				weigth[0] = weigth[0] - (arr[i][0] / unitABS);
				weigth[1] = weigth[1] - (arr[i][1] / unitABS);
				weigth[2] = weigth[2] - (arr[i][2] / unitABS);
				weigth[3] = weigth[3] - (arr[i][3] / unitABS);
				TRAINfaile++;
			}
		}
		if (iter < 30) {//0부터시작~29까지
			cout << "iter " << iter+1 << " traindata 잘못분류개수 : " << TRAINfaile << "\n";
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

void EvaluationBeforetrain1() { // 훈련전 성능 확인
	TRAINfaile = 0;
	double x = 0;
	for (int i = 0; i < trainNum; i++) {
		x = weigth[0] * arr[i][0] + weigth[1] * arr[i][1] + weigth[2] * arr[i][2] + weigth[3] * arr[i][3];
		if (x <= 0 && arr[i][4] == 1) { //class가 1이지만 class0으로 잘 못 분류된 경우
			TRAINfaile++;
		}
		else if (x > 0 && arr[i][4] == 0) { //class가 0이지만 class1로 잘 못 분류된 경우
			TRAINfaile++;
		}
	}
	cout << "Iteration 0 traindata 잘못분류개수 : " << TRAINfaile << "\n";
}

