#include <iostream>
#include <vector>

using namespace std;

// https://www.luogu.com.cn/problem/P1177
void quickSort(vector<int> &nums, int left, int right) {
    if (left >= right)
        return ;
        
    int midNumb = nums[(left+right) >> 1];
    int i = left, j = right;
    while (i <= j) {
        while (nums[i] < midNumb) ++i;
        while (nums[j] > midNumb) --j;
        if (i <= j)
            swap(nums[i++], nums[j--]);
    }
    
    quickSort(nums, left, j);
    quickSort(nums, i, right);
}

void heapSortHelp(vector<int> &nums, int len, int index) {
    int left = index * 2 + 1;
    int right = left + 1;
    while (left < len) {
        int maxIndex = index;
        if (left < len && nums[left] > nums[maxIndex]) {
            maxIndex = left;
        }
        if (right < len && nums[right] > nums[maxIndex]) {
            maxIndex = right;
        }
        if (maxIndex == index) {
            break;
        }
        swap(nums[index], nums[maxIndex]);
        index = maxIndex;
        left = index * 2 + 1;
        right = left + 1;
    }
}

void heapSort(vector<int> &nums, int len) {
    for (int i = (len - 1) / 2; i >= 0; --i) {
        heapSortHelp(nums, len, i);
    }
    for (; len > 1; --len) {
        swap(nums[0], nums[len-1]);
        heapSortHelp(nums, len-1, 0);
    }
}

vector<int> cache;

void mergeSort(vector<int> &nums, int left, int right) {
    if (left >= right)
        return;
    int mid = (left + right) >> 1;
    mergeSort(nums, left, mid);
    mergeSort(nums, mid+1, right);
    int i = left, j = mid + 1, index = left;
    while (i <= mid && j <= right) {
        if (nums[i] < nums[j]) {
            cache[index++] = nums[i++];
        }
        else {
            cache[index++] = nums[j++];
        }
    }
    while (i <= mid) {
        cache[index++] = nums[i++];
    }
    while (j <= right) {
        cache[index++] = nums[j++];
    }
    for (index = left; index <= right; ++index) {
        nums[index] = cache[index];
    }
}

void bubbleSort(vector<int> &nums) {
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = 1; j < nums.size() - i; ++j) {
            if (nums[j-1] > nums[j]) {
                swap(nums[j-1], nums[j]);
            }
        }
    }
}

void selectSort(vector<int> &nums) {
    for (int i = 0; i < nums.size(); ++i) {
        int min_index = i;
        for (int j = i+1; j < nums.size(); ++j) {
            if (nums[j] < nums[min_index]) {
                min_index = j;
            }
        }
        swap(nums[min_index], nums[i]);
    }
}

void insertSort(vector<int> &nums) {
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = i; j >= 1; --j) {
            if (nums[j-1] > nums[j]) {
                swap(nums[j-1], nums[j]);
            }
            else {
                break;
            }
        }
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> nums;
    int temp;
    for (int i = 0; i < n; ++i) {
        cin >> temp;
        nums.push_back(temp);
    }
    //cache = vector<int>(nums.size(), 0);
    
    //mergeSort(nums, 0, nums.size()-1);
    insertSort(nums);
    
    for (auto &num : nums) {
        cout << num << " ";
    }
    
    return 0;
}
