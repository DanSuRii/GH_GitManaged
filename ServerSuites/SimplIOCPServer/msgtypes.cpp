#include "pch.h"


#define REG_MSG(x) {static MsgBuilder<x> builder; msgBuilders[ E##x ] = &builder; }
MsgTypesBuilder::MsgTypesBuilder() 
{
#include "msglist.decl"
}

#undef REG_MSG


// Inherited via IMsgBuilder
template<class _TargetT>
std::shared_ptr<Msg> MsgBuilder<_TargetT>::Build(Msg * msgFrom)
{
	if (nullptr == dynamic_cast<_TargetT*>(msgFrom))
	{
		LOG_FN("Msgs Runtime type is not ", typeid(_TargetT).name(),  ", Real type is : ", typeid(*msgFrom).name() );

		return nullptr;
	}
	_TargetT& targetFrom = *static_cast<_TargetT*>(msgFrom);
	return std::make_shared< _TargetT >(targetFrom);
}

std::shared_ptr<Msg> MsgTypesBuilder::GetMsgCpy(Msg * msgFrom)
{
	int idx = msgFrom->Type();
	if (idx < 0 || idx > _countof(msgBuilders))
	{
		LOG_FN(" Invalid type ");
		return nullptr;
	}

	auto curBuilder = msgBuilders[idx];
	return curBuilder->Build(msgFrom);
}

