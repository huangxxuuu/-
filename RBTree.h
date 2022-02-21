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
		// 对当前节点和父节点的右旋，要求当前节点为父节点的左子节点
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
		// 对当前节点和父节点的左旋，要求当前节点为父节点的右子节点
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


//单纯用来学习红黑树
//使用int val做key，不包含其他数据项
class RBTree {
	RBNode* root;
	int size;
	RBNode* findHelp(RBNode* ro, int val);
	void insertFix(RBNode* t);
	RBNode* getNextNode(RBNode* ro); // 用于找到当前子树的最小值
	void deleteFix(RBNode* t);
	static std::vector<RBNode*> inorderTraversal(RBNode* root);

public:
	// 所有节点的值不能相同
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
	//插入操作插入的是红色节点。
	//每条路径上的黑色节点数目没变。
	//需要调整的是不能两个红色节点相连
	if (t->fa == nullptr) { // 当前节点为root
		t->isRed = false;
		return;
	}
	if (t->fa->isRed == false) { //情况1：父节点为黑色，不需要修复
		return;
	}
	//下面的情况都是父节点为红色
	RBNode* uncle = t->getUncle();
	if (uncle != nullptr && t->getUncle()->isRed) { //情况2：叔叔节点为红色，只需要重新染色，然后向上递归
		t->fa->isRed = false;
		t->getUncle()->isRed = false;
		t->fa->fa->isRed = true;
		insertFix(t->fa->fa);
		return;
	}
	//下面的情况都是叔叔节点为黑色（空节点也是黑色）。
	//只需要关注当前节点t，父节点t->fa和祖父节点t->fa->fa。因为父节点为红色，肯定存在祖父节点
	// 情况3：t t->fa t->fa->fa 呈之字形。调整之后进入情况4
	if (t->isFaLeft() != t->fa->isFaLeft()) {
		if (t->isFaLeft()) {
			//   t->fa->fa 黑色
			//        t->fa 红色
			//   t 红色
			// 对t 右旋
			t->rotateRight(root);
			// 将当前节点改为原来的父节点
			t = t->right;
		}
		else {
			//       t->fa->fa 黑色
			//    t->fa 红色
			//       t 红色
			// 对t 左旋
			t->rotateLeft(root);
			// 将当前节点改为原来的父节点
			t = t->left;
		}

	}
	//情况4：t t->fa t->fa->fa 呈一条直线
	if (t->isFaLeft() == t->fa->isFaLeft()) {
		if (t->isFaLeft()) {
			//       t->fa->fa 黑色
			//     t->fa 红色
			//  t 红色
			// 对t->fa右旋
			t->fa->rotateRight(root);
			//重新染色
			t->fa->isRed = false;
			t->fa->right->isRed = true;

		}
		else {
			// t->fa->fa 黑色
			//     t->fa 红色
			//        t 红色
			// 对t->fa左旋
			t->fa->rotateLeft(root);
			//重新染色
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

RBNode* RBTree::getNextNode(RBNode* ro) { // 获得子树中的最小的节点
	//要求ro!=nullptr。（应该由调用者检查）
	if (ro == nullptr)
		throw "deleteNextNode: ro == nullptr";
	while (ro->left != nullptr) {
		ro = ro->left;
	}
	return ro;
}

void RBTree::deleteFix(RBNode* t) { // 修复表示，以t为根节点的子树比整棵树的其他区域少一个黑色节点
	if (t->isRed) { // 删除红色节点不需要修复
		t->isRed = false;  // 染色是为了多次递归调用这个deleteFix时，t不一定是真正删除的那个节点了
		return;
	}

	if (t->fa == nullptr) { //树的根节点不需要修复
		return;
	}
	// 下面都是待删除节点为黑色的情况
	// 因为删除一个黑色节点，所以当前子树上黑色节点少了一个。
	// 修复的目标是想办法从兄弟节点处拿一个过来，或使兄弟节点少一个
	RBNode* bro = t->getBrother();
	// 情况1：兄弟节点为红色。
	if (bro != nullptr && bro->isRed) { // 红色的兄弟节点没有多余的黑色节点。旋转成其他情况
		//     fa               bro
		//  bro   t    =>           fa
		//                              t
		// 旋转后，t的新兄弟节点为黑色
		if (bro->isFaLeft()) {
			bro->rotateRight(root);  //右旋
			bro->isRed = false;  // 重新染色
			bro->right->isRed = true;
		}
		else {
			bro->rotateLeft(root);  //右旋
			bro->isRed = false;  // 重新染色
			bro->left->isRed = true;
		}
	}
	//下面都是兄弟节点为黑色的情况
	//兄弟节点一定不为空。因为当前待删除节点为黑色，根据红黑树的性质，父节点的另一颗子树至少有一个黑色节点。
	bro = t->getBrother();
	//情况2：兄弟节点的两个子节点都是黑色
	if ((bro->left == nullptr || !bro->left->isRed)
		&& (bro->right == nullptr || !bro->right->isRed)) {
		//将兄弟节点变成红色，使它的两个子树的所有路径都少一个黑色节点。
		//当前子树 t->fa 平衡了，但是相比树的其他区域少了一个黑色的。
		bro->isRed = true;
		deleteFix(t->fa); //向上递归
		return;
	}
	//若t为t->fa的左子节点。
	if (t->isFaLeft()) {
		//情况3：bro的左子节点为红色，右子节点为黑色
		//       fa ？                           fa ?
		//   t 黑    bro 黑       =>       t黑          le黑
		//         le红   ri黑                              bro 红
		//                                                      ri黑
		// 将它调整成情况4，右侧为红
		if (bro->left != nullptr && bro->left->isRed && (bro->right == nullptr || !bro->right->isRed)) {
			bro->left->isRed = false;
			bro->isRed = true;
			bro->left->rotateRight(root);
			bro = t->getBrother();
		}
		//情况4:bro的右子节点为红色
		//       fa ？                           bro ?
		//   t 黑    bro 黑     ==>         fa 黑    ri 黑
		//                ri红           t 黑
		//旋转bro并染色，使左侧多一个黑色节点
		if (bro->right != nullptr && bro->right->isRed) {
			bro->isRed = t->fa->isRed;
			t->fa->isRed = false;
			bro->right->isRed = false;
			bro->rotateLeft(root);
		}
	}
	else {
		//情况3：bro的左子节点为黑色，右子节点为红色
		//            fa ？                               fa?
		//     bro 黑       t 黑             =>      ri黑         t黑
		// le黑   ri红                            bro红
		//                                     le黑
		// 将它调整成情况4，左侧为红
		if (bro->right != nullptr && bro->right->isRed && (bro->left == nullptr || !bro->left->isRed)) {
			bro->right->isRed = false;
			bro->isRed = true;
			bro->right->rotateLeft(root);
			bro = t->getBrother();
		}
		//情况4:bro的左子节点为红色
		//       fa ？                           bro ?
		//   bro 黑    t 黑     ==>         le 黑    fa黑
		// le红                                          t 黑
		//旋转bro并染色，使右侧多一个黑色节点
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
	//正常BST的删除
	if (t->right == nullptr && t->left != nullptr) {
		t->right = t->left;
		t->left = nullptr;
	}
	while (t->right != nullptr) {
		//找到中序后继节点
		RBNode* temp = getNextNode(t->right);
		t->val = temp->val;
		t = temp; //更新待删除的节点
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
		//不应该出现在这里
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
	std::vector<RBNode*> intv = inorderTraversal(tmp);//中序遍历节点数组
	std::string template_str;//模板string，表示每行打印string的长度
	int location = 0;
	std::unordered_map<RBNode*, int> first_locations;//存储节点对应在本行string中的首位置
	for (auto& i : intv) {
		location = template_str.size();
		template_str += std::to_string(i->val) + (i->isRed ? "R" : "B") + " ";
		first_locations[i] = location;
	}
	for (auto& i : template_str)i = ' ';//把模板全部置为空格方便后续使用
	//层序遍历
	std::queue<RBNode*> q;
	q.push(root);
	while (!q.empty()) {
		int currentLevelSize = q.size();
		int cur_loc = 0;
		std::string tmp_str1 = template_str, tmp_str2 = template_str;//1为节点所在行，2为其下一行
		for (int i = 1; i <= currentLevelSize; ++i) {
			auto node = q.front();
			q.pop();
			cur_loc = first_locations[node];
			std::string num_str = std::to_string(node->val) + (node->isRed ? "R" : "B");
			//左边，如果存在左孩子，那么在第二行对应位置打印'/'，在第一行补上'_'
			if (node->left) {
				q.push(node->left);
				int first_loc = first_locations[node->left] + 1;
				tmp_str2[first_loc++] = '/';
				while (first_loc < cur_loc)tmp_str1[first_loc++] = '_';

			}
			//中间,对应位置打印节点值（有可能为多位数）
			for (int j = 0; j < num_str.length(); ++j, ++cur_loc) {
				tmp_str1[cur_loc] = num_str[j];
			}
			//右边，如果存在右孩子，那么在第二行对应位置打印'\'，在第一行补上'_'
			if (node->right) {
				q.push(node->right);
				int last_loc = first_locations[node->right] - 1;
				tmp_str2[last_loc] = '\\';
				while (cur_loc < last_loc) {
					tmp_str1[cur_loc++] = '_';
				}
			}
		}
		//打印两行
		std::cout << tmp_str1 << std::endl;
		std::cout << tmp_str2 << std::endl;
	}
}