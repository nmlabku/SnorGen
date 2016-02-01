//######################################################################
//
//		uniqueCount.h
//		programmed by jinwan park
//		2010.01.04
//
//######################################################################
#ifndef __uniqueCount_h
#define __uniqueCount_h

#include "util.h"

// ������ �� ������ ����
//################################################
class UniqueContainer
{
public:
	int				id;				//�ο��� id
	int				val1;

	UniqueContainer		*next;

public:
	UniqueContainer() { memset(this, 0, sizeof(UniqueContainer)); }
	~UniqueContainer() {}

	void reset() { memset(this, 0, sizeof(UniqueContainer)); }

};
//################################################
class UniqueCount
{
public:
	UniqueContainer		*head;

	unsigned int		count;

public:
	UniqueCount() ;
	~UniqueCount();

	void reset();

	int insert( int val1);

	void print();

	void update(UniqueCount *p_cpUniqueCount);
};


#endif
