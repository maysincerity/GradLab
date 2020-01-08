#ifndef SR_RECEIVER_H
#define SR_RECEIVER_H
#include "RdtReceiver.h"
#include <list>

class SRReceiver :public RdtReceiver
{
private:
	int recv_size;
	int recv_base;
	int expectSequenceNumberRcvd;	// �ڴ��յ�����һ���������
	Packet lastAckPkt;				//�ϴη��͵�ȷ�ϱ���
	std::list<Packet> recv_buffer;

public:
	SRReceiver();
	virtual ~SRReceiver();

public:

	void receive(Packet &packet);	//���ձ��ģ�����NetworkService����
};

#endif
