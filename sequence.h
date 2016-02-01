#ifndef __sequence_h
#define __sequence_h

#include "include.h"
#include "twowayFlow.h"
#include "packet.h"
#include "loadbar.h"
#include "veri.h"
#include "ip.h"
#include "flowHash.h"
#include "uniqueCount.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>    // std::search
using namespace std;

class Header;
class Content;
class Suspect;
class Rule;
class RuleList;
class Sequence;
class SequenceVector;
class ContentSequence;
class ContentSequenceList;

#define BASE_SID			1000000

#define COMMON				0
#define FIRST				1
#define NO_FIRST			2

#define HIGHLIGHT_START_1	"<span class='run_hl1'>"
#define HIGHLIGHT_END_1		"</span>"
#define HIGHLIGHT_START_2	"<span class='run_hl2'>"
#define HIGHLIGHT_END_2		"</span>"
#define HIGHLIGHT_START_3	"<span class='run_hl3'>"
#define HIGHLIGHT_END_3		"</span>"

#define PROT_UNKNOWN				0x00000001
#define PROT_HTTP					0x00000002
#define PROT_TLS					0x00000004

#define PROT_UNKNOWN_DATA			0x00000001

#define PROT_HTTP_METHOD			0x00000001
#define PROT_HTTP_URL				0x00000002
#define PROT_HTTP_HOST				0x00000004
#define PROT_HTTP_USER				0x00000008
#define PROT_HTTP_REF				0x00000010
#define PROT_HTTP_COOK				0x00000020
#define PROT_HTTP_RESPONSE			0x00000040
#define PROT_HTTP_DATA				0x00000080

#define PROT_TLS_CHANGESPEC			0x00000001
#define PROT_TLS_ALERT				0x00000002
#define PROT_TLS_HANDSHAKE			0x00000004
#define PROT_TLS_APPLICATION		0x00000008
#define PROT_TLS_HEARTBEAT			0x00000010
#define PROT_TLS_DATA				0x00000020




//######################################################################
class Header
{
	public:
		typedef struct sAddr
		{
			u_int32_t	uiAddr;
			u_int8_t	uiCIDR;

		}ADDR;

		u_int8_t	uiProtocol;
		list<u_int8_t> liProtocol;
		ADDR		usSrcAddr;
		list<ADDR> lsSrcAddr;
		ADDR		usDstAddr;
		list<ADDR> lsDstAddr;
		u_int16_t	uiSrcPort;
		list<u_int16_t> liSrcPort;
		u_int16_t	uiDstPort;
		list<u_int16_t> liDstPort;

	public:
		Header();
		~Header();
		void reset();
		void print();
		void print(FILE* p_fpFile);
		void print(list<ADDR>* p_lAddr);

		void setProt(u_int8_t p_uiProt);
		void setHeader(Header* p_cpHeader);
		void setHeader(FlowTwowayContainer* p_cFlowTwowayContainer);			//flow�� ��� ������ �̿��Ͽ� cHeader set
		void setHeaderReverse(FlowTwowayContainer* p_cFlowTwowayContainer);		//flow�� ��� ������ �̿��Ͽ� cHeader set
		
		u_int8_t getProt();
		list<u_int8_t>::iterator getProtListBegin();
		list<u_int8_t>::iterator getProtListEnd();

		int cmpHeader(Header* p_cpHeader);										//p_cpHeader�� ���Ͽ� ������ -1, ũ�� 1, ������ 0
		void setMaskSrcAddr(u_int32_t p_uiMask);
		void setMaskDstAddr(u_int32_t p_uiMask);
		u_int32_t getSrcAddrCIDR_start();
		u_int32_t getDstAddrCIDR_start();
		u_int32_t getSrcAddrCIDR_end();
		u_int32_t getDstAddrCIDR_end();
		void finalize();
			struct CompareAddr {																	
				bool operator()(ADDR lhs, ADDR rhs){
					return (ntohl(lhs.uiAddr) > ntohl(rhs.uiAddr));
				}
			};
			struct isSame {																	
				bool operator()(ADDR lhs, ADDR rhs){
					if (ntohl(lhs.uiAddr) == ntohl(rhs.uiAddr))
						return true;
					else
						return false;				
				}
			};
		bool isIndentify(FlowTwowayContainer* p_cpFlowTwowayContainer);
			bool isForwardIndentify(FlowTwowayContainer* p_cpFlowTwowayContainer);
			bool isBackwardIndentify(FlowTwowayContainer* p_cpFlowTwowayContainer);
		bool isIndentify(PacketContainer* p_cpPacketContainer);
};
//######################################################################
class Content
{
	public:
		u_int32_t	uiID;
		
