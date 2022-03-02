#pragma once
#include <vector>
#include <random>
#include <ctime>
// 使用这个题目测试的 https://www.luogu.com.cn/problem/P1177
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

	void HelpHeapFix(std::vector<int>& t, int len, int i) {  // 调整节点i的顺序
		int cur, left, right, maxIndex;
		cur = i;
		while (cur < len) {
			left = cur * 2 + 1;
			right = cur * 2 + 2;
			if (right < len) {
				if (t[right] > t[left])
					maxIndex = right;
				else
					maxIndex = left;
			}
			else {
				if (left < len)
					maxIndex = left;
				else
					maxIndex = cur;
			}
			if (t[cur] < t[maxIndex]) {
				std::swap(t[cur], t[maxIndex]);
				cur = maxIndex;
			}
			else {
				break;
			}
		}

	}

	void HelpHeapBuild(std::vector<int>& t, int len) { // 建堆
		for (int i = (len - 1 - 1) / 2; i >= 0; --i)
			HelpHeapFix(t, len, i);
	}

	void HeapSort(std::vector<int>& t, int len) {
		HelpHeapBuild(t, len);
		for (int end = len - 1; end > 0; --end) { // 每次把堆顶元素移到末尾，然后缩小堆的长度
			std::swap(t[0], t[end]);
			HelpHeapFix(t, end, 0);
		}
	}

};
