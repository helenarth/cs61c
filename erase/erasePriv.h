/*
 *  erasePriv.h
 *  erase
 *
 *  Created by Nathan brummel on 9/26/12.
 *  Copyright 2012 Berkeley. All rights reserved.
 *
 */

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

class erasePriv
{
	public:
		void HelloWorldPriv(const char *);
};

#pragma GCC visibility pop
