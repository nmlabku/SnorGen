/*################################################################################################################
�̸�		:	util.c

��������	:	1.0.0	(2002_07_05)	tmskim	�����ۼ�
				1.1.0	(2010_01_01)	shyoon	����߰�
				1.2.0	(2010_02_01)	shyoon	����߰� (g_setFileName)
				1.3.0	(2010_04_07)	shyoon	����߰� (char* g_setTime(time_t p_cpTime, int p_iUnit))
				1.3.1	(2010_04_12)	shyoon	��ɼ��� (void g_setTime(char* p_cpStr, time_t p_cpTime, int p_iUnit))
				1.4.0	(2010_04_15)	shyoon	����߰� (int g_setLogFileName(char* p_caFileName, char* p_caPathName))
				1.5.0	(2011-07-11)	shyoon	����߰� (int g_setLogFileName(char* p_caFileName, char* p_caPathName, char *p_cpPreFix))
				1.6.0	(2011-10-13)	shyoon	����߰� (int g_isLabAddr(unsigned int p_uiAddr))
				1.7.0	(2011-10-26)	shyoon	����߰� (void g_writeLogYear(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime))

���		:	���α׷��� ���� ����ϴ� �Լ��� ����
#################################################################################################################*/
#include "include.h"

#define MIN			60	
#define HOUR		3600
#define DAY			86400
#define WEEK		604800
#define	MONTH		2592000
#define YEAR		31708800
#define DECADE		315360000
#define NONE		0

#ifndef __util_h
#define __util_h
/*################################################################################################################
�̸�		:	void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix, time_t p_tTargetTime, int p_iUnit);

���		:	�Է����� ������ ������ �̿��Ͽ� ���ϸ� ����
				(path)(prefix)(unit) 
				ex)/data/Traffic_Trace/flow/flow_twoway/flow_twoway_2009_10_01_12_12

				unit define
				#define MIN			60	
				#define HOUR		3600
				#define DAY			86400
				#define WEEK		604800
				#define	MONTH		2592000
				#define YEAR		31708800
				#define DECADE		315360000
				#define NONE		0

�Է�		:	char *p_cpFileName				�ϼ��� �����̸��� ���� �� ��
				char *p_cpPathName				�����̸��� path
				char *p_cpPreFix				�����̸��� prefix
				time_t p_tTargetTime			�����̸��� ���� �ð�
				int p_iUnit						���� �̸��� ���� (YEAR: �����̸�_2009   DAY: �����̸�_2009_10_01 ........)
#################################################################################################################*/
void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix, time_t p_tTargetTime, int p_iUnit);
void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix1, char *p_cpPreFix2, time_t p_tTargetTime, int p_iUnit);

/*################################################################################################################
�̸�		:	void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix, time_t p_tTargetTime, char *p_cpPostFix, int p_iUnit);

���		:	�Է����� ������ ����(postfix ����)�� �̿��Ͽ� ���ϸ� ����
				(path)(prefix)(unit)(postfix) 
				ex)/data/Traffic_Trace/flow/flow_twoway/flow_twoway_2009_10_01_12_12
				
				unit define
				#define MIN			60	
				#define HOUR		3600
				#define DAY			86400
				#define WEEK		604800
				#define	MONTH		2592000
				#define YEAR		31708800
				#define DECADE		315360000
				#define NONE		0

�Է�		:	char *p_cpFileName				�ϼ��� �����̸��� ���� �� ��
				char *p_cpPathName				�����̸��� path
				char *p_cpPreFix				�����̸��� prefix
				time_t p_tTargetTime			�����̸��� ���� �ð�
				char *p_cpPostFix				�����̸��� postfix
				int p_iUnit						���� �̸��� ���� (YEAR: �����̸�_2009   DAY: �����̸�_2009_10_01 ........)
#################################################################################################################*/
void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix, time_t p_tTargetTime, char *p_cpPostFix, int p_iUnit);
void g_setFileName(char *p_cpFileName, char *p_cpPathName, char *p_cpPreFix1, char *p_cpPreFix2, time_t p_tTargetTime, char *p_cpPostFix, int p_iUnit);