		u_int32_t	uiProtocol;					//HTTP, TLS, UNKNOWN
		u_int32_t	uiField;					//HTTP Field, TLS Field
		vector<char> vcChars;
		
		u_int32_t	uiOffset;
		u_int32_t	uiDepth;
		u_int32_t	uiDistance;
		u_int32_t	uiWithin;
		
	public:
		Content();
		~Content();
		void reset();
		
		void print();												//��� ��� ������ ����� ���� ȭ�鿡 ���
		void print(FILE* p_fpFile);									//��� ��� ������ ����� ���� ���Ͽ� ���
		void printContent();										//vcChars�� ����� ���ڸ� ȭ�鿡 ���, printable ���ڴ� �״��, non-printable ���ڴ� 16������ ���
		void printContent(FILE* p_fpFile);							//vcChars�� ����� ���ڸ� ���Ͽ� ���, printable ���ڴ� �״��, non-printable ���ڴ� 16������ ���
		void printLocation(int iFlag=COMMON);						//��ġ ���� ȭ�鿡 ���, COMMON �̸� ��� ���, FIRST �̸� ù content�� �ǹ��ϸ�, offset, depth�� ���, NO_FIRST �̸� �ι�° ���� content�� �ǹ��ϸ�, within�� ��� 
		void printLocation(FILE* p_fpFile, int iFlag=COMMON);		//��ġ ���� ���Ͽ� ���, COMMON �̸� ��� ���, FIRST �̸� ù content�� �ǹ��ϸ�, offset, depth�� ���, NO_FIRST �̸� �ι�° ���� content�� �ǹ��ϸ�, within�� ��� 
		void printSnortForm(int iFlag=COMMON);						//snort ������ �ٷ� ������ �� �ִ� ���·� ȭ�鿡 ���
		void printSnortForm(FILE* p_fpFile, int iFlag=COMMON);		//snort ������ �ٷ� ������ �� �ִ� ���·� ���Ͽ� ���
		void printSnortFormHTML(int iFlag=COMMON);					//���������� �ٷ� ������ �� �ִ� ���·� ȭ�鿡 ���
		void printSnortFormHTML(FILE* p_fpFile, int iFlag=COMMON);	//���������� �ٷ� ������ �� �ִ� ���·� ���Ͽ� ���

		u_int32_t getID();
		u_int32_t getProt();
		u_int32_t getField();
		u_int32_t getLength();
		u_int32_t getOffset();
		u_int32_t getDepth();
		u_int32_t getDistance();
		u_int32_t getWithin();
		void setID(u_int32_t p_iID);
		void setProt(u_int32_t p_iProt);
		void setField(u_int32_t p_iField);
		void addField(u_int32_t p_iField);
		void setOffset(u_int32_t p_iOffset);
		void setDepth(u_int32_t p_iDepth);
		void setDistance(u_int32_t p_iDistance);
		void setWithin(u_int32_t p_iWithin);
		vector<char>::iterator getCharsVectorBegin();
		vector<char>::iterator getCharsVectorEnd();
		void setContent(Content* p_cpContent);
	
		void concatenateOneChar(char* p_cpChar);									//�Է¹��� ���ڸ� vcChar��  ����
		void concatenateChars(char* p_cpChars, int p_iCharsSize);					//�Է¹��� ���ڿ��� p_iCharsSize ��ŭ vcChar��  ����
		void concatenateOneHex(int* p_cpHex);										//�Է¹��� 16������ vcChar��  ����
		void concatenateContent(PacketContainer *p_cPkt, int p_uiMaxLength);		//p_cPkt�� payload �� p_uiMaxLength ���̸�ŭ�� ����. �� p_uiMaxLength -1�̸� ���̷ε� ��ü ����

