
#include "stdafx.h"
#include "MFCApplication6.h"
#include "MFCApplication6Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool eflag = true;
bool dflag = false;
bool bflag = false;			// 괄호 조건
bool beflag = false;		// 괄호가 닫힌 직후 판단

int lpNum = 0;				// 왼쪽 괄호 개수
double valueArray[100];		// 임시값 배열
wchar_t opArray[100];		// 연산자 배열

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CMFCApplication6Dlg::CMFCApplication6Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION6_DIALOG, pParent)
	, m_input(_T(""))
	, m_temp(_T(""))
	, m_op(_T(""))
	, m_exp(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_background.CreateSolidBrush(RGB(235, 242, 251));	// RGB(235, 242, 251)의 색깔을 변수에 할당 (기본 윈도우 계산기 배경색)
}

void CMFCApplication6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	DDX_Text(pDX, IDC_EDIT2, m_temp);
	DDX_Text(pDX, IDC_EDIT3, m_op);
	DDX_Text(pDX, IDC_EXPRESSION, m_exp);
}

BEGIN_MESSAGE_MAP(CMFCApplication6Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication6Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication6Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication6Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication6Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication6Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication6Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication6Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication6Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCApplication6Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON0, &CMFCApplication6Dlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMFCApplication6Dlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EQU, &CMFCApplication6Dlg::OnBnClickedButtonEqu)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CMFCApplication6Dlg::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_C, &CMFCApplication6Dlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMFCApplication6Dlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_ROOT, &CMFCApplication6Dlg::OnBnClickedButtonRoot)
	ON_BN_CLICKED(IDC_BUTTON_SQU, &CMFCApplication6Dlg::OnBnClickedButtonSqu)
	ON_BN_CLICKED(IDC_BUTTON_POW, &CMFCApplication6Dlg::OnBnClickedButtonPow)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CMFCApplication6Dlg::OnBnClickedButtonSub)
	ON_BN_CLICKED(IDC_BUTTON_MUL, &CMFCApplication6Dlg::OnBnClickedButtonMul)
	ON_BN_CLICKED(IDC_BUTTON_DIV, &CMFCApplication6Dlg::OnBnClickedButtonDiv)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CMFCApplication6Dlg::OnBnClickedButtonMod)
	ON_BN_CLICKED(IDC_BUTTON_LP, &CMFCApplication6Dlg::OnBnClickedButtonLp)
	ON_BN_CLICKED(IDC_BUTTON_RP, &CMFCApplication6Dlg::OnBnClickedButtonRp)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


BOOL CMFCApplication6Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	
	SetWindowText(_T("공학용 계산기"));	// 제목 변경

	GotoDlgCtrl(GetDlgItem(IDC_FOCUS));	// 포커스용 입력창으로 포커스 주기 (키 입력시 비프음 출력 방지)

	return FALSE;		// 포커스 안줌
}

void CMFCApplication6Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMFCApplication6Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMFCApplication6Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CMFCApplication6Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	hbr = (HBRUSH)m_background;		// 미리 선언해둔 배경색으로 바꾼다.
	return hbr;
}

// 소수점 아래 자리수 찾기
int getPrecision(double fval) {
	long long n = 10;						// long long의 범위가 -2^63 ~ 2^63 - 1 이고, 최대 소수점 아래 10자리까지만 출력한다.
	long long ival = long long(fval);		// 정수부만 추출
	fval -= ival;							// 소수부만 추출
	fval *= 1e10;							// 10^10을 곱하여 소수점 아래 30자리까지를 정수부로 끌어올림
	ival = (long long)(fval + 0.5);			// IEEE로인한 저장방식때문에 반올림, 소수부를 버리고 정수부만 추출

	// 뒤에서부터 0이면 n을 줄여서, 소수점아래 몇째자리까지 있는지 확인
	while (ival % 10 == 0 && n != 0) {
		ival /= 10;
		n--;
	}

	return (int)n;
}

