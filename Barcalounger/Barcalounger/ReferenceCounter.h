#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H

class ReferenceCounter
{
public:
	ReferenceCounter()
	{
		refCount = 1;
	}

	inline int GetReferenceCount() { return refCount; }

	inline void AddReference() { refCount++; }
	inline bool RemoveReference() { refCount--; return refCount == 0; }
protected:
private:
	int refCount;
};

#endif // REFERENCECOUNTER_H
