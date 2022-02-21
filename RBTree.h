#pragma once
#include <vector>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
class RBNode {
public:
	int val;
	RBNode* fa, *left, *right;
	bool isRed;
	RBNode(int val = 0, RBNode* fa = nullptr, RBNode* left = nullptr, RBNode* right = nullptr, bool isRed = true) :val(val), fa(fa), left(left), right(right), isRed(isRed) {}
	
	bool isLeaf() {
		return left == nullptr && right == nullptr;
	}

	RBNode* getUncle() {
		if (fa == nullptr || fa->fa == nullptr)
			throw "getUncle error";
		RBNode* fa2 = fa->fa;
		if (fa2->left == fa) {
			return fa2->right;
		}
		else {
			return fa2->left;
		}
	}

	RBNode* getBrother() {
		if (fa == nullptr)
			throw "getBrother error";
		if (isFaLeft()) {
			return fa->right;
		}
		else {
			return fa->left;
		}
	}

	bool isFaLeft() {
		if (fa == nullptr)
			return false;
		return fa->left == this;
	}

	RBNode* rotateRight(RBNode* &root) {
		// �Ե�ǰ�ڵ�͸��ڵ��������Ҫ��ǰ�ڵ�Ϊ���ڵ�����ӽڵ�
		//      fa           t
		//   t      ==>         fa
		if (this->fa == nullptr || !isFaLeft()) {
			throw "RBNode rotateRight";
			return nullptr;
		}
		RBNode* old_fafa = fa->fa;
		if (old_fafa == nullptr) {
			root = this;
		}
		else {
			if (fa->isFaLeft())
				old_fafa->left = this;
			else
				old_fafa->right = this;
		}
		fa->left = right;
		if(right != nullptr)
			right->fa = fa;
		fa->fa = this;
		right = fa;
		fa = old_fafa;
		return this;
	}

	RBNode* rotateLeft(RBNode*& root) {
		// �Ե�ǰ�ڵ�͸��ڵ��������Ҫ��ǰ�ڵ�Ϊ���ڵ�����ӽڵ�
		//  fa                  t
		//      t   ==>     fa
		if (this->fa == nullptr || isFaLeft()) {
			throw "RBNode rotateLeft";
			return nullptr;
		}
		RBNode* old_fafa = fa->fa;
		if (old_fafa == nullptr) {
			root = this;
		}
		else {
			if (fa->isFaLeft())
				old_fafa->left = this;
			else
				old_fafa->right = this;
		}
		fa->right = left;
		if(left != nullptr)
			left->fa = fa;
		fa->fa = this;
		left = fa;
		fa = old_fafa;
		return this;
	}
};


//��������ѧϰ�����
//ʹ��int val��key������������������
class RBTree {
	RBNode* root;
	int size;
	RBNode* findHelp(RBNode* ro, int val);
	void insertFix(RBNode* t);
	RBNode* getNextNode(RBNode* ro); // �����ҵ���ǰ��������Сֵ
	void deleteFix(RBNode* t);
	static std::vector<RBNode*> inorderTraversal(RBNode* root);

public:
	// ���нڵ��ֵ������ͬ
	RBTree() :root(nullptr), size(0) {}

	int getSize();
	RBNode* find(int val);
	RBNode* findNext(RBNode* t);
	RBNode* findPrev(RBNode* t);
	int insertNode(int val);
	int deleteNode(int val);
	static void printTree(RBTree &tree);

	
};

int RBTree::getSize() {
	return this->size;
}

RBNode* RBTree::findHelp(RBNode* ro, int val) {
	if (ro == nullptr)
		return ro;
	if (ro->val == val) {
		return ro;
	}
	if (ro->val > val) {
		return findHelp(ro->left, val);
	}
	else {
		return findHelp(ro->right, val);
	}
}

RBNode* RBTree::find(int val) {
	return findHelp(root, val);
}

RBNode* RBTree::findNext(RBNode* t) {
	if (t == nullptr) {
		throw "findNext error : t is nullptr";
	}
	if (t->right != nullptr) {
		return getNextNode(t->right);
	}
	while (t->fa != nullptr && !t->isFaLeft()) {
		t = t->fa;
	}
	if (t->fa == nullptr)
		return nullptr;
	if (t->isFaLeft()) {
		t = t->fa;
		return t;
	}
	throw "findNext error : end";
}