// 버튼 눌렀을때 이벤트 함수로 모으기
void CMFCApplication6Dlg::buttonEvent(bool isNum) {
	((CEdit *)GetDlgItem(IDC_EXPRESSION))->SetSel(0, -1);	// 수식창의 커서를 마지막으로 고정
	GotoDlgCtrl(GetDlgItem(IDC_FOCUS));		// 포커스용 입력창으로 포커스 주기 (키 입력시 비프음 출력 방지)

	if (isNum) {
		// 숫자버튼
		(GetDlgItem(IDC_EDIT1))->ShowWindow(TRUE);		// m_input을 보이게 하고
		(GetDlgItem(IDC_EDIT2))->ShowWindow(FALSE);		// m_temp를 안보이게 한다
		bflag = false;
	}
	else {
		// 연산자 버튼
		(GetDlgItem(IDC_EDIT1))->ShowWindow(FALSE);		// m_input을 안보이게 하고
		(GetDlgItem(IDC_EDIT2))->ShowWindow(TRUE);		// m_temp를 보이게 한다
	}
}

/*********************************************
	   공학용 계산기 버튼 기능 구현하기
**********************************************/

//숫자버튼 1
void CMFCApplication6Dlg::OnBnClickedButton1()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('1');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 2
void CMFCApplication6Dlg::OnBnClickedButton2()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('2');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 3
void CMFCApplication6Dlg::OnBnClickedButton3()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('3');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 4
void CMFCApplication6Dlg::OnBnClickedButton4()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('4');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 5
void CMFCApplication6Dlg::OnBnClickedButton5()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('5');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 6
void CMFCApplication6Dlg::OnBnClickedButton6()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('6');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 7
void CMFCApplication6Dlg::OnBnClickedButton7()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('7');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 8
void CMFCApplication6Dlg::OnBnClickedButton8()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('8');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 9
void CMFCApplication6Dlg::OnBnClickedButton9()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	m_input.AppendChar('9');
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//숫자버튼 0
void CMFCApplication6Dlg::OnBnClickedButton0()
{
	if (eflag == true)
		m_input = "0";
	eflag = false;
	if (m_input != "0") {
		m_input.AppendChar('0');
	}
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//소수점
void CMFCApplication6Dlg::OnBnClickedButtonDot()
{
	if (!dflag)
		m_input.AppendChar('.');
	dflag = true;
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

// Clear
void CMFCApplication6Dlg::OnBnClickedButtonC()
{
	m_input = "";
	m_op = "";
	m_temp = "";
	m_exp = "";
	eflag = true;
	dflag = false;
	bflag = false;
	lpNum = 0;
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

// 1글자 지우기
void CMFCApplication6Dlg::OnBnClickedButtonDel()
{
	char buf[100];
	wcstombs(buf, m_input, 100);
	int len = strlen(buf);
	if (len > 0) {
		if (m_input.GetAt(len - 1) == '.') {
			dflag = false;
		}
		m_input.Delete(len - 1, 1);
	}
	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

/***********************************************
         이하 어셈블리어 구현 필수
************************************************/

//왼쪽 괄호
void CMFCApplication6Dlg::OnBnClickedButtonLp()
{
	if (eflag == true)
		m_input = "";
	eflag = false;

	char vchar[100];
	wcstombs(vchar, m_temp, 100);
	double fval = atof(vchar);	// 입력된 실수
	wchar_t op = m_op.GetAt(0);

	__asm{
		MOV esi, lpNum			// esi ← lpNum
		IMUL esi, 8				// esi에 8을 곱한다. 어셈블리어에선 []안에 바이트가 오기에 double의 크기인 8을 곱한다.
		FLD fval				// ST(0) ← fval
		FSTP valueArray[esi]	// valueArray에 값을 저장한다.

		MOV esi, lpNum			// esi ← lpNum
		IMUL esi, 2				// esi에 2를 곱한다.  어셈블리어에선 []안에 바이트가 오기에 wcahr_t의 크기인 2을 곱한다.
		MOV ax, op				// ax ← op, 여기서 op는 m_op의 값이다.
		CMP ax, 0				// ax와 0을 비교한다.
		JE NONE					// ax == 0일시 NONE레이블로 점프한다.
		MOV opArray[esi], ax	// ax != 0이면 opArray에 op의 값을 저장한다.
		JMP NEXT				// NONE레이블 부분 실행을 방지하기 위해 NEXT로 점프한다.
		NONE:
		MOV opArray[esi], '+'	// ax == 0이면 opArray에 '+'를 저장한다.
		NEXT:
	}

	m_exp += "(";
	m_temp = "";
	m_op = "";
	lpNum++;
	dflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}

//오른쪽 괄호
void CMFCApplication6Dlg::OnBnClickedButtonRp()
{
	if (eflag == true)
		m_input = "";
	eflag = false;
	lpNum--;

	double fval;
	wchar_t op;

	if (m_op != "") {
		double res = calc();
		char rval[100];
		sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
		if (!beflag) {
			m_exp += m_input;
		}
		m_input = rval;
	}

	__asm{
		MOV esi, lpNum			// esi ← lpNum
		IMUL esi, 8				// esi에 8을 곱한다. 어셈블리어에선 []안에 바이트가 오기에 double의 크기인 8을 곱한다.
		FLD valueArray[esi]		// 스택에 저장해놨던 값을 로드한다.
		FSTP fval				// fval에 저장해놨던 값을 저장한다.
		FLDZ					// ST(0) ← 0
		FSTP valueArray[esi]	// valueArray에 값을 저장하고 나서 0으로 초기화를 한다.

		MOV esi, lpNum			// esi ← lpNum
		IMUL esi, 2				// esi에 2를 곱한다.  어셈블리어에선 []안에 바이트가 오기에 wcahr_t의 크기인 2을 곱한다.
		MOV ax, opArray[esi]	// ax ← 스택에 저장해놨던 연산자를 로드한다.
		MOV op, ax				// op에 저장해놨던 연산자를 로드한다.
		MOV opArray[esi], 0		// opArray에 값을 저장하고 나서 0으로 초기화를 한다.
	}

	char rval[100];
	sprintf(rval, "%.*lf", getPrecision(fval), fval);		// 후행0을 빼고 출력 (최대 8자리)
	m_temp = rval;				// 결과 출력

	m_op = op;

	m_exp += ")";
	dflag = false;
	beflag = true;

	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}


/*************
   1항연산자
**************/
//제곱근
void CMFCApplication6Dlg::OnBnClickedButtonRoot()
{
	char vchar[100];
	wcstombs(vchar, m_input, 100);
	double fval = atof(vchar);	//입력된 실수
	double res = 0;				//출력할 결과


	// inline assembly
	__asm{
		FLD fval	// ST(0) : 피연산자
		FSQRT		// ST(0) ← SquareRoot(ST(0))
		FSTP res	// res변수에 ST(0)을 저장하고 ST(0)을 POP
	}
	

	char rval[100];
	sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
	m_input = rval;				//결과 출력

	eflag = true;
	dflag = false;

	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}

//2제곱
void CMFCApplication6Dlg::OnBnClickedButtonSqu()
{
	char vchar[100];
	wcstombs(vchar, m_input, 100);
	double fval = atof(vchar);	//입력된 실수
	double res = 0;				//출력할 결과
	
	
	__asm{
		FLD fval				// ST(0) : 피연산자
		FMUL ST(0), ST(0)		// ST(0) ← ST(0) * ST(0)
		FSTP res				// res변수에 ST(0)을 저장하고 ST(0)을 POP
	}
	
	
	char rval[100];
	sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
	m_input = rval;				//결과 출력

	eflag = true;
	dflag = false;

	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}


/*************
   2항연산자
**************/
//덧셈 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonAdd()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (!beflag) {
				m_exp += m_input;
			}
			m_exp += " + ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			if (bflag) {
				m_exp.Delete(0);
				m_exp.Delete(m_exp.GetLength() - 4);
				bflag = false;
			}
			m_exp.SetAt(m_exp.GetLength() - 2, '+');
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " + ";
	}

	m_op = '+';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
//뺄셈 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonSub()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (!beflag) {
				m_exp += m_input;
			}
			m_exp += " - ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			if (bflag) {
				m_exp.Delete(0);
				m_exp.Delete(m_exp.GetLength() - 4);
				bflag = false;
			}
			m_exp.SetAt(m_exp.GetLength() - 2, '-');
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " - ";
	}

	m_op = '-';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
