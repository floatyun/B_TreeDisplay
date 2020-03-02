/**
 * @file B_Tree.h
 * @brief B树模板实现
 * @data 2019年1月8日
 * @author daoyunmaihua
*/
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Direct2dQuickStart.h"
using namespace std;

/**
 * B_Tree B树实现

 * T是元素类型
 *
 * comparator是一个类型，其必须实现了 int operator()(const T&x,const T&y)
 * 实现了仿函数的功能
 * 它实现了T类型之间的比较，B树将遵循这个比较规则
 * 这个比较函数应当在x小于y时返回负数，x等于y时返回0，x大于y时返回正数
 *
 * printer是一个类型，也是仿函数，其必须实现了
 * print(ostream&,const T&) 实现了T类型的展示方式。用于支持遍历。
 * string to_string(const T&) 实现了T类型转化为字符串的表示方法
 *
 * @exception std::invalid_argument what()是具体错误提示
*/

/*
 * begin for definitions
 */
template<class T, class comparator, class printer>
class B_Tree {
public:
	typedef T value_type;
	struct node;
	typedef node* node_pointer;
protected:
	struct node {
		int key_cnt;
		value_type *key;
		node_pointer *child;
		node_pointer p;

		// 节点pos位置的左孩子
		inline node_pointer left(int pos) {
			return child[pos];
		}
		// 节点pos位置的右孩子
		inline node_pointer right(int pos) {
			return child[pos + 1];
		}
		// 返回孩子数目
		inline int count_child() {
			return key_cnt + 1;
		}
		inline bool is_leaf() {
			return nullptr == child[0];
		}
		// 获取自己是父亲的几号孩子子(或者说是几号关键字的左孩子)
		inline int get_child_id();
		// 在pos位置插入key,同时设置pos位置的右孩子为r_child
		void insert(const value_type&key0, int pos, node_pointer r_child = nullptr);
		// 删除pos位置的key与其r_child
		void remove(int pos);
		// key0与child0分别保存剪下来的头的对应的信息
		void cut_head(value_type&key0, node_pointer &child0);
		// key0与child0分别保存剪下来的尾的对应的信息
		void cut_tail(value_type&key0, node_pointer &child0);
		// 将剪切下来的信息链接到另一节点的头部
		void link_head(value_type&key0, node_pointer child0);
		// 将剪切下来的信息链接到另一节点的尾部
		void link_tail(value_type&key0, node_pointer child0);

		// 寻找key0 pos指定开始查找位置，保存找到的位置
		// 不存在时pos为key_cnt
		void find(const value_type&key0, comparator &cmp, int &pos);
		// 寻找key可以插入位置的上限
		void upper_bound(const value_type&key0, comparator &cmp, int &pos);
	};

	// 新产生一个node
	node_pointer new_node();
	// 回收一个node
	void rm_node(node_pointer r);
	// 释放动态申请的所有空间
	void destory(node_pointer r = root);
public:
	// min_deg:指定最小度数因子
	B_Tree(int min_deg = 2);
	~B_Tree();

	// 按照类似二叉树的中序遍历的方式遍历B树
	void print(ostream&f, node_pointer r);
	void check(ostream &f, node_pointer r);

	void paint(DemoApp &app, int h, float l, float len, /*float p_mid, float id,*/ node_pointer r);

	void insert(const value_type&key);
	value_type *find(const value_type&key);
	void remove(const value_type&key, bool should_be_leaf = false);
	node_pointer get_root() {
		return root;
	}
protected:
	inline bool is_full(node_pointer r) {
		return r->key_cnt == max_key_cnt;
	}

	inline bool is_thinnest(node_pointer r) {
		return r != root && r->key_cnt == min_key_cnt;
	}

