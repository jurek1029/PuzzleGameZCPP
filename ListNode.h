#pragma once
class ListNode
{
public:
	int value;
	ListNode* prev;
	ListNode* next;
	ListNode(int _value, ListNode* _prev, ListNode* _next) :value(_value), prev(_prev), next(_next) {}
};

