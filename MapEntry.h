#ifndef MAPENTRY_H_
#define MAPENTRY_H_

#include "Node.h"

template<typename K, typename V>
class MapEntry
{
private:
	Node<K, V>* backing;
public:
	MapEntry(Node<K, V>*& backingNode)
	{
		this->backing = backingNode;
	}

	bool isEmpty()
	{
		return backing == nullptr;
	}

	K& getKey()
	{
		if (backing == nullptr)
		{
			throw 2;
		}
		return backing->getKey();
	}

	V& getValue()
	{
		if (backing == nullptr)
		{
			throw 2;
		}
		return backing->getValue();
	}
};

#endif /* MAPENTRY_H_ */