		void extract(Content* p_cpResultContent, int p_iStart, int p_iEnd);			//�ڽ��� cContent���� p_iStart~p_iEnd�� �����Ͽ� p_cpResultContent�� ����
		
		void join(Content* p_cpContent);		//p_cpContent�� ������ char�� �ڽ��� vcChars�� �߰�
		int cmpContent(Content* p_cpContent);	//p_cpContent�� ���Ͽ� ���̰� ª���� -1, ũ�� 1, ���̰� ������ ���� 16������ ������ -1, ū 16������ ������ 1, ���̵� ���� ���뵵 ������ 0
		int isInclude(int p_iOffset, Content* p_cpContent);									//p_cpContent�� �ڽſ��� p_iOffset ���� ���ԵǸ� ��Ī offset+contetn ���̸� ����. ���н� 0 ���� 
		int isInclude(int p_iOffset, Content* p_cpContent, int p_iProtocol, int p_iField);	//��������, �ʵ�� �Ľ� �� ���Կ��� Ȯ��. p_cpContent�� �ڽſ��� p_iOffset ���� ���ԵǸ� ��Ī offset+contetn ���̸� ����. ���н� 0 ���� 
		bool getIndex(int* p_iStart, int* p_iEnd, int p_iProtocol, int p_iField);			//�ڽ��� content���� �ʵ� ������ Ȯ��, p_iStart, p_iEnd�� ���
		bool isJoinable(Content* p_cpContent);	// ���հ����Ѱ�? ���� 2 �̻� �Է�, �ڽ��� ������ K-1 ���̿� ����� ù K-1 ���̰� ��������
		
		bool isFixContent();

		void setField();
};
//######################################################################
class Suspect
{
	private:
		u_int32_t	uiSusID;					//�˻��� sequence ID
		u_int32_t	uiSusOffset;				//�˻� ���� offset
	
	public:
		Suspect();
		~Suspect();
		void reset();
		void print() const;
		void print(FILE* p_fpFile) const;
		
		void setSusID(u_int32_t p_uiSusSeqID);
		void setSusOffset(u_int32_t p_uiSusOffset);

		u_int32_t getSusID()	const;
		u_int32_t getSusOffset() const;

		struct lessSuspect {									//seqID �������� set ����
			bool operator()(Suspect lhs, Suspect rhs){
				return lhs.getSusID() < rhs.getSusID();
			}
		};
};
//######################################################################
class Rule
{
	public:
		u_int32_t	uiID;									//rule ID, start from 0
		set<Suspect, Suspect::lessSuspect>	scSuspects;		//suspect sequence id set
		u_int32_t	uiSupp;									//number of unique file
		u_int32_t	uiMaxSupp;								//max number of unique file
		u_int32_t	uiProtocol;								//HTTP, TLS, UNKNOWN
		u_int32_t	uiField;								//HTTP Field, TLS Field
		Header		cHeader;								//header
		u_int32_t	uiContentCount;							//index of content for setting ID
		list<Content> lcContents;							//contents list

		FPB		cRule_PKT_IdentifiedTraffic;				//volume of identified traffic in packet-level
		FPB		cRule_PKT_TotalTraffic;						//volume of total traffic in packet-level
		FPB		cRule_FLOW_IdentifiedTraffic;				//volume of identified traffic in flow-level
		FPB		cRule_FLOW_TotalTraffic;					//volume of total traffic in flow-level

	public:
		Rule();
		~Rule();
		void reset();
		
		void print();									//��� ��� ������ ����� ���� ȭ�鿡 ���
		void print(FILE* p_fpFile);						//��� ��� ������ ����� ���� ���Ͽ� ���
		void printSnortForm();							//snort ������ �ٷ� ������ �� �ִ� ���·� ȭ�鿡 ���
		void printSnortForm(FILE* p_fpFile);			//snort ������ �ٷ� ������ �� �ִ� ���·� ���Ͽ� ���			
		void printSnortFormHTML();						//���������� �ٷ� ������ �� �ִ� ���·� ȭ�鿡 ���
		void printSnortFormHTML(FILE* p_fpFile);		//���������� �ٷ� ������ �� �ִ� ���·� ���Ͽ� ���
		void printSuspectSet();							//���� sequence ������ ȭ�鿡 ���
		void printSuspectSet(FILE* p_fpFile);			//���� sequence ������ ���Ͽ� ���
		
