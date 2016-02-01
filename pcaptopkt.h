#include "loadbar.h"
#include "packet.h"
#include "pcapReader.h"
#include "util.h"

#ifndef __pcaptopkt_h
#define __pcaptopkt_h
void convertFromPcapToPkt(char* p_caCurrentWorkingDirectory, char *p_cpDataPath, char *p_cpPcapPath, u_int32_t p_uiTrial_num, char *p_cpPktPath, char *p_cpLogPath); // pcap ������ Ʈ������ pkt ���·� ��ȯ
void convertFromPcapToPkt(char* p_caTargetDirectory);
void convertFromPcapToPkt(char* p_caTargetDirectory, FILE* p_fpLogFile);
#endif
