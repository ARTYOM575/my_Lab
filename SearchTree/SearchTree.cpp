#include<iostream>
#include <stdio.h>
#include<vector>
#include<string>
#include <fstream>
using namespace std;

template <class T>
class SearchTree
{
public:
	class iterator {
	public:
		T* key_ = new T;
		iterator* l_link = nullptr;
		iterator* r_link = nullptr;
		iterator* parent = nullptr;
	};

	iterator* my_list = nullptr;

	//constructor
	SearchTree() {}

	SearchTree(T key) {
		insert(key);
	}

	SearchTree(std::initializer_list <T> list) {
		for (auto* it = list.begin(); it != list.end(); it++)
			insert(*it);
	}

	SearchTree(SearchTree& copy_my_list)
	{
		cout << "call copy constructor" << endl;
		int h = get_height(copy_my_list.my_list);
		int i;
		for (i = 1; i <= h; i++)
			copy(copy_my_list.my_list, i);
	}

	SearchTree(SearchTree&& mov_my_list) {

		cout << "call move constructor " << endl;
		this->my_list = mov_my_list.my_list;
		mov_my_list.my_list = nullptr;
		//mov_my_list.clear(mov_my_list.my_list);
	}

	~SearchTree() {
		clear(my_list);
	}

	void copy(const iterator* my_list_, int line)
	{
		//copy for copy constructor and = operator
		if (my_list_ == NULL)
			return;
		if (line == 1)
			insert(*(my_list_->key_));
		else if (line > 1) {
			copy(my_list_->l_link, line - 1);
			copy(my_list_->r_link, line - 1);
		}
	}

	//operador
	void  operator=(SearchTree <T>& ob) {
		cout << "cal copy operator" << endl;
		iterator* copy_my_list = ob.my_list;
		int h = get_height(copy_my_list);
		int i;
		for (i = 1; i <= h; i++)
			copy(copy_my_list, i);
	}

	void operator=(SearchTree <T>&& ob) {

		cout << "cal mov operator" << endl;
		this->my_list = ob.my_list;
		ob.my_list = nullptr;
	}

	friend ostream& operator << (ostream& os, SearchTree <T>& ob)
	{
		func(os, ob.my_list);
		return os;
	}

	bool operator ==(SearchTree& ob)
	{
		vector <T> list1 = my_levelorder(ob.my_list);
		vector <T> list2 = my_levelorder(this->my_list);
		if (list1.size() != list2.size())
			return false;
		for (int i = 0; i < list1.size(); ++i)
			if (list1[i] != list2[i])
				return false;
		return true;
	}

	bool operator !=(const SearchTree& ob) {
		vector <T> list1 = my_levelorder(ob.my_list);
		vector <T> list2 = my_levelorder(this->my_list);
		if (list1.size() != list2.size())
			return true;
		for (int i = 0; i < list1.size(); ++i)
			if (list1[i] != list2[i])
				return true;
		return false;
	}

	void operator +=(const SearchTree& ob) {
		vector <T> list = my_levelorder(ob.my_list);
		for (short int i = 0; i < list.size(); ++i)
			insert(list[i]);
	}

	friend SearchTree& operator +(SearchTree& ob1, SearchTree& ob2)
	{
		SearchTree <T>* obbb = new SearchTree;
		vector <T> list1 = my_levelorder(ob1.my_list);
		vector <T> list2 = my_levelorder(ob2.my_list);

		for (int i = 0; i < list1.size(); ++i)
			obbb->insert(list1[i]);
		for (int i = 0; i < list2.size(); ++i)
			obbb->insert(list2[i]);
		return *obbb;
	}

	static void func(ostream& os, iterator* my_list_) {
		//func for << operator
		if (my_list_ == nullptr)
			return;
		func(os, my_list_->r_link);
		func(os, my_list_->l_link);
		if (my_list_->key_ == nullptr)
			return;
		os << "key             " << *(my_list_->key_) << endl;
		os << "length address  " << my_list_->l_link << endl;
		os << "right address   " << my_list_->r_link << endl;
		os << "parent address  " << my_list_->parent << endl << endl;
	}

