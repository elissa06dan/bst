#include "BST.h"
#include <algorithm>

static void* g_bst_root = nullptr;

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right): 
	keyValuePair({key, value}), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::~Node()
{
	delete left;
	delete right;
}

BinarySearchTree::Node::Node(const Node &other): keyValuePair(other.keyValuePair), parent(nullptr), right(nullptr), left(nullptr)
{
	if (other.left)
	{
		left = new Node(*other.left);
		left -> parent = this;
	}
	if (other.right)
	{
		right = new Node(*other.right);
		right -> parent = this;
	}
}

bool BinarySearchTree::Node::operator==(const Node &other) const
{
	return (other.keyValuePair.first == keyValuePair.first && other.keyValuePair.second == keyValuePair.second);
}

void BinarySearchTree::Node::output_node_tree() const
{
	if (left)
	{
		left -> output_node_tree();
	}
	std::cout << "Key: " << keyValuePair.first << " Value: " << keyValuePair.second << std::endl;
	if (right && right -> keyValuePair.first != std::numeric_limits<Key>::max())
	{
		right -> output_node_tree();
	}
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
	if (key < keyValuePair.first)
	{
		if (!left)
		{
			left = new Node(key, value, this);
		}
		else
		{
			left -> insert(key, value);
		}
	}
	else if (key > keyValuePair.first)
	{
		if (!right)
		{
			right = new Node(key, value, this);
		}
		else
		{
			right -> insert(key, value);
		}
	}
	else
	{
		Node *current = this;
		while (current -> right && current -> keyValuePair.first == key)
		{
			current = current -> right;
		}
		current -> right = new Node(key, value, current);
	}
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
	if (!_root)
	{
		_root = new Node(key, value);
		_root -> right = new Node(std::numeric_limits<Key>::max(), 0);
		_root -> right -> parent = _root;
		_size = 1;
	}
	else
	{
		if (key >= max() -> first)
		{
			_root -> erase(std::numeric_limits<Key>::max());
			_root -> insert(key, value);
			_root -> insert(std::numeric_limits<Key>::max(), 0);
		}
		else
		{
			_root -> insert(key, value);
		}
		_size++;
	}
}

BinarySearchTree::Node *BinarySearchTree::Node::erase_child(Node *target)
{
	if (!target->left && !target->right)
	{
		delete target;
		return nullptr;
	}
	else if (!target->left)
	{
		Node *temp = target->right;
		temp->parent = target->parent;
		target->left = nullptr;
		target->right = nullptr;
		delete target;
		return temp;
	}
	else if (!target->right)
	{
		Node *temp = target->left;
		temp->parent = target->parent;
		target->left = nullptr;
		target->right = nullptr;
		delete target;
		return temp;
	}
	else
	{
		Node *successor = target->right;
		while (successor->left)
		{
			successor = successor->left;
		}

		target->keyValuePair = successor->keyValuePair;

       
		if (successor->parent->left == successor)
		{
			successor->parent->left = erase_child(successor);
		}
		else
		{
			successor->parent->right = erase_child(successor);
		}
		return target;
    }
}


