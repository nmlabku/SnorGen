/*#####################################################################
�̸�		:	SnorGen

��������	:	1.0.0	2014-10-24	(shyoon)	test_sequntial_pattern-1.7.0 �� �¶��� �������� ����
				1.1.0	2014-10-27	(shyoon)	content offset ǥ��
				1.2.0	2014-10-30	(shyoon)	content minimum length define������ ����, �Ķ���Ͱ� 2���̸� ��, ./SonrGen A B  B�� fwp���� ����, A fwp�� �ְ� ��� ���, A fwp ����
				1.3.0	2014-11-01	(shyoon)	IS_FLOW_SEQUENCE : flow, packet ���� flog �߰�, MAX_SEQUENCE_LENGTH : sequence �ִ� ���� �߰�
				1.4.0	2014-11-01	(shyoon)	pthread �߰�, MAX_PKT_COUNT �߰� packet ������ �����Ҷ� �ִ� ��� pkt�� �ε����� ���� 
				1.5.0	2014-11-02	(shyoon)	innerthread �߰�, support ��굵 Thread��
				1.6.0	2014-11-03	(shyoon)	support ��길 Thread�� ����
				1.7.0	2014-11-03	(shyoon)	packet �������� ��Ģ�� ��Ī�ϴ� Ʈ���� �� ���
				2.0.0	2014-11-03	(shyoon)	class ����	(A:101rules, 18.045; B: 384rules, 5m24.419;) 
				2.1.0	2014-11-03	(shyoon)	K+1 ������ �ڿ������� support Ȯ��, support ������ ������ ������ K supp 0���� �� (A:133rules, 14.939; B: 613rules, 4m52.227;) 
												���� ���Կ��� �Լ� search() ���� (A:133rules, 10.986; B: 613rules, 3m56.206;) 
												�������� (A:133rules, 6.455; B: 613rules, 2m0.260;) 
				2.2.0	2014-11-11	(shyoon)	snort form protocol ǥ�� 
												setCompleteness()
				2.3.0	2014-11-13	(shyoon)	K+1 ������ �����Ǵ� K+1�� SUPP ���� K�� SUPP�� �� ũ�� ���� ����
				2.4.0	2014-11-13	(shyoon)	protocol any�� ��� ��� ��Ģ �߰�
												log�� Flow-level complteness, Packet-level complteness
												depth ����
												���� : supp -> pkt comp
				2.5.0	2014-11-17	(shyoon)	web������ �°� ����
				2.6.0	2014-11-18	(shyoon)	support 1 ��µǴ� ���� ����
												threshold log�� ���
												total Rule ���� ����
												support hight
				3.0.0	2014-11-26	(shyoon)	�ɼ� ó�� -p tragetPath -b blackList
				3.1.0	2014-11-26	(shyoon)	blacklist ���� content�� blacklist�� �����ϸ� �ش� blacklist�� content �и�
				3.2.0	2014-11-27	(shyoon)	sequence, rule�� �������� ǥ��
												HTTP �̸�, HTTP whiteList ���� method, host, useragent, referer
				3.3.0	2014-11-28	(shyoon)	�ɼ� ���� ó�� ���� �ȸ����� ����
												TLS whitelist
				3.4.0	2014-11-28	(shyoon)	multi content ����
				3.5.0	2014-12-02	(shyoon)	���α׷� ����
												split ����
				3.6.0	2014-12-03	(shyoon)	���α׷� ����
												sequence���� header ���� ����
												1�� �ϼ�
				3.7.0	2014-12-05	(shyoon)	�ļ� ����
				3.8.0	2014-12-08	(shyoon)	black, white, split ���� 
												multi thread ����
				3.9.0	2014-12-14	(shyoon)	suspect list ����, supp �˻��Ҷ� suspect list(seqID, offset)�� �˻�

				3.10.0	2014-12-31	(shyoon)	���� 1�� �Է� �� result.txt�� ���� ǥ��

				4.0.0	2015-02-09	(shyoon)	sequence�� ��ȯ	(fileID, pktIDList, header, content(contentID, protocol, field, chatVector, location))
												TLS parser 0x16 handshake�� ����
												multi contetn lenght 1 ���� �Ϸ�
				
				4.0.1	2015-02-10	(shyoon)	multi contetn ����

				4.0.2	2015-02-11	(shyoon)	header ó��

				4.0.3	2015-02-11	(shyoon)	location ó��
												
���		:	
				
���ǻ���	:	
				
������	:	./SnorGen $traget_path

�ֿ��Լ�	: 
########################################################################*/

#include "include.h" 
#include "util.h"
#include "pcaptopkt.h"
#include "pkttoflowwithpkt.h"
#include "captopcap.h"
#include "sequenceExtracter.h"
#include "timeChecker.h"
#include "loadbar.h"