		void setID(u_int32_t p_uiID);					//get set �Լ�
		void setSupp(u_int32_t p_uiSupp);
		void increaseSupp();									
		void setProt(u_int32_t p_uiProt);
		void setField(u_int32_t p_iField);
		u_int32_t getID();
		u_int32_t getSupp();
		u_int32_t getMaxSupp();
		u_int32_t getProt();
		u_int32_t getField();
		u_int32_t getContentSize();
		Header* getHeader();
		list<Content>::iterator getContentsListBegin();
		list<Content>::iterator getContentsListEnd();
		FPB* getPktIdentifiedTraffic();
		FPB* getPktTotalTraffic();
		FPB* getFlowIdentifiedTraffic();
		FPB* getFlowTotalTraffic();

		void setIntersectionSuspects(Rule* p_cpRule1, Rule* p_cpRule2);	//�� ��Ģ�� ������ �������� ����

		void resetSupspects();

		u_int32_t getFirstContentLength();
		
		void insert(Content* p_cpContent);				//p_cpContent�� ID�� �����ϰ� �߰�							
		void insertSingleContent(Rule* p_cpRule);		//p_cpRule�� ù��° content�� �߰�
		void insertMultiContent(Rule* p_cpRule);		//p_cpRule�� ��� content �߰�
		u_int32_t getSingleContentLength();				//rule�� ù��° content�� ���̸� ����
		u_int32_t getMultiContentCount();				//rule�� content ���� ����
		void joinSingleContent(Rule* p_cpRule);			//p_cpRule�� ù��° content�� ������ ���ڸ� �߰�
		void joinMultiContent(Rule* p_cpRule);			//p_cpRule�� ��� content �߰� �� ����ȭ

		int cmpContent(Rule* p_cpRule);							//p_cRule�� ���Ͽ� ������ ������ -1 ũ�� 1, ������ ���� ���̰� ª���� -1, ũ�� 1, ������ ���̰� ���� ���� 16������ ������ -1, ū 16������ ������ 1, ����, ����, ���뵵 ������ 0
		
		bool isJoinableSingleContent(Rule* p_cpRule);											//rule�� ù��° content�� ������� ���� �κ��� �����ϴ��� ����, �� ���̰� 1�̸� ������ ���� ����
		bool isJoinableMultiContent(Rule* p_cpRule);											//�ڽŰ� p_cpRule�� conten set �������� ���̰� �ڽ��� ���� - 1 �̸� ��
		bool isJoinableProtSingle(Rule* p_cpRule);												//������ ��������, �ʵ��̸� OK
		bool isJoinableProtMulti(Rule* p_cpRule);												//������ ���������̸� OK
		bool isDuplicate(SequenceVector* p_cpSequenceVector);									//��sequence���� ������ �����ϸ� ��

		bool isFixContent();
		void setSingleSuspectSet(SequenceVector* p_cpSequenceVector);								//p_cpSequenceVector�� �о� rule�� ������ sequence ������ ����				
		void setMultiSuspectSet(SequenceVector* p_cpSequenceVector);								//p_cpSequenceVector�� �о� rule�� ������ sequence ������ ����				
		void setSupportSingle(SequenceVector* p_cpSequenceVector, u_int32_t p_uiMaxSupp);			
		void setSupportMulti(SequenceVector* p_cpSequenceVector, u_int32_t p_uiMaxSupp);			
				
		void setLocation(SequenceVector* p_cpSequenceVector);										//cpSequenceList�� �����Ͽ� content�� ��ġ ���� ����
		void setHeader(SequenceVector* p_cpSequenceVector);																				// rule�� suspects list�� ���� ��Ŷ ������ cMultiSequenceVector�� �����Ͽ� ��� ���� set