RBNode* RBTree::findPrev(RBNode* t) {
	if (t == nullptr) {
		throw "findPrev error : t is nullptr";
	}
	if (t->left != nullptr) {
		t = t->left;
		while (t->right != nullptr) {
			t = t->right;
		}
		return t;
	}
	while (t->fa != nullptr && t->isFaLeft()) {
		t = t->fa;
	}
	if (t->fa == nullptr)
		return nullptr;
	if (!t->isFaLeft()) {
		t = t->fa;
		return t;
	}
	throw "findPrev error : end";
}

void RBTree::insertFix(RBNode* t) {
	//�������������Ǻ�ɫ�ڵ㡣
	//ÿ��·���ϵĺ�ɫ�ڵ���Ŀû�䡣
	//��Ҫ�������ǲ���������ɫ�ڵ�����
	if (t->fa == nullptr) { // ��ǰ�ڵ�Ϊroot
		t->isRed = false;
		return;
	}
	if (t->fa->isRed == false) { //���1�����ڵ�Ϊ��ɫ������Ҫ�޸�
		return;
	}
	//�����������Ǹ��ڵ�Ϊ��ɫ
	RBNode* uncle = t->getUncle();
	if (uncle != nullptr && t->getUncle()->isRed) { //���2������ڵ�Ϊ��ɫ��ֻ��Ҫ����Ⱦɫ��Ȼ�����ϵݹ�
		t->fa->isRed = false;
		t->getUncle()->isRed = false;
		t->fa->fa->isRed = true;
		insertFix(t->fa->fa);
		return;
	}
	//����������������ڵ�Ϊ��ɫ���սڵ�Ҳ�Ǻ�ɫ����
	//ֻ��Ҫ��ע��ǰ�ڵ�t�����ڵ�t->fa���游�ڵ�t->fa->fa����Ϊ���ڵ�Ϊ��ɫ���϶������游�ڵ�
	// ���3��t t->fa t->fa->fa ��֮���Ρ�����֮��������4
	if (t->isFaLeft() != t->fa->isFaLeft()) {
		if (t->isFaLeft()) {
			//   t->fa->fa ��ɫ
			//        t->fa ��ɫ
			//   t ��ɫ
			// ��t ����
			t->rotateRight(root);
			// ����ǰ�ڵ��Ϊԭ���ĸ��ڵ�
			t = t->right;
		}
		else {
			//       t->fa->fa ��ɫ
			//    t->fa ��ɫ
			//       t ��ɫ
			// ��t ����
			t->rotateLeft(root);
			// ����ǰ�ڵ��Ϊԭ���ĸ��ڵ�
			t = t->left;
		}

	}
	//���4��t t->fa t->fa->fa ��һ��ֱ��
	if (t->isFaLeft() == t->fa->isFaLeft()) {
		if (t->isFaLeft()) {
			//       t->fa->fa ��ɫ
			//     t->fa ��ɫ
			//  t ��ɫ
			// ��t->fa����
			t->fa->rotateRight(root);
			//����Ⱦɫ
			t->fa->isRed = false;
			t->fa->right->isRed = true;

		}
		else {
			// t->fa->fa ��ɫ
			//     t->fa ��ɫ
			//        t ��ɫ
			// ��t->fa����
			t->fa->rotateLeft(root);
			//����Ⱦɫ
			t->fa->isRed = false;
			t->fa->left->isRed = true;
		}
	}
}

int RBTree::insertNode(int val) {
	RBNode* t = new RBNode(val);
	if (root == nullptr) {
		root = t;
		insertFix(t);
		++size;
		return 1;
	}
	RBNode* temp = root;
	while (temp != nullptr) {
		if (temp->val == t->val) {
			delete t;
			return 0;
		}
		if (temp->val > t->val) {
			if (temp->left == nullptr) {
				temp->left = t;
				t->fa = temp;
				break;
			}
			temp = temp->left;
		}
		else {
			if (temp->right == nullptr) {
				temp->right = t;
				t->fa = temp;
				break;
			}
			temp = temp->right;
		}
	}
	insertFix(t);
	++size;
	return 1;
}

