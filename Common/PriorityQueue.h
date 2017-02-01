/*
 * PriorityQueue.h
 *
 *  Created on: Feb 1, 2017
 *      Author: tom
 */

#ifndef COMMON_PRIORITYQUEUE_H_
#define COMMON_PRIORITYQUEUE_H_

#include <utility>
#include <queue>
#include <vector>

using std::pair;
using std::priority_queue;
using std::vector;

template<typename T, typename priority_t>
struct PriorityQueue
{
	typedef pair<priority_t, T> PQElement;
	priority_queue<PQElement, vector<PQElement>, std::greater<PQElement>> elements;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void put( T item, priority_t priority )
	{
		elements.emplace( priority, item );
	}

	inline T get()
	{
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

#endif /* COMMON_PRIORITYQUEUE_H_ */
