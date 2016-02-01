#include "sequenceExtracter.h"

//########################################################################################
void sequence(char* p_caTargetDirectory, u_int32_t p_uiThreadCount, u_int32_t p_uiParser, u_int32_t p_uiDiffSupp, int p_iMinContentLength, int p_iMaxPktCountForward, int p_iMaxPktCountBackward, int p_iMaxSequenceLength, FILE* p_fpLogFile, char* p_caResultTextFileName, char* p_caResultFileName, char* p_caHTMLFileName)	//2014-12-08
{
	//###################################################
	//###################################################
	//###################################################
	//################ ��ó�� ###########################
	//###################################################
	//###################################################
	//###################################################

	//###################################################
	//p_caTargetDirectory/tmp ���丮���� .fwp ������ �о� hash�� �ε� 
	char		caTragetDirectory[1024]={0,};
	FlowHash	cFlowHash;
	int			iTotalFwpFileCount;

	sprintf(caTragetDirectory, "%s/tmp", p_caTargetDirectory);			//fwp�� ����Ǿ� �ִ� ���丮 �̸� set
	//puts(caTragetDirectory);
	
	cFlowHash.reset();
	iTotalFwpFileCount = cFlowHash.loadFlow(caTragetDirectory);			//caTragetDirectory�� ����Ǿ� �ִ� ��� fwp ������ hash�� �ε��ϰ� ���� ���� ����
	cFlowHash.resetFlowListSortByTime();
//	cFlowHash.printFlowList();
	cFlowHash.resetPktListSortByTime();
//	cFlowHash.printPktList();
	
	printf("total file count : %d\n", iTotalFwpFileCount);
	fprintf(p_fpLogFile,"total file count : %d\r\n", iTotalFwpFileCount);
	
	if (iTotalFwpFileCount >= 64)										//���� ó�� - ���� ���� 64�̻� ���� (support ������ 64bits flag ���)
	{
		fprintf(p_fpLogFile,"ERROR!! : MAX file_count is 64. current file_count : %d\r\n", iTotalFwpFileCount);
		g_err((char*)"sequence() : file count error");
	}

	if (iTotalFwpFileCount < 2)											//����ó�� - ���� ���� �ּ� 2 �̻� �ʿ�
	{
		FILE*					fp;
		if ( (fp = fopen(p_caHTMLFileName, "wt")) != NULL )
		{
			fprintf(fp, "<span class='run_hl1'>ERROR: minimum file count 2 for SronGen</span>\r\n");
			fclose(fp);
		}

		fprintf(p_fpLogFile, "ERROR: minimum file count 2 for SronGen\r\n");
		g_err((char*)"ERROR: minimum file count 2 for SronGen");
	}
	
	//######################################################################
	//hash�� ����� fwp�� �о� SequenceSet�� ����
	SequenceVector cMultiSequenceVector;
	SequenceVector cSingleSequenceVector;
	
	cMultiSequenceVector.reset();
	cMultiSequenceVector.load(&cFlowHash, p_iMaxPktCountForward, p_iMaxPktCountBackward, p_iMaxSequenceLength);	//cFlowHash�� ����� fwp�� �о� cSequence�� ���� 
	//�ε�� ���ÿ� MultiSequenceVector�� �ϼ���

//	cMultiSequenceVector.print();

	printf("after load cMultiSequenceVector : %d\n",cMultiSequenceVector.getSequenceSize());
	fprintf(p_fpLogFile,"after load total Sequence : %d\r\n",cMultiSequenceVector.getSequenceSize());

	cSingleSequenceVector.reset();
	cSingleSequenceVector.insert(&cMultiSequenceVector);
	//�̱� ���������Ϳ� �� ��Ƽ���������͸� �����ұ�??????

	
	if (p_uiParser)
	{
		//######################################################################
		//cSequence�� �������� �ļ��� �ɰ�
		cSingleSequenceVector.parser();
	//	cSingleSequenceVector.print();

		printf("after parser cSingleSequenceVector : %d\n",cSingleSequenceVector.getSequenceSize());
		fprintf(p_fpLogFile,"after parser total cSingleSequenceVector : %d\r\n",cSingleSequenceVector.getSequenceSize());
	}
	
	cSingleSequenceVector.sortContent();			//support ���� ������ �ϱ� ���� fileid ����	
//	cSingleSequenceVector.print();
	cSingleSequenceVector.breadkField();				//field �� sequence ���� ����


	//###################################################
	//###################################################
	//###################################################
	//################ �����ܵ������� ##################
	//###################################################
	//###################################################
	//###################################################

		
	//######################################################################				
	int iTargetSupp = iTotalFwpFileCount - p_uiDiffSupp;
	const int	iMinSupp = 2;
	if (iTargetSupp < iMinSupp) 
		iTargetSupp = iMinSupp;
	
	RuleList	cDetermineSingleRuleList;
	RuleList	cCandiSingleRuleList;
	
	int iContentLenght;

	int			lots_of_thread;
	int			res;
	pthread_t	a_thread[p_uiThreadCount];
	void		*thread_result;
	THRED_ARG	threadArg[p_uiThreadCount];	

	
	//######################################################################
	//���� 1�� �ĺ����� ����
	printf("######### Extract Candidate content 1 (supp:%d)#########\n",iTargetSupp);
	cCandiSingleRuleList.extractSingleConentLength1(&cSingleSequenceVector);
	printf("cCandiSingleRuleList : %d\n",cCandiSingleRuleList.getRuleSize());
	cCandiSingleRuleList.setSingleSuspectSet(&cSingleSequenceVector);					//�߰ߵ� ���ɼ��� �ִ� ������ sequence�� ������ ��Ģ�� �̸� ���
	cCandiSingleRuleList.setSupportSingle(&cSingleSequenceVector, iTotalFwpFileCount);
	cCandiSingleRuleList.deleteUnderSupport(iTargetSupp);
	printf("after delete under supp cCandiSingleRuleList : %d\n",cCandiSingleRuleList.getRuleSize());
//	cCandiSingleRuleList.print();

	//######################################################################
	//���� K�� �ĺ����� ����
	iContentLenght=1;
	while (cCandiSingleRuleList.getRuleSize())
	{
		//######################################################################
		//���� K�� ���� ���� (cCandiSingleRuleList -> cDetermineSingleRuleList)
		printf("#########  Copy Detemine content %d  (supp:%d)#########\n",iContentLenght,iTargetSupp);
		cDetermineSingleRuleList.insert(&cCandiSingleRuleList);
		//cDetermineSingleRuleList.print();
		printf("cDetermineSingleRuleList : %d\n",cDetermineSingleRuleList.getRuleSize());

		//######################################################################
		//	���� K�� �������� iContentLenght+1�� �ĺ� ���� ����
		printf("######### Extract Candidate content %d (supp:%d)#########\n",iContentLenght+1,iTargetSupp);

		cCandiSingleRuleList.reset();
		if (p_uiThreadCount)
		{
			for (lots_of_thread=0; lots_of_thread < p_uiThreadCount; lots_of_thread++)
			{
				threadArg[lots_of_thread].m_iIndex = lots_of_thread;							//thread �ε���
				threadArg[lots_of_thread].m_iTotalThreadCount = p_uiThreadCount;				//thread �� ����
				threadArg[lots_of_thread].m_cpSequenceVector = &cSingleSequenceVector;				//sequence ����Ʈ
				threadArg[lots_of_thread].m_cpDetermineRuleList = &cDetermineSingleRuleList;	//����� rule ����Ʈ
				threadArg[lots_of_thread].m_iTrargetLength = iContentLenght;					//���� ��� ����
				threadArg[lots_of_thread].m_uiStartIndexTargetLength = cDetermineSingleRuleList.getStartIndexTargetLength(iContentLenght);	// ����� rule ����Ʈ���� ���� ��� ���̰� �����ϴ� ���ؽ�
				threadArg[lots_of_thread].m_uiTargetSupp = iTargetSupp;							//target ������
				threadArg[lots_of_thread].m_uiMaxSupp = iTotalFwpFileCount;						//���� ����


				res = pthread_create(&a_thread[lots_of_thread], NULL, insertCandiSingle, (void *)&threadArg[lots_of_thread]);
				if (res != 0)
				{
					perror("thread creation failed");
					exit(EXIT_FAILURE);
				}
			}
			for (lots_of_thread=0; lots_of_thread < p_uiThreadCount; lots_of_thread++)
			{
				res = pthread_join(a_thread[lots_of_thread], &thread_result);
				if (res!=0)
				{
					perror("thread join failed");
					exit(EXIT_FAILURE);
				}
				cCandiSingleRuleList.insert(&threadArg[lots_of_thread].m_cpTempCandiRuleList);
				threadArg[lots_of_thread].m_cpTempCandiRuleList.reset();
			}
		}
		else																						//<------------  no thread
		{
			cCandiSingleRuleList.extractSingleConent(cDetermineSingleRuleList.getStartIndexTargetLength(iContentLenght), cDetermineSingleRuleList.getRuleSize(), &cDetermineSingleRuleList, iContentLenght, &cSingleSequenceVector, iTargetSupp, iTotalFwpFileCount);	// ���� iContentLenght�� ���� �����Ͽ� iContentLenght+1�� ����, ���տ� ����� ���� ���� (supp = 0)

		}
		cDetermineSingleRuleList.deleteUnderSupport(iTargetSupp);
		
		printf("cCandiSingleRuleList : %d\n",cCandiSingleRuleList.getRuleSize());
				
		iContentLenght++;
	}
	printf("after single extraction cDetermineSingleRuleList : %d\n",cDetermineSingleRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after single extraction single content rule : %d\r\n",cDetermineSingleRuleList.getRuleSize());
//	cDetermineSingleRuleList.print();

	if (p_iMinContentLength > 0)
	{
		//MIN_CONTENT_LENGTH ���� ª�� content ����	, �� ��� ���� sequence�� ���̿� �����ϸ� ����(GET ���)								
		cDetermineSingleRuleList.trimUnderContentLength(p_iMinContentLength, &cSingleSequenceVector);
		printf("after trim cDetermineSingleRuleList : %d\n",cDetermineSingleRuleList.getRuleSize());
		fprintf(p_fpLogFile,"after trim single content rule : %d\r\n",cDetermineSingleRuleList.getRuleSize());
	}
//	cDetermineSingleRuleList.print();



	//###################################################
	//###################################################
	//###################################################
	//################ �����ܵ������� ##################
	//###################################################
	//###################################################
	//###################################################

	RuleList	cDetermineMultiRuleList;
	RuleList	cCandiMultiRuleList;


	cSingleSequenceVector.integrate(&cDetermineSingleRuleList);			//cDetermineSingleRuleList�� content�� source sequence�� ����
//	cSingleSequenceVector.print();

	cMultiSequenceVector.integrate(&cSingleSequenceVector);				//cSingleSequenceVector�� multi content�� source pkt�� ����
//	cMultiSequenceVector.print();

	//######################################################################
	//���� 1�� �ĺ����� ����
	printf("######### Extract Candidate multi-content 1 (supp:%d)#########\n",iTargetSupp);
	cCandiMultiRuleList.extractMultiConentLength1(&cMultiSequenceVector);
	printf("cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());
	cCandiMultiRuleList.setMultiSuspectSet(&cMultiSequenceVector);					//�߰ߵ� ���ɼ��� �ִ� ������ sequence�� ������ ��Ģ�� �̸� ���
	cCandiMultiRuleList.setSupportMulti(&cMultiSequenceVector, iTotalFwpFileCount);
	cCandiMultiRuleList.deleteUnderSupport(iTargetSupp);
	printf("after delete under supp cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());
//	cCandiMultiRuleList.print();

	//######################################################################
	//���� K�� �ĺ����� ����
	iContentLenght=1;
	while (cCandiMultiRuleList.getRuleSize())
	{
		//######################################################################
		//���� K�� ���� ���� (cCandiMultiRuleList -> cDetermineMultiRuleList)
		printf("#########  Copy Detemine content %d  (supp:%d)#########\n",iContentLenght,iTargetSupp);
		cDetermineMultiRuleList.insert(&cCandiMultiRuleList);
	//	cDetermineMultiRuleList.print();
		printf("cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());

		//######################################################################
		//	���� K�� �������� iContentLenght+1�� �ĺ� ���� ����
		printf("######### Extract Candidate content %d (supp:%d)#########\n",iContentLenght+1,iTargetSupp);

		cCandiMultiRuleList.reset();
		cCandiMultiRuleList.extractMultiConent(cDetermineMultiRuleList.getStartIndexTargetCount(iContentLenght), cDetermineMultiRuleList.getRuleSize(), &cDetermineMultiRuleList, iContentLenght, &cMultiSequenceVector, iTargetSupp, iTotalFwpFileCount);	// ���� iContentLenght�� ���� �����Ͽ� iContentLenght+1�� ����, ���տ� ����� ���� ���� (supp = 0)
		cDetermineMultiRuleList.deleteUnderSupport(iTargetSupp);
		printf("cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());
				
		iContentLenght++;
	}
	printf("after multi extraction cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after multi extraction multi content rule : %d\r\n",cDetermineMultiRuleList.getRuleSize());
//	cDetermineMultiRuleList.print();


	
	
	//###################################################
	//###################################################
	//###################################################
	//############### ������ ��ġ ���� #################
	//###################################################
	//###################################################
	//###################################################


	cDetermineMultiRuleList.setLocation(&cMultiSequenceVector);																// rule�� suspects list�� ���� ��Ŷ ������ cMultiSequenceVector�� �����Ͽ� ������ ��ġ ���� set
	printf("after setLocation cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after setLocation multi content rule : %d\r\n",cDetermineMultiRuleList.getRuleSize());
//	cDetermineMultiRuleList.print();

	
	
	
	
	
	//###################################################
	//###################################################
	//###################################################
	//################## ��� ���� #####################
	//###################################################
	//###################################################
	//###################################################


	cDetermineMultiRuleList.setHeader(&cMultiSequenceVector);																// rule�� suspects list�� ���� ��Ŷ ������ cMultiSequenceVector�� �����Ͽ� ��� ���� set
	printf("after setHeader cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after setHeader multi content rule : %d\r\n",cDetermineMultiRuleList.getRuleSize());
//	cDetermineMultiRuleList.print();


	//###################################################
	//###################################################
	//###################################################
	//################ �м���Ȯ�� ######################
	//###################################################
	//###################################################
	//###################################################

	cDetermineMultiRuleList.setCompleteness(&cFlowHash);
	printf("after setCompleteness cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after setCompleteness multi content rule : %d\r\n",cDetermineMultiRuleList.getRuleSize());
	cDetermineMultiRuleList.print();
	
	
	
	
return;






/*


	cDetermineSingleRuleList.resetSupspects();			//signle content�� ���� ����� suspect set�� ���̻� ��ȿ���� �ʱ� ������ ����
	
	cDetermineSingleRuleList.setLocation(&cSequenceVector);								//	cSequenceMultiList�� �����Ͽ� content�� ��ġ���� set
	printf("after set location cDetermineSingleRuleList : %d\n",cDetermineSingleRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after set location single content rule : %d\r\n",cDetermineSingleRuleList.getRuleSize());
//	cDetermineSingleRuleList.print();


	//###################################################
	//###################################################
	//###################################################
	//######### ��� �� ���� �м������� ################
	//###################################################
	//###################################################
	//###################################################
		
	cDetermineSingleRuleList.setHeader(&cFlowHash, p_iMaxPktCountForward, p_iMaxPktCountBackward, p_iMaxSequenceLength);		//	cFlowHash�� �����Ͽ� rule�� ������� set
	printf("after setHeader location cDetermineSingleRuleList : %d\n",cDetermineSingleRuleList.getRuleSize());
	fprintf(p_fpLogFile,"after setHeader location single content rule : %d\r\n",cDetermineSingleRuleList.getRuleSize());
	//cDetermineSingleRuleList.print();
	
		
	//###################################################
	//###################################################
	//###################################################
	//################ �м���Ȯ�� ######################
	//###################################################
	//###################################################
	//###################################################

	
	fprintf(p_fpLogFile,"total Rule : %d\r\n",cDetermineSingleRuleList.getRuleSize());
	cDetermineSingleRuleList.setCompleteness(&cFlowHash);
	
	FPB* cpFlowTotal = cDetermineSingleRuleList.getFlowTotalTraffic();
	FPB* cpFlowIdentified = cDetermineSingleRuleList.getFlowIdentifiedTraffic();
	FPB* cpPktTotal = cDetermineSingleRuleList.getPktTotalTraffic();
	FPB* cpPktIdentified = cDetermineSingleRuleList.getPktIdentifiedTraffic();
	if (cpFlowTotal->getFlow())
	{
		fprintf(p_fpLogFile,"Flow-level Completness\r\n flow: %.02f(%llu/%llu) pkt: %.02f(%llu/%llu) byte: %.02f(%llu/%llu)\r\n",
			(float)cpFlowIdentified->getFlow()*100/cpFlowTotal->getFlow(), cpFlowIdentified->getFlow(), cpFlowTotal->getFlow(),
			(float)cpFlowIdentified->getPkt()*100/cpFlowTotal->getPkt(), cpFlowIdentified->getPkt(), cpFlowTotal->getPkt(),
			(float)cpFlowIdentified->getByte()*100/cpFlowTotal->getByte(), cpFlowIdentified->getByte(), cpFlowTotal->getByte());
		printf("Flow-level Completness\r\n flow: %.02f(%llu/%llu) pkt: %.02f(%llu/%llu) byte: %.02f(%llu/%llu)\r\n",
			(float)cpFlowIdentified->getFlow()*100/cpFlowTotal->getFlow(), cpFlowIdentified->getFlow(), cpFlowTotal->getFlow(),
			(float)cpFlowIdentified->getPkt()*100/cpFlowTotal->getPkt(), cpFlowIdentified->getPkt(), cpFlowTotal->getPkt(),
			(float)cpFlowIdentified->getByte()*100/cpFlowTotal->getByte(), cpFlowIdentified->getByte(), cpFlowTotal->getByte());
	}

	if (cpPktTotal->getPkt())
	{
		fprintf(p_fpLogFile,"Pcaket-level Completness\r\n pkt:%.02f(%llu/%llu) byte:%.02f(%llu/%llu)\r\n",
			(float)cpPktIdentified->getPkt()*100/cpPktTotal->getPkt(), cpPktIdentified->getPkt(), cpPktTotal->getPkt(),
			(float)cpPktIdentified->getByte()*100/cpPktTotal->getByte(), cpPktIdentified->getByte(), cpPktTotal->getByte());
		printf("Pcaket-level Completness\r\n pkt:%.02f(%llu/%llu) byte:%.02f(%llu/%llu)\r\n",
			(float)cpPktIdentified->getPkt()*100/cpPktTotal->getPkt(), cpPktIdentified->getPkt(), cpPktTotal->getPkt(),
			(float)cpPktIdentified->getByte()*100/cpPktTotal->getByte(), cpPktIdentified->getByte(), cpPktTotal->getByte());
	}
	//cDetermineSingleRuleList.print();


	//###################################################
	//###################################################
	//###################################################
	//################ ���� #############################
	//###################################################
	//###################################################
	//###################################################

	//���� 
	cDetermineSingleRuleList.sortFlowLevelCompPkt();
	cDetermineSingleRuleList.sortFirstContentFix();
	cDetermineSingleRuleList.sortSupport();


		
	//###################################################
	//###################################################
	//###################################################
	//################ ��� #############################
	//###################################################
	//###################################################
	//###################################################

	//���� ��� ���
	cDetermineSingleRuleList.print();
	cDetermineSingleRuleList.print(p_caResultTextFileName);
	cDetermineSingleRuleList.printSnortFormHTML(p_caHTMLFileName);
	cDetermineSingleRuleList.printSnortForm(p_caResultFileName);
	
	return;


*/











	



	
	

/*


		
	//###################################################
	//###################################################
	//###################################################
	//################ �����ܵ������� ##################
	//###################################################
	//###################################################
	//###################################################

	RuleList	cDetermineMultiRuleList;
	RuleList	cCandiMultiRuleList;

	int iContentCount;

	//######################################################################
	//���� 1�� �ĺ����� ����
	printf("######### Extract Candidate Sequence 1 (supp:%d)#########\n",iTargetSupp);
	cCandiMultiRuleList.extractMultiConentLength1(&cDetermineSingleRuleList);			//���� content�� ���� 1�� ���� content�� �ǹ�
	printf("cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());
	cCandiMultiRuleList.setMultiSuspectSet(&cSequenceMultiVector);					//�߰ߵ� ���ɼ��� �ִ� ������ sequence�� ������ ��Ģ�� �̸� ���
	cCandiMultiRuleList.setSupportMulti(&cSequenceMultiVector, iTotalFwpFileCount);
	cCandiMultiRuleList.deleteUnderSupport(iTargetSupp);
	printf("after delete under supp cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());
//	cCandiMultiRuleList.print();
//	return;

	//######################################################################
	//���� K�� �ĺ����� ����
	iContentCount=1;
	while (cCandiMultiRuleList.getRuleSize())
	{
		//######################################################################
		//���� K�� ���� ���� (cCandiMultiRuleList -> cDetermineMultiRuleList)
		printf("#########  Copy Detemine Sequence %d  (supp:%d)#########\n",iContentCount,iTargetSupp);
		cDetermineMultiRuleList.insert(&cCandiMultiRuleList);
		//cDetermineMultiRuleList.print();
		printf("cDetermineMultiRuleList : %d\n",cDetermineMultiRuleList.getRuleSize());

		//######################################################################
		//	���� K�� �������� iContentCount+1�� �ĺ� ���� ����
		printf("######### Extract Candidate Sequence %d (supp:%d)#########\n",iContentCount+1,iTargetSupp);

		cCandiMultiRuleList.reset();
		if (p_uiThreadCount)
		{
			for (lots_of_thread=0; lots_of_thread < p_uiThreadCount; lots_of_thread++)
			{
				threadArg[lots_of_thread].m_iIndex = lots_of_thread;
				threadArg[lots_of_thread].m_iTotalThreadCount = p_uiThreadCount;
				threadArg[lots_of_thread].m_cpSequenceVector = &cSequenceMultiVector;
				threadArg[lots_of_thread].m_cpDetermineRuleList = &cDetermineMultiRuleList;
				threadArg[lots_of_thread].m_iTrargetLength = iContentCount;
				threadArg[lots_of_thread].m_uiStartIndexTargetLength = cDetermineMultiRuleList.getStartIndexTargetCount(iContentCount);	
				threadArg[lots_of_thread].m_uiTargetSupp = iTargetSupp;
				threadArg[lots_of_thread].m_uiMaxSupp = iTotalFwpFileCount;						//���� ����


				res = pthread_create(&a_thread[lots_of_thread], NULL, insertCandiMulti, (void *)&threadArg[lots_of_thread]);
				if (res != 0)
				{
					perror("thread creation failed");
					exit(EXIT_FAILURE);
				}
			}
			for (lots_of_thread=0; lots_of_thread < p_uiThreadCount; lots_of_thread++)
			{
				res = pthread_join(a_thread[lots_of_thread], &thread_result);
				if (res!=0)
				{
					perror("thread join failed");
					exit(EXIT_FAILURE);
				}
				cCandiMultiRuleList.insert(&threadArg[lots_of_thread].m_cpTempCandiRuleList);
				threadArg[lots_of_thread].m_cpTempCandiRuleList.reset();
			}

		}
		else
		{
			cCandiMultiRuleList.extractMultiConent(cDetermineMultiRuleList.getStartIndexTargetCount(iContentCount), cDetermineMultiRuleList.getRuleSize(), &cDetermineMultiRuleList, iContentCount, &cSequenceMultiVector, iTargetSupp, iTotalFwpFileCount);	// ���� iContentLenght�� ���� �����Ͽ� iContentCount+1�� ����, ���տ� ����� ���� ���� (supp = 0)
		}
		cCandiMultiRuleList.unique();			//������ ��Ģ ������ �ʵ�, content�� �����ϰ�, �� ��Ģ�� ��������, 
	
		cDetermineMultiRuleList.deleteUnderSupport(iTargetSupp);
		
		printf("cCandiMultiRuleList : %d\n",cCandiMultiRuleList.getRuleSize());

		iContentCount++;
	}
//		cDetermineMultiRuleList.print();
	//getchar();
//	return;









	cDetermineMultiRuleList.uniqueField();									//�ʵ尡 ������ ��õ� ��Ģ�� content�� ���� �� �͸� ����� ����
//	cDetermineMultiRuleList.setLocation(&cSequenceMultiVector);				//	cSequenceMultiList�� �����Ͽ� content�� ��ġ���� set
	cDetermineMultiRuleList.print();
	
	return;
	*/
	
}
//########################################################################################
void *insertCandiSingle(void *arg)
{
	
	THRED_ARG* spArg = (THRED_ARG*)arg;

	int iLow = (int)( (spArg->m_cpDetermineRuleList->getRuleSize() - spArg->m_uiStartIndexTargetLength) / spArg->m_iTotalThreadCount) * spArg->m_iIndex;
	int iHigh = (int)((spArg->m_cpDetermineRuleList->getRuleSize() - spArg->m_uiStartIndexTargetLength) / spArg->m_iTotalThreadCount) * (spArg->m_iIndex+1);

	iLow += spArg->m_uiStartIndexTargetLength;
	iHigh += spArg->m_uiStartIndexTargetLength;

	if (spArg->m_iIndex+1 == spArg->m_iTotalThreadCount) //������ ������� ������ �˻�
		iHigh = spArg->m_cpDetermineRuleList->getRuleSize();

	//printf("%d	%d	%d\n",spArg->m_iIndex, iLow, iHigh);

	spArg->m_cpTempCandiRuleList.reset();

	spArg->m_cpTempCandiRuleList.extractSingleConent(iLow, iHigh, spArg->m_cpDetermineRuleList, spArg->m_iTrargetLength, spArg->m_cpSequenceVector, spArg->m_uiTargetSupp, spArg->m_uiMaxSupp);

//	printf("thread %d finish\n",spArg->m_iIndex);
	
	pthread_exit(NULL);
}
//########################################################################################
void *insertCandiMulti(void *arg)
{
	
	THRED_ARG* spArg = (THRED_ARG*)arg;

	int iLow = (int)( (spArg->m_cpDetermineRuleList->getRuleSize() - spArg->m_uiStartIndexTargetLength) / spArg->m_iTotalThreadCount) * spArg->m_iIndex;
	int iHigh = (int)((spArg->m_cpDetermineRuleList->getRuleSize() - spArg->m_uiStartIndexTargetLength) / spArg->m_iTotalThreadCount) * (spArg->m_iIndex+1);

	iLow += spArg->m_uiStartIndexTargetLength;
	iHigh += spArg->m_uiStartIndexTargetLength;

	if (spArg->m_iIndex+1 == spArg->m_iTotalThreadCount) //������ ������� ������ �˻�
		iHigh = spArg->m_cpDetermineRuleList->getRuleSize();

	//printf("%d	%d	%d\n",spArg->m_iIndex, iLow, iHigh);

	spArg->m_cpTempCandiRuleList.reset();

	spArg->m_cpTempCandiRuleList.extractMultiConent(iLow, iHigh, spArg->m_cpDetermineRuleList, spArg->m_iTrargetLength, spArg->m_cpSequenceVector, spArg->m_uiTargetSupp, spArg->m_uiMaxSupp);

//	printf("thread %d finish\n",spArg->m_iIndex);
	
	pthread_exit(NULL);
}
