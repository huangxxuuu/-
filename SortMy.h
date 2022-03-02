#pragma once
#include <vector>
#include <random>
#include <ctime>

class SortMy {
	const static int smallLen = 11;
public:
	SortMy() {
		srand(time(NULL)); // 初始化随机数种子 。rand()实际上只能在0-65535之间随机。如果实际数据长度更大，需要将多个rand()的二进制位拼起来
	}
	void InsertSort(std::vector<int>& t, int left, int right) {
		for (int i = left + 1; i <= right; ++i)
			for (int j = i; j > 0 && t[j - 1] > t[j]; --j)
				std::swap(t[j - 1], t[j]);
	}

	void QuickSort(std::vector<int>& t, int left, int right) {
		if (left >= right - smallLen) { // 当长度比较短时，改用插入排序
			InsertSort(t, left, right);
			return;
		}
		int pivit = t[(rand() % (right - left+1)) + left]; // 随机选择轴值，减少特殊序列的最差时间复杂度
		int i = left, j = right;
		while (i <= j) {
			// while中没有 i <= j，但ij都不会超界的原因。
			// 以i为例解释。因为比较的时候不包含==，以及pivit的存在，所以 i 的右侧（包含i的位置）一定存在至少一个元素来使它的while停止。
			// 当 i 遇到这个元素时。1）若 i <= j，则进行swap，这个元素移动到更右侧的位置，保证 i 的下一轮也会停止。2）若 i > j，退出循环。
			// 所以这里 一定不能有等于号
			while (t[i] < pivit) ++i;
			while (t[j] > pivit) --j;
			if (i <= j) {
				std::swap(t[i++], t[j--]);
			}
		}
		// while循环结束后，[left, i-1] 包含 <= pivit 的所有元素， [i, right] 包含 >= pivit 的所有元素
		QuickSort(t, left, i - 1);
		QuickSort(t, i, right);
	}

};