	static vector <T> my_levelorder(iterator* my_list_)
	{
		//levelorder_for_equal for operator  ==   &&   !=   &&   +=
		vector <T> return_list;
		int h = get_height(my_list_);
		int i;
		for (i = 1; i <= h; i++)
		{
			currentline_for_equal(my_list_, i, return_list);
		}
		return return_list;
	}

	static void currentline_for_equal(iterator* my_list_, int line, vector <T>& vec)
	{
		//currentline function for levelorder
		if (my_list_ == NULL)
			return;
		if (line == 1)
			vec.push_back(*(my_list_->key_));
		else if (line > 1) {
			currentline_for_equal(my_list_->l_link, line - 1, vec);
			currentline_for_equal(my_list_->r_link, line - 1, vec);
		}
	}


	//function
	void insert(T key)
	{
		iterator* new_iterator = new iterator;
		*(new_iterator->key_) = key;
		if (my_list == nullptr || my_list->key_ == nullptr)
		{
			my_list = new_iterator;
			return;
		}
		iterator* it = my_list;
		while (true)
		{
			if (*(it->key_) == key)
				return;
			if (*(it->key_) < key)
			{
				if (it->r_link == nullptr)
				{
					it->r_link = new_iterator;
					new_iterator->parent = it;
					return;
				}
				it = it->r_link;
			}
			else {
				if (it->l_link == nullptr)
				{
					it->l_link = new_iterator;
					new_iterator->parent = it;
					return;
				}
				it = it->l_link;
			}
		}
	}

	void clear(iterator* my_list_) {
		if (my_list_)
		{
			clear(my_list_->r_link);
			clear(my_list_->l_link);
			my_list_->key_ = nullptr;
			my_list_->l_link = nullptr;
			my_list_->r_link = nullptr;
			my_list_->parent = nullptr;
			my_list_ = nullptr;
		}
	}

	static int get_height(const iterator* my_list_)
	{
		if (my_list_ == nullptr)
			return 0;
		else {
			int lside = get_height(my_list_->l_link);
			int rside = get_height(my_list_->r_link);
			if (lside > rside)
				return (lside + 1);
			else
				return (rside + 1);
		}
	}

	void del(iterator* it)
	{
		it->key_ = nullptr;
		it->l_link = nullptr;
		it->r_link = nullptr;
		it = nullptr;
	}

	void erase(iterator* my_list_, T key) {
		iterator* clear_it = find(key);
		if (clear_it == nullptr) {
			cout << "key doesn't find";
			return;
		}
		if (clear_it->l_link == nullptr && clear_it->r_link == nullptr) {
			if (my_list_ == clear_it) {
				my_list_ = nullptr;
				del(clear_it);
				return;
			}

			if (clear_it->parent->l_link == clear_it) {
				clear_it->parent->l_link = nullptr;
				clear_it->parent->l_link = nullptr;
			}
			else {
				clear_it->parent->r_link = nullptr;
			}
			del(clear_it);
			return;
		}

		if (clear_it->l_link == nullptr) {
			if (*(my_list->key_) == key) {
				my_list_ = clear_it->r_link;
				del(clear_it);
				return;
			}
			clear_it->r_link->parent = clear_it->parent;
			if (clear_it->parent->l_link == clear_it) {
				clear_it->parent->l_link = clear_it->r_link;
			}
			else {
				clear_it->parent->r_link = clear_it->r_link;
			}
			del(clear_it);
			return;
		}

		if (clear_it->r_link == nullptr) {
			if (*(my_list_->key_) == key) {
				my_list_ = clear_it->l_link;
				del(clear_it);
				return;
			}

			clear_it->l_link->parent = clear_it->parent;
			if (clear_it->parent->l_link == clear_it) {
				clear_it->parent->l_link = clear_it->l_link;
			}
			else {
				clear_it->parent->r_link = clear_it->l_link;
			}
			del(clear_it);
			return;
		}

		iterator* next = clear_it;
		while (next->l_link != nullptr) {
			next = next->l_link;
		}

		int keep = *(clear_it->key_);
		*(clear_it->key_) = *(next->key_);
		*(next->key_) = keep;

		if (next->r_link != nullptr) {
			next->r_link->parent = next->parent;
		}
		next->parent->l_link = next->r_link;
		del(next);
	}