RBNode* RBTree::getNextNode(RBNode* ro) { // ��������е���С�Ľڵ�
	//Ҫ��ro!=nullptr����Ӧ���ɵ����߼�飩
	if (ro == nullptr)
		throw "deleteNextNode: ro == nullptr";
	while (ro->left != nullptr) {
		ro = ro->left;
	}
	return ro;
}

void RBTree::deleteFix(RBNode* t) { // �޸���ʾ����tΪ���ڵ��������������������������һ����ɫ�ڵ�
	if (t->isRed) { // ɾ����ɫ�ڵ㲻��Ҫ�޸�
		t->isRed = false;  // Ⱦɫ��Ϊ�˶�εݹ�������deleteFixʱ��t��һ��������ɾ�����Ǹ��ڵ���
		return;
	}

	if (t->fa == nullptr) { //���ĸ��ڵ㲻��Ҫ�޸�
		return;
	}
	// ���涼�Ǵ�ɾ���ڵ�Ϊ��ɫ�����
	// ��Ϊɾ��һ����ɫ�ڵ㣬���Ե�ǰ�����Ϻ�ɫ�ڵ�����һ����
	// �޸���Ŀ������취���ֵܽڵ㴦��һ����������ʹ�ֵܽڵ���һ��
	RBNode* bro = t->getBrother();
	// ���1���ֵܽڵ�Ϊ��ɫ��
	if (bro != nullptr && bro->isRed) { // ��ɫ���ֵܽڵ�û�ж���ĺ�ɫ�ڵ㡣��ת���������
		//     fa               bro
		//  bro   t    =>           fa
		//                              t
		// ��ת��t�����ֵܽڵ�Ϊ��ɫ
		if (bro->isFaLeft()) {
			bro->rotateRight(root);  //����
			bro->isRed = false;  // ����Ⱦɫ
			bro->right->isRed = true;
		}
		else {
			bro->rotateLeft(root);  //����
			bro->isRed = false;  // ����Ⱦɫ
			bro->left->isRed = true;
		}
	}
	//���涼���ֵܽڵ�Ϊ��ɫ�����
	//�ֵܽڵ�һ����Ϊ�ա���Ϊ��ǰ��ɾ���ڵ�Ϊ��ɫ�����ݺ���������ʣ����ڵ����һ������������һ����ɫ�ڵ㡣
	bro = t->getBrother();
	//���2���ֵܽڵ�������ӽڵ㶼�Ǻ�ɫ
	if ((bro->left == nullptr || !bro->left->isRed)
		&& (bro->right == nullptr || !bro->right->isRed)) {
		//���ֵܽڵ��ɺ�ɫ��ʹ������������������·������һ����ɫ�ڵ㡣
		//��ǰ���� t->fa ƽ���ˣ������������������������һ����ɫ�ġ�
		bro->isRed = true;
		deleteFix(t->fa); //���ϵݹ�
		return;
	}
	//��tΪt->fa�����ӽڵ㡣
	if (t->isFaLeft()) {
		//���3��bro�����ӽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ
		//       fa ��                           fa ?
		//   t ��    bro ��       =>       t��          le��
		//         le��   ri��                              bro ��
		//                                                      ri��
		// �������������4���Ҳ�Ϊ��
		if (bro->left != nullptr && bro->left->isRed && (bro->right == nullptr || !bro->right->isRed)) {
			bro->left->isRed = false;
			bro->isRed = true;
			bro->left->rotateRight(root);
			bro = t->getBrother();
		}
		//���4:bro�����ӽڵ�Ϊ��ɫ
		//       fa ��                           bro ?
		//   t ��    bro ��     ==>         fa ��    ri ��
		//                ri��           t ��
		//��תbro��Ⱦɫ��ʹ����һ����ɫ�ڵ�
		if (bro->right != nullptr && bro->right->isRed) {
			bro->isRed = t->fa->isRed;
			t->fa->isRed = false;
			bro->right->isRed = false;
			bro->rotateLeft(root);
		}
	}
	else {
		//���3��bro�����ӽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ
		//            fa ��                               fa?
		//     bro ��       t ��             =>      ri��         t��
		// le��   ri��                            bro��
		//                                     le��
		// �������������4�����Ϊ��
		if (bro->right != nullptr && bro->right->isRed && (bro->left == nullptr || !bro->left->isRed)) {
			bro->right->isRed = false;
			bro->isRed = true;
			bro->right->rotateLeft(root);
			bro = t->getBrother();
		}
		//���4:bro�����ӽڵ�Ϊ��ɫ
		//       fa ��                           bro ?
		//   bro ��    t ��     ==>         le ��    fa��
		// le��                                          t ��
		//��תbro��Ⱦɫ��ʹ�Ҳ��һ����ɫ�ڵ�
		if (bro->left != nullptr && bro->left->isRed) {
			bro->isRed = t->fa->isRed;
			t->fa->isRed = false;
			bro->left->isRed = false;
			bro->rotateRight(root);
		}
	}
}

