#include <iostream>
#include <random>

using namespace std;

class matcal {
private:
	int one[3][3]{};
	int two[3][3]{};


public:
	matcal() {
		reset();
	}

	void reset();

	void mul();					//뭔가 연산자 오버로딩으로 풀면 효율적일거 같지만 패스
	void add();
	void sub();
	void det();
	void trans();
	void cngfour();
	void rndmat();
};

int det3(int(*arr)[3]);


int main()
{
	matcal play;
	

	while (true) {
		cout << "입력하세요 : ";
		char order{};
		cin >> order;
		switch (order) {
		case 'm':
			play.mul();
			break;
		case 'a':
			play.add();
			break;
		case 'd':
			play.sub();
			break;
		case 'r':
			play.det();
			break;
		case 't':
			play.trans();
			break;
		case 'h':
			play.cngfour();
			break;
		case 's':
			play.rndmat();
			break;
		case 'q':
			return 0;
			break;
		default:
			cout << "잘못입력함" << endl;
			break;
		}
	}
	

}

void matcal::reset()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 1);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			one[i][j] = dis(gen);
			two[i][j] = dis(gen);
		}
	}
}

void matcal::mul()
{
	int value[3][3]{};

	for (int k = 0; k < 3 * 3; ++k) {								//생각해보니 이거 어차피 n^3짜리 식인데 3중 for 해도 똑같은거 아닐까?
		for (int i = 0; i < 3; ++i) {
			value[k / 3][k % 3] += one[k / 3][i] * two[i][k % 3];
		}
	}

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << "   " << "|" << two[0][0] << two[0][1] << two[0][2] << "|" << "   " << "|" << value[0][0] << value[0][1] << value[0][2] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << " * " << "|" << two[1][0] << two[1][1] << two[1][2] << "|" << " = " << "|" << value[1][0] << value[1][1] << value[1][2] << "|" << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << "   " << "|" << two[2][0] << two[2][1] << two[2][2] << "|" << "   " << "|" << value[2][0] << value[2][1] << value[2][2] << "|" << endl;

}

void matcal::add()
{
	int value[3][3]{};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			value[i][j] = one[i][j] + two[i][j];
		}
	}

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << "   " << "|" << two[0][0] << two[0][1] << two[0][2] << "|" << "   " << "|" << value[0][0] << value[0][1] << value[0][2] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << " * " << "|" << two[1][0] << two[1][1] << two[1][2] << "|" << " = " << "|" << value[1][0] << value[1][1] << value[1][2] << "|" << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << "   " << "|" << two[2][0] << two[2][1] << two[2][2] << "|" << "   " << "|" << value[2][0] << value[2][1] << value[2][2] << "|" << endl;
}

void matcal::sub()
{
	int value[3][3]{};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			value[i][j] = one[i][j] - two[i][j];
		}
	}

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << "   " << "|" << two[0][0] << two[0][1] << two[0][2] << "|" << "   " << "|" << value[0][0] << value[0][1] << value[0][2] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << " * " << "|" << two[1][0] << two[1][1] << two[1][2] << "|" << " = " << "|" << value[1][0] << value[1][1] << value[1][2] << "|" << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << "   " << "|" << two[2][0] << two[2][1] << two[2][2] << "|" << "   " << "|" << value[2][0] << value[2][1] << value[2][2] << "|" << endl;
}

void matcal::det()
{
	int value1 = det3(one);
	int value2 = det3(two);

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << "" << "		|" << two[0][0] << two[0][1] << two[0][2] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << " = " << value1 << "	|" << two[1][0] << two[1][1] << two[1][2] << "|" << " = " << value2 << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << "" << "		|" << two[2][0] << two[2][1] << two[2][2] << "|" << endl;
}

void matcal::trans()
{
	int transone[3][3]{};
	int transtwo[3][3]{};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j){
			transone[i][j] = one[j][i];
			transtwo[i][j] = two[j][i];
		}
	}

	int value1 = det3(transone);
	int value2 = det3(transtwo);

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << "    " << "|" << transone[0][0] << transone[0][1] << transone[0][2] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << " → " << "|" << transone[1][0] << transone[1][1] << transone[1][2] << "|" << " = " << value1 << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << "    " << "|" << transone[2][0] << transone[2][1] << transone[2][2] << "|\n" << endl;

	cout << "|" << two[0][0] << two[0][1] << two[0][2] << "|" << "    " << "|" << transtwo[0][0] << transtwo[0][1] << transtwo[0][2] << "|" << endl;
	cout << "|" << two[1][0] << two[1][1] << two[1][2] << "|" << " → " << "|" << transtwo[1][0] << transtwo[1][1] << transtwo[1][2] << "|" << " = " << value2 << endl;
	cout << "|" << two[2][0] << two[2][1] << two[2][2] << "|" << "    " << "|" << transtwo[2][0] << transtwo[2][1] << transtwo[2][2] << "|" << endl;

}

void matcal::cngfour()
{
	int mat_four1[4][4]{};
	int mat_four2[4][4]{};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat_four1[i][j] = one[i][j];
			mat_four2[i][j] = two[i][j];
		}
	}

	mat_four1[3][3] = mat_four2[3][3] = 1;


	int value1 = det3(one);						//생각해보니 4x4로 변환할때 단위행렬로 추가했으니 기존 3x3의 행렬식 값이랑 같잖아?
	int value2 = det3(two);

	cout << "|" << one[0][0] << one[0][1] << one[0][2] << "|" << " " << "	|" << mat_four1[0][0] << mat_four1[0][1] << mat_four1[0][2] << mat_four1[0][3] << "|" << endl;
	cout << "|" << one[1][0] << one[1][1] << one[1][2] << "|" << "→" << "	|" << mat_four1[1][0] << mat_four1[1][1] << mat_four1[1][2] << mat_four1[1][3] << "|" << " = " << value1 << endl;
	cout << "|" << one[2][0] << one[2][1] << one[2][2] << "|" << " " << "	|" << mat_four1[2][0] << mat_four1[2][1] << mat_four1[2][2] << mat_four1[2][3] << "|" << endl;
	cout << "	" << "|" << mat_four1[3][0] << mat_four1[3][1] << mat_four1[3][2] << mat_four1[3][3] << "|\n" << endl;

	cout << "|" << two[0][0] << two[0][1] << two[0][2] << "|" << "  " << "	|" << mat_four2[0][0] << mat_four2[0][1] << mat_four2[0][2] << mat_four2[0][3] << "|" << endl;
	cout << "|" << two[1][0] << two[1][1] << two[1][2] << "|" << "→" << "	|" << mat_four2[1][0] << mat_four2[1][1] << mat_four2[1][2] << mat_four2[1][3] << "|" << " = " << value2 << endl;
	cout << "|" << two[2][0] << two[2][1] << two[2][2] << "|" << "  " << "	|" << mat_four2[2][0] << mat_four2[2][1] << mat_four2[2][2] << mat_four2[2][3] << "|" << endl;
	cout << "	" << "|" << mat_four2[3][0] << mat_four2[3][1] << mat_four2[3][2] << mat_four2[3][3] << "|" << endl;


}

void matcal::rndmat()
{
	reset();
}

int det3(int(*arr)[3])
{
	return arr[0][0] * (arr[1][1] * arr[2][2] - arr[1][2] * arr[2][1]) - arr[0][1] * (arr[1][0] * arr[2][2] - arr[1][2] * arr[2][0]) + arr[0][2] * (arr[1][0] * arr[2][1] - arr[1][1] * arr[2][0]);
}
