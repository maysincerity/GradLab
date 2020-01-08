#include "stdafx.h"
#include "Global.h"
#include "GBNSender.h"


GBNSender::GBNSender() :size(3),base(0),expectSequenceNumberSend(0), waitingState(false)
{
}


GBNSender::~GBNSender()
{
}

bool GBNSender::getWaitingState() {
	if (this->buffer_packets.size()==3)
	{
		this->waitingState = true;
	}
	else
	{
		this->waitingState = false;
	}
	return waitingState;
}

bool GBNSender::send(Message &message) {

	if (this->getWaitingState()) { //发送方处于等待确认状态
		return false;
	}

	this->packetWaitingAck.acknum = -1; //忽略该字段
	this->packetWaitingAck.seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck.checksum = 0;
	memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
	pUtils->printPacket("发送方发送报文", this->packetWaitingAck);
	if (this->base == this->expectSequenceNumberSend)//基序号为下一发送序号
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//启动发送方定时器
	}
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);								//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
	this->buffer_packets.push_back(this->packetWaitingAck); //在list尾添加元素
	this->expectSequenceNumberSend = ((this->expectSequenceNumberSend + 1) % 4);//修改下一发送序号
	return true;
}

void GBNSender::receive(Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	//如果校验和正确
	if (checkSum == ackPkt.checksum)
	{
		if (this->base != ((ackPkt.acknum + 1)%4))//基序号与确认号不同
		{
			this->buffer_packets.pop_front(); //滑动窗口，删除list头元素
		}
		this->base = ((ackPkt.acknum + 1)%4);
		list<Packet>::iterator it;
		cout << "此时窗口的base值为：" << this->base << endl;
		for (it = this->buffer_packets.begin(); it != this->buffer_packets.end(); it++)
		{
			pUtils->printPacket("缓存区中报文 ",*it);
		}
		if (this->base == this->expectSequenceNumberSend)//基序号等于下一接受序号
		{
			pUtils->printPacket("发送方正确收到确认", ackPkt);
			pns->stopTimer(SENDER, 0);		//关闭定时器
		}
		else
		{
			//先关后开
			pns->stopTimer(SENDER, 0);										//首先关闭定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, 0);			//重新启动发送方定时器
		}
	}
}

void GBNSender::timeoutHandler(int seqNum) {
	//唯一一个定时器,无需考虑seqNum
	pUtils->printPacket("发送方定时器时间到，重新发送此报文之后的报文", this->packetWaitingAck);
	pns->stopTimer(SENDER, seqNum);										//首先关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//重新启动发送方定时器
	list<Packet>::iterator it;
	for (it = this->buffer_packets.begin(); it != this->buffer_packets.end(); it++)
	{
		pns->sendToNetworkLayer(RECEIVER, *it);			//重新发送数据包
	}
}
