
// MFCNotePadDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCNotePad.h"
#include "MFCNotePadDlg.h"
#include "afxdialogex.h"
#include <vector>
#include "string.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCNotePadDlg 대화 상자

CMFCNotePadDlg::CMFCNotePadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCNOTEPAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCNotePadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCNotePadDlg, CDialogEx)
	ON_WM_PAINT() 
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCNotePadDlg::OnEnChangeEdit1)
	ON_COMMAND(BtnLoad, &CMFCNotePadDlg::OnBtnload)
	ON_COMMAND(BtnSave, &CMFCNotePadDlg::OnBtnSave)
	ON_COMMAND(BtnSsave, &CMFCNotePadDlg::OnBtnSsave)
	ON_COMMAND(BtnClose, &CMFCNotePadDlg::OnBtnClose)
END_MESSAGE_MAP()

// CMFCNotePadDlg 메시지 처리기

BOOL CMFCNotePadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_ACCELERATOR_EXIT));

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//동적 레이아웃 설정.
	SetupForMFCNotePad();

	//리사이징 '최소' 크기를 위한 영역 획득
	GetClientRect(m_rcMinimumDialog);
	CalcWindowRect(m_rcMinimumDialog);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCNotePadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCNotePadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCNotePadDlg::SetupForMFCNotePad()
{
	// 동적 레이아웃 활성화.
	this->EnableDynamicLayout();

	/* 이동 옵션 */
	//이동하지 않음.
	auto move_none = CMFCDynamicLayout::MoveSettings{};
	
	/*크기 옵션*/
	//X,Y 방향으로 100% 비율 크기 조정.
	auto size_both_100 = CMFCDynamicLayout::SizeSettings{};
	size_both_100.m_nXRatio = 100;
	size_both_100.m_nYRatio = 100;
	
	// Dialog의 동적 레이아웃 포인터 획득.
	auto manager = this->GetDynamicLayout();
	// 동적 레이아웃 생성.
	manager->Create(this);
	// 동적 레이아웃에 등록할 컨트롤 추가.
	manager->AddItem(IDC_EDIT1, move_none, size_both_100);
}

//파라미터의 초기값과 최소값을 구함.
void CMFCNotePadDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = m_rcMinimumDialog.Width();
	lpMMI->ptMinTrackSize.y = m_rcMinimumDialog.Height();
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CMFCNotePadDlg::OnEnChangeEdit1() //수정사항이 있나요? 예/아니오
{
	is_modified = true; //true면 수정된거니까 .. 수정사항이 잇을시 저장 이벤트발생
}

//불러오기
void CMFCNotePadDlg::OnBtnload() 
{
	CString m_strPath,m_strFileName, str; //data 
	CStdioFile rFile; //file 버퍼

	static TCHAR BASED_CODE szFilter[] = _T("텍스트 문서(*.txt) | *.txt; |모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("*.txt"), _T(""), OFN_FILEMUSTEXIST, szFilter);

	if (dlg.DoModal() == IDOK) //확인 버튼이 눌리면 실행
	{
		m_strPath = dlg.GetPathName(); //경로 이름
		nowFilePath = m_strPath;
		m_strFileName = dlg.GetFileName(); //파일 이름
		ChangeCaption(m_strFileName);

		if (rFile.Open(m_strPath, CFile::modeRead)) { //modeRead 읽기전용
			CString data;
			while (rFile.ReadString(data)) {
				str.Append(data + '\n');
			}
			SetDlgItemText(IDC_EDIT1, str);
		}
		is_modified = false;
		rFile.Close();
	}
}

//저장 //사용자가 "저장" 레이블이 있는 버튼을 클릭할 때 트리거됨
void CMFCNotePadDlg::OnBtnSave()
{	
	if (is_modified == false) {
		return; //OnBtnSave 함수 종료
	}
	else if(is_modified == true) {
		OnSave();
		//팝업창 없이 그냥
	}
	else {
		MessageBox(_T("저장 실패"), _T("알림"), MB_OK | MB_ICONERROR);
	}
}

//저장하기 //
BOOL CMFCNotePadDlg::OnSave() {
	
	CString m_strPath, str;
	FILE* m_pFile;
	TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.TXT) | *.TXT;*.txt; | 모든파일(*.*) |*.*|");
	CFileDialog dlg(FALSE, _T("*.txt"), 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (IDOK == dlg.DoModal()) { //OK버튼 클릭시
		m_strPath = dlg.GetPathName(); //선택된 파일의 전체 경로를 반환

		errno_t err = _tfopen_s(&m_pFile, m_strPath, _T("wt,ccs=UNICODE"));
		if (err != 0) { return 0; }
		CStdioFile m_File(m_pFile);

		// 텍스트 박스에서 글자를 가져오자
		GetDlgItemText(IDC_EDIT1, str);
		m_File.WriteString(str); //str의 내용을 파일에 씀
		m_File.Close();
		MessageBox(L"저장 성공!", L"알림", MB_OK | MB_ICONINFORMATION);
	}
}

//다른 이름으로 저장
void CMFCNotePadDlg::OnBtnSsave()
{	//"다른 이름으로 저장" 클릭 시 트리거

	static TCHAR BASED_CODE szFilter[] = _T("텍스트 문서(*.txt) | *.txt; |모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("*.txt"), _T(""), OFN_FILEMUSTEXIST, szFilter);

	if (IDOK == dlg.DoModal()) //OK 클릭시
	{
		CStdioFile file;

		CString pathName = dlg.GetPathName();
		nowFilePath = pathName;
		CString fileName = dlg.GetFileName();
		ChangeCaption(fileName);

		if (file.Open(pathName, CFile::modeCreate | CFile::modeWrite)) //만약 파일이 열리면
		{
			CString data;
			GetDlgItemText(IDC_EDIT1, data); //IDC_EDIT1에 쓰인 텍스트가 data에 저장됨
			file.WriteString(data); 
			file.Close();
		}
		is_modified = true; //수정되면 바뀌게
		//저장이 완료되면 여기서 true를 반환함.
	}
	is_modified = false;
	//저장이 정상적으로 완료되지 않으면 false임
	
}

void CMFCNotePadDlg::ChangeCaption(CString newFileName) 
{
	CString windowTitle = L"Untitled - MFC Notepad"; // 기본 제목
	if (!newFileName.IsEmpty()) //FileName이 비어있지 않다면
	{
		windowTitle = newFileName + L" - MFC Notepad"; //제목 업데이트
	}
	SetWindowText(windowTitle); //업데이트 된 제목 설정
}


//닫기//"닫기" 클릭시 트리거
void CMFCNotePadDlg::OnBtnClose()
{
	if (is_modified == true) {
		int i = MessageBox(L"저장하시겠습니까?", L"알림", MB_YESNO | MB_ICONINFORMATION);
			if (i == IDYES) {
				if (OnSave())
					DestroyWindow();
				else{
					OnBtnSave();
					DestroyWindow();
				}
			}
			else if(i==IDNO)
				DestroyWindow();
	}
	else
		DestroyWindow();
}


BOOL CMFCNotePadDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (::TranslateAccelerator(this->m_hWnd, m_hAccel, pMsg))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
