#include "loadbar.h"
#include "util.h"
#include "packet.h"
#include "flowHash.h"

#ifndef __pkttoflowwithpkt_h
#define __pkttoflowwithpkt_h
void convertFromPktToFwp(char* p_caCurrentWorkingDirectory, char *p_cpDataPath, char *p_cpPktPath, u_int32_t p_uiTrial_num, char *p_cpFwpPath, char *p_cpLogPath); //pkt Ʈ������ flow with pkt ���·� ��ȯ
void convertFromPktToFwp(char* p_caTargetDirectory);
void convertFromPktToFwp(char* p_caTargetDirectory, FILE* p_fpLogFile);

#endif