//곱셈 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonMul()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			bool tflag = false;		// 임시 플래그 (if문 복잡도 줄이기)
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (m_exp.GetAt(m_exp.GetLength() - 2) == '+' || m_exp.GetAt(m_exp.GetLength() - 2) == '-') {
				m_exp.Insert(0, _T("("));
				bflag = true;
			}
			if (!beflag) {
				m_exp += m_input;
			}
			if (bflag) {
				m_exp.Insert(m_exp.GetLength(), _T(")"));
			}
			m_exp += " * ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			switch (m_exp.GetAt(m_exp.GetLength() - 2)) {
			case '+':
			case '-':
				m_exp.Insert(0, _T("("));
				m_exp.Insert(m_exp.GetLength() - 3, _T(")"));
				bflag = true;
				break;
			}
			m_exp.SetAt(m_exp.GetLength() - 2, '*');
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " * ";
	}

	m_op = '*';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
//나눗셈 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonDiv()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			bool tflag = false;		// 임시 플래그 (if문 복잡도 줄이기)
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (m_exp.GetAt(m_exp.GetLength() - 2) == '+' || m_exp.GetAt(m_exp.GetLength() - 2) == '-') {
				m_exp.Insert(0, _T("("));
				bflag = true;
			}
			if (!beflag) {
				m_exp += m_input;
			}
			if (bflag) {
				m_exp.Insert(m_exp.GetLength(), _T(")"));
			}
			m_exp += " / ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			switch (m_exp.GetAt(m_exp.GetLength() - 2)) {
			case '+':
			case '-':
				m_exp.Insert(0, _T("("));
				m_exp.Insert(m_exp.GetLength() - 3, _T(")"));
				bflag = true;
				break;
			}
			m_exp.SetAt(m_exp.GetLength() - 2, '/');
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " / ";
	}

	m_op = '/';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
