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
		cout << "��ɾ� �Է� : ";
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
				if (j * 5 + 2 > str[i].length() - 1) {		//�������̶� ���ڰ� 3���� �ƴ� 1, 2���� ���� ��Ȳ�̸�
					if (str[i].length() - (j * 5) == 2)		//������ ������ ���� ������ 1, 2�� �� �Ѱ����ٵ�, 1�ΰ��� �ȹٲ㵵 �ǰ� 2���� ��츸 ����
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
				if (j == temp.length() - 1 || temp[j + 1] == ' ') {			//ù��°�� ���̸� �ι�°���� �˻� ���ϹǷ� range���� �ȳ���
					end = j;												//�� ������ �ε����� �����ϰų� ������ space�� ���
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
