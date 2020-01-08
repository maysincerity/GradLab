#include "stdafx.h"
#include "Global.h"
#include <cmath>
#include "SRReceiver.h"

using namespace std;

SRReceiver::SRReceiver() :recv_size(4),recv_base(0),expectSequenceNumberRcvd(0)
{
	lastAckPkt.acknum = -1; //��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//���Ը��ֶ�
	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}


SRReceiver::~SRReceiver()
{
}

void SRReceiver::receive(Packet &packet) {
	if (this->recv_buffer.size() == 4) {//��������
		return ;
	}
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ
	if (checkSum == packet.checksum ) {
		pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
		int m_seq= (packet.seqnum + 8 - this->recv_base) % 8;
		if (m_seq >= 0 && m_seq<this->recv_size)
		{
			if (this->recv_buffer.size()==0)
			{
				this->recv_buffer.push_front(packet);//����
			}
			else
			{
				int is_duplicate = 0, not_biggest = 0;
				list<Packet>::iterator it; 
				for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
				{
					if (it->seqnum == packet.seqnum)
					{
						is_duplicate = 1;    //���ܵı���Ϊ�ظ�����
						break;
					}
				}
				if (is_duplicate == 0)
				{//���ظ�
					for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
					{
						int pos_temp = (it->seqnum - this->recv_base + 8) % 8;
						int pos_ack_pack = (packet.seqnum - this->recv_base + 8) % 8;
						if (pos_temp > pos_ack_pack)
						{
							not_biggest = 1;    //�����ҵ�����λ��
							this->recv_buffer.insert(it, packet);
							break;
						}
					}
				}
				if (is_duplicate == 0 && not_biggest == 0)//��Ŷ���
				{
					this->recv_buffer.push_back(packet);//���β
				}
			}
		}
		m_seq = (this->recv_base + 8 - packet.seqnum) % 8;
		if (m_seq >= 1 && m_seq <= this->recv_size)
		{
			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		}
		cout << "���շ�������" << this->expectSequenceNumberRcvd << endl;
		cout << "��ʱ���շ����ڵ�baseֵΪ��" << this->recv_base << endl;
		list<Packet>::iterator it;
		for (it = this->recv_buffer.begin(); it != this->recv_buffer.end(); it++)
		{
			pUtils->printPacket("���շ��������б��� ", *it);
		}

		//�ӽ��շ�������ȡ�����ݲ�����
		if (this->recv_buffer.size())
		{
			if (this->recv_buffer.front().seqnum == this->expectSequenceNumberRcvd)
			{
				//ȡ��Message�����ϵݽ���Ӧ�ò�
				Message msg;
				memcpy(msg.data, this->recv_buffer.front().payload, sizeof(packet.payload));
				pns->delivertoAppLayer(RECEIVER, msg);

				lastAckPkt.acknum = this->recv_buffer.front().seqnum; //ȷ����ŵ����յ��ı������
				lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
				pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
				pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�

				//�ӻ�����ɾ��
				this->recv_buffer.pop_front();
				//�������1,���ڻ���
				this->expectSequenceNumberRcvd = ((this->expectSequenceNumberRcvd + 1) % 8);
				this->recv_base = this->expectSequenceNumberRcvd;
			}
		}
	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		}
		else {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,������Ų���", packet);
		}
	}
}