#ifndef MAPITERATOR_H_
#define MAPITERATOR_H_

#include "MapEntry.h"
#include <iostream>
using namespace std;

template<typename K, typename V>
class MapIterator
{
private:
	Node<K, V>** buckets;
	int size;
	int curIndex;
	long initialMods;
	long* totalMods;
	Node<K, V>* curNode;

	bool isComplete()
	{
		return curIndex >= size;
	}

	void findNextNode()
	{
		while (curNode == nullptr)
		{
			curIndex++;
			if (curIndex >= size)
			{
				break;
			}
			curNode = buckets[curIndex];
		}
	}

	void checkForModification()
	{
		if (initialMods != *totalMods)
		{
			throw 3;
		}
	}
public:
	MapIterator(Node<K, V>** buckets, int size, long* modifications, bool complete = false)
	{
		this->buckets = buckets;
		this->size = size;
		this->initialMods = *modifications;
		this->totalMods = modifications;

		if (complete)
		{
			this->curIndex = size;
			this->curNode = nullptr;
		}
		else
		{
			this->curIndex = 0;
			this->curNode = buckets[0];
			findNextNode();
		}
	}

	MapEntry<K, V> operator*()
	{
		checkForModification();
		return MapEntry<K, V>(curNode);
	}

	void operator++()
	{
		if (isComplete())
		{
			return;
		}
		checkForModification();
		if (curNode->getNext() == nullptr)
		{
			curNode = nullptr;
			findNextNode();
		}
		else
		{
			curNode = curNode->getNext();
		}
	}

	bool operator!=(const MapIterator<K, V>& rhs)
	{
		if (rhs.buckets != buckets)
		{
			return true;
		}
		if (rhs.size != size)
		{
			return true;
		}
		return rhs.curNode != curNode || rhs.curIndex != curIndex;
	}
};

#endif /* MAPITERATOR_H_ */