/*################################################################################################################
�̸�		:	void g_writeLogYear(char *p_cpPreFix, char *p_cpStr);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "year"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
#################################################################################################################*/
void g_writeLogYear(char *p_cpPreFix, char *p_cpStr);

/*################################################################################################################
�̸�		:	void g_writeLogMonth(char *p_cpPreFix, char *p_cpStr);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "month"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009_01

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
#################################################################################################################*/
void g_writeLogMonth(char *p_cpPreFix, char *p_cpStr);

/*################################################################################################################
�̸�		:	void g_writeLogDay(char *p_cpPreFix, char *p_cpStr);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "day"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009_01_23

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
#################################################################################################################*/
void g_writeLogDay(char *p_cpPreFix, char *p_cpStr);

/*################################################################################################################
�̸�		:	void g_writeLogYear(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "year"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
				time_t p_tTagetTime				�����ð�
#################################################################################################################*/
void g_writeLogYear(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

/*################################################################################################################
�̸�		:	void g_writeLogMonth(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "month"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009_01

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
				time_t p_tTagetTime				�����ð�
#################################################################################################################*/
void g_writeLogMonth(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

/*################################################################################################################
�̸�		:	void g_writeLogDay(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

���		:	�Է� ���� p_cpPreFix �̸����� /var/log ������ �Է¹��� p_cpStr�� logging �Ѵ�. ���� ������ "day"�̴�.
				ex)/var/log/���α׷���_(p_cpPreFix)_2009_01_23

�Է�		:	char *p_cpPreFix				�α����� prefix
				char *p_cpStr					�α� ����
				time_t p_tTagetTime				�����ð�
#################################################################################################################*/
void g_writeLogDay(char *p_cpPreFix, char *p_cpStr, time_t p_tTagetTime);

/*################################################################################################################
�̸�		:	void g_err(char* p_cpStr);

���		:	�Է� ���� p_cpStr�� errorno�� ����ϰ� ����

�Է�		:	char* p_cpStr					ȭ�鿡 ����� �����޼���
#################################################################################################################*/
void g_err(char* p_cpStr);

/*################################################################################################################
�̸�		:	void g_p(char* p_cpStr);

���		:	�Է� ���� p_cpStr�� ���

�Է�		:	char* p_cpStr					ȭ�鿡 ����� �޼���
#################################################################################################################*/
void g_p(char* p_cpStr);

/*################################################################################################################
�̸�		:	void g_p_time();

���		:	���� �ð��� ȭ�鿡 ����Ѵ�.
#################################################################################################################*/
void g_p_time();

/*################################################################################################################
�̸�		:	void g_delete(char* p_cpStr);

���		:	�Է� ���� p_cpStr ���� �̸��� ����

�Է�		:	char* p_cpStr					������ ���� �̸�
#################################################################################################################*/
void g_delete(char* p_cpStr);

/*################################################################################################################
�̸�		:	int g_setTime(char *p_cpTimeStr);

���		:	�Է� ���� p_cpTimeStr(ex: 2009-12-26-00-00)�� �ʷ� �ٲ��ش�.

�Է�		:	char* p_cpTimeStr					�ʷ� �ٲ� �������� �ð� ��Ʈ��

���		:	int									��ȯ�� ��
#################################################################################################################*/
int g_setTime(char *p_cpTimeStr);

/*################################################################################################################
�̸�		:	void g_setTime(char* p_cpStr, time_t p_cpTime, int p_iUnit)

���		:	�Է� ���� p_cpTime�� string���� ��ȯ �� p_cpStr�� ���� (ex: 2009-12-26-00-00)

�Է�		:	char* p_cpStr					����� ����
				time_t p_cpTime					��ȯ�� ��
				int p_iUnit						���� 

���		:	
################################################################################################################*/
void g_setTime(char* p_cpStr, time_t p_cpTime, int p_iUnit);

/*################################################################################################################
�̸�		:	void g_calTime(char *p_cpLogPreFix, struct timeval p_stStartTime, struct timeval p_stEndTime);

���		:	p_stStartTime�� p_stEndTime���� ���� ����Ͽ� logging 
				ex)
				/var/log/���α׷� �̸�_(p_cpLogPreFix)_2009_12_00

�Է�		:	char* p_cpLogPreFix					�α� prefix
				struct timeval p_stStartTime		start Ÿ��
				struct timeval p_stEndTime			end Ÿ��
#################################################################################################################*/
void g_calTime(char *p_cpLogPreFix, struct timeval p_stStartTime, struct timeval p_stEndTime);

/*################################################################################################################
�̸�		:	int g_isValidAddr(unsigned int p_uiAddr);

���		:	�Է� ���� IP �ּ�(p_uiAddr)�� ��ȿ�� �ּ����� Ȯ��

�Է�		:	unsigned int p_uiAddr				�����ϱ� ���� IP �ּ�

���		:	1								��ȿ
				0								����ȿ
#################################################################################################################*/
int g_isValidAddr(unsigned int p_uiAddr);

/*################################################################################################################
�̸�		:	int g_isLocalAddr(unsigned int p_uiAddr);

���		:	�Է� ���� IP �ּ�(p_uiAddr)�� ���� �ּ����� Ȯ�� (163.152.207.0 ~ 163.152.239.255)

�Է�		:	unsigned int p_uiAddr				�����ϱ� ���� IP �ּ�

���		:	1								����
				0								�ܺ�
#################################################################################################################*/
int g_isLocalAddr(unsigned int p_uiAddr);

/*################################################################################################################
�̸�		:	void g_convertAddrToString(char* p_cpStr, u_int32_t p_ui32Addr);

���		:	�Է� ���� IP �ּ�(p_ui32Addr)�� ��Ʈ�� ���·� ��ȯ �� p_cpStr�� �����Ѵ�.

�Է�		:	char* p_cpStr					��ȯ�� ��Ʈ���� ����� ��
				u_int32_t p_ui32Addr			��ȯ�ǰ� ���ϴ� �ּ�
#################################################################################################################*/
void g_convertAddrToString(char* p_cpStr, u_int32_t p_ui32Addr);

/*################################################################################################################
�̸�		:	u_int32_t g_convertStringtoAddr(char* p_cpStr);

���		:	�Է� ���� ��Ʈ�� ������ �ּҸ� ���ڷ� ��ȯ

�Է�		:	char* p_cpStr					��ȯ�� ��Ʈ�� ������ �ּ�

���		:	u_int32_t						��ȯ�� �ּ�
#################################################################################################################*/
u_int32_t g_convertStringtoAddr(char* p_cpStr);

/*################################################################################################################
�̸�		:	int g_setLogFileName(char* p_caFileName, char* p_caPathName);

���		:	�Է� ���� �н����� ���� �ֱٿ� ������ �����̸��� �˻�

�Է�		:	char* p_caFileName					ã�� ���� ���� ������ ����
				char* p_caPathName					ã�� ���� �̸�

���		:	int									1: ã��     0:���� ����
#################################################################################################################*/
int g_setLogFileName(char* p_caFileName, char* p_caPathName);

/*################################################################################################################
�̸�		:	int g_setLogFileName(char* p_caFileName, char* p_caPathName, char *p_cpPreFix);

���		:	�Է� ���� �н����� ���� �ֱٿ� ������ �����̸��� �˻�

�Է�		:	char* p_caFileName					ã�� ���� ���� ������ ����
				char* p_caPathName					ã�� ���� �̸�
				char* p_cpPreFix					ã�� ������ frefix

���		:	int									1: ã��     0:���� ����
#################################################################################################################*/
int g_setLogFileName(char* p_caFileName, char* p_caPathName, char *p_cpPreFix);

/*################################################################################################################
�̸�		:	int g_isLabAddr(unsigned int p_uiAddr);

���		:	�Է� ���� IP �ּ�(p_uiAddr)�� ������ �ּ����� Ȯ�� (163.152.219.184 ~ 163.152.219.220)

�Է�		:	unsigned int p_uiAddr				�����ϱ� ���� IP �ּ�

���		:	1								����
				0								�ܺ�
#################################################################################################################*/
int g_isLabAddr(unsigned int p_uiAddr);

bool isDigit(char ch);			//�Է� ���� ch�� ���� ����(0~9)���� �˻�
bool isHexChar(char ch);		//�Է� ���� ch�� 16���� ����(a~z or A~Z)���� �˻�
#endif
