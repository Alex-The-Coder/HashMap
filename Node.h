#ifndef NODE_H_
#define NODE_H_

template<typename K, typename V>
class Node
{
private:
	long hash;
	K key;
	V value;
	Node<K, V>* prev;
	Node<K, V>* next;
public:
	Node(long hash, K& key, V& value, Node<K, V>* prev = nullptr, Node<K, V>* next = nullptr)
	{
		this->hash = hash;
		this->key = key;
		this->value = value;
		this->prev = prev;
		this->next = next;
	}

	Node<K, V>* deepCopy(Node<K, V>* deepPrev)
	{
		Node<K, V>* newNode = new Node<K, V>(hash, key, value, deepPrev);
		if (next != nullptr)
		{
			newNode->getNext() = next->deepCopy(newNode);
		}

		return newNode;
	}

	bool equals(long hash, K& key)
	{
		return this->hash == hash && this->key == key;
	}

	K& getKey()
	{
		return this->key;
	}

	V& getValue()
	{
		return this->value;
	}

	Node<K, V>*& getPrev()
	{
		return this->prev;
	}

	Node<K, V>*& getNext()
	{
		return this->next;
	}
};

#endif /* NODE_H_ */