	// pre:it为满节点，it的父亲为空或者父亲为非满节点
	// 返回的时分裂后的右半部分的节点的指针
	node_pointer split(node_pointer it);
	// 返回it的相邻的左兄弟
	node_pointer l_sibling(node_pointer it);
	// 返回it的相邻的右兄弟
	node_pointer r_sibling(node_pointer it);
	// 从左兄弟借一个
	// 如果不能借，则返回nullptr
	// 如果可以借，返回it
	node_pointer move_from_l_sibling(node_pointer it);
	// 从右兄弟借一个
	// 如果不能借，则返回nullptr
	// 如果可以借，返回it
	node_pointer move_from_r_sibling(node_pointer it);
	// 左 父 it 合并成一个节点
	// 返回合并后的节点的节点指针
	// 如果不能合并，返回nullptr
	// 如果因合并导致父节点变成空了，则会回收父节点。事实上这种情况仅当父节点是根时发生。
	node_pointer merge_with_l_sibling(node_pointer it);
	// it 父 右 合并成一个节点
	// 返回合并后的节点的节点指针
	// 如果不能合并，返回nullptr
	// 如果因合并导致父节点变成空了，则会回收父节点。事实上这种情况仅当父节点是根时发生。
	node_pointer merge_with_r_sibling(node_pointer it);
	node_pointer merge_siblings(node_pointer l, node_pointer r);

protected:
	int min_deg;
	int max_deg;
	int min_key_cnt;
	int max_key_cnt;
	node_pointer root;
	comparator cmp;
	printer value_printer;
};
/*end for definitions*/


/* implements of methods area
 *  basic methods of b_tree
*/

template<class T, class comparator, class printer>
inline B_Tree<T, comparator, printer>::B_Tree(int min_deg)
{
	this->min_deg = min_deg;
	max_deg = 2 * min_deg;
	min_key_cnt = min_deg - 1;
	max_key_cnt = max_deg - 1;
	root = new_node();
}

