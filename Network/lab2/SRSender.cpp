#include "stdafx.h"
#include "Global.h"
#include "SRSender.h"

SRSender::SRSender() :size(4), base(0), expectSequenceNumberSend(0), waitingState(false)
{
}

SRSender::~SRSender()
{
}

bool SRSender::getWaitingState() {
	if (this->send_buffer.size() == 4)
	{
		this->waitingState = true;
	}
	else
	{
		this->waitingState = false;
	}
	return waitingState;
}

bool SRSender::send(Message &message) {
	if (this->getWaitingState()) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}

	this->packetWaitingAck.acknum = -1; //���Ը��ֶ�
	this->packetWaitingAck.seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck.checksum = 0;
	memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
	pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck);
	pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck.seqnum);			//�������ͷ���ʱ��
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);								//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
	this->send_buffer.push_back(this->packetWaitingAck);//��β����
	this->expectSequenceNumberSend = ((this->expectSequenceNumberSend + 1) % 8);//��ż�1
	return true;
}

void SRSender::receive(Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	//���У�����ȷ
	if (checkSum == ackPkt.checksum)
	{
		pUtils->printPacket("���ͷ��յ���Ӧ���ĵ�ȷ�ϣ�", ackPkt);
		pns->stopTimer(SENDER,ackPkt.acknum);
		list<Packet>::iterator it;
		for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
		{//ɾ������������
			if (it->seqnum == ackPkt.acknum)
			{
				this->send_buffer.erase(it);
				break;
			}
		}
		cout << "��ʱ���ͷ����ڵ�baseֵΪ��" << this->base << endl;
		for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
		{
			pUtils->printPacket("���ͷ��������б��� ", *it);
		}
		//��������
		if (!this->send_buffer.size())
		{
			this->base = this->expectSequenceNumberSend;
		}
		else
		{
			this->base = this->send_buffer.front().seqnum;
		}
	}
}

void SRSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	pns->stopTimer(SENDER, seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//�����������ͷ���ʱ��
	list<Packet>::iterator it;
	for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
	{
		if (it->seqnum == seqNum)
		{
			pUtils->printPacket("���ͷ���ʱ��ʱ�䵽�����·��ʹ˱���", *it);
			pns->sendToNetworkLayer(RECEIVER, *it);			//���·������ݰ�
			break;
		}
	}
}