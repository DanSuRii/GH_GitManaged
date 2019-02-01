#include "pch.h"
#include "BufferPool.h"
#include <cassert>

#include <algorithm>

BufferPool& gBuffPool = BufferPool::GetInstance();

BuffAccessor::BuffAccessor(TBUFIDX idx)
	: _idx(idx)
{
	IncreaseRef();
}

BuffAccessor::BuffAccessor(BuffAccessor & rhs)
{
	/*
	++ref;
	*/
	IncreaseRef();

}


BuffAccessor::~BuffAccessor()
{
	DecreaseRef();
	//possibly already 0	

	BufferPool::GetInstance().AccessorDestruct( _idx );
}

int BuffAccessor::GetRefRemain()
{
	assert(nullptr != _contRef[_idx]);
	
	return *(_contRef[_idx]);
}

int BuffAccessor::GetRefRemain(TBUFIDX idx)
{
	assert(nullptr != _contRef[idx]);
	assert(idx <= _contRef.size());

	return *(_contRef[idx]);
}

void BuffAccessor::IncreaseRef()
{
	if (_idx > _contRef.size())
	{
		std::lock_guard< std::mutex > grd( _mtxCont );
		if (_idx > _contRef.size())
			_contRef.reserve(_contRef.size() * 2);
	}

	{
		//std::lock_guard< std::mutex > grd(_mtxCont); //container possiblely resize...
		_contRef[_idx]->operator++();
	}
}

int BuffAccessor::DecreaseRef()
{
	assert(_idx < _contRef.size());

	_contRef[_idx]->operator++();

	return *(_contRef[_idx]);
}


BufferPool::BufferPool()
	: _cont(DEFAULT_LEN)
{
	//populate if use

	for (decltype(_available.size()) i = 0; i < _available.size(); ++i)
		_available.push_back(i);
}

BufferPool::~BufferPool()
{
}

BuffAccessor BufferPool::GetNewAccessor()
{
	return BuffAccessor(GetAvailable());
}

char * BufferPool::GetBuffer(BuffAccessor key)
{
	assert(nullptr != _cont[key.GetIdx()]);
	auto buffInfo = _cont[key.GetIdx()]->GetBuffer();
	return buffInfo.second;
}

void BufferPool::AccessorDestruct(TBUFIDX key)
{
	if (0 == BuffAccessor::GetRefRemain(key))
	{
		BufferRecycle(key);
	}
}

size_t BufferPool::GetContainerLen()
{
	return _cont.size();
}

size_t BufferPool::GetAvailableCnt()
{
	return std::count_if(_cont.begin(), _cont.end(),
		[](auto cur) {
		return false == cur->CheckInUse()
	});
}

bool BufferPool::AllAvailable()
{
	return _cont.end() ==
	std::find_if(_cont.begin(), _cont.end(), [](auto cur) { return nullptr == cur ? false : cur->CheckInUse(); }
}

void BufferPool::BufferRecycle(TBUFIDX key)
{
	assert(nullptr != _cont[key]);
	
	_cont[key]->ResetInUse();
	
	std::lock_guard< std::mutex > grd(_mtxAvailable);
	_available.push_back(key);
}

void BufferPool::_grow_up()
{
	std::list< TBUFIDX > newAvailableList;

	auto firstAvail = _cont.size();
	//growth container up
	_cont.reserve(_cont.size() * 2);
	
	auto end = _cont.size();

	for( decltype(firstAvail) i= firstAvail ; i < end; ++i)
		newAvailableList.push_back(i);

	//new available container list generate to tempory avaiable list
	_available.splice(_available.end(),std::move( newAvailableList ));
}

TBUFIDX BufferPool::GetAvailable()
{
	TBUFIDX ret(INVALID_IDX);
	if ( _available.size() <= 0 )
	{
		std::lock_guard< std::mutex > grd(_mtxAvailable);
		if (_available.size() <= 0)
		{
			_grow_up();
		}
	}

	{
		std::lock_guard<std::mutex> grd(_mtxAvailable);		
		ret = _available.front();
		_available.pop_front();
	}
	if (nullptr == _cont[ret])
		_cont[ret] = New<Buffer>(ret);

	_cont[ret]->SetInUse();

	return ret;
}

std::pair<size_t, char*> Buffer::GetBuffer()
{ 
	return std::pair<size_t, char*>(sizeof(buffer), buffer);
}
