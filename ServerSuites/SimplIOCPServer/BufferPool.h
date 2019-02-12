#pragma once

const unsigned int DEFAULT_LEN = 256;

class BufferPool;

class Buffer
{
public:
	Buffer(TBUFIDX idx)
		: _idx(idx)
	{

	}

	inline bool CheckInUse() { return bInUse; }
	
	inline void SetInUse() { bInUse = true; }
	inline void ResetInUse() { bInUse = false; }

	std::pair< size_t, char*> GetBuffer();
private:
	std::atomic_bool bInUse = false;

	TBUFIDX _idx;
	char buffer[BUFFER_LEN];
};
using PBuffer = std::shared_ptr<Buffer>;

using PREF_CNT = std::shared_ptr< std::atomic_int >;

class BuffAccessor
{
	friend BufferPool;
	BuffAccessor() = delete;
	BuffAccessor(TBUFIDX idx);
public:
	BuffAccessor(BuffAccessor&);
	BuffAccessor(BuffAccessor&&);

	~BuffAccessor();


	TBUFIDX GetIdx() { return _idx; }

	inline bool IsValid() { return _idx != INVALID_IDX; }


private:
	int GetRefRemain();
	static int GetRefRemain(TBUFIDX idx);

	void IncreaseRef();
	int DecreaseRef();

	TBUFIDX _idx;

	static std::shared_mutex		_mtxCont;
	static std::vector< PREF_CNT > _contRef;
};

struct BufInfo
{
public:
	char*	_pBuffer;
	size_t	_len;
};

class BufferPool
{
public:

	BufferPool();
	virtual ~BufferPool();

	BuffAccessor GetNewAccessor();

	char* GetBuffer(BuffAccessor key);
	BufInfo GetBufInfo(BuffAccessor key);
	void AccessorDestruct(TBUFIDX key);

	static BufferPool& GetInstance() 
	{
		static BufferPool _inst;
		return _inst;
	}

	size_t GetContainerLen();
	size_t GetAvailableCnt();
	bool AllAvailable();
private:
	void BufferRecycle(TBUFIDX key);


	void _grow_up();
	TBUFIDX GetAvailable();

	
	std::shared_mutex		_mtxCont;
	std::vector	< PBuffer > _cont;
	
	std::mutex				_mtxAvailable;
	std::list	< TBUFIDX > _available;

	//statstics,
	std::atomic_int			idxLatest;
};

