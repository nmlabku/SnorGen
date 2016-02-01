/*###################################################################################################
�̸�		:	TimeChecker

���� ����	:	1.0.0(2010-01-14)	(shyoon)	�����ۼ�


���		:	���α׷� ���� �ð� üũ, �α�

�ֿ� �Լ�	:	void startClock();
				���� �ð� üũ

				void endClock();
				���� �ð� üũ, ȭ�鿡 ���

				void logClock(char* p_cpPreFix, char* p_caDutyPerson, int iPeriod);
				�ð� ���� �α�
				
###################################################################################################*/
#include "include.h"

#define MIN			60	
#define HOUR		3600
#define DAY			86400
#define WEEK		604800
#define	MONTH		2592000
#define YEAR		31708800
#define DECADE		315360000
#define NONE		0

#ifndef __TimeChecker_h
#define __TimeChecker_h


//#################################################
class TimeChecker
{
	public:
		clock_t		m_cStartTime;
		clock_t		m_cEndTime;
		struct tms	m_stStartTime;
		struct tms	m_stEndTime;
	

	public:
		TimeChecker(void){memset(this, 0, sizeof(TimeChecker));};
		~TimeChecker(void){};

		void reset(){memset(this, 0, sizeof(TimeChecker));};

		void startClock();
		void endClock();

		void print();
		void print(FILE* fp);

		void logClock(char* p_cpPreFix, char* p_caDutyPerson, int iPeriod);
};


	


#endif