//Mod 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonMod()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			bool tflag = false;		// 임시 플래그 (if문 복잡도 줄이기)
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (m_exp.GetAt(m_exp.GetLength() - 2) == '+' || m_exp.GetAt(m_exp.GetLength() - 2) == '-') {
				m_exp.Insert(0, _T("("));
				bflag = true;
			}
			if (!beflag) {
				m_exp += m_input;
			}
			if (bflag) {
				m_exp.Insert(m_exp.GetLength(), _T(")"));
			}
			m_exp += " % ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			m_exp.Insert(0, _T("("));
			m_exp.Insert(m_exp.GetLength() - 3, _T(")"));
			m_exp.SetAt(m_exp.GetLength() - 2, '%');
			bflag = true;
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " % ";
	}

	m_op = '%';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
//n제곱 연산자 입력
void CMFCApplication6Dlg::OnBnClickedButtonPow()
{
	if (m_op != "") {
		if (m_input != "") {	// input에 숫자 입력없이 다른 연산자를 눌렀을때 0이 연산되는것을 방지
			double res = calc();
			char rval[100];
			bool tflag = false;		// 임시 플래그 (if문 복잡도 줄이기)
			sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
			m_temp = rval;

			if (m_exp.GetAt(m_exp.GetLength() - 2) == '+' || m_exp.GetAt(m_exp.GetLength() - 2) == '-') {
				m_exp.Insert(0, _T("("));
				bflag = true;
			}
			if (!beflag) {
				m_exp += m_input;
			}
			if (bflag) {
				m_exp.Insert(m_exp.GetLength(), _T(")"));
			}
			m_exp += " ^ ";
		}
		else {
			// 수식창에서 뒤에서 2번째 위치 (연산자)를 갱신한다.
			m_exp.Insert(0, _T("("));
			m_exp.Insert(m_exp.GetLength() - 3, _T(")"));
			m_exp.SetAt(m_exp.GetLength() - 2, '^');
			bflag = true;
		}
	}
	else {
		m_temp = m_input;
		if (!beflag) {
			m_exp += m_input;
		}
		m_exp += " ^ ";
	}

	m_op = '^';
	m_input = "";
	dflag = false;
	beflag = false;
	UpdateData(false);

	buttonEvent(false);		// 연산자버튼 : 포커스창에 포커스주기 + m_temp 보이고 m_input 안보이기
}
/**********
  계산
***********/
double CMFCApplication6Dlg::calc()
{
	char vchar1[100], vchar2[100];
	wcstombs(vchar1, m_temp, 100);
	wcstombs(vchar2, m_input, 100);

	double fval1 = atof(vchar1);	// m_temp에 입력되어 있는 실수
	double fval2 = atof(vchar2);	// m_input에 입력되어 있는 실수
	double res = 0;					// 출력할 결과
									// m_op에 입력되어 있는 값에 따라 연산
	switch (m_op.GetAt(0)) {
	case '+':
		// inline assembly
		__asm {
			FLD fval1	// ST(0) : 왼쪽 피연산자
			FLD fval2	// ST(0) : 오른쪽 피연산자, ST(1) : 왼쪽 피연산자
			FADD		// ST(1) ← ST(1) + ST(0), POP ST(0)
			FSTP res	// res변수에 ST(0)을 저장하고 ST(0)을 POP
		}
		break;
	case '-':
		// inline assembly
		__asm{
			FLD fval1	// ST(0) : 왼쪽 피연산자
			FLD fval2	// ST(0) : 오른쪽 피연산자, ST(1) : 왼쪽 피연산자
			FSUB		// ST(1) ← ST(1) - ST(0), POP ST(0)
			FSTP res	// res변수에 ST(0)을 저장하고 ST(0)을 POP
		}
		
		break;
	case '*':
		// inline assembly
		__asm{
			FLD fval1	// ST(0) : 왼쪽 피연산자
			FLD fval2	// ST(0) : 오른쪽 피연산자, ST(1) : 왼쪽 피연산자
			FMUL		// ST(1) ← ST(1) * ST(0), POP ST(0)
			FSTP res	// res변수에 ST(0)을 저장하고 ST(0)을 POP
		}
		
		break;
	case '/':
		// inline assembly
		__asm{
			FLD fval1	// ST(0) : 왼쪽 피연산자
			FLD fval2	// ST(0) : 오른쪽 피연산자, ST(1) : 왼쪽 피연산자
			FDIV		// ST(1) ← ST(1) / ST(0), POP ST(0)
			FSTP res	// res변수에 ST(0)을 저장하고 ST(0)을 POP
		}
		break;
	case '%':
		// inline assembly
		__asm{
			FLD fval2				// ST(0) : 오른쪽 피연산자
			FLD fval1				// ST(0) : 왼쪽 피연산자, ST(1) : 오른쪽 피연산자
			FLDZ					// ST(0)에 0을 추가 / ST(0) : 0, ST(1) : 왼쪽 피연산자, ST(2) : 오른쪽 피연산자
			FCOMIP ST(0), ST(1)		// ST(1) - ST(0)을 CMP하고난 후, ST(0)을 POP / FCOMI = FCOM + FNSTSW + SAHF이다.
			JBE PLUS				// 0 ≤ 왼쪽피연산자이면 PLUS로 점프 / CF = 1 OR ZF = 1
			// ELSE 왼쪽 피연산자가 음수
			FPREM1					// 음수의 나머지연산 / ST(0) ← ST(0) − (Q * ST(1))
			JMP RESULT				// RESULT로 JUMP
			PLUS:
			// IF 왼쪽 피연산자가 양수
			FPREM					// 양수의 나머지연산 / ST(0) ← ST(0) − (Q * ST(1))
			RESULT:					
			FSTP res				// res변수에 ST(0)을 저장하고 ST(0)을 POP
			FSTP st(0)				// 쓸모없는 ST(0)를 POP
		}
		break;
	case '^':
		// inline assembly
		__asm{
			FLD fval2				// ST(0) ← fval2 									 / ST(0) : 오른쪽 피연산자(y)
			FLD fval1				// ST(0) ← fval1									 / ST(0) : 왼쪽 피연산자(x), ST(1) : 오른쪽 피연산자(y)
			FYL2X					// ST(1) ← ST(1) * log2ST(0), POP ST(0)			 / ST(0) : y * log2(x) = log2(x^y)
			FLD ST(0)				// ST(0)를 복사										 / ST(0) : y * log2(x), ST(1) : y * log2(x)
			// 소수부분 추출
			FLD1					// ST(0) ← 1										 / ST(0) : 1, ST(1) : y * log2(x), ST(2) : y * log2(x)
			FSCALE					// ST(0) ← ST(0) * 2^RoundTowardZero(ST(1))		 / ST(0) : 2^(y * log2(x)의 정수부분), ST(1) : y * log2(x), ST(2) : y * log2(x)
			FLD1					// ST(0) ← 1										 / ST(0) : 1, ST(1) : 2^(y * log2(x)의 정수부분), ST(2) : y * log2(x), ST(3) : y * log2(x)
			FXCH					// ST(0) 과 ST(1) 바꿈								 / ST(0) : 2^(y * log2(x)의 정수부분), ST(1) : 1, ST(2) : y * log2(x), ST(3) : y * log2(x)
			FYL2X					// ST(1) ← ST(1) * log2ST(0), POP ST(0)			 / ST(0) : log2(2^(y * log2(x)의 정수부분)) = y * log2(x)의 정수부분, ST(1) : y * log2(x), ST(2) : y * log2(x)
			FSUB					// ST(0) ← ST(1) - ST(0), POP ST(0)				 / ST(0) : y * log2(x) - (y * log2(x)의 정수부분) = y * log2(x)의 소수부분, ST(1) : y * log2(x)
			// F2XM1은 ST(0)의 값이 [-1, 1]이어야 하기때문에 소수부분을 추출한다.
			F2XM1					// ST(0) ← 2^ST(0) - 1								 / ST(0) : 2^(y * log2(x)의 소수부분) - 1, ST(1) : y * log2(x)
			FLD1					// ST(0) ← 1										 / ST(0) : 1, ST(1) : 2^(y * log2(x)의 소수부분) - 1, ST(2) : y * log2(x)
			FADD					// ST(1) ← ST(0) + ST(1), POP ST(0)				 / ST(0) : 2^(y * log2(x)의 소수부분), ST(1) : y * log2(x)
			FSCALE					// ST(0) ← ST(0) * 2^RoundTowardZero(ST(1))		 / ST(0) : 2^(y * log2(x)의 소수부분) * 2^(y * log2(x)의 정수부분) = 2^(y * log2(x)) = x^y, ST(1) : y * log2(x)
			FSTP res				// res변수에 ST(0)를 저장하고 ST(0)를 POP			 / ST(0) : y * log2(x)
			FSTP ST(0)				// 쓸모없는 ST(0)를 POP
		}
		break;
	default:
		res = fval2;
		break;
	}
	return res;
}
//등호 버튼
void CMFCApplication6Dlg::OnBnClickedButtonEqu()
{
	for (int i = lpNum; i > 0; i--) {
		OnBnClickedButtonRp();
		lpNum--;
	}

	double res = calc();
	char rval[100];
	sprintf(rval, "%.*lf", getPrecision(res), res);		// 후행0을 빼고 출력 (최대 8자리)
	m_input = rval;

	m_temp = "";
	m_op = "";
	m_exp = "";
	eflag = true;
	dflag = false;
	bflag = false;
	beflag = false;

	UpdateData(false);

	buttonEvent(true);		// 숫자버튼 : 포커스창에 포커스주기 + m_input 보이고 m_temp 안보이기
}



