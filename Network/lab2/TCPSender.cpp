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
	if (this->getWaitingState()) { //发送方处于等待确认状态
		return false;
	}

	this->packetWaitingAck.acknum = -1; //忽略该字段
	this->packetWaitingAck.seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck.checksum = 0;
	memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
	pUtils->printPacket("发送方发送报文", this->packetWaitingAck);
	if (this->base == this->expectSequenceNumberSend)
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//启动发送方定时器
	}
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);								//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
	this->send_buffer.push_back(this->packetWaitingAck);
	this->expectSequenceNumberSend = ((this->expectSequenceNumberSend + 1) % 4);
	return true;
}

void TCPSender::receive(Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	//如果校验和正确
	if (checkSum == ackPkt.checksum)
	{
		if (this->last_ack == ackPkt.acknum)
		{
			this->count++;
			if (this->count == 3) //快速重传
			{
				this->count = 0;
				//遍历缓存区找到包
				list<Packet>::iterator it;
				for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
				{
					if (it->seqnum == ackPkt.acknum)
					{
						pUtils->printPacket("对于该报文进行快速重传 ", *it);
						pns->stopTimer(SENDER, ackPkt.acknum);										//首先关闭定时器
						pns->startTimer(SENDER, Configuration::TIME_OUT, ackPkt.acknum);			//重新启动发送方定时器
						pns->sendToNetworkLayer(RECEIVER, *it);			//重新发送数据包
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
		{//滑动窗口
			this->send_buffer.pop_front(); 
		}
		this->base = ((ackPkt.acknum + 1) % 4);
		list<Packet>::iterator it;
		cout << "此时窗口的base值为：" << this->base << endl;
		for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
		{
			pUtils->printPacket("缓存区中报文 ", *it);
		}
		if (this->base == this->expectSequenceNumberSend)
		{
			pUtils->printPacket("发送方正确收到确认", ackPkt);
			pns->stopTimer(SENDER, 0);		//关闭定时器
		}
		else
		{
			pns->stopTimer(SENDER, 0);									//首先关闭定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//重新启动发送方定时器
		}
	}
}

void TCPSender::timeoutHandler(int seqNum) {
	//唯一一个定时器,无需考虑seqNum
	pUtils->printPacket("发送方定时器时间到，重新发送此报文之后的报文", this->packetWaitingAck);
	pns->stopTimer(SENDER, seqNum);										//首先关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//重新启动发送方定时器
	list<Packet>::iterator it;                                          //定义数据便于输出send_buffer中存储的报文
	for (it = this->send_buffer.begin(); it != this->send_buffer.end(); it++)
	{
		pns->sendToNetworkLayer(RECEIVER, *it);			//重新发送数据包
	}
}