	int get_number_of_nodes(const iterator* my_list_)
	{
		if (my_list_) {
			int counntr = get_number_of_nodes(my_list_->r_link);
			int counntl = get_number_of_nodes(my_list_->l_link);
			return (counntr + counntl + 1);
		}
		return 0;
	}

	void cout_key(T key_)
	{
		//cout_key function for preprder(), inorder(), levelorder()
		cout << key_ << "  ";
	}

	void preorder(iterator* my_list_) {
		if (my_list_ != nullptr) {
			cout_key(*(my_list_->key_));
			preorder(my_list_->l_link);
			preorder(my_list_->r_link);
		}
	}

	void inorder(iterator* my_list_)
	{
		if (my_list_ == NULL)
			return;
		inorder(my_list_->l_link);
		cout_key(*(my_list_->key_));
		inorder(my_list_->r_link);
	}

	void postorder(iterator* my_list_)
	{
		if (my_list_ == nullptr)
			return;
		postorder(my_list_->l_link);
		postorder(my_list_->r_link);
		cout_key(*(my_list_->key_));
	}

	void levelorder(iterator* my_list_)
	{
		int h = get_height(my_list_);
		int i;
		for (i = 1; i <= h; i++)
		{
			currentline(my_list_, i);
			cout << endl;

		}
	}

	void currentline(iterator* my_list_, int line)
	{
		//currentline function for levelorder
		if (my_list_ == NULL)
			return;
		if (line == 1)
			cout << *(my_list_->key_) << "  ";
		else if (line > 1) {
			currentline(my_list_->l_link, line - 1);
			currentline(my_list_->r_link, line - 1);
		}
	}

	void get_root_data(iterator* my_list_) {
		if (my_list_ == nullptr)
			return;
		cout << endl;

		cout << "the address of this class             " << my_list_ << endl;
		cout << "the address of the list on the left   " << my_list_->l_link << endl;
		cout << "the address of the list on the rigth  " << my_list_->r_link << endl;
		cout << "class address of the parent           " << my_list_->parent << endl;
		cout << "value                                 " << *(my_list_->key_) << endl;
		get_root_data(my_list_->l_link);
		get_root_data(my_list_->r_link);
	}

	bool contains(int key)
	{
		iterator* my_list_ = my_list;
		while (my_list_) {
			if (*(my_list_->key_) == key)
				return true;
			my_list_ = (key < *(my_list_->key_)) ?
				my_list_->l_link :
				my_list_->r_link;
		}
		return false;
	}

	void currentline(iterator* my_list_, int line, vector <T>& vec_) {
		// curerentline for element
		if (my_list_ == NULL)
			return;
		if (line == 1)
			vec_.push_back(*(my_list_->key_));
		else if (line > 1) {
			currentline(my_list_->l_link, line - 1, vec_);
			currentline(my_list_->r_link, line - 1, vec_);
		}
	}

	void element(iterator* my_list_, vector <T>& vec) {
		//element for merge
		int h = get_height(my_list_);
		int i;
		for (i = 1; i <= h; i++)
		{
			currentline(my_list_, i, vec);
		}

	}

	void  merge(iterator* my_list2)
	{
		vector <T> vector_merge;
		element(my_list2, vector_merge);
		for (int i = 0; i < vector_merge.size(); ++i)
			insert(vector_merge[i]);
	}

	iterator* find(int key) const {
		iterator* current = my_list;
		while (current) {
			if (*(current->key_) == key)
				return current;
			current = (key < *(current->key_)) ?
				current->l_link :
				current->r_link;
		}
		return 0;
	}

};

int main()
{
	return 0;
}
