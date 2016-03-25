// 24pointsDoc.cpp : C24pointsDoc 类的实现
//

#include "stdafx.h"
#include "24points.h"

#include "solutions.h"
#include "24pointsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C24pointsDoc

IMPLEMENT_DYNCREATE(C24pointsDoc, CDocument)

BEGIN_MESSAGE_MAP(C24pointsDoc, CDocument)
END_MESSAGE_MAP()


// C24pointsDoc 构造/析构

void swap(int & a, int & b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}

C24pointsDoc::C24pointsDoc()
{	
	A[0][0] = 0;
	A[0][1] = 1;
	A[0][2] = 2;
	A[0][3] = 3;
	int k = 0;
	for (int j = 3; j > 0; --j)
	{
		if (A[k][j] > A[k][j - 1])
		{
			memcpy(A[k + 1], A[k], sizeof(A[0]));
			for (int ii = 3; ii >= j; --ii)
			{
				if (A[k][ii] > A[k][j - 1])
				{
					A[k + 1][j - 1] = A[k][ii];
					A[k + 1][ii] = A[k][j - 1];
					break;
				}
			}
			for (int ii = j; ii < 2 + j / 2; ++ii)
			{
				swap(A[k + 1][ii], A[k + 1][4 - ii + j - 1]);
			}
			++k;
			j = 4;
		}
	}
	TCHAR  sz[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, sz);
	saveFilePath = sz;
	saveFilePath.Append(_T("\\24points"));
	CreateDirectory(saveFilePath, NULL);
	saveFilePath.Append(_T("\\save.dat"));
	FILE *fr = fopen(saveFilePath.GetBuffer(), "rb");
	if (fr)
	{
		fread(&cntSuccess, 4, 1, fr);
		fread(&cntTotal, 4, 1, fr);
		fread(&cntTotalTime, sizeof(time_t), 1, fr);
		fclose(fr);
	}
	else
	{
		cntSuccess = 0;
		cntTotal = 0;
		cntTotalTime = 0;
	}
}

C24pointsDoc::~C24pointsDoc()
{
}

BOOL C24pointsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	srand(time(NULL));
	cards[0] = rand() % 52;
	do
	{
		cards[1] = rand() % 52;
	} while (cards[0] == cards[1]);
	do
	{
		cards[2] = rand() % 52;
	} while (cards[0] == cards[2] || cards[1] == cards[2]);
	do
	{
		cards[3] = rand() % 52;
	} while (cards[0] == cards[3] || cards[1] == cards[3] || cards[2] == cards[3]);
	recount();

	countOnSuccess = true;
	countOnTotal = true;

	return TRUE;
}

void C24pointsDoc::recount()
{
	memset(CountLeft, 0, sizeof(CountLeft));
	for (int i = 0; i < 4; ++i)
	{
		++CountLeft[cards[i] % 13];
	}
	CountTotalLeft = 4;
}


int C24pointsDoc::get_number(TCHAR* s, int& i)
{
    if (--CountTotalLeft < 0)
	{
		return -1;
	}

	int n = 0;
    do
	{
	    n *= 10;
		n += s[i] - '0';
		++i;
	} while (s[i] >= '0' && s[i] <= '9');
	--i;

	if (n < 1 || n > 13)
	{
		return -2;
	}
	if (--CountLeft[n - 1] < 0)
	{
		return -3;
	}

	return n;
}

double C24pointsDoc::calc_exp(TCHAR *s)
{
	double num[5];
	int    opt[5];
	int    cn = 0;
	int    t;

	opt[0] = 0;
	for (int i = 0; s[i]; ++i)
	{
         if (s[i] >= '0' && s[i] <= '9')
		 {
			 t = get_number(s, i);
			 if (t < 0)
				 return -999999 - t;
			 num[cn] = t;
			 if (opt[cn] == '/')
			 {
				 num[cn - 1] /= num[cn];
			 }
			 else if (opt[cn] == '*')
			 {
				 num[cn - 1] *= num[cn];
			 }
			 else
			 {
				 ++cn;
			 }
		 }
		 else if(s[i] == '+' || s[i] == '-')
		 {
			 if (cn > 1 && (opt[cn - 1] == '+' || opt[cn - 1] == '-'))
			 {
				 if (opt[cn - 1] == '+')
					 num[cn - 2] += num[cn - 1];
				 else
					 num[cn - 2] -= num[cn - 1];
				 --cn;
			 }
			 opt[cn] = s[i];
		 }
		 else if (s[i] == '*')
		 {
			 /*
		     ++i;
			 t = get_number(s, i);
			 if (t < 0)
				 return -999999 - t;
			 num[cn - 1] *= t;
			 */
			 opt[cn] = '*';
		 }
		 else if (s[i] == '/')
		 {
			 /*
			 ++i;
			 t = get_number(s, i);
			 if (t < 0)
				 return -999999 - t;
			 num[cn - 1] /= t;
			 */
			 opt[cn] = '/';
		 }
		 else if (s[i] == '(')
		 {
			 ++i;
			 TCHAR *subs = s + i;
			 while (s[i] && s[i] != ')')
				 ++i;
			 if (!s[i])
				 return -999995;
			 s[i] = 0;
			 num[cn] = calc_exp(subs);
			 if (num[cn] < -900000)
				 return num[cn];
			 if (opt[cn] == '/')
			 {
				 num[cn - 1] /= num[cn];
			 }
			 else if (opt[cn] == '*')
			 {
				 num[cn - 1] *= num[cn];
			 }
			 else
			 {
				 ++cn;
			 }
		 }
		 else if (s[i] == ')')
		 {
			 return -999995;
		 }
		 else if (s[i] == ' ')
		 {
			 ;
		 }
		 else
		 {
			 return -999999;
		 }
	}
	//ASSERT(cn == 1);
	if (cn > 1)
	{
		if (opt[1] == '+')
		{
			num[0] += num[1];
		}
		else
		{
			num[0] -= num[1];
		}
	}
	return num[0];
}

