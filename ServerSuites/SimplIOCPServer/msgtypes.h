#pragma once

#define REG_MSG(x) E##x,
enum EMSG_TYPES
{
#include "msglist.decl"
	EMSG_TYPES_CNT,
};
#undef REG_MSG

class IMsgBuilder
{
public:	
	virtual std::shared_ptr< Msg > Build( Msg* msgFrom ) = 0;
};

template< class _TargetT >
class MsgBuilder : public IMsgBuilder
{
public:
	MsgBuilder()
	{

	}
	// Inherited via IMsgBuilder
	virtual std::shared_ptr<Msg> Build(Msg * msgFrom) override;

};

class MsgTypesBuilder
{
	MsgTypesBuilder();
public:
	
	std::shared_ptr<Msg> GetMsgCpy(Msg* msgFrom);
private:
	IMsgBuilder* msgBuilders[EMSG_TYPES_CNT];
};

