#pragma once
#include <string>
#include <stack>


// expr1 -> ( expr1 )
//		| expr2 + expr1
//		| expr2 - expr1
//		| expr2
// expr2 -> ( expr1 )
//		| number * expr2
//		| number / expr2
//		| number
// number -> ( expr )
//		| - ( expr )
//		| singleNumber
//		| - singleNumber
// singleNumber == [1-9][0-9]*



class calculator {
	calculator() {};

	static long long Mystoi(std::string& str, size_t& index) {
		// transform to an integer start at the index of the str
		// index finally points to the next element at the end of the integer
		bool isNeg = false;
		if (str[index] == '-') {
			isNeg = true;
			++index;
		}
		while (index < str.size() && str[index] == ' ')
			++index;
		if (str[index] == '(') { // bracket expression
			++index;
			if (isNeg)
				return -Compute(str, index);
			return Compute(str, index);
		}
		// single integer
		long long res = 0;
		while (index < str.size() && isdigit(str[index])) {
			res = res * 10 + str[index++] - '0';
		}
		if (isNeg)
			return -res;
		return res;
	}
public:
	static  long long Compute(std::string& str, size_t& index) {
		// input an arithmetic expression. it can include + - * / ( ) and space。 The minus sign can be interpreted as a binary or unary operator
		if (index >= str.size()) {
			throw "输入下标超出范围";
		}
		std::deque<long long> nums;
		std::deque<char> ops;
		int state = 0; // for unary operator minus sign. 0 means get an integer, 1 means get an operator
		while (index < str.size()) {
			if (str[index] == ' ') {
				++index; continue;
			}
			if (str[index] == ')') {
				++index;
				break;
			}
			if (state == 1) { // operator
				ops.push_back(str[index++]);
				state = 0;
			}
			else { // integer
				nums.push_back(Mystoi(str, index));
				if (!ops.empty()) { // compute the / * . because the / * is the highest priority in deque.
					if (ops.back() == '*' || ops.back() == '/') {
						long long b = nums.back(); nums.pop_back();
						if (ops.back() == '*') {
							nums.back() *= b;
						}
						else {
							nums.back() /= b;
						}
						ops.pop_back();
					}
				}
				state = 1;
			}
		}
		// now there are only plus and minus signs in the deque. 
		// compute them from front to back.
		while (!ops.empty()) { 
			char op = ops.front(); ops.pop_front();
			long long b = nums.front(); nums.pop_front();
			switch (op) {
			case '+':
				nums.front() += b; break;
			case '-':
				nums.front() = b - nums.front(); break;
			case '*':
				nums.front() *= b; break;
			case '/':
				nums.front() = b / nums.front(); break;
			}
		}
		return nums.front();
	}
};
