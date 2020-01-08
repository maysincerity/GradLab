#ifndef SR_RECEIVER_H
#define SR_RECEIVER_H
#include "RdtReceiver.h"
#include <list>

class SRReceiver :public RdtReceiver
{
private:
	int recv_size;
	int recv_base;
	int expectSequenceNumberRcvd;	// 期待收到的下一个报文序号
	Packet lastAckPkt;				//上次发送的确认报文
	std::list<Packet> recv_buffer;

public:
	SRReceiver();
	virtual ~SRReceiver();

public:

	void receive(Packet &packet);	//接收报文，将被NetworkService调用
};

#endif
