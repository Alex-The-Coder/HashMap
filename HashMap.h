#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <functional>
#include "MapIterator.h"
using namespace std;

enum MapSize {SMALL, MEDIUM, LARGE, X_LARGE};

template<typename K, typename V>
class HashMap
{
private:
	Node<K, V>** buckets;
	long size;
	long count;
	function<long (K&)> hasher;
	long* mods = new long(0);

	unsigned long calculateHash(K& key)
	{
		return hasher(key);
	}

	unsigned long calculateBucket(long hash)
	{
		return hash & this->size;
	}
public:
	HashMap(function<long (K&)> hasher)
	{
		this->count = 0;
		this->size = 16;

		this->buckets = new Node<K, V>*[this->size];
		for (int i = 0; i < this->size; i++)
		{
			this->buckets[i] = nullptr;
		}

		this->hasher = hasher;
	}

	HashMap(MapSize size, function<long (K&)> hasher)
	{
		switch (size)
		{
		case SMALL:
			this->size = 8; // 2^3
			break;
		default: // default to medium
		case MEDIUM:
			this->size = 16; // 2^4
			break;
		case LARGE:
			this->size = 32; // 2^5
			break;
		case X_LARGE:
			this->size = 64; // 2^6
			break;
		}

		this->count = 0;
		this->buckets = new Node<K, V>*[this->size];
		for (int i = 0; i < this->size; i++)
		{
			this->buckets[i] = nullptr;
		}

		this->hasher = hasher;
	}

	~HashMap()
	{
		for (int i = 0; i < size; i++)
		{
			Node<K, V>* cur = this->buckets[i];
			while (cur != nullptr)
			{
				Node<K, V>* next = cur->getNext();
				delete cur;
				cur = next;
			}
		}
		delete[] buckets;
		delete mods;
	}

	HashMap(const HashMap<K, V>& orig)
	{
		this->count = orig.count;
		this->size = orig.size;
		this->hasher = orig.hasher;
		this->mods = new long(*orig.mods);

		this->buckets = new Node<K, V>*[this->size];
		for (int i = 0; i < this->size; i++)
		{
			if (orig.buckets[i] != nullptr)
			{
				this->buckets[i] = orig.buckets[i]->deepCopy(nullptr);
			}
			else
			{
				this->buckets[i] = nullptr;
			}
		}
	}

	HashMap<K, V>& operator=(const HashMap<K, V>& copy)
	{
		if (this != &copy)
		{
			for (int i = 0; i < size; i++)
			{
				Node<K, V>* cur = this->buckets[i];
				while (cur != nullptr)
				{
					Node<K, V>* next = cur->getNext();
					delete cur;
					cur = next;
				}
			}
			delete[] buckets;
			delete mods;

			this->count = copy.count;
			this->size = copy.size;
			this->hasher = copy.hasher;
			this->mods = new long(*copy.mods);

			this->buckets = new Node<K, V>*[this->size];
			for (int i = 0; i < this->size; i++)
			{
				if (copy.buckets[i] != nullptr)
				{
					this->buckets[i] = copy.buckets[i].deepCopy(nullptr);
				}
				else
				{
					this->buckets[i] = nullptr;
				}
			}
		}

		return *this;
	}

	bool containsKey(K& key)
	{
		long hash = calculateHash(key);
		long bucket = calculateBucket(hash);

		Node<K, V>* nodePtr = this->buckets[bucket];

		while (nodePtr != nullptr)
		{
			if (nodePtr->equals(hash, key))
			{
				return true;
			}
			nodePtr = nodePtr->getNext();
		}

		return false;
	}

	void removeKey(K& key)
	{
		long hash = calculateHash(key);
		long bucket = calculateBucket(hash);

		Node<K, V>* nodePtr = this->buckets[bucket];

		while (nodePtr != nullptr)
		{
			if (nodePtr->equals(hash, key))
			{
				if (nodePtr->getPrev() == nullptr)
				{
					this->buckets[bucket] = nodePtr->getNext();
				}
				else
				{
					nodePtr->getPrev()->getNext() = nodePtr->getNext();
				}
				delete nodePtr;
				count--;
				(*mods)++;
				return;
			}
			nodePtr = nodePtr->getNext();
		}

		return;
	}

	void put(K& key, V& value)
	{
		long hash = calculateHash(key);
		long bucket = calculateBucket(hash);

		Node<K, V>* nodePtr = this->buckets[bucket];

		if (nodePtr == nullptr)
		{
			Node<K, V>* node = new Node<K, V>(hash, key, value);
			this->buckets[bucket] = node;
			count++;
			(*mods)++;
		}
		else
		{
			Node<K, V>* prev = nodePtr;
			while (nodePtr != nullptr)
			{
				if (nodePtr->equals(hash, key))
				{
					V val = value;
					nodePtr->getValue() = val;
					count++;
					(*mods)++;
					return;
				}
				prev = nodePtr;
				nodePtr = nodePtr->getNext();
			}
			prev->getNext() = new Node<K, V>(hash, key, value, prev);
			count++;
			(*mods)++;
		}
	}

	V& getValue(K& key)
	{
		long hash = calculateHash(key);
		long bucket = calculateBucket(hash);

		Node<K, V>* nodePtr = this->buckets[bucket];
		while (nodePtr != nullptr)
		{
			if (nodePtr->equals(hash, key))
			{
				return nodePtr->getValue();
			}
			nodePtr = nodePtr->getNext();
		}

		throw 1;
	}

	void iterate(function<void (K&, V&)> function)
	{
		for (int i = 0; i < this->size; i++)
		{
			Node<K, V>* nodePtr = this->buckets[i];
			while (nodePtr != nullptr)
			{
				K key = nodePtr->getKey();
				V val = nodePtr->getValue();
				function(key, val);

				nodePtr = nodePtr->getNext();
			}
		}
	}

	int getCount()
	{
		return count;
	}

	MapIterator<K, V> begin()
	{
		return MapIterator<K, V>(buckets, size, mods);
	}

	MapIterator<K, V> end()
	{
		return MapIterator<K, V>(buckets, size, mods, true);
	}
};

#endif /* HASHMAP_H_ */
