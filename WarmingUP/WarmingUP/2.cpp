#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
	std::ifstream in("data.txt");

	std::string s[10];

	for (int i = 0; in; ++i) {
		getline(in, s[i]);
	}


	int word{};
	int num{};
	int cap{};

	for (int i = 0; i < 10; ++i) {
			std::istringstream iss(s[i]);
			std::string str_buf;
			while (getline(iss, str_buf, ' ')) {
				bool isNum = true;
				//bool isCap = false;
				for (int i = 0; i < str_buf.length(); ++i) {
					if (!(str_buf[i] >= '0' && str_buf[i] <= '9'))
						isNum = false;
					/*if (str_buf[i] >= 'A' && str_buf[i] <= 'Z')
						isCap = true;*/
				}

				/*if (isCap) {
					++cap;
				}*/
				if (str_buf[0] >= 'A' && str_buf[0] <= 'Z') {
					++cap;
				}
				if (isNum) {
					++num;
				}
				else {
					++word;
				}
			}
	}

	std::cout << "word count : " << word << std::endl;
	std::cout << "number count : " << num << std::endl;
	std::cout << "Capital count : " << cap << std::endl;


}