int C24pointsDoc::Solver(CString& s)
{
	int a[4];
	for (int i = 0; i < 4; ++i)
		a[i] = cards[i] % 13 + 1;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = i; j < 4; ++j)
		{
			if (a[i] > a[j])
			{
				a[i] ^= a[j];
				a[j] ^= a[i];
				a[i] ^= a[j];
			}
		}
	}
	CString pattern;
	pattern.Format(_T("[%d,%d,%d,%d]"), a[0], a[1], a[2], a[3]);
	int i, j;
	if ((i = solutions.Find(pattern)) < 0)
		return 0; // error
	i = solutions.Find(']', i);
	j = solutions.Find('[', i);
	if (j - i < 4)
	{
		return 0;
	}
	else
	{
		s = solutions.Mid(i + 2, j - i - 3);
		s.Replace(_T(" "), _T("\r\n"));
		return 1;
	}

#if 0
	CString exp;
	bool hasA[65536] = {0};
	//bool has[2][4][4][4][6656] = {0};
	int opt[3];
	const char ops[5] = "+-*/";
	bool brackets[2];
	for (int iA = 0; iA < 24; ++iA)
	{
		if (hasA[ cards[A[iA][0]] << 12 | cards[A[iA][1]] << 8 | cards[A[iA][2]] << 4 | cards[A[iA][3]] ])
			continue;
		hasA[ cards[A[iA][0]] << 12 | cards[A[iA][1]] << 8 | cards[A[iA][2]] << 4 | cards[A[iA][3]] ] = true;

		for (opt[0] = 0; opt[0] < 4; ++opt[0])
		{
			for (opt[1] = 0; opt[1] < 4; ++opt[1])
			{
				for (opt[2] = 0; opt[2] < 4; ++opt[2])
				{
					// situation 1: ((A o B) o C) o D
					brackets[0] = (opt[1] > 1 && opt[0] < 2);
					brackets[1] = (opt[2] > 1 && opt[1] < 2);
					exp.Format(_T("%c%c%d%c%d%c%c%d%c%c%d"),
						brackets[0] ? '(' : ' ',
						brackets[1] ? '(' : ' ',
						cards[A[iA][0]],
						ops[opt[0]],
						cards[A[iA][1]],
						brackets[0] ? ')' : ' ',
						ops[opt[1]],
						cards[A[iA][2]],
						brackets[1] ? ')' : ' ',
						ops[opt[2]],
						cards[A[iA][3]]);

					if (calc_exp(exp.GetBuffer()) == 24)
					{
						exp.Remove(' ');
						s.Append(exp);
						s.Append(_T("\n"));
					}

					// situation 2: (A o B) o (C o D)
					if (opt[1] < 2 && opt[2] > 1 || opt[1] > 1 && opt[2] < 2)
					{
						brackets[1] = (opt[1] > 1 && opt[2] < 2);
						exp.Format(_T("%c%d%c%d%c%c%c%d%c%d%c"),
							brackets[0] ? '(' : ' ',
							cards[A[iA][0]],
							ops[opt[0]],
							cards[A[iA][1]],
							brackets[0] ? ')' : ' ',
							ops[opt[1]],
							brackets[1] ? '(' : ' ',
							cards[A[iA][2]],
							ops[opt[2]],
							cards[A[iA][3]],
							brackets[1] ? ')' : ' ');

						if (calc_exp(exp.GetBuffer()) == 24)
						{
							exp.Remove(' ');
							s.Append(exp);
							s.Append(_T("\n"));
						}
					}
				}
			}
		}
	}
#endif
}

void C24pointsDoc::SelfCheck_Solutions()
{

}

bool C24pointsDoc::hasSolution()
{
	CString s;
	return Solver(s);
}

void C24pointsDoc::setCards(int* arr)
{
	srand(time(0));	
	cards[0] = rand() % 4 * 13 + arr[0] - 1;
	do
	{
		cards[1] = rand() % 4 * 13 + arr[1] - 1;
	} while (cards[0] == cards[1]);
	do
	{
		cards[2] = rand() % 4 * 13 + arr[2] - 1;
	} while (cards[0] == cards[2] || cards[1] == cards[2]);
	do
	{
		cards[3] = rand() % 4 * 13 + arr[3] - 1;
	} while (cards[0] == cards[3] || cards[1] == cards[3] || cards[2] == cards[3]);
	recount();
}

void C24pointsDoc::done(bool success)
{
	if (success && countOnSuccess)
	{
		countOnSuccess = false;
		incrementSuccess();
		incrementTotalTime( time_passed() );
	}
	if (countOnTotal)
	{
		countOnTotal = false;
		incrementTotal();
	}
}

void C24pointsDoc::saveFile()
{
	FILE *fw = fopen(saveFilePath.GetBuffer(), "wb");
	if (fw)
	{
		fwrite(&cntSuccess, 4, 1, fw);
		fwrite(&cntTotal, 4, 1, fw);
		fwrite(&cntTotalTime, sizeof(time_t), 1, fw);
		fclose(fw);
	}
	else
	{
		MessageBox(NULL, _T("数据文件打开失败！"), _T("Error"), MB_OK);
	}
}