#pragma once

#include <list>

/** Class implementing a memory pool for fast allocation of objects.
*	The class is templated on the type of the object.
*/
template<class T>
class MemPool
{
public:
	MemPool();
	MemPool(int chunksize);
	~MemPool();

	void init(int chunksize);
	T* allocate(int n);

private:
	int mChunkSize;				///< Memory chunk size.
	T* mCurrentChunk;			///< Pointer to the currently active chunk.
	int mCurrentPos;			///< Position of free memory.
	std::list<T*> mChunks;		///< Memory chunks.

};

// Include the implementation of the templated member functions.
/** Default constructor. */
template<class T>
MemPool<T>::MemPool()
	: mChunkSize(1000)
	, mCurrentPos(mChunkSize)
	, mCurrentChunk(0)
{
}


/** Constructor initializing the chunk size. */
template<class T>
MemPool<T>::MemPool(int chunksize)
	: mChunkSize(chunksize)
	, mCurrentPos(mChunkSize)
	, mCurrentChunk(0)
{
}


/** Destructor. Deletes all memory used by the memory pool. */
template<class T>
MemPool<T>::~MemPool()
{
	typename std::list<T*>::iterator itr = mChunks.begin();
	for (; itr != mChunks.end(); ++itr)
		if (*itr) delete[] * itr;
	mChunks.clear();
}


/** Clears all previously allocated memory and sets the chunk size. */
template<class T>
void MemPool<T>::init(int chunksize)
{
	typename std::list<T*>::iterator itr = mChunks.begin();
	for (; itr != mChunks.end(); ++itr)
		if (*itr) delete[] * itr;
	mChunks.clear();
	mCurrentChunk = 0;
	mChunkSize = mCurrentPos = chunksize;
}


/** Allocate an array of n objects, returning a pointer to the array. */
template<class T>
T* MemPool<T>::allocate(int n)
{
	T* chunk;
	if (mChunkSize - mCurrentPos < n)
	{
		// There is not enough room in the current chunk, allocate a new.
		int amount = n > mChunkSize ? n : mChunkSize;
		chunk = mCurrentChunk = new T[amount];
		mChunks.push_back(mCurrentChunk);
		mCurrentPos = n;
		//		std::cout << "allocating a new chunk of size " << amount << std::endl;
	}
	else
	{
		chunk = &mCurrentChunk[mCurrentPos];
		mCurrentPos += n;
	}
	return chunk;
}