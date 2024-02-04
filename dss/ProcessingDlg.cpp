#include "dss_common.h"

#include "DeepSkyStacker.h"
#include "ProcessingDlg.h"
#include "progressdlg.h"
#include "selectrect.h"
#include "FrameInfoSupport.h"

#define dssApp DeepSkyStacker::instance()

/* ------------------------------------------------------------------- */

namespace DSS {
	ProcessingDlg::ProcessingDlg(QWidget *parent)
		: QDialog(parent),
		dirty_ { false },
		redAdjustmentCurve_{ HistogramAdjustmentCurve::Linear },
		greenAdjustmentCurve_{ HistogramAdjustmentCurve::Linear },
		blueAdjustmentCurve_{ HistogramAdjustmentCurve::Linear },
		hacMenu{ this }
	{
		ZFUNCTRACE_RUNTIME();
		setupUi(this);
		tabWidget->setCurrentIndex(0);	// Position on the RGB/K tab

		redGradient->setColorAt(0.5, QColor(qRgb(128, 0, 0)));
		redGradient->setColorAt(0.999, Qt::red);
		redGradient->setColorAt(1.0, Qt::red);

		greenGradient->setColorAt(0.5, QColor(qRgb(0, 128, 0)));
		greenGradient->setColorAt(0.999, Qt::green);
		greenGradient->setColorAt(1.0, Qt::green);

		blueGradient->setColorAt(0.5, QColor(qRgb(0, 0, 128)));
		blueGradient->setColorAt(0.999, Qt::blue);
		blueGradient->setColorAt(1.0, Qt::blue);

		connectSignalsToSlots();

#if QT_VERSION >= 0x00060500
		Qt::ColorScheme colorScheme { QGuiApplication::styleHints()->colorScheme() };
		if (Qt::ColorScheme::Dark == colorScheme)
			iconModifier = "-dark";
#endif

		setButtonIcons();

		linearAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::Linear));
		cubeRootAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::CubeRoot));
		squareRootAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::SquareRoot));
		logAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::Log));
		logLogAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::LogLog));
		logSquareRootAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::LogSquareRoot));
		asinHAction = hacMenu.addAction(HistoAdjustTypeText(HistogramAdjustmentCurve::ASinH));

		//
		// Allow selection of partial image, don't display "Drizzle" rectangles.
		//
		selectRect = new SelectRect(picture);
		selectRect->setShowDrizzle(false);

		connect(selectRect, &SelectRect::selectRectChanged, this, &ProcessingDlg::setSelectionRect);
	}

	ProcessingDlg::~ProcessingDlg()
	{
		ZFUNCTRACE_RUNTIME();
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::connectSignalsToSlots()
	{
#if QT_VERSION >= 0x00060500
		connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
			this, &ProcessingDlg::onColorSchemeChanged);
#endif

		//
		// The source for the slots below are in RGBTab.cpp
		//
		connect(redGradient, &QLinearGradientCtrl::pegMove, this, &ProcessingDlg::redChanging);
		connect(redGradient, &QLinearGradientCtrl::pegMoved, this, &ProcessingDlg::redChanged);

		connect(greenGradient, &QLinearGradientCtrl::pegMove, this, &ProcessingDlg::greenChanging);
		connect(greenGradient, &QLinearGradientCtrl::pegMoved, this, &ProcessingDlg::greenChanged);

		connect(blueGradient, &QLinearGradientCtrl::pegMove, this, &ProcessingDlg::blueChanging);
		connect(blueGradient, &QLinearGradientCtrl::pegMoved, this, &ProcessingDlg::blueChanged);

		connect(redHAC, &QPushButton::pressed, this, &ProcessingDlg::redButtonPressed);
		connect(greenHAC, &QPushButton::pressed, this, &ProcessingDlg::greenButtonPressed);
		connect(blueHAC, &QPushButton::pressed, this, &ProcessingDlg::blueButtonPressed);

	}

	/* ------------------------------------------------------------------- */

	bool ProcessingDlg::saveOnClose()
	{
		ZFUNCTRACE_RUNTIME();
		return true;
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::copyToClipboard()
	{
		ZFUNCTRACE_RUNTIME();
		qDebug() << "Copy to clipboard";
		return;
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::createStarMask()
	{
		ZFUNCTRACE_RUNTIME();
		qDebug() << "Create star mask";
		return;
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::loadFile(const fs::path& file)
	{
		ZFUNCTRACE_RUNTIME();
		qDebug() << "Load File";
		ProgressDlg dlg{ DeepSkyStacker::instance() };
		bool ok { false };

		QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		dssApp->deepStack().Clear();
		dssApp->deepStack().SetProgress(&dlg);
		ok = dssApp->deepStack().LoadStackedInfo(file);
		dssApp->deepStack().SetProgress(nullptr);
		QGuiApplication::restoreOverrideCursor();

		if (ok)
		{
			currentFile = file;

			modifyRGBKGradientControls();
			updateInformation();
			QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

			dssApp->deepStack().GetStackedBitmap().GetBezierAdjust(processParms.bezierAdjust_);
			dssApp->deepStack().GetStackedBitmap().GetHistogramAdjust(processParms.histoAdjust_);
			/*
			updateControlsFromParams();

			ShowOriginalHistogram(false);
			ResetSliders();

			int height = dssApp->deepStack().GetHeight();
			m_ToProcess.Init(dssApp->deepStack().GetWidth(), height, height / 3);

			m_lProcessParams.clear();
			m_Picture.SetImg((HBITMAP)nullptr);
			ProcessAndShow(true);
			*/
			QGuiApplication::restoreOverrideCursor();
			setDirty(false);
		};

		//SetTimer(1, 100, nullptr);
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::loadImage()
	{
		ZFUNCTRACE_RUNTIME();
		qDebug() << "Load image";

		if (saveOnClose())
		{
		}
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::saveImage()
	{
		ZFUNCTRACE_RUNTIME();
		qDebug() << "Save image to file";
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::updateInformation()
	{
		int		isoSpeed;
		int		gain;
		int		totalTime;
		int		nrFrames;
		QString text{ tr("No information available", "IDS_NOINFO") };

		if (!currentFile.empty())
		{
			StackedBitmap& bmp{ dssApp->deepStack().GetStackedBitmap() };
			isoSpeed = bmp.GetISOSpeed();
			gain = bmp.GetGain();
			totalTime = bmp.GetTotalTime();
			nrFrames = bmp.GetNrStackedFrames();
			QString isoText, gainText, timeText, framesText;

			if (isoSpeed) isoText = QString("%1 ISO - ").arg(isoSpeed);
			if (gain >= 0) gainText = QString("%1 Gain - ").arg(gain);
			if (totalTime) timeText = tr("Exposure: %1 "), exposureToString(totalTime);
			if (nrFrames) framesText = tr("%n frames", "IDS_NRFRAMES", nrFrames);

			text = QString("%1\n%2%3%4(%5)").arg(currentFile.c_str())
				.arg(isoText)
				.arg(gainText)
				.arg(timeText)
				.arg(framesText);
		}

		information->setText(text);
	}

	void ProcessingDlg::updateControlsFromParams()
	{
		/*
		m_tabLuminance.m_MidTone.SetPos(processParms.m_BezierAdjust.m_fMidtone * 10);
		m_tabLuminance.m_MidAngle.SetPos(processParms.m_BezierAdjust.m_fMidtoneAngle);

		m_tabLuminance.m_DarkAngle.SetPos(processParms.m_BezierAdjust.m_fDarknessAngle);
		m_tabLuminance.m_DarkPower.SetPos(processParms.m_BezierAdjust.m_fDarknessPower * 10);

		m_tabLuminance.m_HighAngle.SetPos(processParms.m_BezierAdjust.m_fHighlightAngle);
		m_tabLuminance.m_HighPower.SetPos(processParms.m_BezierAdjust.m_fHighlightPower * 10);

		m_tabLuminance.UpdateTexts();

		m_tabSaturation.m_Saturation.SetPos(processParms.m_BezierAdjust.m_fSaturationShift + 50);
		m_tabSaturation.UpdateTexts();

		double					fMinRed,
			fMaxRed,
			fShiftRed;
		double					fMinGreen,
			fMaxGreen,
			fShiftGreen;
		double					fMinBlue,
			fMaxBlue,
			fShiftBlue;

		fMinRed = processParms.m_HistoAdjust.GetRedAdjust().GetMin();
		fMaxRed = processParms.m_HistoAdjust.GetRedAdjust().GetMax();
		fShiftRed = processParms.m_HistoAdjust.GetRedAdjust().GetShift();

		fMinGreen = processParms.m_HistoAdjust.GetGreenAdjust().GetMin();
		fMaxGreen = processParms.m_HistoAdjust.GetGreenAdjust().GetMax();
		fShiftGreen = processParms.m_HistoAdjust.GetGreenAdjust().GetShift();

		fMinBlue = processParms.m_HistoAdjust.GetBlueAdjust().GetMin();
		fMaxBlue = processParms.m_HistoAdjust.GetBlueAdjust().GetMax();
		fShiftBlue = processParms.m_HistoAdjust.GetBlueAdjust().GetShift();

		double				fAbsMin,
			fAbsMax;
		double				fOffset;
		double				fRange;

		fAbsMin = min(fMinRed, min(fMinGreen, fMinBlue));
		fAbsMax = max(fMaxRed, min(fMaxGreen, fMaxBlue));

		fRange = fAbsMax - fAbsMin;
		if (fRange * 1.10 <= 65535.0)
			fRange *= 1.10;

		fOffset = (fAbsMin + fAbsMax - fRange) / 2.0;
		if (fOffset < 0)
			fOffset = 0.0;

		m_fGradientOffset = fOffset;
		m_fGradientRange = fRange;

		CGradient& RedGradient = m_tabRGB.m_RedGradient.GetGradient();
		RedGradient.SetPeg(RedGradient.IndexFromId(0), (float)((fMinRed - m_fGradientOffset) / m_fGradientRange));
		RedGradient.SetPeg(RedGradient.IndexFromId(1), (float)(fShiftRed / 2.0 + 0.5));
		RedGradient.SetPeg(RedGradient.IndexFromId(2), (float)((fMaxRed - m_fGradientOffset) / m_fGradientRange));
		m_tabRGB.m_RedGradient.Invalidate(true);
		m_tabRGB.SetRedAdjustMethod(processParms.m_HistoAdjust.GetRedAdjust().GetAdjustMethod());

		CGradient& GreenGradient = m_tabRGB.m_GreenGradient.GetGradient();
		GreenGradient.SetPeg(GreenGradient.IndexFromId(0), (float)((fMinGreen - m_fGradientOffset) / m_fGradientRange));
		GreenGradient.SetPeg(GreenGradient.IndexFromId(1), (float)(fShiftGreen / 2.0 + 0.5));
		GreenGradient.SetPeg(GreenGradient.IndexFromId(2), (float)((fMaxGreen - m_fGradientOffset) / m_fGradientRange));
		m_tabRGB.m_GreenGradient.Invalidate(true);
		m_tabRGB.SetGreenAdjustMethod(processParms.m_HistoAdjust.GetGreenAdjust().GetAdjustMethod());

		CGradient& BlueGradient = m_tabRGB.m_BlueGradient.GetGradient();
		BlueGradient.SetPeg(BlueGradient.IndexFromId(0), (float)((fMinBlue - m_fGradientOffset) / m_fGradientRange));
		BlueGradient.SetPeg(BlueGradient.IndexFromId(1), (float)(fShiftBlue / 2.0 + 0.5));
		BlueGradient.SetPeg(BlueGradient.IndexFromId(2), (float)((fMaxBlue - m_fGradientOffset) / m_fGradientRange));
		m_tabRGB.m_BlueGradient.Invalidate(true);
		m_tabRGB.SetBlueAdjustMethod(processParms.m_HistoAdjust.GetBlueAdjust().GetAdjustMethod());
		*/
	};

	//
	// Slots
	//

	void ProcessingDlg::setSelectionRect(const QRectF& rect)
	{
		selectionRect = DSSRect(rect.x(), rect.y(), rect.right(), rect.bottom());
	}

	/* ------------------------------------------------------------------- */

#if QT_VERSION >= 0x00060500
	void ProcessingDlg::onColorSchemeChanged(Qt::ColorScheme colorScheme)
	{
		iconModifier.clear();
		//
		// Dark colour scheme?
		//
		if (Qt::ColorScheme::Dark == colorScheme)
			iconModifier = "-dark";

		setButtonIcons();		// in RGBTab.cpp
		update();
	}
#endif

} // namespace DSS
#if (0)
/* ------------------------------------------------------------------- */
/////////////////////////////////////////////////////////////////////////////
// CProcessingDlg dialog


CProcessingDlg::CProcessingDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CProcessingDlg::IDD, pParent), m_Settings(_T(""))
{
	//{{AFX_DATA_INIT(CProcessingDlg)
	//}}AFX_DATA_INIT
	m_bDirty = false;
	m_fGradientOffset = 0;
	m_fGradientRange = 0;
}

/* ------------------------------------------------------------------- */

void CProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessingDlg)
	DDX_Control(pDX, IDC_INFO, m_Info);
	DDX_Control(pDX, IDC_PROCESSING_PROGRESS, m_ProcessingProgress);
	DDX_Control(pDX, IDC_SETTINGS, m_SettingsRect);
	DDX_Control(pDX, IDC_ORIGINAL_HISTOGRAM, m_OriginalHistogramStatic);
	DDX_Control(pDX, IDC_PICTURE, m_PictureStatic);
	//}}AFX_DATA_MAP
}

/* ------------------------------------------------------------------- */

BEGIN_MESSAGE_MAP(CProcessingDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessingDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PROCESS, OnProcess)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_INITNEWPICTURE, OnInitNewPicture)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GC_SELCHANGE, IDC_REDGRADIENT, OnNotifyRedChangeSelPeg)
	ON_NOTIFY(GC_PEGMOVE, IDC_REDGRADIENT, OnNotifyRedPegMove)
	ON_NOTIFY(GC_PEGMOVED, IDC_REDGRADIENT, OnNotifyRedPegMove)
	ON_NOTIFY(GC_SELCHANGE, IDC_GREENGRADIENT, OnNotifyGreenChangeSelPeg)
	ON_NOTIFY(GC_PEGMOVE, IDC_GREENGRADIENT, OnNotifyGreenPegMove)
	ON_NOTIFY(GC_PEGMOVED, IDC_GREENGRADIENT, OnNotifyGreenPegMove)
	ON_NOTIFY(GC_SELCHANGE, IDC_BLUEGRADIENT, OnNotifyBlueChangeSelPeg)
	ON_NOTIFY(GC_PEGMOVE, IDC_BLUEGRADIENT, OnNotifyBluePegMove)
	ON_NOTIFY(GC_PEGMOVED, IDC_BLUEGRADIENT, OnNotifyBluePegMove)
