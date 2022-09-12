#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void reverseStr(string* str);
void insertChar(string* str);
void reverseBySpace(string* str);
void replaceChar(string* str);
void compareChar(string* str);


int main()
{
	ifstream in("data.txt");

	string s[10];

	for (int i = 0; in; ++i) {
		getline(in, s[i]);
	}

	while (true) {
		cout << "명령어 입력 : ";
		char order{};
		cin >> order;

		switch (order) {
		case 'd':
			reverseStr(s);
			break;

		case 'e':
			insertChar(s);
			break;

		case 'f':
			reverseBySpace(s);
			break;

		case 'g':
			replaceChar(s);
			break;

		case 'h':
			compareChar(s);
			break;

		case 'q':
			return 0;
			break;
		}

		cout << endl;
	}
}

void reverseStr(string* str)
{
	for (int i = 0; i < 10; ++i) {
		string temp = str[i];
		for (int j = 0; j < temp.length(); ++j) {
			str[i][j] = temp[temp.length() - 1 - j];
		}
	}
	for (int i = 0; i < 10; ++i)
		cout << str[i] << endl;
}

void insertChar(string* str)
{
	if (str[0][3] == '@')
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; 3 * (j + 1) < str[i].length(); ++j) {
				str[i].replace(3 * (j + 1), 2, "");
			}
		}
	else
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; 5 * (j + 1) - 2 < str[i].length(); ++j) {
				str[i].insert(5 * (j + 1) - 2, "@@");
			}
		}
	for (int i = 0; i < 10; ++i)
		cout << str[i] << endl;
}

void reverseBySpace(string* str)
{
	if (str[0][3] == '@')
		for (int i = 0; i < 10; ++i) {
			string temp = str[i];
			for (int j = 0; j * 5 < str[i].length(); ++j) {
				if (j * 5 + 2 > str[i].length() - 1) {		//마지막이라서 문자가 3개가 아닌 1, 2개가 남은 상황이면
					if (str[i].length() - (j * 5) == 2)		//마지막 순번은 문자 갯수가 1, 2개 중 한개일텐데, 1인경우는 안바꿔도 되고 2개일 경우만 생각
						for (int k = 0; k < 2; ++k) {
							str[i][j * 5 + k] = temp[j * 5 + 1 - k];
						}
				}
				else
					for (int k = 0; k < 3; ++k) {
						str[i][j * 5 + k] = temp[j * 5 + 2 - k];
					}
			}
		}
	else
		for (int i = 0; i < 10; ++i) {
			string temp = str[i];
			int start = 0, end = 0;
			for (int j = 0; j < temp.length(); ++j) {
				if (j == temp.length() - 1 || temp[j + 1] == ' ') {			//첫번째가 참이면 두번째조건 검사 안하므로 range오류 안날듯
					end = j;												//ㄴ 마지막 인덱스에 도달하거나 다음이 space인 경우
					for (int k = 0; k < end - start + 1; ++k) {
						str[i][start + k] = temp[end - k];
					}
					start = end + 2;
				}
			}
		}

	for (int i = 0; i < 10; ++i)
		cout << str[i] << endl;
}

void replaceChar(string* str)
{
	char org{}, cng{};
	cin >> org >> cng;

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < str[i].length(); ++j) {
			if (str[i][j] == org)
				str[i][j] = cng;
		}
	}

	for (int i = 0; i < 10; ++i)
		cout << str[i] << endl;
}

void compareChar(string* str)
{
	for (int i = 0; i < 10; ++i) {
		string temp{};
		for (int j = 0;; ++j) {
			if (j < str[i].length() - 1 - j) {
				if (str[i][j] == str[i][str[i].length() - 1 - j]) {
					temp.append(1, str[i][j]);
				}
				else
					break;
			}
			else
				break;
		}
		if (temp.empty())
			temp.append("0");

		cout << str[i] << " : " << temp << endl;
	}
}