void setBtnColor(CDC  *dc, RECT rect, COLORREF color) {
	dc->FillSolidRect(&rect, color);
	dc->SetBkColor(color);
}

void CMFCApplication6Dlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
#define green RGB(0,200,0)
#define red RGB(200,0,0)
	CDC dc;
	RECT rect;
	dc.Attach(lpDrawItemStruct->hDC);   
	dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
	rect = lpDrawItemStruct->rcItem;

	//버튼 색생 변경하기
	if (nIDCtl == IDC_BUTTON_ADD)		//덧셈
	{
		setBtnColor(&dc, rect, green);	//구현 완료 : green
	}
	if (nIDCtl == IDC_BUTTON_SUB)		//뺄셈
	{
		setBtnColor(&dc, rect, green);	//미구현 : red
	}
	if (nIDCtl == IDC_BUTTON_MUL)		//곱셈
	{
		setBtnColor(&dc, rect, green);
	}	
	if (nIDCtl == IDC_BUTTON_DIV)		//나눗셈
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_MOD)		//MOD
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_ROOT)		//제곱근
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_SQU)		//2제곱
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_POW)		//n제곱
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_LP)		//왼쪽괄호
	{
		setBtnColor(&dc, rect, green);
	}
	if (nIDCtl == IDC_BUTTON_RP)		//오른쪽괄호
	{
		setBtnColor(&dc, rect, green);
	}
	
	UINT state = lpDrawItemStruct->itemState;  //This defines the state of the Push button either pressed or not. 
	if ((state & ODS_SELECTED))
		dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
	else
		dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

	TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
	ZeroMemory(buffer, MAX_PATH);     //Intializing the buffer to zero
	::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH); //Get the Caption of Button Window 

	dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//Redraw the  Caption of Button Window 
	dc.Detach();  // Detach the Button DC
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);

}