template<class T, class comparator, class printer>
inline B_Tree<T, comparator, printer>::~B_Tree()
{
	destory(root);
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::print(ostream&f, node_pointer r)
{
	if (nullptr == r)
		return;
	for (int i = 0; i < r->key_cnt; ++i) {
		print(f, r->left(i));
		value_printer.print(f, r->key[i]);
	}
	print(f, r->child[r->key_cnt]);
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::check(ostream & f, node_pointer r)
{
	if (r == nullptr) return;
	if (r->key_cnt == 0) {
		f << "error:" << r << " node is empty." << '\n';
		f << r->key[0] << '\n';
	}
	for (int i = 0; i <= r->key_cnt; ++i) {
		if (r->child[i] != nullptr && r->child[i]->p != r) {
			f << "error:" << r->child[i] << "不是父亲的儿子" << " p= " << r << '\n';
			f << r->child[i]->key[0] << "\t" << r->key[0] << '\n';
		}
	}
	for (int i = 0; i <= r->key_cnt; ++i)
		check(f, r->child[i]);
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer B_Tree<T, comparator, printer>::l_sibling(node_pointer it)
{
	int id = it->get_child_id();
	if (id > 0)
		return it->p->child[id - 1];
	else
		return nullptr;
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer B_Tree<T, comparator, printer>::r_sibling(node_pointer it)
{
	int id = it->get_child_id();
	if (id < it->p->key_cnt)
		return it->p->child[id + 1];
	else
		return nullptr;
}


template<class T, class comparator, class printer>
inline void  B_Tree<T, comparator, printer>::destory(node_pointer r)
{
	if (nullptr == r)
		return;
	for (int i = 0; i < r->key_cnt; ++i)
		destory(r->left(i));
	destory(r->child[r->key_cnt]);
	rm_node(r);
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer B_Tree<T, comparator, printer>::new_node()
{
	node_pointer p = new node();
	p->key_cnt = 0;
	p->key = new value_type[max_key_cnt];
	p->child = new node_pointer[max_deg];
	p->child[0] = nullptr;
	p->p = nullptr;
	return p;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::rm_node(node_pointer r)
{
	delete[]r->child;
	delete[]r->key;
	delete r;
}


// basic methods of b_tree

//
// core functions area

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::split(node_pointer it)
{
	// pre:it为满节点，it的父亲为空或者父亲为非满节点
	if (it == root) {
		root = new_node();
		it->p = root; // 父拉子1
		root->child[0] = it; // 子拉父1
	}
	int id = it->get_child_id();
	node_pointer right_part = new_node();
	int mid = it->key_cnt / 2;

	bool flag = !it->is_leaf(); // it，right_part同层
	right_part->child[0] = it->right(mid); // 父拉子2
	if (flag)
		it->right(mid)->p = right_part; // 子拉父2
	for (int i = mid + 1; i < it->key_cnt; ++i) {
		right_part->insert(it->key[i], right_part->key_cnt, it->right(i));  // 父拉子3
		// 需要更新右半部分的孩子的父亲节点的指针
		if (flag)
			it->right(i)->p = right_part; // 子拉父3
	}

	// 只有root的父亲为空，因为前面已经处理，故it的p必然非空
	it->p->insert(it->key[mid], id, right_part); // 父拉子4
	right_part->p = it->p; // 子拉父4
	it->key_cnt = mid;
	return right_part;
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::move_from_l_sibling(node_pointer it)
{
	node_pointer l = l_sibling(it);
	if (nullptr == l || is_thinnest(l))
		return nullptr;
	node_pointer p = it->p;
	int id = l->get_child_id();
	value_type l_tail_val;
	node_pointer l_tail_child;
	l->cut_tail(l_tail_val, l_tail_child);
	it->link_head(p->key[id], l_tail_child);
	if (nullptr != l_tail_child)
		l_tail_child->p = it;
	p->key[id] = l_tail_val;
	return it;
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::move_from_r_sibling(node_pointer it)
{
	node_pointer r = r_sibling(it);
	if (nullptr == r || is_thinnest(r))
		return nullptr;
	node_pointer p = r->p;
	int id = it->get_child_id();
	value_type r_head_val;
	node_pointer r_head_child;
	r->cut_head(r_head_val, r_head_child);
	it->link_tail(p->key[id], r_head_child);
	if (nullptr != r_head_child)
		r_head_child->p = it;
	p->key[id] = r_head_val;
	return it;
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::merge_siblings(node_pointer l, node_pointer r)
{
	node_pointer p = r->p;
	int id = l->get_child_id();
	l->insert(p->key[id], l->key_cnt, r->child[0]);		// 父拉子1
	if (nullptr != r->child[0])
		r->child[0]->p = l;								// 子拉父1
	p->remove(id);  // 将会删除的父->子（r）边，r将会释放，无需删除r->p的边
	node_pointer t;
	for (int i = 0; i < r->key_cnt; ++i) {
		t = r->right(i);
		l->insert(r->key[i], l->key_cnt, t); // 父拉子2
		if (nullptr != t)
			t->p = l;   // 子拉父2
	}
	rm_node(r);
	if (p->key_cnt == 0) { // 实际上，只有当p是root才可能出现这种情况
		// 空了，回收节点，并更新root的值
		rm_node(p);
		root = l; root->p = nullptr;
	}
	return l;
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::merge_with_l_sibling(node_pointer it)
{
	node_pointer l = l_sibling(it);
	if (nullptr == l)
		return nullptr;
	return merge_siblings(l, it);
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::node_pointer
B_Tree<T, comparator, printer>::merge_with_r_sibling(node_pointer it)
{
	node_pointer r = r_sibling(it);
	if (nullptr == r)
		return nullptr;
	node_pointer p = r->p;
	return merge_siblings(it, r);
}

template<class T, class comparator, class printer>
inline typename B_Tree<T, comparator, printer>::value_type *
B_Tree<T, comparator, printer>::find(const value_type & key)
{
	node_pointer it = root;
	int pos;
	while (true) {
		it->upper_bound(key, cmp, pos);
		if (pos > 0 && cmp(key, it->key[pos - 1]) == 0)
			return it->key + (pos - 1);
		if (it->is_leaf())
			return nullptr;
		else
			it = it->child[pos];
	}
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::insert(const value_type & key)
{
	if (nullptr != find(key))
		throw invalid_argument(value_printer.to_string(key) + "已存在，无需插入 ");
	node_pointer it = root;
	int pos;
	while (true) {
		it->upper_bound(key, cmp, pos);
		if (is_full(it)) {
			node_pointer right_part = split(it);
			if (pos >= it->key_cnt + 1) {// 应该跳到分裂后的右半部分
				pos -= it->key_cnt + 1;
				it = right_part;
			}
		}
		// now it must be not full
		if (it->is_leaf()) {
			it->insert(key, pos);
			return;
		}
		else {
			it = it->child[pos];
		}
	}
}


template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::remove(const value_type & key, bool should_be_leaf)
{
	node_pointer it;
	if (nullptr == find(key))
		throw invalid_argument(value_printer.to_string(key) + "不存在，无需删除 ");
	it = root;
	int pos;
	while (true) {
		if (is_thinnest(it)) {
			// 应当让it变胖
			// 借,左兄弟最后一个
			// 借,右兄弟第一个
			// 左，父，it合并
			// it,父，右合并
			node_pointer tmp = nullptr;
			if (nullptr == tmp)
				tmp = move_from_l_sibling(it);
			if (nullptr == tmp)
				tmp = move_from_r_sibling(it);
			if (nullptr == tmp)
				tmp = merge_with_l_sibling(it);
			if (nullptr == tmp)
				tmp = merge_with_r_sibling(it);
			// now tmp must not be nullptr
			it = tmp;
		}
		// now it is not thinnest
		it->upper_bound(key, cmp, pos);
		if (pos > 0 && cmp(key, it->key[pos - 1]) == 0)
		{
			// 发现了待删除的元素
			--pos;
			if (should_be_leaf && (!it->is_leaf())) {
				//认定为未找到待删除的元素
				//并且认定为比这个不需要删除的元素小（基于位置的认定）
				it = it->child[pos];
				continue;
			}
			if (it->is_leaf()) {
				it->remove(pos);
				return;
			}
			// 非叶子节点删除
			// 前驱（左子树的最后一个（必然为叶子节点））替代，递归删除后继
			// 前驱必然存在，否则it是叶子节点
			node_pointer pre = it->left(pos);
			int pre_pos;
			while (nullptr != pre->child[pre->key_cnt])
				pre = pre->child[pre->key_cnt];
			pre_pos = pre->key_cnt - 1;

			//it->key[pos] = pre->key[pre_pos];
			//remove(pre->key[pre_pos], it->left(pos)); // 若不指定start将会再次找到it->key[pos]进行删除

			//最隐蔽的bug
			//无法简单的通过替换后再递归调用删除
			//2019年1月8日11:39:36
			//①先替换，再递归删除是不可行的，因为替换后这个节点的关键字与递归调用删除的是相同的，再次递归调用又会找到这个点
			//
			//先用一个临时变量备份pre->key[pre_pos]，再递归调用
			//例如 auto tmp = pre->key[pre_pos]; remove(pre->key[pre_pos]);
			//②表面上避免了再次找到这个点的问题，但依旧是不可行的，
			//因为借点及合并节点的操作使得中间可能导致it节点已经被释放或者<it,pos>的在内存中的位置已经变动了
			//
			//③先替换，在增加一个start=<it,pos>的左孩子，表示从这个点开始往下找点递归删除
			//表面上是避免了①的问题，但是实际上因为借点，合并节点操作的存在，使得<it,pos>这个点会下移，导致递归删除时依旧会碰到
			//解决方案，先替换,替换后依旧满足B树的性质
			//然后找到要删除的元素除了值得判定外还要增加判断其是否为叶节点的判定
			//此时，如果<it,pos>这个点没有移动到叶子节点，则<it,pos>与<pre,pre_pos>通过叶节点的判定区分开来了
			//如果<it,pos>这个点移动到了叶子节点，由于替换后依旧满足B树的性质，所有的操作都是在满足B树的性质下操作的
			//因此根据B树的性质，<it,pos>与<pre,pre_pos>之间的元素全部是同一个值
			//这些元素中任意删除一个叶子结点（只要维护B树节点的大小上下限），都是一样的。
			//因此remove增加一个bool shouldBeLeaf参数
			it->key[pos] = pre->key[pre_pos];
			value_type tmp_bac = pre->key[pre_pos];
			remove(tmp_bac, true);
			return;
		}
		else {
			it = it->child[pos];
		}
	}
}

//
// end of core functions area


//typename B_Tree<T, comparator,printer>::

// methods of node
template<class T, class comparator, class printer>
inline int B_Tree<T, comparator, printer>::node::get_child_id()
{
	if (nullptr == p)
		throw invalid_argument("get_child_id:当前节点的父亲不存在，是根节点。");
	for (int i = 0; i <= p->key_cnt; ++i)
		if (this == p->child[i])
			return i;
	time(NULL);
	throw invalid_argument("get_child_id:不是父亲节点的任何孩子，请debug。");
}


template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::insert(const value_type & key0, int pos, node_pointer r_child)
{
	// 在pos位置插入key0 并且设置右子节点为r_child
	for (int i = key_cnt; i > pos; --i) {
		key[i] = key[i - 1];
		child[i + 1] = child[i];
	}
	key[pos] = key0;
	child[pos + 1] = r_child;
	++key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::remove(int pos)
{
	// 删除key[pos] 和其右孩子节点
	for (; pos + 1 < key_cnt; ++pos) {
		key[pos] = key[pos + 1];
		child[pos + 1] = child[pos + 2];
	}
	--key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::find(const value_type & key0, comparator & cmp, int & pos)
{
	int d;
	for (; pos < key_cnt; ++pos) {
		d = cmp(key0, key[pos]);
		if (d == 0)
			return;
		if (d < 0)
			break;
	}
	pos = key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::upper_bound(const value_type & key0, comparator & cmp, int & pos)
{
	int d;
	for (pos = 0; pos < key_cnt; ++pos) {
		d = cmp(key0, key[pos]);
		if (d < 0)
			return;
	}
	pos = key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::cut_head(value_type & key0, node_pointer& child0)
{
	key0 = key[0];
	child0 = child[0];
	for (int i = 0; i + 1 < key_cnt; ++i)
		key[i] = key[i + 1];
	for (int i = 0; i + 1 <= key_cnt; ++i)
		child[i] = child[i + 1];
	--key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::cut_tail(value_type & key0, node_pointer& child0)
{
	key0 = key[key_cnt - 1];
	child0 = child[key_cnt];
	--key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::link_head(value_type & key0, node_pointer child0)
{
	for (int i = key_cnt; i > 0; --i)
		key[i] = key[i - 1];
	for (int i = key_cnt + 1; i > 0; --i)
		child[i] = child[i - 1];
	key[0] = key0;
	child[0] = child0;
	++key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::link_tail(value_type & key0, node_pointer child0)
{
	key[key_cnt] = key0;
	child[key_cnt + 1] = child0;
	++key_cnt;
}

template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::paint(DemoApp & app, int h, float l, float len,/* float p_mid, float id,*/ node_pointer r)
{
	if (nullptr == r)
		return;
	// draw node framework at first
	int count = r->count_child();
	if (r == root || r->p->child[r->p->key_cnt] != r)
		app.Draw_Node_Framework(h, l, l + len, count,false/*,p_mid,id*/);
	else
		app.Draw_Node_Framework(h, l, l + len, count, true/*,p_mid,id*/);
	// draw keys of r node
	float each_len = len / count;
	for (int i = 0; i < r->key_cnt; ++i) {
		string str = value_printer.to_string(r->key[i]);
		app.Draw_Text(str, h, l + i * each_len, l + (i + 1) * each_len);
	}
	for (int i = 0; i < r->key_cnt; ++i) {
		paint(app, h + 1, l + i * each_len, each_len,/*l+0.5*len,i,*/r->child[i]);
	}
	paint(app, h + 1, l + r->key_cnt * each_len, each_len, /*l + 0.5*len, r->key_cnt,*/ r->child[r->key_cnt]);
}

// end of  methods of node