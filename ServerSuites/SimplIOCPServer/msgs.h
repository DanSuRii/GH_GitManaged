#pragma once

typedef std::int32_t	MsgType;
typedef size_t			MsgLen;

#pragma pack(push, 1)

struct Msg
{
	struct MsgHeader {
		MsgType _type;
		MsgLen _len;
	}		_header;

	Msg()
	{
		_header._type = Type();
		_header._len = Len();
	}

	virtual MsgType Type() = 0;
	virtual MsgLen Len() = 0;
};

template< typename Derived >
struct MsgInterfaceImpl : public Msg
{
	MsgInterfaceImpl()
	{
		//Regist self type info and msg length
	}
	// Inherited via Msg
	virtual MsgType Type() override
	{
		return MsgType();
	}
	virtual MsgLen Len() override
	{
		//return sizeof(* static_cast<Derived*>( this ));
		return sizeof(Derived);
	}
};

struct MsgReqLiquiditaets : public MsgInterfaceImpl<MsgReqLiquiditaets>
{
	time_t tmBegin;
	time_t tmEnd;
};

struct MsgResLiquidiates : public MsgInterfaceImpl<MsgResLiquidiates>
{
	enum { MSG_SUCCESS, MSG_FAIL } eResult;
};

struct MsgLiquiditaetsInfo : public MsgInterfaceImpl<MsgLiquiditaetsInfo>
{
	//if the receive Path, then COPY that file to local
	char UNCPath[MAX_PATH];
};

struct MsgChangeKonditionInfo : public MsgInterfaceImpl<MsgChangeKonditionInfo>
{

};

struct MsgAddKonditionInfo : public MsgInterfaceImpl<MsgAddKonditionInfo>
{
	int nGVNumber;
	
	int nKondFunction;
	int nKondPct;
};


#pragma pack(pop)