
// TINY+Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TINY+.h"
#include "TINY+Dlg.h"
#include "afxdialogex.h"
#include "create_tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>

using namespace std;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTINYDlg 对话框



CTINYDlg::CTINYDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TINY_DIALOG, pParent)
	, program(_T(""))
	, syntax_tree(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTINYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, program);
	DDX_Text(pDX, IDC_EDIT2, syntax_tree);
	DDX_Control(pDX, IDC_EDIT1, program_c);
}

BEGIN_MESSAGE_MAP(CTINYDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTINYDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTINYDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CTINYDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, &CTINYDlg::OnBnClickedButtonDisplay)
END_MESSAGE_MAP()


// CTINYDlg 消息处理程序

BOOL CTINYDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTINYDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTINYDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTINYDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTINYDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(TRUE);
	if (fdlg.DoModal() == IDOK)
	{
		ifstream file(fdlg.GetPathName());
		if (!file)
		{
			MessageBox(L"文件打开失败");
		}
		char temp1[1024];
		string temp2 = "";
		while (!file.eof())
		{
			file.getline(temp1, 1024);
			temp2 += temp1;
			temp2 += "\r\n";
		}
		file.close();
		program = temp2.c_str();
		UpdateData(false);
	}
	else
	{
		return;
	}
}


void CTINYDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(FALSE);
	fdlg.m_ofn.lpstrFilter = L"Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	fdlg.m_ofn.lpstrDefExt = L"txt";
	if (fdlg.DoModal() == IDOK)
	{
		ofstream file(fdlg.GetPathName());
		UpdateData(true);
		program_c.GetWindowTextW(program);
		for (int i = 0; i < program.GetLength(); i++)
		{
			if (program[i] != '\r')
			{
				file.put(program[i]);
			}
		}
		file.close();
	}
}

void CTINYDlg::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	ofstream file("temp.out");
	program_c.GetWindowTextW(program);
	for (int i = 0; i < program.GetLength(); i++)
	{
		if (program[i] != '\r')
		{
			file.put(program[i]);
		}
	}
	file.close();
	create_tree("temp.out", "syntaxtree.out");
}


void CTINYDlg::OnBnClickedButtonDisplay()
{
	// TODO: 在此添加控件通知处理程序代码
	ifstream file("syntaxtree.out");
	if (!file)
	{
		MessageBox(L"未生成语法树");
		return;
	}
	char temp[1024];
	string tree = "";
	while (!file.eof())
	{
		file.getline(temp, 1024);
		tree += temp;
		tree += "\r\n";
	}
	file.close();
	syntax_tree = tree.c_str();
	UpdateData(false);
}
