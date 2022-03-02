#pragma once
#include <vector>
#include <random>
#include <ctime>

class SortMy {
	const static int smallLen = 11;
public:
	SortMy() {
		srand(time(NULL)); // ��ʼ����������� ��rand()ʵ����ֻ����0-65535֮����������ʵ�����ݳ��ȸ�����Ҫ�����rand()�Ķ�����λƴ����
	}
	void InsertSort(std::vector<int>& t, int left, int right) {
		for (int i = left + 1; i <= right; ++i)
			for (int j = i; j > 0 && t[j - 1] > t[j]; --j)
				std::swap(t[j - 1], t[j]);
	}

	void QuickSort(std::vector<int>& t, int left, int right) {
		if (left >= right - smallLen) { // �����ȱȽ϶�ʱ�����ò�������
			InsertSort(t, left, right);
			return;
		}
		int pivit = t[(rand() % (right - left+1)) + left]; // ���ѡ����ֵ�������������е����ʱ�临�Ӷ�
		int i = left, j = right;
		while (i <= j) {
			// while��û�� i <= j����ij�����ᳬ���ԭ��
			// ��iΪ�����͡���Ϊ�Ƚϵ�ʱ�򲻰���==���Լ�pivit�Ĵ��ڣ����� i ���Ҳࣨ����i��λ�ã�һ����������һ��Ԫ����ʹ����whileֹͣ��
			// �� i �������Ԫ��ʱ��1���� i <= j�������swap�����Ԫ���ƶ������Ҳ��λ�ã���֤ i ����һ��Ҳ��ֹͣ��2���� i > j���˳�ѭ����
			// �������� һ�������е��ں�
			while (t[i] < pivit) ++i;
			while (t[j] > pivit) --j;
			if (i <= j) {
				std::swap(t[i++], t[j--]);
			}
		}
		// whileѭ��������[left, i-1] ���� <= pivit ������Ԫ�أ� [i, right] ���� >= pivit ������Ԫ��
		QuickSort(t, left, i - 1);
		QuickSort(t, i, right);
	}

};