END_MESSAGE_MAP()

/* ------------------------------------------------------------------- */
/////////////////////////////////////////////////////////////////////////////
// CProcessingDlg message handlers

BOOL CProcessingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect			rc;

	CSize	size;
	m_Info.SetBkColor(RGB(224, 244, 252), RGB(138, 185, 242), CLabel::Gradient);

	m_Picture.CreateFromStatic(&m_PictureStatic);

	m_SettingsRect.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.left = 0;
	rc.top -= 11; /* odd */
	size = rc.Size();
	size.cx += 45;
	size.cy += 10;
	rc = CRect(rc.TopLeft(), size);

	m_Settings.AddPage(&m_tabRGB);
	m_Settings.AddPage(&m_tabLuminance);
	m_Settings.AddPage(&m_tabSaturation);

	m_Settings.EnableStackedTabs(false);
	m_Settings.Create(this, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0);

	m_Settings.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_Settings.ModifyStyle(0, WS_TABSTOP);
	m_Settings.MoveWindow(&rc, true);
	m_Settings.ShowWindow(SW_SHOWNA);

	m_OriginalHistogram.CreateFromStatic(&m_OriginalHistogramStatic);
	m_OriginalHistogram.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.left += 45;
	m_OriginalHistogram.MoveWindow(&rc, true);

	m_OriginalHistogram.SetBltMode(CWndImage::bltFitXY);
	m_OriginalHistogram.SetAlign(CWndImage::bltCenter, CWndImage::bltCenter);

	m_tabLuminance.m_MidTone.SetRange(0, 1000, true);
	m_tabLuminance.m_MidTone.SetPos(200);
	m_tabLuminance.m_MidAngle.SetRange(0, 45, true);
	m_tabLuminance.m_MidAngle.SetPos(20);

	m_tabLuminance.m_DarkAngle.SetRange(0, 45, true);
	m_tabLuminance.m_DarkAngle.SetPos(0);
	m_tabLuminance.m_DarkPower.SetRange(0, 1000, true);
	m_tabLuminance.m_DarkPower.SetPos(500);

	m_tabLuminance.m_HighAngle.SetRange(0, 45, true);
	m_tabLuminance.m_HighAngle.SetPos(0);
	m_tabLuminance.m_HighPower.SetRange(0, 1000, true);
	m_tabLuminance.m_HighPower.SetPos(500);

	m_tabLuminance.UpdateTexts();

	m_tabSaturation.m_Saturation.SetRange(0, 100, true);
	m_tabSaturation.m_Saturation.SetPos(50);
	m_tabSaturation.UpdateTexts();

	m_ControlPos.SetParent(this);

	m_ControlPos.AddControl(IDC_PICTURE, CP_RESIZE_VERTICAL | CP_RESIZE_HORIZONTAL);
	m_ControlPos.AddControl(&m_Settings, CP_MOVE_VERTICAL);
	m_ControlPos.AddControl(&m_OriginalHistogram, CP_MOVE_VERTICAL);
	m_ControlPos.AddControl(&m_Info, CP_RESIZE_HORIZONTAL);
	m_ControlPos.AddControl(&m_ProcessingProgress, CP_MOVE_VERTICAL | CP_RESIZE_HORIZONTAL);

	m_Picture.EnableZoom(true);
	m_Picture.SetImageSink(&m_SelectRectSink);
	m_Picture.SetBltMode(CWndImage::bltFitXY);
	m_Picture.SetAlign(CWndImage::bltCenter, CWndImage::bltCenter);

	COLORREF		crReds[3] = { RGB(0, 0, 0), RGB(128, 0, 0), RGB(255, 0, 0) };
	COLORREF		crGreens[3] = { RGB(0, 0, 0), RGB(0, 128, 0), RGB(0, 255, 0) };
	COLORREF		crBlues[3] = { RGB(0, 0, 0), RGB(0, 0, 128), RGB(0, 0, 255) };

	SetTimer(1, 100, nullptr);

	m_fGradientOffset = 0.0;
	m_fGradientRange = 65535.0;

	m_tabRGB.m_RedGradient.SetOrientation(CGradientCtrl::ForceHorizontal);
	m_tabRGB.m_RedGradient.SetPegSide(true, false);
	m_tabRGB.m_RedGradient.SetPegSide(false, true);
	m_tabRGB.m_RedGradient.GetGradient().SetStartPegColour(RGB(0, 0, 0));
	m_tabRGB.m_RedGradient.GetGradient().AddPeg(crReds[0], 0.0, 0);
	m_tabRGB.m_RedGradient.GetGradient().AddPeg(crReds[1], 0.5, 1);
	m_tabRGB.m_RedGradient.GetGradient().AddPeg(crReds[2], 1.0, 2);
	m_tabRGB.m_RedGradient.GetGradient().SetEndPegColour(RGB(255, 0, 0));
	m_tabRGB.m_RedGradient.GetGradient().SetBackgroundColour(RGB(255, 255, 255));
	m_tabRGB.m_RedGradient.GetGradient().SetInterpolationMethod(CGradient::Linear);
	m_tabRGB.SetRedAdjustMethod(HAT_LINEAR);

	m_tabRGB.m_GreenGradient.SetOrientation(CGradientCtrl::ForceHorizontal);
	m_tabRGB.m_GreenGradient.SetPegSide(true, false);
	m_tabRGB.m_GreenGradient.SetPegSide(false, true);
	m_tabRGB.m_GreenGradient.GetGradient().SetStartPegColour(RGB(0, 0, 0));
	m_tabRGB.m_GreenGradient.GetGradient().AddPeg(crGreens[0], 0.0, 0);
	m_tabRGB.m_GreenGradient.GetGradient().AddPeg(crGreens[1], 0.5, 1);
	m_tabRGB.m_GreenGradient.GetGradient().AddPeg(crGreens[2], 1.0, 2);
	m_tabRGB.m_GreenGradient.GetGradient().SetEndPegColour(RGB(0, 255, 0));
	m_tabRGB.m_GreenGradient.GetGradient().SetBackgroundColour(RGB(255, 255, 255));
	m_tabRGB.m_GreenGradient.GetGradient().SetInterpolationMethod(CGradient::Linear);
	m_tabRGB.SetGreenAdjustMethod(HAT_LINEAR);

	m_tabRGB.m_BlueGradient.SetOrientation(CGradientCtrl::ForceHorizontal);
	m_tabRGB.m_BlueGradient.SetPegSide(true, false);
	m_tabRGB.m_BlueGradient.SetPegSide(false, true);
	m_tabRGB.m_BlueGradient.GetGradient().SetStartPegColour(RGB(0, 0, 0));
	m_tabRGB.m_BlueGradient.GetGradient().AddPeg(crBlues[0], 0.0, 0);
	m_tabRGB.m_BlueGradient.GetGradient().AddPeg(crBlues[1], 0.5, 1);
	m_tabRGB.m_BlueGradient.GetGradient().AddPeg(crBlues[2], 1.0, 2);
	m_tabRGB.m_BlueGradient.GetGradient().SetEndPegColour(RGB(0, 0, 255));
	m_tabRGB.m_BlueGradient.GetGradient().SetBackgroundColour(RGB(255, 255, 255));
	m_tabRGB.m_BlueGradient.GetGradient().SetInterpolationMethod(CGradient::Linear);
	m_tabRGB.SetBlueAdjustMethod(HAT_LINEAR);

	m_ProcessingProgress.SetRange(0, 100);
	m_ProcessingProgress.SetPos(100);

	UpdateControls();

	return true;  // return true unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return false
}

