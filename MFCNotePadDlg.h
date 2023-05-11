// MFCNotePadDlg.h: 헤더 파일
//
#pragma once

// 최소 영역을 위한 변수 정의(헤더에)

// CMFCNotePadDlg 대화 상자
class CMFCNotePadDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCNotePadDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CRect m_rcMinimumDialog;
	CString nowFilePath; //파일 현재 저장 위치
	BOOL is_modified=false;
private:
	HACCEL m_hAccel;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCNOTEPAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void SetupForMFCNotePad();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBtnload();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnSsave();
	BOOL OnSave();
	afx_msg void OnBtnClose();
	afx_msg void ChangeCaption(CString newFileName);
	CEdit m_strLoad;
	CEdit m_strSave;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