#define MAXSTR 256
//#######################################################################
#define THREAD_COUNT				4
#define PARSER						1		// 0 : parser off, 1: parser on
#define DIFF_MIN_SUPP				2		// 0 : min_supp is file_count, 1 : min_supp is file_count-1 ....
#define MIN_CONTENT_LENGTH			3		// -1 : nolimit
#define MAX_PKT_COUNT_FORWARD		-1		// -1 : nolimit
#define MAX_PKT_COUNT_BACKWARD		-1		// -1 : nolimit
#define MAX_SEQUENCE_LENGTH			-1		// -1 : nolimit
//#######################################################################
int main(int argc, char** argv)
{ 
	int						opt;
	char					caTargetDirectory[1024]={0,};
	char					caResultDirectory[1024]={0,};
	char					caLogFileName[1024]={0,};
	char					caResultFileName[1024]={0,};
	char					caResultTextFileName[1024]={0,};
	char					caHTMLFileName[1024]={0,};
	FILE*					fp_log;
	TimeChecker				cTimeChecker;

	int						iTotalFwpFileCount;
	struct					dirent **filelist;
	DIR						*dp;
	u_int32_t				iIndex;
	struct					stat statbuf;
	char					caTargetFwpFileName[1024]={0,};
	char					caStoreFwpFileName[1024]={0,};
	char					caCMD[1024]={0,};

	while((opt = getopt(argc, argv, ":hp:")) != -1 )
	{
		switch(opt)
		{
			case 'h':
				printf("[USAGE} : ./Snorgen -p targetPath [-b -w]\n");
				exit(0);
				break;
			case 'p':
				strcpy(caTargetDirectory, optarg);
				//puts(caTargetDirectory);
				break;
			case ':':
				printf("%c option needs a value\n",optopt);
				exit(0);
				break;
			case '?':
				printf("unknown option: %c\n",optopt);
				exit(0);
				break;
		}
	}
	if (optind < argc)
	{
		printf("[USAGE} : ./Snorgen -p targetPath\n");
		exit(0);
	}

	//make result path															<--------   log, result ���� ���
	sprintf(caResultDirectory, "%s/tmp", caTargetDirectory);
	sprintf(caCMD, "rm -rf %s",caResultDirectory);
	system(caCMD);
	mkdir(caResultDirectory, 0777);

	sprintf(caResultDirectory, "%s/result", caTargetDirectory);
	sprintf(caCMD, "rm -rf %s",caResultDirectory);
	system(caCMD);
	mkdir(caResultDirectory, 0777);

	sprintf(caLogFileName,"%s/log.txt", caResultDirectory);
	sprintf(caResultFileName,"%s/rule.rules", caResultDirectory);
	sprintf(caResultTextFileName,"%s/rule.txt", caResultDirectory);
	sprintf(caHTMLFileName,"%s/ruleHTML.txt", caResultDirectory);

	//log file open
	if ( (fp_log = fopen(caLogFileName, "wt")) != NULL )
	{


		fprintf(fp_log,"SnorGen\r\n");
		fprintf(fp_log,"Made by Sung-Ho Yoon	(sungho_yoon@korea.ac.kr)\r\n");
		fprintf(fp_log,"Network Management Lab. Korea Univ.   (nmlab.korea.ac.kr)\r\n");
		fprintf(fp_log,"final version. 3.8.0 (2014-12-08)\r\n");
		fprintf(fp_log,"\r\n");
		//
		//puts(caTargetDirectory);

		fprintf(fp_log,"--Configuration--\r\n");
		fprintf(fp_log,"THREAD_COUNT : %d\r\n", THREAD_COUNT);
		fprintf(fp_log,"PARSER : %s\r\n", PARSER?"ON":"OFF");
		fprintf(fp_log,"DIFF_MIN_SUPP : %d (1 : min_supp is (file_count-1))\r\n", DIFF_MIN_SUPP);
		fprintf(fp_log,"MIN_CONTENT_LENGTH : %d\r\n", MIN_CONTENT_LENGTH);
		fprintf(fp_log,"MAX_PKT_COUNT_FORWARD : %d (-1 : nolimit)\r\n", MAX_PKT_COUNT_FORWARD);
		fprintf(fp_log,"MAX_PKT_COUNT_BACKWARD : %d (-1 : nolimit)\r\n", MAX_PKT_COUNT_BACKWARD);
		fprintf(fp_log,"MAX_SEQUENCE_LENGTH : %d (-1 : nolimit)\r\n", MAX_SEQUENCE_LENGTH);
		fprintf(fp_log,"\r\n");

	
		cTimeChecker.reset();
		cTimeChecker.startClock();
		fprintf(fp_log,"--file checking start.--\r\n");
		


		///�м� 1�ܰ� �Ϸ� _ �̼�ȣ(1.18)
		//�⺻���� ��ó�� ����
		//**************************************************************************************************
			//convert cap to pcap with snaplen	captopcap.h   $tatget_path/tmp/file_name.pcap ���� ����
			convertFromCapToPcap(caTargetDirectory, fp_log);
			fprintf(fp_log,"\r\n");
			
			//convert pcap to pkt
			convertFromPcapToPkt(caTargetDirectory, fp_log);
			fprintf(fp_log,"\r\n");
			
			
			//convert pkt to flow_with_pkt
			convertFromPktToFwp(caTargetDirectory, fp_log);
		//**************************************************************************************************

		fprintf(fp_log,"--file checking end.--\r\n");
		cTimeChecker.endClock();
		cTimeChecker.print(fp_log);
		fprintf(fp_log,"\r\n");

		

		cTimeChecker.reset();
		cTimeChecker.startClock();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Snorgen �ٽ� ��Ʈ , �� ����
		fprintf(fp_log,"--rule generating start.--\r\n");

			//gernerate pattern
			sequence(caTargetDirectory, THREAD_COUNT, PARSER, DIFF_MIN_SUPP, MIN_CONTENT_LENGTH, MAX_PKT_COUNT_FORWARD, MAX_PKT_COUNT_BACKWARD, MAX_SEQUENCE_LENGTH, fp_log, caResultTextFileName, caResultFileName, caHTMLFileName);			//<--------   ��Ģ ����
		
		fprintf(fp_log,"--rule generating end.--\r\n");
		cTimeChecker.endClock();
		cTimeChecker.print(fp_log);
		fprintf(fp_log,"\r\n");


		fclose(fp_log);
	}
	else
	{
		g_err((char*)"Snorget main() : logfile fopen error");
	}

}
