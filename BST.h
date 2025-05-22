#pragma once
#include <iostream>
#include <stdexcept>
#include <limits>
#include <utility>
#include <cstdint>
#include <string>

using Key = uint32_t;
using Value = double;

class BinarySearchTree
{
	struct Node 
	{
		Node(Key key, Value value, Node *parent = nullptr, Node *left = nullptr, Node *right = nullptr);
		~Node();

		Node(const Node &other);
		bool operator==(const Node &other) const;

		void output_node_tree() const;
		void insert(const Key &key, const Value &value);
		void erase(const Key &key);
		static Node *erase_child(Node *target);

		std::pair<Key, Value> keyValuePair;
		Node *parent = nullptr;
		Node *left = nullptr;
		Node *right = nullptr;

		int height = 1;
	};

public:
	BinarySearchTree() = default;
	explicit BinarySearchTree(const BinarySearchTree &other);
	BinarySearchTree &operator=(const BinarySearchTree &other);
	explicit BinarySearchTree(BinarySearchTree &&other) noexcept;
	BinarySearchTree &operator=(BinarySearchTree &&other) noexcept;
	~BinarySearchTree();

	class Iterator 
	{
		public:
			explicit Iterator(Node *node);
			std::pair<Key, Value> &operator*();
			const std::pair<Key, Value> &operator*() const;
			std::pair<Key, Value> *operator->();
			const std::pair<Key, Value> *operator->() const;
			Iterator operator++();
			Iterator operator++(int);
			Iterator operator--();
			Iterator operator--(int);
			bool operator==(const Iterator &other) const;
			bool operator!=(const Iterator &other) const;
			friend std::ostream& operator<<(std::ostream& os, const Iterator& it);

		private:
			Node *_node;
	};

	class ConstIterator 
	{
		public:
			explicit ConstIterator(const Node *node);
			const std::pair<Key, Value> &operator*() const;
			const std::pair<Key, Value> *operator->() const;
			ConstIterator operator++();
			ConstIterator operator++(int);
			ConstIterator operator--();
			ConstIterator operator--(int);
			bool operator==(const ConstIterator &other) const;
			bool operator!=(const ConstIterator &other) const;
			friend std::ostream& operator<<(std::ostream& os, const ConstIterator& it);

		private:
			const Node *_node;
	};

	void insert(const Key &key, const Value &value);
	void erase(const Key &key);
	Node* get_root();
	size_t traversing(Node *node) const;
	ConstIterator find(const Key &key) const;
	Iterator find(const Key &key);
	std::pair<Iterator, Iterator> equalRange(const Key &key);
	std::pair<ConstIterator, ConstIterator> equalRange(const Key &key) const;
	ConstIterator min() const;
	ConstIterator max() const;
	ConstIterator min(const Key &key) const;
	ConstIterator max(const Key &key) const;
	Iterator begin();
	Iterator end();
	ConstIterator cbegin() const;
	ConstIterator cend() const;
	size_t size() const;
	void output_tree();
	size_t max_height() const;

	void avl_insert(const Key &key, const Value &value);
	void avl_erase(const Key &key);

private:
	size_t _size = 0;
	Node *_root = nullptr;

	Node* avl_insert(Node* node, const Key &key, const Value &value);
	Node* avl_erase(Node* node, const Key &key);
	int height(Node* node);
	int balanceFactor(Node* node);
	void updateHeight(Node* node);
	Node* rotateRight(Node* y);
	Node* rotateLeft(Node* x);
	Node* rebalance(Node* node);
};

