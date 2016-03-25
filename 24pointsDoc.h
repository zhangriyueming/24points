// 24pointsDoc.h : C24pointsDoc 类的接口
//


#pragma once


class C24pointsDoc : public CDocument
{
protected: // 仅从序列化创建
	C24pointsDoc();
	DECLARE_DYNCREATE(C24pointsDoc)

// 属性
public:
	int cards[4];
	int CountTotalLeft;
	int CountLeft[13];
	int A[24][4];

	bool countOnTotal;
	bool countOnSuccess;

	void recount();
	
	double calc_exp(TCHAR *s);
	int get_number(TCHAR* s, int& i);

	int  Solver(CString& s);
	void SelfCheck_Solutions();
	bool hasSolution();

	void setCards(int* arr);

	void done(bool success);
	void saveFile();
	void incrementSuccess()
	{
		++cntSuccess;
		saveFile();
	}
	void incrementTotal()
	{
		++cntTotal;
		saveFile();
	}
	void incrementTotalTime(time_t t)
	{
		cntTotalTime += t;
		saveFile();
	}
	int  countSuccess()
	{
		return cntSuccess;
	}
	int  countTotal()
	{
		return cntTotal;
	}
	time_t  countTotalTime()
	{
		return cntTotalTime;
	}
	
	time_t startTime;
	time_t timePassed;
	void start_timer()
	{
		startTime = time(0);
	}
	time_t time_passed()
	{
		return timePassed = time(0) - startTime;
	}

protected:
	int    cntSuccess;
	int    cntTotal;
	time_t cntTotalTime;
	CString saveFilePath;

public:

// 重写
public:
	virtual BOOL OnNewDocument();

// 实现
public:
	virtual ~C24pointsDoc();

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