/* ------------------------------------------------------------------- */

void CProcessingDlg::UpdateControls()
{
	bool			bUndo = false,
		bRedo = false;

	if (dssApp->deepStack().IsLoaded())
	{
		m_tabRGB.EnableWindow(true);
		m_tabLuminance.EnableWindow(true);
		m_tabSaturation.EnableWindow(true);

		bUndo = m_lProcessParams.IsBackwardAvailable();
		bRedo = m_lProcessParams.IsForwardAvailable();

		m_tabRGB.m_Undo.EnableWindow(bUndo);
		m_tabRGB.m_Redo.EnableWindow(bRedo);
		m_tabLuminance.m_Undo.EnableWindow(bUndo);
		m_tabLuminance.m_Redo.EnableWindow(bRedo);
		m_tabSaturation.m_Undo.EnableWindow(bUndo);
		m_tabSaturation.m_Redo.EnableWindow(bRedo);
	}
	else
	{
		m_tabRGB.EnableWindow(false);
		m_tabLuminance.EnableWindow(false);
		m_tabSaturation.EnableWindow(false);
	};
};

/* ------------------------------------------------------------------- */



/* ------------------------------------------------------------------- */

void CProcessingDlg::OnUndo()
{
	m_lProcessParams.MoveBackward();
	m_lProcessParams.GetCurrentParams(processParms);
	UpdateControlsFromParams();
	ProcessAndShow(false);
	ShowOriginalHistogram(false);
	UpdateControls();
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnRedo()
{
	m_lProcessParams.MoveForward();
	m_lProcessParams.GetCurrentParams(processParms);
	UpdateControlsFromParams();
	ProcessAndShow(false);
	ShowOriginalHistogram(false);
	UpdateControls();
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnSettings()
{
	CSettingsDlg			dlg;
	ProcessingSettingsSet& Settings = dssApp->processingSettingsSet();

	KillTimer(1);
	dlg.SetDSSSettings(&Settings, processParms);
	dlg.DoModal();

	if (dlg.IsLoaded())
	{
		dlg.GetCurrentSettings(processParms);
		UpdateControlsFromParams();
		ProcessAndShow(false);
		ShowOriginalHistogram(false);
		UpdateControls();
		m_bDirty = true;
	};
	SetTimer(1, 100, nullptr);
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	CWnd* parent{ GetParent() };
	if (parent)
	{
		parent->GetClientRect(&rc);
	}

	CDialog::OnSize(nType, cx, cy);

	m_ControlPos.MoveControls();
}

/* ------------------------------------------------------------------- */


LRESULT CProcessingDlg::OnInitNewPicture(WPARAM, LPARAM)
{
	ShowOriginalHistogram(false);
	ResetSliders();

	int height = dssApp->deepStack().GetHeight();
	m_ToProcess.Init(dssApp->deepStack().GetWidth(), height, height / 3);

	m_lProcessParams.clear();
	m_Picture.SetImg((HBITMAP)nullptr);
	ProcessAndShow(true);

	UpdateInfos();

	return 1;
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow && dssApp->deepStack().IsNewStackedBitmap(true))
		OnInitNewPicture(0, 0);
}

/* ------------------------------------------------------------------- */

void	CProcessingDlg::LoadFile(LPCTSTR szFileName)
{

};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnLoaddsi()
{
	if (AskToSave())
	{
		bool				bOk = false;
		CString				strFilter;
		QSettings			settings;

		CString	strBaseDirectory = (LPCTSTR)settings.value("Folders/SaveDSIFolder", QString("")).toString().utf16();

		strFilter.LoadString(IDS_FILTER_DSIIMAGETIFF);
		CFileDialog			dlgOpen(true,
			_T(".DSImage"),
			nullptr,
			OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_PATHMUSTEXIST,
			strFilter,
			this);
		TCHAR				szBigBuffer[20000] = _T("");
		DSS::ProgressDlg dlg{ DeepSkyStacker::instance() };

		if (strBaseDirectory.GetLength())
			dlgOpen.m_ofn.lpstrInitialDir = strBaseDirectory.GetBuffer(_MAX_PATH);

		KillTimer(1);

		dlgOpen.m_ofn.lpstrFile = szBigBuffer;
		dlgOpen.m_ofn.nMaxFile = sizeof(szBigBuffer) / sizeof(szBigBuffer[0]);

		if (dlgOpen.DoModal() == IDOK)
		{
			CString			strFile;
			POSITION		pos;

			pos = dlgOpen.GetStartPosition();
			while (pos && !bOk)
			{
				BeginWaitCursor();
				strFile = dlgOpen.GetNextPathName(pos);
				dssApp->deepStack().Clear();
				dssApp->deepStack().SetProgress(&dlg);
				bOk = dssApp->deepStack().LoadStackedInfo(strFile);
				dssApp->deepStack().SetProgress(nullptr);
				EndWaitCursor();
			};

			if (bOk)
			{
				m_strCurrentFile = strFile;

				TCHAR		szDir[1 + _MAX_DIR];
				TCHAR		szDrive[1 + _MAX_DRIVE];

				_tsplitpath(strFile, szDrive, szDir, nullptr, nullptr);
				strBaseDirectory = szDrive;
				strBaseDirectory += szDir;

				settings.setValue("Folders/SaveDSIFolder", QString::fromWCharArray(strBaseDirectory.GetString()));

				UpdateMonochromeControls();
				UpdateInfos();
				BeginWaitCursor();
				dssApp->deepStack().GetStackedBitmap().GetBezierAdjust(processParms.m_BezierAdjust);
				dssApp->deepStack().GetStackedBitmap().GetHistogramAdjust(processParms.m_HistoAdjust);

				UpdateControlsFromParams();

				ShowOriginalHistogram(false);
				// ResetSliders();
				int height = dssApp->deepStack().GetHeight();
				m_ToProcess.Init(dssApp->deepStack().GetWidth(), height, height / 3);

				m_lProcessParams.clear();
				m_Picture.SetImg((HBITMAP)nullptr);
				ProcessAndShow(true);
				EndWaitCursor();
				m_bDirty = false;
			};
		};

		SetTimer(1, 100, nullptr);
	};
}

/* ------------------------------------------------------------------- */

bool CProcessingDlg::AskToSave()
{
	bool				bResult = false;

	if (m_bDirty)
	{
		int				nResult;

		nResult = AfxMessageBox(IDS_MSG_SAVEMODIFICATIONS, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nResult == IDCANCEL)
			bResult = false;
		else if (nResult == IDNO)
			bResult = true;
		else
			bResult = SavePictureToFile();
	}
	else
		bResult = true;

	return bResult;
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::ProcessAndShow(bool bSaveUndo)
{
	UpdateHistogramAdjust();

	processParms.m_BezierAdjust.m_fMidtone = m_tabLuminance.m_MidTone.GetPos() / 10.0;
	processParms.m_BezierAdjust.m_fMidtoneAngle = m_tabLuminance.m_MidAngle.GetPos();
	processParms.m_BezierAdjust.m_fDarknessAngle = m_tabLuminance.m_DarkAngle.GetPos();
	processParms.m_BezierAdjust.m_fHighlightAngle = m_tabLuminance.m_HighAngle.GetPos();
	processParms.m_BezierAdjust.m_fHighlightPower = m_tabLuminance.m_HighPower.GetPos() / 10.0;
	processParms.m_BezierAdjust.m_fDarknessPower = m_tabLuminance.m_DarkPower.GetPos() / 10.0;
	processParms.m_BezierAdjust.m_fSaturationShift = m_tabSaturation.m_Saturation.GetPos() - 50;
	processParms.m_BezierAdjust.Clear();

	if (bSaveUndo)
		m_lProcessParams.AddParams(processParms);

	UpdateControls();

	CRect			rcSelect;

	if (m_SelectRectSink.GetSelectRect(rcSelect))
		m_ToProcess.SetProcessRect(rcSelect);
	else
	{
		rcSelect.SetRectEmpty();
		m_ToProcess.SetProcessRect(rcSelect);
	};

	m_ToProcess.Reset();
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::CopyPictureToClipboard()
{
	HBITMAP			hBitmap;

	hBitmap = m_Picture.GetBitmap();

	if (hBitmap)
		CopyBitmapToClipboard(hBitmap);
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::CreateStarMask()
{
	if (dssApp->deepStack().IsLoaded())
	{
		KillTimer(1);

		CStarMaskDlg dlgStarMask;
		dlgStarMask.SetBaseFileName(m_strCurrentFile);
		if (dlgStarMask.DoModal() == IDOK)
		{
			DSS::ProgressDlg dlg{ DeepSkyStacker::instance() };
			CStarMaskEngine starmask;

			dlg.SetJointProgress(true);
			std::shared_ptr<CMemoryBitmap> pBitmap = dssApp->deepStack().GetStackedBitmap().GetBitmap(&dlg);
			if (std::shared_ptr<CMemoryBitmap> pStarMask = starmask.CreateStarMask2(pBitmap.get(), &dlg))
			{
				// Save the star mask to a file
				CString strFileName;
				CString strDescription;
				bool bFits;

				strDescription.LoadString(IDS_STARMASKDESCRIPTION);
				const QString description(QString::fromWCharArray(strDescription.GetString())); // TODO: Should be loading direct.

				dlgStarMask.GetOutputFileName(strFileName, bFits);
				const QString strText(QCoreApplication::translate("ProcessingDlg", "Saving the Star Mask in %1", "IDS_SAVINGSTARMASK").arg(QString::fromWCharArray(strFileName.GetString())));
				dlg.Start2(strText, 0);
				if (bFits)
					WriteFITS(strFileName.GetString(), pStarMask.get(), &dlg, description);
				else
					WriteTIFF(strFileName.GetString(), pStarMask.get(), &dlg, description);
			}
		}
		SetTimer(1, 100, nullptr);
	}
	else
	{
		AfxMessageBox(IDS_MSG_NOPICTUREFORSTARMASK, MB_OK | MB_ICONSTOP);
	}
}


bool CProcessingDlg::SavePictureToFile()
{
	bool				bResult = false;
	QSettings			settings;
	CString				strBaseDirectory;
	CString				strBaseExtension;
	bool				applied = false;
	uint				dwCompression;
	CRect				rcSelect;

	if (dssApp->deepStack().IsLoaded())
	{
		strBaseDirectory = (LPCTSTR)settings.value("Folders/SavePictureFolder").toString().utf16();
		strBaseExtension = (LPCTSTR)settings.value("Folders/SavePictureExtension").toString().utf16();
		auto dwFilterIndex = settings.value("Folders/SavePictureIndex", 0).toUInt();
		applied = settings.value("Folders/SaveApplySetting", false).toBool();
		dwCompression = settings.value("Folders/SaveCompression", (uint)TC_NONE).toUInt();

		if (!strBaseExtension.GetLength())
			strBaseExtension = _T(".tif");

		CSavePicture				dlgOpen(false,
			_T(".TIF"),
			nullptr,
			OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_ENABLESIZING,
			OUTPUTFILE_FILTERS,
			this);

		if (m_SelectRectSink.GetSelectRect(rcSelect))
			dlgOpen.SetUseRect(true, true);
		if (applied)
			dlgOpen.SetApplied(true);

		dlgOpen.SetCompression((TIFFCOMPRESSION)dwCompression);

		if (strBaseDirectory.GetLength())
			dlgOpen.m_ofn.lpstrInitialDir = strBaseDirectory.GetBuffer(_MAX_PATH);
		dlgOpen.m_ofn.nFilterIndex = dwFilterIndex;

		TCHAR				szBigBuffer[20000] = _T("");
		DSS::ProgressDlg dlg{ DeepSkyStacker::instance() };

		dlgOpen.m_ofn.lpstrFile = szBigBuffer;
		dlgOpen.m_ofn.nMaxFile = sizeof(szBigBuffer) / sizeof(szBigBuffer[0]);

		if (dlgOpen.DoModal() == IDOK)
		{
			POSITION		pos;

			pos = dlgOpen.GetStartPosition();
			if (pos)
			{
				CString			strFile;
				LPRECT			lpRect = nullptr;
				bool			bApply;
				bool			bUseRect;
				TIFFCOMPRESSION	Compression;

				bApply = dlgOpen.GetApplied();
				bUseRect = dlgOpen.GetUseRect();
				Compression = dlgOpen.GetCompression();

				if (bUseRect && m_SelectRectSink.GetSelectRect(rcSelect))
					lpRect = &rcSelect;

				BeginWaitCursor();
				strFile = dlgOpen.GetNextPathName(pos);
				if (dlgOpen.m_ofn.nFilterIndex == 1)
					dssApp->deepStack().GetStackedBitmap().SaveTIFF16Bitmap(strFile, lpRect, &dlg, bApply, Compression);
				else if (dlgOpen.m_ofn.nFilterIndex == 2)
					dssApp->deepStack().GetStackedBitmap().SaveTIFF32Bitmap(strFile, lpRect, &dlg, bApply, false, Compression);
				else if (dlgOpen.m_ofn.nFilterIndex == 3)
					dssApp->deepStack().GetStackedBitmap().SaveTIFF32Bitmap(strFile, lpRect, &dlg, bApply, true, Compression);
				else if (dlgOpen.m_ofn.nFilterIndex == 4)
					dssApp->deepStack().GetStackedBitmap().SaveFITS16Bitmap(strFile, lpRect, &dlg, bApply);
				else if (dlgOpen.m_ofn.nFilterIndex == 5)
					dssApp->deepStack().GetStackedBitmap().SaveFITS32Bitmap(strFile, lpRect, &dlg, bApply, false);
				else if (dlgOpen.m_ofn.nFilterIndex == 6)
					dssApp->deepStack().GetStackedBitmap().SaveFITS32Bitmap(strFile, lpRect, &dlg, bApply, true);

				TCHAR		szDir[1 + _MAX_DIR];
				TCHAR		szDrive[1 + _MAX_DRIVE];
				TCHAR		szExt[1 + _MAX_EXT];

				_tsplitpath(strFile, szDrive, szDir, nullptr, szExt);
				strBaseDirectory = szDrive;
				strBaseDirectory += szDir;
				strBaseExtension = szExt;

				dwFilterIndex = dlgOpen.m_ofn.nFilterIndex;
				settings.setValue("Folders/SavePictureFolder", QString::fromWCharArray(strBaseDirectory.GetString()));
				settings.setValue("Folders/SavePictureExtension", QString::fromWCharArray(strBaseExtension.GetString()));
				settings.setValue("Folders/SavePictureIndex", (uint)dwFilterIndex);
				settings.setValue("Folders/SaveApplySetting", bApply);
				settings.setValue("Folders/SaveCompression", (uint)Compression);

				EndWaitCursor();

				m_strCurrentFile = strFile;
				UpdateInfos();
				m_bDirty = false;
				bResult = true;
			};
		};
	}
	else
	{
		AfxMessageBox(IDS_MSG_NOPICTURETOSAVE, MB_OK | MB_ICONSTOP);
	};

	return bResult;
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnProcess()
{
	ProcessAndShow(true);
}

/* ------------------------------------------------------------------- */

void CProcessingDlg::ResetSliders()
{
	RGBHistogram& Histogram = dssApp->deepStack().GetOriginalHistogram();

	processParms.m_BezierAdjust.Reset();

	float				RedMarks[3];
	float				GreenMarks[3];
	float				BlueMarks[3];

	m_fGradientOffset = 0.0;
	m_fGradientRange = 65535.0;

	RedMarks[0] = Histogram.GetRedHistogram().GetMin();
	GreenMarks[0] = Histogram.GetGreenHistogram().GetMin();
	BlueMarks[0] = Histogram.GetBlueHistogram().GetMin();

	RedMarks[2] = Histogram.GetRedHistogram().GetMax();
	GreenMarks[2] = Histogram.GetGreenHistogram().GetMax();
	BlueMarks[2] = Histogram.GetBlueHistogram().GetMax();


	CGradient& RedGradient = m_tabRGB.m_RedGradient.GetGradient();
	RedGradient.SetPeg(RedGradient.IndexFromId(0), (float)((RedMarks[0] - m_fGradientOffset) / m_fGradientRange));
	RedGradient.SetPeg(RedGradient.IndexFromId(1), (float)0.5);
	RedGradient.SetPeg(RedGradient.IndexFromId(2), (float)((RedMarks[2] - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_RedGradient.Invalidate(true);
	m_tabRGB.SetRedAdjustMethod(processParms.m_HistoAdjust.GetRedAdjust().GetAdjustMethod());

	CGradient& GreenGradient = m_tabRGB.m_GreenGradient.GetGradient();
	GreenGradient.SetPeg(GreenGradient.IndexFromId(0), (float)((GreenMarks[0] - m_fGradientOffset) / m_fGradientRange));
	GreenGradient.SetPeg(GreenGradient.IndexFromId(1), (float)0.5);
	GreenGradient.SetPeg(GreenGradient.IndexFromId(2), (float)((GreenMarks[2] - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_GreenGradient.Invalidate(true);
	m_tabRGB.SetGreenAdjustMethod(processParms.m_HistoAdjust.GetGreenAdjust().GetAdjustMethod());

	CGradient& BlueGradient = m_tabRGB.m_BlueGradient.GetGradient();
	BlueGradient.SetPeg(BlueGradient.IndexFromId(0), (float)((BlueMarks[0] - m_fGradientOffset) / m_fGradientRange));
	BlueGradient.SetPeg(BlueGradient.IndexFromId(1), (float)0.5);
	BlueGradient.SetPeg(BlueGradient.IndexFromId(2), (float)((BlueMarks[2] - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_BlueGradient.Invalidate(true);
	m_tabRGB.SetBlueAdjustMethod(processParms.m_HistoAdjust.GetBlueAdjust().GetAdjustMethod());

	m_tabLuminance.m_MidTone.SetPos(processParms.m_BezierAdjust.m_fMidtone * 10);
	m_tabLuminance.m_MidAngle.SetPos(processParms.m_BezierAdjust.m_fMidtoneAngle);

	m_tabLuminance.m_DarkAngle.SetPos(processParms.m_BezierAdjust.m_fDarknessAngle);
	m_tabLuminance.m_DarkPower.SetPos(processParms.m_BezierAdjust.m_fDarknessPower * 10);

	m_tabLuminance.m_HighAngle.SetPos(processParms.m_BezierAdjust.m_fHighlightAngle);
	m_tabLuminance.m_HighPower.SetPos(processParms.m_BezierAdjust.m_fHighlightPower * 10);

	m_tabLuminance.UpdateTexts();

	m_tabSaturation.m_Saturation.SetPos(processParms.m_BezierAdjust.m_fSaturationShift + 50);
	m_tabSaturation.UpdateTexts();

	ShowOriginalHistogram(false);
};

/* ------------------------------------------------------------------- */

class CColorOrder
{
public:
	COLORREF		m_crColor;
	int			m_lSize;

private:
	void	CopyFrom(const CColorOrder& co)
	{
		m_crColor = co.m_crColor;
		m_lSize = co.m_lSize;
	};

public:
	CColorOrder()
	{
		m_crColor = 0;
		m_lSize = 0;
	}
	CColorOrder(COLORREF crColor, int lSize)
	{
		m_crColor = crColor;
		m_lSize = lSize;
	};
	virtual ~CColorOrder() {};
	CColorOrder(const CColorOrder& co)
	{
		CopyFrom(co);
	};

	CColorOrder& operator = (const CColorOrder& co)
	{
		CopyFrom(co);
		return *this;
	};

	bool operator < (const CColorOrder& co) const
	{
		return m_lSize < co.m_lSize;
	};
};

void CProcessingDlg::DrawHistoBar(Graphics* pGraphics, int lNrReds, int lNrGreens, int lNrBlues, int X, int lHeight)
{
	std::vector<CColorOrder>	vColors;
	int						lLastHeight = 0;

	vColors.emplace_back(RGB(255, 0, 0), lNrReds);
	vColors.emplace_back(RGB(0, 255, 0), lNrGreens);
	vColors.emplace_back(RGB(0, 0, 255), lNrBlues);

	std::sort(vColors.begin(), vColors.end());

	for (int i = 0; i < vColors.size(); i++)
	{
		if (vColors[i].m_lSize > lLastHeight)
		{
			// Create a color from the remaining values
			double			fRed, fGreen, fBlue;
			int			lNrColors = 1;

			fRed = GetRValue(vColors[i].m_crColor);
			fGreen = GetGValue(vColors[i].m_crColor);
			fBlue = GetBValue(vColors[i].m_crColor);

			for (int j = i + 1; j < vColors.size(); j++)
			{
				fRed += GetRValue(vColors[j].m_crColor);
				fGreen += GetGValue(vColors[j].m_crColor);
				fBlue += GetBValue(vColors[j].m_crColor);
				lNrColors++;
			};

			Pen				ColorPen(Color(fRed / lNrColors, fGreen / lNrColors, fBlue / lNrColors));

			pGraphics->DrawLine(&ColorPen, X, lHeight - lLastHeight, X, lHeight - vColors[i].m_lSize);
			/*
						HPEN			hPen,
										hOldPen;

						hPen = ::CreatePen(PS_SOLID, 1, RGB(fRed/lNrColors, fGreen/lNrColors, fBlue/lNrColors));
						hOldPen = (HPEN)::SelectObject(hDC, hPen);
						::MoveToEx(hDC, X, lHeight-lLastHeight, nullptr);
						::LineTo(hDC, X, lHeight-vColors[i].m_lSize);
						::SelectObject(hDC, hOldPen);
						::DeleteObject(hPen);*/

			lLastHeight = vColors[i].m_lSize;
		};
	};
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::DrawBezierCurve(Graphics* pGraphics, int lWidth, int lHeight)
{
	CBezierAdjust		BezierAdjust;
	POINT				pt;

	BezierAdjust.m_fMidtone = m_tabLuminance.m_MidTone.GetPos() / 10.0;
	BezierAdjust.m_fMidtoneAngle = m_tabLuminance.m_MidAngle.GetPos();
	BezierAdjust.m_fDarknessAngle = m_tabLuminance.m_DarkAngle.GetPos();
	BezierAdjust.m_fHighlightAngle = m_tabLuminance.m_HighAngle.GetPos();
	BezierAdjust.m_fHighlightPower = m_tabLuminance.m_HighPower.GetPos() / 10.0;
	BezierAdjust.m_fDarknessPower = m_tabLuminance.m_DarkPower.GetPos() / 10.0;

	BezierAdjust.Clear();

	Pen					BlackPen(Color(0, 0, 0));
	std::vector<PointF>	vPoints;

	BlackPen.SetDashStyle(DashStyleDash);

	for (double i = 0; i <= 1.0; i += 0.01)
	{
		double	j;

		j = BezierAdjust.GetValue(i);
		pt.x = i * lWidth;
		pt.y = lHeight - j * lHeight;
		vPoints.emplace_back(pt.x, pt.y);
	};

	pGraphics->DrawLines(&BlackPen, &vPoints[0], (int)vPoints.size());
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::DrawGaussCurves(Graphics* pGraphics, RGBHistogram& Histogram, int lWidth, int lHeight)
{
	int				lNrValues;
	double				fAverage[3] = { 0, 0, 0 };
	double				fStdDev[3] = { 0, 0, 0 };
	double				fSum[3] = { 0, 0, 0 };
	double				fSquareSum[3] = { 0, 0, 0 };
	double				fTotalPixels[3] = { 0, 0, 0 };
	int				i;

	lNrValues = Histogram.GetRedHistogram().GetNrValues();

	if (lNrValues)
	{
		for (i = 0; i < lNrValues; i++)
		{
			int			lNrReds;
			int			lNrGreens;
			int			lNrBlues;

			Histogram.GetValues(i, lNrReds, lNrGreens, lNrBlues);

			fSum[0] += lNrReds * i;
			fSum[1] += lNrGreens * i;
			fSum[2] += lNrBlues * i;
			fTotalPixels[0] += lNrReds;
			fTotalPixels[1] += lNrGreens;
			fTotalPixels[2] += lNrBlues;
		};

		fAverage[0] = fSum[0] / fTotalPixels[0];
		fAverage[1] = fSum[1] / fTotalPixels[1];
		fAverage[2] = fSum[2] / fTotalPixels[2];

		for (i = 0; i < lNrValues; i++)
		{
			int			lNrReds;
			int			lNrGreens;
			int			lNrBlues;

			Histogram.GetValues(i, lNrReds, lNrGreens, lNrBlues);

			fSquareSum[0] += pow(i - fAverage[0], 2) * lNrReds;
			fSquareSum[1] += pow(i - fAverage[1], 2) * lNrGreens;
			fSquareSum[2] += pow(i - fAverage[2], 2) * lNrBlues;
		};

		fStdDev[0] = sqrt(fSquareSum[0] / fTotalPixels[0]);
		fStdDev[1] = sqrt(fSquareSum[1] / fTotalPixels[1]);
		fStdDev[2] = sqrt(fSquareSum[2] / fTotalPixels[2]);


		std::vector<PointF>	vReds;
		std::vector<PointF>	vGreens;
		std::vector<PointF>	vBlues;

		bool				bShow = true;

		for (i = 0; i < lNrValues; i++)
		{
			double		fX,
				fY;
			fX = i;

			fY = exp(-(fX - fAverage[0]) * (fX - fAverage[0]) / (fStdDev[0] * fStdDev[0] * 2)) * lWidth / lNrValues;
			fY = lHeight - fY * lHeight;
			vReds.emplace_back(fX, fY);

			bShow = bShow && (fX < 1000 && fY < 1000);

			fY = exp(-(fX - fAverage[1]) * (fX - fAverage[1]) / (fStdDev[1] * fStdDev[1] * 2)) * lWidth / lNrValues;
			fY = lHeight - fY * lHeight;
			vGreens.emplace_back(fX, fY);

			bShow = bShow && (fX < 1000 && fY < 1000);

			fY = exp(-(fX - fAverage[2]) * (fX - fAverage[2]) / (fStdDev[2] * fStdDev[2] * 2)) * lWidth / lNrValues;
			fY = lHeight - fY * lHeight;
			vBlues.emplace_back(fX, fY);

			bShow = bShow && (fX < 1000 && fY < 1000);
		};

		Pen				RedPen(Color(128, 255, 0, 0));
		Pen				GreenPen(Color(128, 0, 255, 0));
		Pen				BluePen(Color(128, 0, 0, 255));

		RedPen.SetDashStyle(DashStyleDash);
		GreenPen.SetDashStyle(DashStyleDash);
		BluePen.SetDashStyle(DashStyleDash);

		if (bShow)
		{
			pGraphics->DrawLines(&RedPen, &vReds[0], (int)vReds.size());
			pGraphics->DrawLines(&GreenPen, &vGreens[0], (int)vGreens.size());
			pGraphics->DrawLines(&BluePen, &vBlues[0], (int)vBlues.size());
		};
	};
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::ShowHistogram(CWndImage& wndImage, RGBHistogram& Histogram, bool bLog)
{
	CRect				rcClient;
	HBITMAP				hBitmap;
	HBITMAP				hOldBitmap;
	HDC					hScreenDC;
	HDC					hMemDC;
	double				fLog = 0;

	wndImage.GetClientRect(&rcClient);

	hScreenDC = ::GetDC(nullptr);
	hMemDC = ::CreateCompatibleDC(hScreenDC);
	hBitmap = ::CreateCompatibleBitmap(hScreenDC, rcClient.Width(), rcClient.Height());

	hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	::FillRect(hMemDC, &rcClient, (HBRUSH)::GetStockObject(WHITE_BRUSH));

	Graphics* pGraphics = new Graphics(hMemDC);
	if (pGraphics)
	{
		pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
		int				lNrValues;
		int				lMaxValue = 0;

		lMaxValue = max(lMaxValue, Histogram.GetRedHistogram().GetMaximumNrValues());
		lMaxValue = max(lMaxValue, Histogram.GetGreenHistogram().GetMaximumNrValues());
		lMaxValue = max(lMaxValue, Histogram.GetBlueHistogram().GetMaximumNrValues());

		lNrValues = Histogram.GetRedHistogram().GetNrValues();

		if (lNrValues)
		{
			if (bLog)
			{
				if (lMaxValue)
					fLog = exp(log((double)lMaxValue) / rcClient.Height());
				else
					bLog = false;
			};

			for (int i = 0; i < lNrValues; i++)
			{
				int			lNrReds;
				int			lNrGreens;
				int			lNrBlues;

				Histogram.GetValues(i, lNrReds, lNrGreens, lNrBlues);

				if (bLog)
				{
					if (lNrReds)
						lNrReds = log((double)lNrReds) / log(fLog);
					if (lNrGreens)
						lNrGreens = log((double)lNrGreens) / log(fLog);
					if (lNrBlues)
						lNrBlues = log((double)lNrBlues) / log(fLog);
				}
				else
				{
					lNrReds = (double)lNrReds / (double)lMaxValue * rcClient.Height();
					lNrGreens = (double)lNrGreens / (double)lMaxValue * rcClient.Height();
					lNrBlues = (double)lNrBlues / (double)lMaxValue * rcClient.Height();
				};

				DrawHistoBar(pGraphics/*hMemDC*/, lNrReds, lNrGreens, lNrBlues, i, rcClient.Height());
			};

		};

		DrawGaussCurves(pGraphics /*hMemDC*/, Histogram, rcClient.Width(), rcClient.Height());
		DrawBezierCurve(pGraphics /*hMemDC*/, rcClient.Width(), rcClient.Height());
		delete pGraphics;
		::SelectObject(hMemDC, hOldBitmap);
	};

	::DeleteDC(hMemDC);
	::ReleaseDC(nullptr, hScreenDC);

	wndImage.SetImg(hBitmap);
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::UpdateHistogramAdjust()
{
	CGradient& RedGradient = m_tabRGB.m_RedGradient.GetGradient();
	double				fMinRed = m_fGradientOffset + RedGradient.GetPeg(RedGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftRed = (RedGradient.GetPeg(RedGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxRed = m_fGradientOffset + RedGradient.GetPeg(RedGradient.IndexFromId(2)).position * m_fGradientRange;

	CGradient& GreenGradient = m_tabRGB.m_GreenGradient.GetGradient();
	double				fMinGreen = m_fGradientOffset + GreenGradient.GetPeg(GreenGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftGreen = (GreenGradient.GetPeg(GreenGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxGreen = m_fGradientOffset + GreenGradient.GetPeg(GreenGradient.IndexFromId(2)).position * m_fGradientRange;


	CGradient& BlueGradient = m_tabRGB.m_BlueGradient.GetGradient();
	double				fMinBlue = m_fGradientOffset + BlueGradient.GetPeg(BlueGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftBlue = (BlueGradient.GetPeg(BlueGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxBlue = m_fGradientOffset + BlueGradient.GetPeg(BlueGradient.IndexFromId(2)).position * m_fGradientRange;


	processParms.m_HistoAdjust.GetRedAdjust().SetNewValues(fMinRed, fMaxRed, fShiftRed);
	processParms.m_HistoAdjust.GetGreenAdjust().SetNewValues(fMinGreen, fMaxGreen, fShiftGreen);
	processParms.m_HistoAdjust.GetBlueAdjust().SetNewValues(fMinBlue, fMaxBlue, fShiftBlue);

	processParms.m_HistoAdjust.GetRedAdjust().SetAdjustMethod(m_tabRGB.GetRedAdjustMethod());
	processParms.m_HistoAdjust.GetGreenAdjust().SetAdjustMethod(m_tabRGB.GetGreenAdjustMethod());
	processParms.m_HistoAdjust.GetBlueAdjust().SetAdjustMethod(m_tabRGB.GetBlueAdjustMethod());


	// Update gradient adjust values
	double				fAbsMin,
		fAbsMax;
	double				fOffset;
	double				fRange;

	fAbsMin = min(fMinRed, min(fMinGreen, fMinBlue));
	fAbsMax = max(fMaxRed, min(fMaxGreen, fMaxBlue));

	fRange = fAbsMax - fAbsMin;
	if (fRange * 1.10 <= 65535.0)
		fRange *= 1.10;

	fOffset = (fAbsMin + fAbsMax - fRange) / 2.0;
	if (fOffset < 0)
		fOffset = 0.0;

	m_fGradientOffset = fOffset;
	m_fGradientRange = fRange;

	RedGradient.SetPeg(RedGradient.IndexFromId(0), (float)((fMinRed - m_fGradientOffset) / m_fGradientRange));
	RedGradient.SetPeg(RedGradient.IndexFromId(1), (float)(fShiftRed / 2.0 + 0.5));
	RedGradient.SetPeg(RedGradient.IndexFromId(2), (float)((fMaxRed - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_RedGradient.Invalidate(true);

	GreenGradient.SetPeg(GreenGradient.IndexFromId(0), (float)((fMinGreen - m_fGradientOffset) / m_fGradientRange));
	GreenGradient.SetPeg(GreenGradient.IndexFromId(1), (float)(fShiftGreen / 2.0 + 0.5));
	GreenGradient.SetPeg(GreenGradient.IndexFromId(2), (float)((fMaxGreen - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_GreenGradient.Invalidate(true);

	BlueGradient.SetPeg(BlueGradient.IndexFromId(0), (float)((fMinBlue - m_fGradientOffset) / m_fGradientRange));
	BlueGradient.SetPeg(BlueGradient.IndexFromId(1), (float)(fShiftBlue / 2.0 + 0.5));
	BlueGradient.SetPeg(BlueGradient.IndexFromId(2), (float)((fMaxBlue - m_fGradientOffset) / m_fGradientRange));
	m_tabRGB.m_BlueGradient.Invalidate(true);
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::ShowOriginalHistogram(bool bLog)
{
	// Adjust Histogram
	RGBHistogram			Histo;
	RGBHistogramAdjust		HistoAdjust;
	CRect					rcClient;

	m_OriginalHistogram.GetClientRect(&rcClient);

	Histo.SetSize(65535.0, (int)rcClient.Width());

	CGradient& RedGradient = m_tabRGB.m_RedGradient.GetGradient();
	double				fMinRed = m_fGradientOffset + RedGradient.GetPeg(RedGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftRed = (RedGradient.GetPeg(RedGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxRed = m_fGradientOffset + RedGradient.GetPeg(RedGradient.IndexFromId(2)).position * m_fGradientRange;

	CGradient& GreenGradient = m_tabRGB.m_GreenGradient.GetGradient();
	double				fMinGreen = m_fGradientOffset + GreenGradient.GetPeg(GreenGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftGreen = (GreenGradient.GetPeg(GreenGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxGreen = m_fGradientOffset + GreenGradient.GetPeg(GreenGradient.IndexFromId(2)).position * m_fGradientRange;

	CGradient& BlueGradient = m_tabRGB.m_BlueGradient.GetGradient();
	double				fMinBlue = m_fGradientOffset + BlueGradient.GetPeg(BlueGradient.IndexFromId(0)).position * m_fGradientRange,
		fShiftBlue = (BlueGradient.GetPeg(BlueGradient.IndexFromId(1)).position - 0.5) * 2.0,
		fMaxBlue = m_fGradientOffset + BlueGradient.GetPeg(BlueGradient.IndexFromId(2)).position * m_fGradientRange;


	HistoAdjust.GetRedAdjust().SetAdjustMethod(m_tabRGB.GetRedAdjustMethod());
	HistoAdjust.GetRedAdjust().SetNewValues(fMinRed, fMaxRed, fShiftRed);
	HistoAdjust.GetGreenAdjust().SetAdjustMethod(m_tabRGB.GetGreenAdjustMethod());
	HistoAdjust.GetGreenAdjust().SetNewValues(fMinGreen, fMaxGreen, fShiftGreen);
	HistoAdjust.GetBlueAdjust().SetAdjustMethod(m_tabRGB.GetBlueAdjustMethod());
	HistoAdjust.GetBlueAdjust().SetNewValues(fMinBlue, fMaxBlue, fShiftBlue);

	dssApp->deepStack().AdjustOriginalHistogram(Histo, HistoAdjust);

	ShowHistogram(m_OriginalHistogram, Histo, bLog);
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect			rcCell;

	if (m_ToProcess.GetNextUnProcessedRect(rcCell))
	{
		HBITMAP			hBitmap = nullptr;
		bool			bInitialized = true;

		hBitmap = m_Picture.GetBitmap();
		if (!hBitmap)
			bInitialized = false;

		hBitmap = dssApp->deepStack().PartialProcess(rcCell, processParms.m_BezierAdjust, processParms.m_HistoAdjust);

		if (!bInitialized)
			m_Picture.SetImg(hBitmap, true);

		m_Picture.Invalidate(true);

		if (!m_OriginalHistogram.GetBitmap())
		{
			ShowOriginalHistogram(false);
			ResetSliders();
		};
		const int nProgress = static_cast<int>(m_ToProcess.GetPercentageComplete());
		m_ProcessingProgress.SetPos(min(max(0, nProgress), 100));
	};

	CDialog::OnTimer(nIDEvent);
}

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnReset()
{
	m_bDirty = true;
	ResetSliders();
}

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnNotifyRedChangeSelPeg(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyRedPegMove(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyRedPegMoved(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyGreenChangeSelPeg(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyGreenPegMove(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyGreenPegMoved(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyBlueChangeSelPeg(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyBluePegMove(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

void CProcessingDlg::OnNotifyBluePegMoved(NMHDR*, LRESULT*)
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::UpdateBezierCurve()
{
	m_bDirty = true;
	ShowOriginalHistogram();
};

/* ------------------------------------------------------------------- */

void CProcessingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_bDirty = true;
	ShowOriginalHistogram();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/* ------------------------------------------------------------------- */
#endif