		void setCompleteness(FlowHash* p_cpFlowHash);											//p_cpFlowHash�� FlowList, PktList�� �̿��Ͽ� �м��� ���
			bool isIndentify(FlowTwowayContainer* p_cpFlowTwowayContainer);							//p_cpFlowTwowayContainer�� �м� ����
				bool isForwardIdentifyConentList(FlowTwowayContainer* p_cpFlowTwowayContainer);		//lcContents ��ΰ� p_cpFlowTwowayContainer�� �м� ����
				bool isBackwardIdentifyConentList(FlowTwowayContainer* p_cpFlowTwowayContainer);	//lcContents ��ΰ� p_cpFlowTwowayContainer�� �м� ����
			bool isIndentify(PacketContainer* p_cpPacketContainer);									//p_cpPacketContainer�� �м� ����
				bool isIdentifyConentList(PacketContainer* p_cpPacketContainer);
		
		bool isIdentify(Content* p_cpContent);
		
		void uniqueContent();									//�ڽ��� content���� ����ȭ
		void uniqueField();										//DATA�� ������ HTTP�� �ʵ庰�� ���� �� ���� ���̰� �� content�� ����� ����
		
		//stl sort������ return false�� ��� �ڸ� �ٲ�
		struct CompareContent {									//content �������� ����	
			bool operator()(Content lhs, Content rhs){
				if ((lhs.cmpContent(&rhs)) > 0) return true;
				return false;
			}
		};
		struct CompareField {									//field �������� ����	
			bool operator()(Content lhs, Content rhs){
				if (lhs.getField() < rhs.getField()) return true;
				return false;
			}
		};
		

		//stl unique������ return true�� ��� �ϳ��� ��ħ
		struct isSameContent {									// content, field �� �����ϸ� �ϳ��� ��ħ												
			bool operator()(Content lhs, Content rhs){
				if (((lhs.cmpContent(&rhs)) == 0) && (lhs.getField() == rhs.getField()))
					return true;
				else
					return false;				
			}
		};
		struct isSameField {									// field �� �����ϸ� �ϳ��� ��ħ												
			bool operator()(Content lhs, Content rhs){
				if ((lhs.getProt() & rhs.getProt()) != PROT_HTTP) return false;
				if ((lhs.getField() & rhs.getField()) & PROT_HTTP_DATA) return false;
				if (lhs.getField() != rhs.getField()) return false;
				return true;				
			}
		};
};
//######################################################################
class RuleList
{
	public:
		u_int32_t	uiRuleCount;								//index of content for setting ID
		list<Rule> lcRules;

		FPB		cRuleList_PKT_IdentifiedTraffic;
		FPB		cRuleList_PKT_TotalTraffic;

		FPB		cRuleList_FLOW_IdentifiedTraffic;
		FPB		cRuleList_FLOW_TotalTraffic;
		
	public:
		RuleList();
		~RuleList();
		void reset();
		void print();
		void print(char* p_cpFileName);
		void printSnortForm();
		void printSnortForm(char* p_cpFileName);
		void printSnortFormHTML();
		void printSnortFormHTML(char* p_cpFileName);

		void setRuleCount(u_int32_t p_uiRuleCount);
		u_int32_t getRuleCount();
		u_int32_t getRuleSize();
		FPB* getPktIdentifiedTraffic();
		FPB* getPktTotalTraffic();
		FPB* getFlowIdentifiedTraffic();
		FPB* getFlowTotalTraffic();
		list<Rule>::iterator getRuleListBegin();
		list<Rule>::iterator getRuleListEnd();

		u_int32_t getStartIndexTargetLength(u_int32_t p_uiTargetLength);	// rule ����Ʈ���� ���� ��� ����(p_uiTargetLength)�� �����ϴ� ���ؽ�
		u_int32_t getStartIndexTargetCount(u_int32_t p_uiTargetcount);		// rule ����Ʈ���� ���� ��� ����(p_uiTargetCount)�� �����ϴ� ���ؽ�

		void resetSupspects();

		void insert(Rule* p_cpRule);							//p_cpRule�� ID�� �����ϰ� lcRules ����Ʈ�� �߰�, ��, content�� �����ϴ� ���
		void insert(RuleList* p_cpRuleList);
				
