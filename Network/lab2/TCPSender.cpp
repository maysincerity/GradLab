#include "stdafx.h"
#include "Global.h"
#include "TCPSender.h"


TCPSender::TCPSender() :size(3), base(0), count(0),last_ack(0), expectSequenceNumberSend(0), waitingState(false)
{
}


TCPSender::~TCPSender()
{
}

bool TCPSender::getWaitingState() {
	if (this->send_buffer.size() == 3)
	{
		this->waitingState = true;
	}
	else
	{
		this->waitingState = false;
	}
	return waitingState;
}

bool TCPSender::send(Message &message) {
	if (this->getWaitingState()) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}

	this->packetWaitingAck.acknum = -1; //���Ը��ֶ�
	this->packetWaitingAck.seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck.checksum = 0;
	memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
	pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck);
	if (this->base == this->expectSequenceNumberSend)
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//�������ͷ���ʱ��
	}
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);								//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
	this->send_buffer.push_back(this->packetWaitingAck);
	this->expectSequenceNumberSend = ((this->expectSequenceNumberSend + 1) % 4);
	return true;
}

void TCPSender::receive(Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	//���У�����ȷ
	if (checkSum == ackPkt.checksum)
	{
		if (this->last_ack == ackPkt.acknum)
		{
			this->count++;
			if (this->count == 3) //�����ش�
			{
				this->count = 0;
				//�����������ҵ���
				list<Packet>::iterator it;
				for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
				{
					if (it->seqnum == ackPkt.acknum)
					{
						pUtils->printPacket("���ڸñ��Ľ��п����ش� ", *it);
						pns->stopTimer(SENDER, ackPkt.acknum);										//���ȹرն�ʱ��
						pns->startTimer(SENDER, Configuration::TIME_OUT, ackPkt.acknum);			//�����������ͷ���ʱ��
						pns->sendToNetworkLayer(RECEIVER, *it);			//���·������ݰ�
						return;
					}
				}
			}
			else
			{
				this->last_ack = ackPkt.acknum;
			}
		}
		if (this->base != ((ackPkt.acknum + 1) % 4))
		{//��������
			this->send_buffer.pop_front(); 
		}
		this->base = ((ackPkt.acknum + 1) % 4);
		list<Packet>::iterator it;
		cout << "��ʱ���ڵ�baseֵΪ��" << this->base << endl;
		for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
		{
			pUtils->printPacket("�������б��� ", *it);
		}
		if (this->base == this->expectSequenceNumberSend)
		{
			pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
			pns->stopTimer(SENDER, 0);		//�رն�ʱ��
		}
		else
		{
			pns->stopTimer(SENDER, 0);									//���ȹرն�ʱ��
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//�����������ͷ���ʱ��
		}
	}
}

void TCPSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽�����·��ʹ˱���֮��ı���", this->packetWaitingAck);
	pns->stopTimer(SENDER, seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//�����������ͷ���ʱ��
	list<Packet>::iterator it;                                          //�������ݱ������send_buffer�д洢�ı���
	for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
	{
		pns->sendToNetworkLayer(RECEIVER, *it);			//���·������ݰ�
	}
}
