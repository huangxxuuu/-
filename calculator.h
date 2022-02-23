#pragma once
#include <string>
#include <stack>

template<class T>
class calculator {
	calculator() {};

	static T Mystoi(std::string& str, size_t& index) {
		//�������ֻ�ʺ�������
		//��Ϊϵͳ��stoi��Ҫ��str�Ŀ�ͷ��ʼת��
		bool isNeg = false;
		if (str[index] == '-') {
			isNeg = true;
			++index;
		}
		T res = 0;
		while (index < str.size() && isdigit(str[index])) {
			res = res * 10 + str[index++] - '0';
		}
		if (isNeg)
			return -res;
		return res;
	}
public:
	static T Compute(std::string &str, size_t &index) {
		//Ҫ����������Ч�ģ�int��Χ�ģ��Ҽ��������Ҳ���ܳ���int��Χ
		//�����и���
		if (index >= str.size()) {
			throw "�����±곬����Χ";
		}
		std::stack<T> nums;
		std::stack<char> ops;
		int state = 0; //Ϊ��Ӧ�Ը��š�0��ʾ�������֣�1��ʾ���������
		while (index < str.size()) {
			if (str[index] == ')') {
				++index;
				break;
			}
			if (state == 1) {
				ops.push(str[index++]);
				state = 0;
			}
			else {
				if (str[index] == '(') {
					++index;
					nums.push(Compute(str, index));
				}
				else {
					nums.push(Mystoi(str, index));
				}
				if (!ops.empty()) {
					if (ops.top() == '*' || ops.top() == '/') {
						T b = nums.top(); nums.pop();
						//T a = nums.top(); nums.pop();
						if (ops.top() == '*') {
							nums.top() *= b;
						}
						else {
							nums.top() /= b;
						}
						ops.pop();
					}
				}
				state = 1;
			}
		}
		while (!ops.empty()) {
			char op = ops.top(); ops.pop();
			int b = nums.top(); nums.pop();
			//int a = nums.top(); nums.pop();
			switch (op) {
			case '+':
				nums.top() += b; break;
			case '-':
				nums.top() -= b; break;
			case '*':
				nums.top() *= b; break;
			case '/':
				nums.top() /= b; break;
			}
		}

		return nums.top();


	}
};