		void extractSingleConentLength1(SequenceVector* p_cpSequenceVector);	//p_cpSequenceVector�� �����ϴ� protocol, field�� ���� ��� ���(256)�� ���� 1�� content�� RuleList�� �߰�
		void extractMultiConentLength1(SequenceVector* p_cpSequenceVector);		//p_cpSequenceVector�� multi-content�� �� ��Ҹ� ����
		void extractSingleConent(u_int32_t p_uiLow, u_int32_t p_uiHigh, RuleList* p_cpRuleList, u_int32_t p_uiTargetContentLength, SequenceVector* cpSequenceList, u_int32_t p_uiMinSupp, u_int32_t p_uiMaxSupp);	//p_cpRuleList �� low~high rule�� ��ü rule�� ����Ͽ� p_uiTargetContentLength+1 content ����, ������ ������ ���� �����ϱ� ���� supp =0
		void extractMultiConent(u_int32_t p_uiLow, u_int32_t p_uiHigh, RuleList* p_cpRuleList, u_int32_t p_uiTargetContentCount, SequenceVector* cpSequenceList, u_int32_t p_uiMinSupp, u_int32_t p_uiMaxSupp);	//p_cpRuleList �� ��ü rule�� ����Ͽ� p_uiTargetContentCount+1 content ����, ������ ������ ���� �����ϱ� ���� supp =0
		void setSingleSuspectSet(SequenceVector* p_cpSequenceVector);					//p_cpSequenceVector�� �о� rule�� ������ sequence ������ ����				
		void setMultiSuspectSet(SequenceVector* p_cpSequenceVector);					//p_cpSequenceVector�� �о� rule�� ������ sequence ������ ����				
		void setSupportSingle(SequenceVector* p_cpSequenceVector, u_int32_t p_uiMaxSupp);
		void setSupportMulti(SequenceVector* p_cpSequenceVector, u_int32_t p_uiMaxSupp);
		
		void deleteUnderSupport(u_int32_t p_uiMinSupp);
		void deleteDuplicate(SequenceVector* p_cpSequenceVector, u_int32_t p_uiMinSupp); //�� sequence���� ������ �����ϴ� rule ����
		void trimUnderContentLength(u_int32_t p_uiMinLength, SequenceVector* p_cpSequenceVector);				//p_uiMinLength ���� ª�� content ����	, �� ��� ���� sequence�� ���̿� �����ϸ� ����(GET ���)	
		
		void setLocation(SequenceVector* p_cpSequenceVector);				//cpSequenceList�� �����Ͽ� content�� ��ġ ���� ����
		void setHeader(SequenceVector* p_cpSequenceVector);																				// rule�� suspects list�� ���� ��Ŷ ������ cMultiSequenceVector�� �����Ͽ� ��� ���� set

		void setCompleteness(FlowHash* p_cpFlowHash);				//���� ��Ģ���� �м��� üũ
		
		void unique();											//��������, content ������ ���� ��, ������ ��Ģ �ϳ��� ��ħ
		void uniqueField();										//�ʵ庰�� ���� �� ���� ���̰� �� content�� ����� ����
		void sortFlowLevelCompByte();
		void sortFlowLevelCompPkt();
		void sortContentFix();
		void sortSupport();

		//stl sort������ return false�� ��� �ڸ� �ٲ�
		struct CompareContent {									//content �������� ����	 (������ ���ų� ���̰� ª�ų� ���� ���ĺ� �켱)													
			bool operator()(Rule lhs, Rule rhs){
				if (lhs.cmpContent(&rhs) < 0) return true;
				return false;
			}
		};
		struct CompareProt {									//protocol �������� ����
			bool operator()(Rule lhs, Rule rhs){
				if (lhs.getProt() < rhs.getProt()) return true;
				return false;
			}
		};
		struct CompareFlowLevelCompByte {																	
			bool operator()(Rule lhs, Rule rhs){
				FPB* cpLhsFlowIdentifiedTraffic = lhs.getFlowIdentifiedTraffic();
				FPB* cpRhsFlowIdentifiedTraffic = rhs.getFlowIdentifiedTraffic();
				if(cpLhsFlowIdentifiedTraffic->getByte() > cpRhsFlowIdentifiedTraffic->getByte()) 
					return true;
				return false;
			}
		};
		struct CompareFlowLevelCompPkt {																	
			bool operator()(Rule lhs, Rule rhs){
				FPB* cpLhsFlowIdentifiedTraffic = lhs.getFlowIdentifiedTraffic();
				FPB* cpRhsFlowIdentifiedTraffic = rhs.getFlowIdentifiedTraffic();
				if(cpLhsFlowIdentifiedTraffic->getPkt() > cpRhsFlowIdentifiedTraffic->getPkt()) 
					return true;
				return false;
			}
		};
		struct CompareContentFix {																	
			bool operator()(Rule lhs, Rule rhs){
				if(lhs.isFixContent() && !rhs.isFixContent()) 
					return true;
				return false;
			}
		};
		struct CompareSupport {																	
			bool operator()(Rule lhs, Rule rhs){
				if(lhs.getSupp() > rhs.getSupp()) 
					return true;
				return false;
			}
		};