void BinarySearchTree::Node::erase(const Key &key)
{
	if (key < keyValuePair.first)
	{
		if (left)
		{
			if (left -> keyValuePair.first == key)
			{
				left = erase_child(left);
			}
			else
			{
				left -> erase(key);
			}
		}
		else
		{
			return;
		}
	}
	else if (key > keyValuePair.first)
	{
		if (right)
		{
			if (right -> keyValuePair.first == key)
			{
				right = erase_child(right);
			}
			else
			{
				right -> erase(key);
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}

void BinarySearchTree::erase(const Key &key)
{
	if (!_root) 
	{
		return;
	}
	while (find(key) != end())
	{
		if (_root)
		{
			if (_root -> keyValuePair.first == key)
			{
				_root = Node::erase_child(_root);
				if (_root)
				{
					_root -> parent = nullptr;
				}
			}
			else
			{
				_root -> erase(key);
			}
			_size--;
		}
	} 
}


BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
	: _size(other._size), _root(other._root ? new Node(*other._root) : nullptr) {}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
	if (this != &other) {
		while (_root) erase(_root->keyValuePair.first);
		_size = other._size;
		_root = other._root ? new Node(*other._root) : nullptr;
	}
	return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept : _root(other._root), _size(other._size)
{
	other._root = nullptr;
	other._size = 0;
}


BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
	if (this != &other) {
		while (_root) erase(_root->keyValuePair.first);
		_root = other._root;
		_size = other._size;
		other._root = nullptr;
		other._size = 0;
	}
	return *this;
}



BinarySearchTree::~BinarySearchTree()
{
	delete _root;
	_size = 0;
}

BinarySearchTree::Iterator::Iterator(Node *node): _node(node) {}
std::pair<Key, Value>& BinarySearchTree::Iterator::operator*()
{
	return _node -> keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const
{
	return _node -> keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->()
{
	return &(_node -> keyValuePair);
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const
{
	return &(_node -> keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
	if (!_node) return *this;
	if (_node->right) {
		_node = _node->right;
		while (_node->left) _node = _node->left;
	} else {
		Node* parent = _node->parent;
		while (parent && _node == parent->right) {
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
	Iterator tmp = *this;
	++(*this);
	return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
	if (!_node) {
		Node* current = static_cast<Node*>(g_bst_root);
		if (!current) return *this;
		while (current->right) current = current->right;
		_node = current;
		return *this;
	}
	if (_node->left) {
		_node = _node->left;
		while (_node->right) _node = _node->right;
	} else {
		Node* parent = _node->parent;
		while (parent && _node == parent->left) {
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
	Iterator tmp = *this;
	--(*this);
	return tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
	return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
	return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node): _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const
{
	return _node -> keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const
{
	return &(_node -> keyValuePair);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
	if (!_node) return *this;
	if (_node->right) {
		_node = _node->right;
		while (_node->left) _node = _node->left;
	} else {
		const Node* parent = _node->parent;
		while (parent && _node == parent->right) {
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
	ConstIterator tmp = *this;
	++(*this);
	return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
	if (!_node) {
		const Node* current = static_cast<const Node*>(g_bst_root);
		if (!current) return *this;
		while (current->right) current = current->right;
		_node = current;
		return *this;
	}
	if (_node->left) {
		_node = _node->left;
		while (_node->right) _node = _node->right;
	} else {
		const Node* parent = _node->parent;
		while (parent && _node == parent->left) {
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
	ConstIterator tmp = *this;
	--(*this);
	return tmp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
	return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
	return _node != other._node;
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
	Node *current = _root;
	while (current)
	{
		if (key < current -> keyValuePair.first)
		{
			current = current -> left;
		}
		else if (current->keyValuePair.first < key)
		{
			current = current -> right;
		}
		else
		{
			return ConstIterator(current);
		}
	}
	return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
	Node *current = _root;
	while (current)
	{
		if (key < current -> keyValuePair.first)
		{
			current = current -> left;
		}
		
		else if (current->keyValuePair.first < key)
		{
			current = current -> right;
		}
		
		else
		{
			return Iterator(current);
		}
	}
	return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key)
{
	Iterator itfirst = find(key);
	if (itfirst == end())
	{
		return std::make_pair(itfirst, end());
	}
	Iterator itsecond = itfirst;
	while (itsecond != end() && (*itsecond).first == key)
	{
		itsecond++;
	}
	return std::make_pair(itfirst, itsecond);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const
{
	ConstIterator itfirst = find(key);
	if (itfirst == cend())
	{
		return std::make_pair(itfirst, cend());
	}
	ConstIterator itsecond = itfirst;
	while (itsecond != cend() && (*itsecond).first == key)
	{
		itsecond++;
	}
	return std::make_pair(itfirst, itsecond);
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
	if (_root)
	{
		Node *current = _root;
		while (current -> left)
		{
			current = current -> left;
		}
		return ConstIterator(current);
	}
	return cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
	if (_root)
	{
		Node *current = _root;
		while (current -> right)
		{
			current = current -> right;
		}
		return ConstIterator(current -> parent);
	}
	return cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{   
	ConstIterator result = cend();
	if (_root)
	{
		Value minimum = std::numeric_limits<Value>::max();
		std::pair<ConstIterator, ConstIterator> r = equalRange(key);
		auto p1 = r.first;
		auto p2 = r.second;
		
		while (p1 != p2)
		{
			if (minimum > p1 -> second)
			{
				minimum = p1 -> second;
				result = p1;
			}
			p1++;
		}
	}
	return result;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{   
	ConstIterator result = cend();
	if (_root)
	{
		Value maximum = std::numeric_limits<Value>::lowest();
		std::pair<ConstIterator, ConstIterator> r = equalRange(key);
		auto p1 = r.first;
		auto p2 = r.second;
		
		while (p1 != p2)
		{
			if (maximum < p1 -> second)
			{
				maximum = p1 -> second;
				result = p1;
			}
			p1++;
		}
	}
	return result;
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
	Node *current = _root;
	if (current)
	{
		while (current -> left)
		{
			current = current -> left;
		}
		return Iterator(current);
	}
	return end();
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
	if (!_root)
	{
		throw std::logic_error("Tree is empty");
	}
	Node *current = _root;
	while (current -> right && current -> right -> keyValuePair.first != std::numeric_limits<Key>::max())
	{
		current = current -> right;
	}
	if (current -> right && current -> right -> keyValuePair.first == std::numeric_limits<Key>::max())
	{
		return Iterator(current -> right);
	}
	return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
	Node *current = _root;
	if (current)
	{
		while (current -> left)
		{
			current = current -> left;
		}
		return ConstIterator(current);
	}
	return cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
	if (!_root)
	{
		throw std::logic_error("Tree is empty");
	}
	Node *current = _root;
	while (current -> right && current -> right -> keyValuePair.first != std::numeric_limits<Key>::max())
	{
		current = current -> right;
	}
	if (current -> right && current -> right -> keyValuePair.first == std::numeric_limits<Key>::max())
	{
		return ConstIterator(current -> right);
	}
	return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const
{
	return _size;
}

void BinarySearchTree::output_tree()
{
	if (_root)
	{
		_root -> output_node_tree();
	}
}

size_t BinarySearchTree::traversing(Node *node) const
{
	if (!node)
	{
		return 0;
	}
	size_t left_height = traversing(node -> left);
	size_t right_height = traversing(node -> right);
	return std::max(left_height, right_height) + 1;
}

size_t BinarySearchTree::max_height() const
{
	if (!_root)
	{
		return 0;
	}
	return traversing(_root);
}

std::ostream& operator<<(std::ostream& os, const BinarySearchTree::Iterator& it)
{
	std::pair<Key, Value> p = *it;
	os << "(" << p.first << ", " << p.second << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const BinarySearchTree::ConstIterator& it)
{
	std::pair<Key, Value> p = *it;
	os << "(" << p.first << ", " << p.second << ")";
	return os;
}


int BinarySearchTree::height(Node* node) {
	return node ? node->height : 0;
}

void BinarySearchTree::updateHeight(Node* node) {
	if (node) {
		node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int BinarySearchTree::balanceFactor(Node* node) {
	return node ? height(node->left) - height(node->right) : 0;
}

BinarySearchTree::Node* BinarySearchTree::rotateRight(Node* y) {
	Node* x = y->left;
	Node* T2 = x->right;

	x->right = y;
	y->left = T2;

	if (T2) T2->parent = y;
	x->parent = y->parent;
	y->parent = x;

	updateHeight(y);
	updateHeight(x);

	return x;
}

BinarySearchTree::Node* BinarySearchTree::rotateLeft(Node* x) {
	Node* y = x->right;
	Node* T2 = y->left;

	y->left = x;
	x->right = T2;

	if (T2) T2->parent = x;
	y->parent = x->parent;
	x->parent = y;

	updateHeight(x);
	updateHeight(y);

	return y;
}

BinarySearchTree::Node* BinarySearchTree::rebalance(Node* node) {
	updateHeight(node);
	int balance = balanceFactor(node);

	if (balance > 1) {
		if (balanceFactor(node->left) < 0) {
			node->left = rotateLeft(node->left);
		}
		return rotateRight(node);
	}

	if (balance < -1) {
		if (balanceFactor(node->right) > 0) {
			node->right = rotateRight(node->right);
		}
		return rotateLeft(node);
	}

	return node; 
}

BinarySearchTree::Node* BinarySearchTree::avl_insert(Node* node, const Key &key, const Value &value) {
	if (!node) {
		_size++;
		return new Node(key, value);
	}

	if (key < node->keyValuePair.first) {
		node->left = avl_insert(node->left, key, value);
		if (node->left) node->left->parent = node;
	} else if (key > node->keyValuePair.first) {
		node->right = avl_insert(node->right, key, value);
		if (node->right) node->right->parent = node;
	} else {
		node->keyValuePair.second = value;
		return node;
	}

	return rebalance(node);
}

void BinarySearchTree::avl_insert(const Key &key, const Value &value) {
	_root = avl_insert(_root, key, value);
	if (_root) _root->parent = nullptr;
}

BinarySearchTree::Node* BinarySearchTree::avl_erase(Node* node, const Key &key) {
	if (!node) return nullptr;

	if (key < node->keyValuePair.first) {
		node->left = avl_erase(node->left, key);
		if (node->left) node->left->parent = node;
	} else if (key > node->keyValuePair.first) {
		node->right = avl_erase(node->right, key);
		if (node->right) node->right->parent = node;
	} else {
		if (!node->left || !node->right) {
			Node* temp = node->left ? node->left : node->right;
			if (temp) temp->parent = node->parent;
			delete node;
			_size--;
			return temp;
        }

		Node* pred = node->left;
		while (pred->right) pred = pred->right;

		node->keyValuePair = pred->keyValuePair;
		node->left = avl_erase(node->left, pred->keyValuePair.first);
		if (node->left) node->left->parent = node;
	}

	return rebalance(node);
}

void BinarySearchTree::avl_erase(const Key &key) {
	_root = avl_erase(_root, key);
	if (_root) _root->parent = nullptr;
}
	
