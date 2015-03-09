// protobufremoteEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ProtoBufRemote/MutableParameterList.h"
#include "ProtoBufRemote/PendingCall.h"
#include "ProtoBufRemote/RpcClient.h"
#include "ProtoBufRemote/RpcController.h"
#include "ProtoBufRemote/RpcMessage.pb.h"

using ProtoBufRemote::RpcClient;
using ProtoBufRemote::RpcMessage;
using ProtoBufRemote::MutableParameterList;
using ProtoBufRemote::PendingCall;

#pragma comment(lib, "protobufremote.lib")
#pragma comment(lib, "libprotobuf.lib")

class RpcControllerImpl : public ProtoBufRemote::RpcController
{
public:

	void SetRpcController(RpcClient* pRPCClient)
	{
		m_pRPCClient = pRPCClient;
	}

	void Send(const ProtoBufRemote::RpcMessage& message) override
	{
		ProcessFakeServiceLogic(message);
	}

	bool CheckValidMessage(const ProtoBufRemote::RpcMessage& message)
	{
		if (!message.has_id() || !message.has_call_message())
			return false;
		if (message.call_message().service() != "LogicService" ||
			message.call_message().method() != "Add100")
			return false;
		if (message.call_message().parameters_size() != 1)
			return false;

		return true;
	}

	void ProcessFakeServiceLogic(const ProtoBufRemote::RpcMessage& message)
	{
		const RpcMessage::Parameter& parameter = message.call_message().parameters(0);
		if (CheckValidMessage(message) == false || !parameter.has_int_param())
		{
			return;
		}

		RpcMessage resultMessage;
		resultMessage.set_id(message.id());
		RpcMessage::Result* result = resultMessage.mutable_result_message();
		RpcMessage::Parameter* param = result->mutable_call_result();
		param->set_int_param(parameter.int_param() + 100);

		m_pRPCClient->ReceiveResult(resultMessage);
	}

private:
	RpcClient* m_pRPCClient;
};

int _tmain(int argc, _TCHAR* argv[])
{
	RpcControllerImpl controller;
	RpcClient* m_pClient = new RpcClient(&controller);
	controller.SetRpcController(m_pClient);
	
	RpcMessage message;
	MutableParameterList parameters(&message);
	parameters.Add().SetInt(42);
	PendingCall* call = m_pClient->Call("LogicService", "Add100", parameters);
	call->Wait();
	
	if (142 != call->GetResult()->GetInt())
	{
		printf("remote procedure call fail!!\n");
	}
	else
		printf("remote procedure call success!!\n");

	m_pClient->ReleaseCall(call);

	delete m_pClient;

	return 0;
}