		//stl unique������ return true�� ��� �ϳ��� ��ħ
		struct isSame {											// content, protocol�� �����ϸ� �ϳ��� ��ħ												
			bool operator()(Rule lhs, Rule rhs){
				if ((lhs.cmpContent(&rhs) == 0) && (lhs.getProt() == rhs.getProt()))
					return true;
				else
					return false;				
			}
		};
};
//######################################################################
class Sequence
{
	public:
	u_int32_t		uiSequenceID;
	u_int32_t		uiFileID;
	Suspect			cPktID;
	Header			cHeader;
	Content			cContent;
	vector<Content> vcMultiContent;

	public:
		Sequence();
		~Sequence();
		void reset();
		void resetSequenceID();
		void resetPktID();
		void resetHeader();
		void resetContent();
		void resetMultiContent();
		void print();
		
		void setSequenceID(u_int32_t p_uiSequenceID);
		void setFileID(u_int32_t p_uiFileID);
		void setPktID(Suspect	p_cPktID);
		void setHeader(Header* p_cpHeader);
		void setContent(Content* p_cpContent);
		
		u_int32_t getSequenceID();
		u_int32_t getFileID();
		Suspect getPktID();
		Header* getHeader();
		u_int32_t getContentProt();
		u_int32_t getContentField();
		u_int32_t getContentLength();

		int isInclude(int p_iOffset, Rule* p_cpRule);									//ruel�� content list�� sequence�� multiContent�� ������ �°� �ִ��� Ȯ��, �����ϸ� ���� ��ġ, �����ϸ� -1, content ID ���, ���ӵ��� �ʾƵ� ������ ������ OK
		
		int cmpContent(Sequence p_cSequence);							//p_cSequence�� ���Ͽ� ���̰� ª���� -1, ũ�� 1, ���̰� ������ ���� 16������ ������ -1, ū 16������ ������ 1, ���̵� ���� ���뵵 ������ 0
		u_int32_t getNumberofExistence(Rule* p_cpRule);					//p_cpRule�� Sequence�� �����Ǵ� Ƚ�� ����
						
		void parserHTTP(SequenceVector* p_cpResultSequenceVector);			//�ڽ��� cContent�� HTTP �ʵ庰�� �Ľ��Ͽ� p_cpResultSequenceVector�� ����
			bool isHttpReqest();										//HTTP request ���� Ȯ��
			bool isHttpResponse();										//HTTP response ���� Ȯ��
			void parserHTTPMethod(Sequence* p_cpResultSequence);		//method �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPUrl(Sequence* p_cpResultSequence);			//url �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPHost(Sequence* p_cpResultSequence);			//host �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPUser(Sequence* p_cpResultSequence);			//user-agent �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPReferer(Sequence* p_cpResultSequence);		//referer �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPCookie(Sequence* p_cpResultSequence);		//cookie �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPResponse(Sequence* p_cpResultSequence);		//response �ʵ常 �����Ͽ� p_cpResultSequence�� ����
			void parserHTTPData(Sequence* p_cpResultSequence);			//data �ʵ常 �����Ͽ� p_cpResultSequence�� ����
		void parserTLS(SequenceVector* p_cpResultSequenceVector);			//�ڽ��� cContent�� TLS �ʵ庰�� �Ľ��Ͽ� p_cpResultSequenceVector�� ����
			bool isTLSHeader(char* p_cpStr);							//p_cpStr�� �������� 3����Ʈ�� TLS header���� �˻�

		void uniqueMultiContentForSameOffset();							//offset�� �����ϸ� ���̰� ��͸� ����
		void uniqueMultiContentForSameID();								//ID�� �����ϸ� �ϳ��� ����

