#include "dss_common.h"

#include "DeepSkyStacker.h"
#include "ProcessingDlg.h"
#include "progressdlg.h"

namespace DSS
{

	void ProcessingDlg::setButtonIcons()
	{
		setRedButtonIcon();
		setGreenButtonIcon();
		setBlueButtonIcon();
	}

	//
	// Slots
	//
	void ProcessingDlg::setRedButtonIcon()
	{
		const QString curveName = iconNames[static_cast<qsizetype>(redAdjustmentCurve_)];
		QString iconName = QString{ ":/processing/%1%2.png" }.arg(curveName).arg(iconModifier);
		redHAC->setIcon(QIcon(iconName));
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::setGreenButtonIcon()
	{
		const QString curveName = iconNames[static_cast<qsizetype>(greenAdjustmentCurve_)];
		QString iconName = QString{ ":/processing/%1%2.png" }.arg(curveName).arg(iconModifier);
		greenHAC->setIcon(QIcon(iconName));
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::setBlueButtonIcon()
	{
		const QString curveName = iconNames[static_cast<qsizetype>(blueAdjustmentCurve_)];
		QString iconName = QString{ ":/processing/%1%2.png" }.arg(curveName).arg(iconModifier);
		blueHAC->setIcon(QIcon(iconName));
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::redChanging(int peg)
	{
		double blackPoint{ 0.0 },
			greyPoint{ 0.0 },
			whitePoint{ 0.0 };

		QLinearGradient& gradient{ redGradient->gradient() };
		QGradientStops stops{ gradient.stops() };

		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		blackPoint = stops[1].first;
		greyPoint = stops[2].first;
		whitePoint = stops[3].first;
		bool adjust{ false };

		switch (peg)
		{
		case 1:
			// Black point moving
			if (blackPoint > whitePoint - 0.02)
			{
				blackPoint = whitePoint - 0.02;
				adjust = true;
			};
			if (blackPoint > greyPoint - 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			break;
		case 2:
			// Gray point moving
			if (greyPoint < blackPoint + 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			if (greyPoint > whitePoint - 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		case 3:
			// White point moving
			if (whitePoint < blackPoint + 0.02)
			{
				whitePoint = blackPoint + 0.02;
				adjust = true;
			};
			if (whitePoint < greyPoint + 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		};
		if (adjust)
		{
			stops[1].first = blackPoint;
			stops[2].first = greyPoint;
			stops[3].first = whitePoint;
			gradient.setStops(stops);
			redGradient->update();
		};

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::redChanged(int peg)
	{
		//
		// Before applying the changes, make any corrections necessary by invoking gammaChanging 
		// one final time
		//
		redChanging(peg);

		QLinearGradient& gradient{ redGradient->gradient() };
		QGradientStops stops{ gradient.stops() };
		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		//
		// If the settings are linked, the green and blue pegs also need to move
		// 
		if (linkedSettings->isChecked())
		{
			greenGradient->setPeg(peg, stops[peg].first);
			blueGradient->setPeg(peg, stops[peg].first);
		}

		qDebug() << "red sliders changed";

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::greenChanging(int peg)
	{
		double blackPoint{ 0.0 },
			greyPoint{ 0.0 },
			whitePoint{ 0.0 };

		QLinearGradient& gradient{ greenGradient->gradient() };
		QGradientStops stops{ gradient.stops() };

		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		blackPoint = stops[1].first;
		greyPoint = stops[2].first;
		whitePoint = stops[3].first;
		bool adjust{ false };

		switch (peg)
		{
		case 1:
			// Black point moving
			if (blackPoint > whitePoint - 0.02)
			{
				blackPoint = whitePoint - 0.02;
				adjust = true;
			};
			if (blackPoint > greyPoint - 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			break;
		case 2:
			// Gray point moving
			if (greyPoint < blackPoint + 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			if (greyPoint > whitePoint - 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		case 3:
			// White point moving
			if (whitePoint < blackPoint + 0.02)
			{
				whitePoint = blackPoint + 0.02;
				adjust = true;
			};
			if (whitePoint < greyPoint + 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		};
		if (adjust)
		{
			stops[1].first = blackPoint;
			stops[2].first = greyPoint;
			stops[3].first = whitePoint;
			gradient.setStops(stops);
			greenGradient->update();
		}

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::greenChanged(int peg)
	{
		//
		// Before applying the changes, make any corrections necessary by invoking gammaChanging 
		// one final time
		//
		greenChanging(peg);

		QLinearGradient& gradient{ greenGradient->gradient() };
		QGradientStops stops{ gradient.stops() };
		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		//
		// If the settings are linked, the red and blue pegs also need to move
		// 
		if (linkedSettings->isChecked())
		{
			redGradient->setPeg(peg, stops[peg].first); redGradient->update();
			blueGradient->setPeg(peg, stops[peg].first); blueGradient->update();
		}

		qDebug() << "green sliders changed";

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::blueChanging(int peg)
	{
		double blackPoint{ 0.0 },
			greyPoint{ 0.0 },
			whitePoint{ 0.0 };

		QLinearGradient& gradient{ blueGradient->gradient() };
		QGradientStops stops{ gradient.stops() };

		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		blackPoint = stops[1].first;
		greyPoint = stops[2].first;
		whitePoint = stops[3].first;
		bool adjust{ false };

		switch (peg)
		{
		case 1:
			// Black point moving
			if (blackPoint > whitePoint - 0.02)
			{
				blackPoint = whitePoint - 0.02;
				adjust = true;
			};
			if (blackPoint > greyPoint - 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			break;
		case 2:
			// Gray point moving
			if (greyPoint < blackPoint + 0.01)
			{
				greyPoint = blackPoint + 0.01;
				adjust = true;
			};
			if (greyPoint > whitePoint - 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		case 3:
			// White point moving
			if (whitePoint < blackPoint + 0.02)
			{
				whitePoint = blackPoint + 0.02;
				adjust = true;
			};
			if (whitePoint < greyPoint + 0.01)
			{
				greyPoint = whitePoint - 0.01;
				adjust = true;
			};
			break;
		};
		if (adjust)
		{
			stops[1].first = blackPoint;
			stops[2].first = greyPoint;
			stops[3].first = whitePoint;
			gradient.setStops(stops);
			blueGradient->update();
		}

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::blueChanged(int peg)
	{
		//
		// Before applying the changes, make any corrections necessary by invoking gammaChanging 
		// one final time
		//
		blueChanging(peg);

		QLinearGradient& gradient{ blueGradient->gradient() };
		QGradientStops stops{ gradient.stops() };
		//
		// Adjust stop values if necessary
		//
		ZASSERT(5 == stops.size());

		//
		// If the settings are linked, the red and green pegs also need to move
		// 
		if (linkedSettings->isChecked())
		{
			redGradient->setPeg(peg, stops[peg].first);
			greenGradient->setPeg(peg, stops[peg].first);
		}

		qDebug() << "blue sliders changed";

	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::redButtonPressed()
	{
		HistogramAdjustmentCurve hac{ HistogramAdjustmentCurve::Linear };
		const QAction* a = hacMenu.exec(QCursor::pos());
		if (a == linearAction) hac = HistogramAdjustmentCurve::Linear;
		if (a == cubeRootAction) hac = HistogramAdjustmentCurve::CubeRoot;
		if (a == squareRootAction) hac = HistogramAdjustmentCurve::SquareRoot;
		if (a == logAction) hac = HistogramAdjustmentCurve::Log;
		if (a == logLogAction) hac = HistogramAdjustmentCurve::LogLog;
		if (a == logSquareRootAction) hac = HistogramAdjustmentCurve::LogSquareRoot;
		if (a == asinHAction) hac = HistogramAdjustmentCurve::ASinH;

		redAdjustmentCurve_ = hac;
		setRedButtonIcon();
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::greenButtonPressed()
	{
		HistogramAdjustmentCurve hac{ HistogramAdjustmentCurve::Linear };
		const QAction* a = hacMenu.exec(QCursor::pos());
		if (a == linearAction) hac = HistogramAdjustmentCurve::Linear;
		if (a == cubeRootAction) hac = HistogramAdjustmentCurve::CubeRoot;
		if (a == squareRootAction) hac = HistogramAdjustmentCurve::SquareRoot;
		if (a == logAction) hac = HistogramAdjustmentCurve::Log;
		if (a == logLogAction) hac = HistogramAdjustmentCurve::LogLog;
		if (a == logSquareRootAction) hac = HistogramAdjustmentCurve::LogSquareRoot;
		if (a == asinHAction) hac = HistogramAdjustmentCurve::ASinH;

		greenAdjustmentCurve_ = hac;
		setGreenButtonIcon();
	}

	/* ------------------------------------------------------------------- */

	void ProcessingDlg::blueButtonPressed()
	{
		HistogramAdjustmentCurve hac{ HistogramAdjustmentCurve::Linear };
		const QAction* a = hacMenu.exec(QCursor::pos());
		if (a == linearAction) hac = HistogramAdjustmentCurve::Linear;
		if (a == cubeRootAction) hac = HistogramAdjustmentCurve::CubeRoot;
		if (a == squareRootAction) hac = HistogramAdjustmentCurve::SquareRoot;
		if (a == logAction) hac = HistogramAdjustmentCurve::Log;
		if (a == logLogAction) hac = HistogramAdjustmentCurve::LogLog;
		if (a == logSquareRootAction) hac = HistogramAdjustmentCurve::LogSquareRoot;
		if (a == asinHAction) hac = HistogramAdjustmentCurve::ASinH;

		blueAdjustmentCurve_ = hac;
		setBlueButtonIcon();
	}
} // namespace DSS

