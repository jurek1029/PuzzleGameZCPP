#pragma once
#include "ListNode.h";
class List
{
public:
	ListNode* start;
	ListNode* end;

	List();
	void Add(int c);
	ListNode* Pop();
};