		//stl sort������ return false�� ��� �ڸ� �ٲ�
		struct CompareOffset {									//content offset �������� ����	 (���� offset �켱)							
			bool operator()(Content lhs, Content rhs){
				if (lhs.getOffset() < rhs.getOffset()) return true;
				return false;
			}
		};
		struct CompareLength {									//content length �������� ����	 (�� length �켱)							
			bool operator()(Content lhs, Content rhs){
				if (lhs.getLength() > rhs.getLength()) return true;
				return false;
			}
		};
		struct CompareID {										//content ID �������� ����	 (���� ID �켱)							
			bool operator()(Content lhs, Content rhs){
				if (lhs.getID() < rhs.getID()) return true;
				return false;
			}
		};

		//stl unique������ return true�� ��� �ϳ��� ��ħ
		struct isSameOffset {											//offset�� �������� ���η� ����ȭ									
			bool operator()(Content lhs, Content rhs){
				if (lhs.getOffset() == rhs.getOffset())
					return true;
				else
					return false;				
			}
		};
		struct isSameID {												//ID�� �������� ���η� ����ȭ									
			bool operator()(Content lhs, Content rhs){
				if (lhs.getID() == rhs.getID())
					return true;
				else
					return false;				
			}
		};
};
//######################################################################
class SequenceVector
{
	public:
		vector<Sequence> vcSequence;
	
	public:
		SequenceVector();
		~SequenceVector();
		void reset();
		void print();

		u_int32_t getSequenceSize();
		
		void load(FlowHash* p_cpFlowHash, int p_iMaxPktCountForward, int p_iMaxPktCountBackward, int p_iMaxSequenceLength);	//p_cpFlowHash�� ����� fwp�� �о� vcSequence�� ���� 
			bool isHTTP(FlowTwowayContainer* p_cpFlow);
			bool isTLS(FlowTwowayContainer* p_cpFlow);
		
		void integrate(RuleList* p_cpRuleList);				//p_cpRuleList�� content�� source sequence�� ����
		void integrate(SequenceVector* p_cpSequenceVector);	//p_cpSequenceVector�� multi content�� source pkt�� ����
		
		void parser();										//�������� �ʵ庰�� sequence �ɰ�

		void breadkField();									//field �� sequence ���� ����

		void insert(Sequence* p_cpSequence);				//p_cpSequence�� ID�� �����ϰ� vcSequence�� �߰� 
		void insert(SequenceVector* p_cpSequenceVector);	//p_cpSequenceList�� ��� sequence�� ����

		
		
		
		void unique();											//fileID, content�� �����ϸ� �ϳ���ħ, content, fileId ������ ����
		void sortContent();

		//stl sort������ return false�� ��� �ڸ� �ٲ�
		struct CompareContent {									//content �������� ����	 (���̰� ª�ų� ���� ���ĺ� �켱)							
			bool operator()(Sequence lhs, Sequence rhs){
				if (lhs.cmpContent(rhs) < 0 ) return true;
				return false;
			}
		};
		struct CompareContentField {									//content Field�������� ����	 
			bool operator()(Sequence lhs, Sequence rhs){
				if (lhs.getContentField() < rhs.getContentField()) return true;
				return false;
			}
		};
		struct CompareContentProt {									//content Prot�������� ����	 
			bool operator()(Sequence lhs, Sequence rhs){
				if (lhs.getContentProt() < rhs.getContentProt()) return true;
				return false;
			}
		};
		struct CompareFileID {									//fileID �������� ����	 (���� ���� �켱)									
			bool operator()(Sequence lhs, Sequence rhs){
				if (lhs.getFileID() < rhs.getFileID()) return true;
				return false;
			}
		};
		//stl unique������ return true�� ��� �ϳ��� ��ħ
		struct isSame {											//fileID, content�� �������� ���η� ����ȭ									
			bool operator()(Sequence lhs, Sequence rhs){
				if ((lhs.getFileID() == rhs.getFileID()) 
					&& (lhs.getContentProt() == rhs.getContentProt()) 
					&& (lhs.getContentField() == rhs.getContentField()) 
					&& (lhs.cmpContent(rhs)==0))
					return true;
				else
					return false;				
			}
		};
};

#endif
