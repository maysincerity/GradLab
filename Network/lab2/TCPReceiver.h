#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H
#include "RdtReceiver.h"

class TCPReceiver :public RdtReceiver
{
private:
	int expectSequenceNumberRcvd;	// �ڴ��յ�����һ���������
	Packet lastAckPkt;				//�ϴη��͵�ȷ�ϱ���

public:
	TCPReceiver();
	virtual ~TCPReceiver();

public:

	void receive(Packet &packet);	//���ձ��ģ�����NetworkService����
};

#endif

