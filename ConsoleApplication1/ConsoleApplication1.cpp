// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <map>
#include <limits>

template<typename K, typename V>
class interval_map {
	std::map<K, V> m_map;

public:
	// constructor associates whole range of K with val by inserting (K_min, val)
	// into the map
	interval_map(V const& val) {
		m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
	}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {

		//std::cout << "Assigning [" << keyBegin << "," << keyEnd << "," << val << ")" << std::endl;
		if (!(keyBegin < keyEnd)) return;

		using it = typename std::map<K, V>::iterator;
		it itlow = m_map.lower_bound(keyBegin);
		it inik = itlow;
		it start = m_map.size() > 0 ? itlow-- : itlow;
		it init = start != m_map.end() ? start : itlow;
		if (init == m_map.end()) {
			m_map.insert(m_map.end(), std::make_pair(keyBegin, val));
			init = m_map.lower_bound(keyBegin);
		}

		it endkl = m_map.lower_bound(keyEnd);
		it endku = m_map.upper_bound(keyEnd);
		it endk = endkl == endku ? --endkl : endkl;

		K secondtKey = endk->first;
		V secondValue = endk->second;

		m_map.erase(inik, ++endk);

		if (secondtKey < keyEnd) {
			m_map.insert(m_map.begin(), std::make_pair(keyEnd, secondValue));
		}
		else
		{
			m_map.insert(m_map.begin(), std::make_pair(secondtKey, secondValue));
		}

		m_map.insert(m_map.begin(), std::make_pair(keyBegin, val));
		m_map.insert(m_map.begin(), std::make_pair(keyEnd, val));

		K firstKey = itlow != m_map.end() ? itlow->first : keyBegin;
		V firstValue = itlow != m_map.end() ? itlow->second : val;

		init = ++m_map.find(firstKey);
		endk = m_map.upper_bound(keyEnd);
		if (endk != m_map.end()) endk++;

		while (init != endk) {
			if (init->second == firstValue) {
				it discard = init;
				init++;
				m_map.erase(discard);
			}
			else {
				firstValue = init->second;
				init++;
			}
		}
	}

	void test_interval_map() {
		m_map.insert(std::make_pair(1, 'b'));
		m_map.insert(std::make_pair(4, 'a'));
		m_map.insert(std::make_pair(7, 'd'));
		m_map.insert(std::make_pair(10, 'e'));
		m_map.insert(std::make_pair(13, 'a'));
	}

	// a print function for debugging
	void show() {
		std::cout << "show" << std::endl;
		for (auto entry : m_map) {
			std::cout << entry.first << entry.second << std::endl;			
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		return (--m_map.upper_bound(key))->second;
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of unsigned int intervals to char.


int main()
{
	interval_map<unsigned int, char> imap{ 'a' };

	imap.test_interval_map();
	imap.show();

	std::cout << std::endl;

	//imap.assign(8, 10, 'k');
	//imap.assign(8, 12, 'k');
	//imap.assign(2, 12, 'k');
	//imap.assign(2, 12, 'b');
	//imap.assign(5, 12, 'b');
	//imap.assign(4, 10, 'b');
	//imap.assign(4, 12, 'b');
	//imap.assign(4, 13, 'b');
	//imap.assign(4, 14, 'b');
	//imap.assign(7, 13, 'a');
	//imap.assign(0, 10, 'e');
	//imap.assign(0, 10, 'k');
	//imap.assign(0, 10, 'a');
	//imap.assign(1, 13, 'a');
	//imap.assign(4, 7, 'e');
	//imap.assign(7, 7, 'e');
	imap.assign(8, 7, 'e');
	imap.show();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