BOOL CMFCApplication6Dlg::PreTranslateMessage(MSG* pMsg)
{
	BOOL bShiftKey = (0x8000 == (0x8000 & GetKeyState(VK_SHIFT)));
	// 쉬프트를 누른채 입력하는 특수 키들 먼저 처리
	if (pMsg->message == WM_KEYDOWN && bShiftKey) {
		switch (pMsg->wParam) {
		case 0x39:						// 쉬프트 누른채 숫자 9 : (
			OnBnClickedButtonLp();
			break;
		case 0x30:						// 쉬프트 누른채 숫자 0 : )
			OnBnClickedButtonRp();
			break;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case 0x30:					// 자판위 : 숫자 0
		case VK_NUMPAD0:			// 숫자패드 : 숫자 0
			OnBnClickedButton0();
			break;
		case 0x31:					// 자판위 : 숫자 1
		case VK_NUMPAD1:			// 숫자패드 : 숫자 1
			OnBnClickedButton1();
			break;
		case 0x32:					// 자판위 : 숫자 2
		case VK_NUMPAD2:			// 숫자패드 : 숫자 2
			OnBnClickedButton2();
			break;
		case 0x33:					// 자판위 : 숫자 3
		case VK_NUMPAD3:			// 숫자패드 : 숫자 3
			OnBnClickedButton3();
			break;
		case 0x34:					// 자판위 : 숫자 4
		case VK_NUMPAD4:			// 숫자패드 : 숫자 4
			OnBnClickedButton4();
			break;
		case 0x35:					// 자판위 : 숫자 5
		case VK_NUMPAD5:			// 숫자패드 : 숫자 5
			OnBnClickedButton5();
			break;
		case 0x36:					// 자판위 : 숫자 6
		case VK_NUMPAD6:			// 숫자패드 : 숫자 6
			OnBnClickedButton6();
			break;
		case 0x37:					// 자판위 : 숫자 7
		case VK_NUMPAD7:			// 숫자패드 : 숫자 7
			OnBnClickedButton7();
			break;
		case 0x38:					// 자판위 : 숫자 8
		case VK_NUMPAD8:			// 숫자패드 : 숫자 8
			OnBnClickedButton8();
			break;
		case 0x39:					// 자판위 : 숫자 9
		case VK_NUMPAD9:			// 숫자패드 : 숫자 9
			OnBnClickedButton9();
			break;
		case VK_ADD:				// 숫자패드 : 기호 +
			OnBnClickedButtonAdd();
			break;
		case VK_SUBTRACT:			// 숫자패드 : 기호 -
			OnBnClickedButtonSub();
			break;
		case VK_MULTIPLY:			// 숫자패드 : 기호 *
			OnBnClickedButtonMul();
			break;
		case VK_DIVIDE:				// 숫자패드 : 기호 /
			OnBnClickedButtonDiv();
			break;
		case VK_DECIMAL:			// 숫자패드 : 기호 .
			OnBnClickedButtonDot();
			break;
		case VK_BACK:				// 자판위 : Backspace
			OnBnClickedButtonDel();
			break;
		case VK_RETURN:				// 엔터버튼
			OnBnClickedButtonEqu();
			return TRUE;			// 엔터키로 다이얼로그 종료 방지
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
