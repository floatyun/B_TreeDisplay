/**
 * @file B_Tree.h
 * @brief B��ģ��ʵ��
 * @data 2019��1��8��
 * @author daoyunmaihua
*/
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Direct2dQuickStart.h"
using namespace std;

/**
 * B_Tree B��ʵ��

 * T��Ԫ������
 *
 * comparator��һ�����ͣ������ʵ���� int operator()(const T&x,const T&y)
 * ʵ���˷º����Ĺ���
 * ��ʵ����T����֮��ıȽϣ�B������ѭ����ȽϹ���
 * ����ȽϺ���Ӧ����xС��yʱ���ظ�����x����yʱ����0��x����yʱ��������
 *
 * printer��һ�����ͣ�Ҳ�Ƿº����������ʵ����
 * print(ostream&,const T&) ʵ����T���͵�չʾ��ʽ������֧�ֱ�����
 * string to_string(const T&) ʵ����T����ת��Ϊ�ַ����ı�ʾ����
 *
 * @exception std::invalid_argument what()�Ǿ��������ʾ
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

		// �ڵ�posλ�õ�����
		inline node_pointer left(int pos) {
			return child[pos];
		}
		// �ڵ�posλ�õ��Һ���
		inline node_pointer right(int pos) {
			return child[pos + 1];
		}
		// ���غ�����Ŀ
		inline int count_child() {
			return key_cnt + 1;
		}
		inline bool is_leaf() {
			return nullptr == child[0];
		}
		// ��ȡ�Լ��Ǹ��׵ļ��ź�����(����˵�Ǽ��Źؼ��ֵ�����)
		inline int get_child_id();
		// ��posλ�ò���key,ͬʱ����posλ�õ��Һ���Ϊr_child
		void insert(const value_type&key0, int pos, node_pointer r_child = nullptr);
		// ɾ��posλ�õ�key����r_child
		void remove(int pos);
		// key0��child0�ֱ𱣴��������ͷ�Ķ�Ӧ����Ϣ
		void cut_head(value_type&key0, node_pointer &child0);
		// key0��child0�ֱ𱣴��������β�Ķ�Ӧ����Ϣ
		void cut_tail(value_type&key0, node_pointer &child0);
		// ��������������Ϣ���ӵ���һ�ڵ��ͷ��
		void link_head(value_type&key0, node_pointer child0);
		// ��������������Ϣ���ӵ���һ�ڵ��β��
		void link_tail(value_type&key0, node_pointer child0);

		// Ѱ��key0 posָ����ʼ����λ�ã������ҵ���λ��
		// ������ʱposΪkey_cnt
		void find(const value_type&key0, comparator &cmp, int &pos);
		// Ѱ��key���Բ���λ�õ�����
		void upper_bound(const value_type&key0, comparator &cmp, int &pos);
	};

	// �²���һ��node
	node_pointer new_node();
	// ����һ��node
	void rm_node(node_pointer r);
	// �ͷŶ�̬��������пռ�
	void destory(node_pointer r = root);
public:
	// min_deg:ָ����С��������
	B_Tree(int min_deg = 2);
	~B_Tree();

	// �������ƶ���������������ķ�ʽ����B��
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

	// pre:itΪ���ڵ㣬it�ĸ���Ϊ�ջ��߸���Ϊ�����ڵ�
	// ���ص�ʱ���Ѻ���Ұ벿�ֵĽڵ��ָ��
	node_pointer split(node_pointer it);
	// ����it�����ڵ����ֵ�
	node_pointer l_sibling(node_pointer it);
	// ����it�����ڵ����ֵ�
	node_pointer r_sibling(node_pointer it);
	// �����ֵܽ�һ��
	// ������ܽ裬�򷵻�nullptr
	// ������Խ裬����it
	node_pointer move_from_l_sibling(node_pointer it);
	// �����ֵܽ�һ��
	// ������ܽ裬�򷵻�nullptr
	// ������Խ裬����it
	node_pointer move_from_r_sibling(node_pointer it);
	// �� �� it �ϲ���һ���ڵ�
	// ���غϲ���Ľڵ�Ľڵ�ָ��
	// ������ܺϲ�������nullptr
	// �����ϲ����¸��ڵ��ɿ��ˣ������ո��ڵ㡣��ʵ����������������ڵ��Ǹ�ʱ������
	node_pointer merge_with_l_sibling(node_pointer it);
	// it �� �� �ϲ���һ���ڵ�
	// ���غϲ���Ľڵ�Ľڵ�ָ��
	// ������ܺϲ�������nullptr
	// �����ϲ����¸��ڵ��ɿ��ˣ������ո��ڵ㡣��ʵ����������������ڵ��Ǹ�ʱ������
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
			f << "error:" << r->child[i] << "���Ǹ��׵Ķ���" << " p= " << r << '\n';
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
	// pre:itΪ���ڵ㣬it�ĸ���Ϊ�ջ��߸���Ϊ�����ڵ�
	if (it == root) {
		root = new_node();
		it->p = root; // ������1
		root->child[0] = it; // ������1
	}
	int id = it->get_child_id();
	node_pointer right_part = new_node();
	int mid = it->key_cnt / 2;

	bool flag = !it->is_leaf(); // it��right_partͬ��
	right_part->child[0] = it->right(mid); // ������2
	if (flag)
		it->right(mid)->p = right_part; // ������2
	for (int i = mid + 1; i < it->key_cnt; ++i) {
		right_part->insert(it->key[i], right_part->key_cnt, it->right(i));  // ������3
		// ��Ҫ�����Ұ벿�ֵĺ��ӵĸ��׽ڵ��ָ��
		if (flag)
			it->right(i)->p = right_part; // ������3
	}

	// ֻ��root�ĸ���Ϊ�գ���Ϊǰ���Ѿ�������it��p��Ȼ�ǿ�
	it->p->insert(it->key[mid], id, right_part); // ������4
	right_part->p = it->p; // ������4
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
	l->insert(p->key[id], l->key_cnt, r->child[0]);		// ������1
	if (nullptr != r->child[0])
		r->child[0]->p = l;								// ������1
	p->remove(id);  // ����ɾ���ĸ�->�ӣ�r���ߣ�r�����ͷţ�����ɾ��r->p�ı�
	node_pointer t;
	for (int i = 0; i < r->key_cnt; ++i) {
		t = r->right(i);
		l->insert(r->key[i], l->key_cnt, t); // ������2
		if (nullptr != t)
			t->p = l;   // ������2
	}
	rm_node(r);
	if (p->key_cnt == 0) { // ʵ���ϣ�ֻ�е�p��root�ſ��ܳ����������
		// ���ˣ����սڵ㣬������root��ֵ
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
		throw invalid_argument(value_printer.to_string(key) + "�Ѵ��ڣ�������� ");
	node_pointer it = root;
	int pos;
	while (true) {
		it->upper_bound(key, cmp, pos);
		if (is_full(it)) {
			node_pointer right_part = split(it);
			if (pos >= it->key_cnt + 1) {// Ӧ���������Ѻ���Ұ벿��
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
		throw invalid_argument(value_printer.to_string(key) + "�����ڣ�����ɾ�� ");
	it = root;
	int pos;
	while (true) {
		if (is_thinnest(it)) {
			// Ӧ����it����
			// ��,���ֵ����һ��
			// ��,���ֵܵ�һ��
			// �󣬸���it�ϲ�
			// it,�����Һϲ�
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
			// �����˴�ɾ����Ԫ��
			--pos;
			if (should_be_leaf && (!it->is_leaf())) {
				//�϶�Ϊδ�ҵ���ɾ����Ԫ��
				//�����϶�Ϊ���������Ҫɾ����Ԫ��С������λ�õ��϶���
				it = it->child[pos];
				continue;
			}
			if (it->is_leaf()) {
				it->remove(pos);
				return;
			}
			// ��Ҷ�ӽڵ�ɾ��
			// ǰ���������������һ������ȻΪҶ�ӽڵ㣩��������ݹ�ɾ�����
			// ǰ����Ȼ���ڣ�����it��Ҷ�ӽڵ�
			node_pointer pre = it->left(pos);
			int pre_pos;
			while (nullptr != pre->child[pre->key_cnt])
				pre = pre->child[pre->key_cnt];
			pre_pos = pre->key_cnt - 1;

			//it->key[pos] = pre->key[pre_pos];
			//remove(pre->key[pre_pos], it->left(pos)); // ����ָ��start�����ٴ��ҵ�it->key[pos]����ɾ��

			//�����ε�bug
			//�޷��򵥵�ͨ���滻���ٵݹ����ɾ��
			//2019��1��8��11:39:36
			//�����滻���ٵݹ�ɾ���ǲ����еģ���Ϊ�滻������ڵ�Ĺؼ�����ݹ����ɾ��������ͬ�ģ��ٴεݹ�����ֻ��ҵ������
			//
			//����һ����ʱ��������pre->key[pre_pos]���ٵݹ����
			//���� auto tmp = pre->key[pre_pos]; remove(pre->key[pre_pos]);
			//�ڱ����ϱ������ٴ��ҵ����������⣬�������ǲ����еģ�
			//��Ϊ��㼰�ϲ��ڵ�Ĳ���ʹ���м���ܵ���it�ڵ��Ѿ����ͷŻ���<it,pos>�����ڴ��е�λ���Ѿ��䶯��
			//
			//�����滻��������һ��start=<it,pos>�����ӣ���ʾ������㿪ʼ�����ҵ�ݹ�ɾ��
			//�������Ǳ����ˢٵ����⣬����ʵ������Ϊ��㣬�ϲ��ڵ�����Ĵ��ڣ�ʹ��<it,pos>���������ƣ����µݹ�ɾ��ʱ���ɻ�����
			//������������滻,�滻����������B��������
			//Ȼ���ҵ�Ҫɾ����Ԫ�س���ֵ���ж��⻹Ҫ�����ж����Ƿ�ΪҶ�ڵ���ж�
			//��ʱ�����<it,pos>�����û���ƶ���Ҷ�ӽڵ㣬��<it,pos>��<pre,pre_pos>ͨ��Ҷ�ڵ���ж����ֿ�����
			//���<it,pos>������ƶ�����Ҷ�ӽڵ㣬�����滻����������B�������ʣ����еĲ�������������B���������²�����
			//��˸���B�������ʣ�<it,pos>��<pre,pre_pos>֮���Ԫ��ȫ����ͬһ��ֵ
			//��ЩԪ��������ɾ��һ��Ҷ�ӽ�㣨ֻҪά��B���ڵ�Ĵ�С�����ޣ�������һ���ġ�
			//���remove����һ��bool shouldBeLeaf����
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
		throw invalid_argument("get_child_id:��ǰ�ڵ�ĸ��ײ����ڣ��Ǹ��ڵ㡣");
	for (int i = 0; i <= p->key_cnt; ++i)
		if (this == p->child[i])
			return i;
	time(NULL);
	throw invalid_argument("get_child_id:���Ǹ��׽ڵ���κκ��ӣ���debug��");
}


template<class T, class comparator, class printer>
inline void B_Tree<T, comparator, printer>::node::insert(const value_type & key0, int pos, node_pointer r_child)
{
	// ��posλ�ò���key0 �����������ӽڵ�Ϊr_child
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
	// ɾ��key[pos] �����Һ��ӽڵ�
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