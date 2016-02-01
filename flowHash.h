#include "include.h"
#include "twowayFlow.h"
#include "veri.h"
#include "loadbar.h"
#include <list>
using namespace std;

#ifndef __FlowHash_h
#define __FlowHash_h

#define	MAX_STORE_PKT		10
#define MAXHASH				32768	

#define SAME_SEQUENCE		0		//������ ������ packet
#define AFTER_REPACKET		1		//re-paketer ó��
//#############################################################################
class FlowHash		
{
	public:
		FlowTwowayContainer		*m_cpFlowTwowayContainerHT;
		
		FPB						m_cFPBToTal;
		FPB						m_cFPBIdentified;

		int						m_iFlowIndex;

		list<FlowTwowayContainer*> m_cFlowTwoWayContainerList;
		list<PacketContainer*> m_cPacketContainerList;

	public:
		FlowHash();
		~FlowHash();

		void reset();
		void resetAnal();
		void recal();										//m_cFPBToTal, m_cFPBIdentified ���
	
		void print();										//m_cFPBToTal,m_cFPBIdentified ȭ�鿡 ��� 
		void printDetail();									//fwp ȭ�鿡 ��� 
		void printFlowList();								//m_cFlowTwoWayContainerList ȭ�鿡 ���
		void printPktList();
		void print(char *p_caLogFileName);					//m_cFPBToTal,m_cFPBIdentified ���Ͽ� ��� 
		void printDetail(char *p_caLogFileName);			//fwp ���Ͽ� ��� 
		void printFlowList(char *p_caLogFileName);			//m_cFlowTwoWayContainerList ���Ͽ� ���
		void printPktList(char *p_caLogFileName);			

		
		u_int32_t loadFlow(char* p_caTragetDirectory);			//p_caTragetDirectory ����Ǿ� �ִ� ��� fwp ������ hash�� �ε��ϰ� ���� ���� ����
		int loadFlow(char *p_caFlowFileName, int p_iFileCount);	//flow�� fwp�� �о� �޸𸮿� �ε�, p_iFileCount�� flow�� fileID�� set
		int loadPktToFlowWithPkt(char *p_caFlowFileName);		//pkt�� �о� fwp�� �޸𸮿� �ε�
		void insert(PacketContainer *r);						//r(pkt)�� �޸𸮿� fwp�� �ε�
		
		
		void store(char *p_caFlowFileName);					//�޸𸮿� �ε�� flow�� fwp�� �ش� ���Ͽ� ����
		void store(char *p_caResultFlowFilePathName, char *p_caResultFlowFileName); //�޸𸮿� �ε�� flow�� fwp�� �ش� ���Ͽ� ����

		void deleteNonData();								//data pkt�� ���� fwp ����
		void deleteSynAckRst();								//TCP �� Syn-AckRst ��Ŷ���θ� ������ �÷ο츦 ����
		void deleteTcpOneWay();								//TCP �� one way �÷ο츦 ����
		void deleteRetransmission();						//TCP �� Retransmission pkt�� ���� ���� : TCP && ���� ��Ŷ�� 2�� �̳� �߻� && ���� ��Ŷ�� Sequence Num  ���� && paylaod ��
		
		void resetFlowListSortByTime();															//FlowTwowayContainer�� list�� �����ϰ� forward�� start �������� sort
			struct Compare {																	
				bool operator()(FlowTwowayContainer* lhs, FlowTwowayContainer* rhs)						
				{
					if (lhs->forward.start.tv_sec < rhs->forward.start.tv_sec)
						return true;
					if ((lhs->forward.start.tv_sec == rhs->forward.start.tv_sec) && (lhs->forward.start.tv_usec < rhs->forward.start.tv_usec))
						return true;
					return false;
				}
			};
		void resetPktListSortByTime();
			struct ComparePkt {																	
				bool operator()(PacketContainer* lhs, PacketContainer* rhs)						
				{
					if (lhs->pkt.time_sec < rhs->pkt.time_sec)
						return true;
					if ((lhs->pkt.time_sec == rhs->pkt.time_sec) && (lhs->pkt.time_usec < rhs->pkt.time_usec))
						return true;
					return false;
				}
			};

		int crossOrderResolver(void);
		int crossOrderDetector(FlowBasic flow, PacketContainer *cp_pre_pkt, PacketContainer *cp_go_pkt);
	
	private:
		void insert(FlowTwowayContainer *p_cpFlowTwowayContainer, FILE *p_fpLoad);	//flow�� �޸𸮿� �ε� fwp�ϼ��� �ֱ� ������ file pointer�� ���� �ѱ�
		void insertForFlowWithPkt(PacketContainer *r, FlowTwowayContainer *go);		//go(flow)�� pkt�� ����
			
		unsigned int hashing(FlowBasic *r);
		int isSameRecord(FlowBasic *a, FlowBasic *b);
		int isSameRecordReverse(FlowBasic *a, FlowBasic *b);
		int isSameRecord(FlowTwowayContainer *a, FlowTwowayContainer *b);
		int isSameRecordReverse(FlowTwowayContainer *a, FlowTwowayContainer *b);

		unsigned int hashing(Packet *r);											//r(pkt)�� �̿��Ͽ� hash key ����
		int isSameRecord(Packet *a, FlowBasic *b);									//a(pkt)�� b(flow)�� ���������� ���� ���ڵ�����
		int isSameRecordReverse(Packet *a, FlowBasic *b);							//a(pkt)�� b(flow)�� ���������� ���� ���ڵ�����

		int isSameRecord(Packet *a, Packet *b);

		int setCode(FlowTwowayContainer* p_cpFlowTwowayContainer);
		void storeToTargetPathEachFlow(char *p_caResultFlowFilePathName);
		void storeToTargetPathUDPFlow(char *p_caResultFlowFilePathName);

		void modRetransmission(FlowTwowayContainer *go, PacketContainer *sameDirectionRecentPkt, PacketContainer *curPkt, int status);
		void modOutoforder(FlowTwowayContainer *go);
};

#endif
