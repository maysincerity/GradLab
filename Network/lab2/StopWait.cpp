// StopWait.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "StopWaitRdtSender.h"
#include "StopWaitRdtReceiver.h"
#include "GBNSender.h"
#include "GBNReceiver.h"
#include "SRSender.h"
#include "SRReceiver.h"
#include "TCPSender.h"
#include "TCPReceiver.h"


int main(int argc, char** argv[])
{
	//RdtSender *ps = new StopWaitRdtSender();
	//RdtReceiver * pr = new StopWaitRdtReceiver();
	//RdtSender *ps = new GBNSender();
	//RdtReceiver * pr = new GBNReceiver();
	RdtSender *ps = new SRSender();
	RdtReceiver * pr = new SRReceiver();
	//RdtSender *ps = new TCPSender();
	//RdtReceiver * pr = new TCPReceiver();

	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("D:\\Assignment\\Network_Lab\\lab2\\StopWait\\Debug\\input.txt");
	pns->setOutputFile("D:\\Assignment\\Network_Lab\\lab2\\StopWait\\Debug\\output.txt");
	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
	delete pns;										//ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete
	
	return 0;
}