int RBTree::deleteNode(int val) {
	RBNode* t = find(val);
	if (t == nullptr)
		return 0;
	//����BST��ɾ��
	if (t->right == nullptr && t->left != nullptr) {
		t->right = t->left;
		t->left = nullptr;
	}
	while (t->right != nullptr) {
		//�ҵ������̽ڵ�
		RBNode* temp = getNextNode(t->right);
		t->val = temp->val;
		t = temp; //���´�ɾ���Ľڵ�
	}
	if (t->isLeaf()) {
		deleteFix(t);
		if (t == root) {
			root = nullptr;
		}
		else {
			if (t->isFaLeft()) {
				t->fa->left = nullptr;
			}
			else {
				t->fa->right = nullptr;
			}
		}
		delete t;
	}
	else {
		//��Ӧ�ó���������
		throw "deleteNode : error";
	}
	--size;
	return 1;
}

std::vector<RBNode*> RBTree::inorderTraversal(RBNode* root) {
	std::vector<RBNode*> res;
	std::stack<RBNode*> stk;
	while (root != nullptr || !stk.empty()) {
		while (root != nullptr) {
			stk.push(root);
			root = root->left;
		}
		root = stk.top();
		stk.pop();
		res.push_back(root);
		root = root->right;
	}
	return res;
}

void RBTree::printTree(RBTree &tree) {
	RBNode* root = tree.root;
	if (!root)return;
	auto tmp = root;
	std::vector<RBNode*> intv = inorderTraversal(tmp);//��������ڵ�����
	std::string template_str;//ģ��string����ʾÿ�д�ӡstring�ĳ���
	int location = 0;
	std::unordered_map<RBNode*, int> first_locations;//�洢�ڵ��Ӧ�ڱ���string�е���λ��
	for (auto& i : intv) {
		location = template_str.size();
		template_str += std::to_string(i->val) + (i->isRed ? "R" : "B") + " ";
		first_locations[i] = location;
	}
	for (auto& i : template_str)i = ' ';//��ģ��ȫ����Ϊ�ո񷽱����ʹ��
	//�������
	std::queue<RBNode*> q;
	q.push(root);
	while (!q.empty()) {
		int currentLevelSize = q.size();
		int cur_loc = 0;
		std::string tmp_str1 = template_str, tmp_str2 = template_str;//1Ϊ�ڵ������У�2Ϊ����һ��
		for (int i = 1; i <= currentLevelSize; ++i) {
			auto node = q.front();
			q.pop();
			cur_loc = first_locations[node];
			std::string num_str = std::to_string(node->val) + (node->isRed ? "R" : "B");
			//��ߣ�����������ӣ���ô�ڵڶ��ж�Ӧλ�ô�ӡ'/'���ڵ�һ�в���'_'
			if (node->left) {
				q.push(node->left);
				int first_loc = first_locations[node->left] + 1;
				tmp_str2[first_loc++] = '/';
				while (first_loc < cur_loc)tmp_str1[first_loc++] = '_';

			}
			//�м�,��Ӧλ�ô�ӡ�ڵ�ֵ���п���Ϊ��λ����
			for (int j = 0; j < num_str.length(); ++j, ++cur_loc) {
				tmp_str1[cur_loc] = num_str[j];
			}
			//�ұߣ���������Һ��ӣ���ô�ڵڶ��ж�Ӧλ�ô�ӡ'\'���ڵ�һ�в���'_'
			if (node->right) {
				q.push(node->right);
				int last_loc = first_locations[node->right] - 1;
				tmp_str2[last_loc] = '\\';
				while (cur_loc < last_loc) {
					tmp_str1[cur_loc++] = '_';
				}
			}
		}
		//��ӡ����
		std::cout << tmp_str1 << std::endl;
		std::cout << tmp_str2 << std::endl;
	}
}