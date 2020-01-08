#include "stdafx.h"
#include "Global.h"
#include <cmath>
#include "SRReceiver.h"

using namespace std;

SRReceiver::SRReceiver() :recv_size(4),recv_base(0),expectSequenceNumberRcvd(0)
{
	lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//忽略该字段
	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}


SRReceiver::~SRReceiver()
{
}

void SRReceiver::receive(Packet &packet) {
	if (this->recv_buffer.size() == 4) {//缓存区满
		return ;
	}
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确
	if (checkSum == packet.checksum ) {
		pUtils->printPacket("接收方正确收到发送方的报文", packet);
		int m_seq= (packet.seqnum + 8 - this->recv_base) % 8;
		if (m_seq >= 0 && m_seq<this->recv_size)
		{
			if (this->recv_buffer.size()==0)
			{
				this->recv_buffer.push_front(packet);//队首
			}
			else
			{
				int is_duplicate = 0, not_biggest = 0;
				list<Packet>::iterator it; 
				for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
				{
					if (it->seqnum == packet.seqnum)
					{
						is_duplicate = 1;    //接受的报文为重复报文
						break;
					}
				}
				if (is_duplicate == 0)
				{//不重复
					for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
					{
						int pos_temp = (it->seqnum - this->recv_base + 8) % 8;
						int pos_ack_pack = (packet.seqnum - this->recv_base + 8) % 8;
						if (pos_temp > pos_ack_pack)
						{
							not_biggest = 1;    //可以找到合适位置
							this->recv_buffer.insert(it, packet);
							break;
						}
					}
				}
				if (is_duplicate == 0 && not_biggest == 0)//序号都大
				{
					this->recv_buffer.push_back(packet);//插队尾
				}
			}
		}
		m_seq = (this->recv_base + 8 - packet.seqnum) % 8;
		if (m_seq >= 1 && m_seq <= this->recv_size)
		{
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		}
		cout << "接收方期望：" << this->expectSequenceNumberRcvd << endl;
		cout << "此时接收方窗口的base值为：" << this->recv_base << endl;
		list<Packet>::iterator it;
		for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
		{
			pUtils->printPacket("接收方缓存区中报文 ", *it);
		}

		//从接收方缓存区取出数据并处理
		if (this->recv_buffer.size())
		{
			if (this->recv_buffer.front().seqnum == this->expectSequenceNumberRcvd)
			{
				//取出Message，向上递交给应用层
				Message msg;
				memcpy(msg.data, this->recv_buffer.front().payload, sizeof(packet.payload));
				pns->delivertoAppLayer(RECEIVER, msg);

				lastAckPkt.acknum = this->recv_buffer.front().seqnum; //确认序号等于收到的报文序号
				lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
				pUtils->printPacket("接收方发送确认报文", lastAckPkt);
				pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方

				//从缓存区删除
				this->recv_buffer.pop_front();
				//序号增加1,窗口滑动
				this->expectSequenceNumberRcvd = ((this->expectSequenceNumberRcvd + 1) % 8);
				this->recv_base = this->expectSequenceNumberRcvd;
			}
		}
	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
		}
		else {
			pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
		}
	}
}