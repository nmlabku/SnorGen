#include "loadbar.h"
#include "util.h"

#ifndef __captopcap_h
#define __captopcap_h
void convertFromCapToPcap(char* p_caCurrentWorkingDirectory, char *p_cpDataPath, char *p_cpCapPath, u_int32_t p_uiSnapLen, u_int32_t p_uiTrial_num, char *p_cpPcapPath); // cap ������ Ʈ������ pcap ���·� ��ȯ
void convertFromCapToPcap(char* p_caTargetDirectory); // cap or pcap ������ Ʈ������ pcap ���·� ��ȯ
void convertFromCapToPcap(char* p_caTargetDirectory, FILE* p_fpLogFile); // ���� 1�� ���� ����, capinfos ��ɾ�� trace ����
#endif
