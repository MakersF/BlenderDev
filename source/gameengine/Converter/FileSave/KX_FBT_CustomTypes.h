/*
struct DataList
	{
		DataList*   next;
		DataList*   prev;
		void*	 data;
	};

class fbtDataList
{
public:

	fbtDataList() : first(0), last(0) {}
	~fbtDataList() { clear(); }

	void clear(void) { first = last = 0; }

	void push_back(void* v)
	{
		DataList* datalist = ((DataList*)v);
		if (!datalist)
			return;

		datalist->prev = last;
		if (last)
			last->next = datalist;

		if (!first)
			first = datalist;

		last = datalist;
	}

public:

	DataList*   first;
	DataList*   last;

};*/