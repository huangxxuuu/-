#pragma once
#include <vector>
#include <random>
#include <ctime>

class SortMy {
	const static int smallLen = 32;
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
														   // 或者选择 left 与 right 的中间值(重要)，不能选择最边上的
														   // 解释：当选择中间位置时，即使它是最大或最小值，也会在这次遍历后被交换到两侧，使得下一层递归的中间值不是最大最小值
														   // 若选择最边上的下标，又正好是最大最小值，则这次遍历不会移动它的位置，下一层递归还会选择它。导致无限递归。
		int i = left, j = right;
		while (i <= j) { // 这里要有等于。因为可能在 j == i+2 处进行一次交换。使得 i == j。这时可能 t[i] < pivit。而循环结束后，i的含义是左侧（<=pivit）的最后元素的后一个下标
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
