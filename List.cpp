#include "List.h"

List::List()
{
	start = nullptr;
	end = nullptr;
}

void List::Add(int c)
{
	if (start == nullptr){
		ListNode* node = new ListNode(c, nullptr, nullptr);
		start = end = node;
	}
	else {
		ListNode* node = new ListNode(c, end, nullptr);
		end->next = node;
		end = node;
	}
}

ListNode* List::Pop()
{
	ListNode* temp = end;
	if (end == nullptr) start = nullptr;
	else end = end->prev;
	return temp;
}
