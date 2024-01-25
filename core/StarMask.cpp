#include "dss_common.h"

#include "StarMask.h"
#include "DSSProgress.h"
#include "RegisterEngine.h"
#include "GrayBitmap.h"

using namespace DSS;

// Can't be const due to RegisterPicture(pBitmap).
std::shared_ptr<CMemoryBitmap> CStarMaskEngine::CreateStarMask2(CMemoryBitmap* pBitmap, ProgressBase* pProgress)
{
	CLightFrameInfo LightFrame;

	LightFrame.SetDetectionThreshold(m_fMinLuminancy);
	LightFrame.SetHotPixelRemoval(m_bRemoveHotPixels);
	LightFrame.SetRoundnessTolerance(2.0);

	LightFrame.SetProgress(pProgress);
	LightFrame.RegisterPicture(pBitmap);

	const STARVECTOR vStars = LightFrame.GetStars();

	// Draw the stars
	std::shared_ptr<C16BitGrayBitmap> pOutBitmap = std::make_shared<C16BitGrayBitmap>();
	std::unique_ptr<CStarMaskFunction> pStarMaskFunction = GetShapeFunction();
	if (static_cast<bool>(pOutBitmap))
	{
		pOutBitmap->Init(pBitmap->Width(), pBitmap->Height());
		const double fWidth	= pBitmap->Width();
		const double fHeight = pBitmap->Height();

		if (pProgress != nullptr)
		{
			const QString strText(QCoreApplication::translate("StarMask", "Creating Star Mask...", "IDS_CREATINGSTARMASK"));
			pProgress->Start2(strText, static_cast<int>(vStars.size()));
		}

		for (size_t k = 0; k < vStars.size(); k++)
		{
			double fRadius = vStars[k].m_fMeanRadius * (2.35 / 1.5);
			const double fXCenter = vStars[k].m_fX;
			const double fYCenter = vStars[k].m_fY;

			if (2 * fRadius >= m_fMinSize && 2 * fRadius <= m_fMaxSize)
			{
				//double		fFactor1 = 1.0/exp(-0.5);
				//double		fFactor2 = 2*fRadius*fRadius;

				fRadius *= m_fPercentIncrease;
				if (m_fPixelIncrease)
					fRadius += m_fPixelIncrease;

				pStarMaskFunction->SetRadius(fRadius);
				for (double i = std::max(0.0, fXCenter - 3*fRadius);i<=std::min(fXCenter + 3*fRadius, fWidth-1);i++)
				{
					for (double j = std::max(0.0, fYCenter - 3*fRadius);j<=std::min(fYCenter + 3*fRadius, fHeight-1);j++)
					{
						// Compute the distance to the center
						double		fDistance;
						double		fXDistance = fabs(i-fXCenter);
						double		fYDistance = fabs(j-fYCenter);
						double		fPixelValue;
						double		fOldPixelValue;

						fDistance = sqrt(fXDistance * fXDistance + fYDistance * fYDistance);

						fPixelValue = pStarMaskFunction->Compute(fDistance);

						pOutBitmap->GetPixel(i+0.5, j+0.5, fOldPixelValue);
						fPixelValue = std::max(fOldPixelValue, std::max(0.0, std::min(fPixelValue*255.0, 255.0)));
						pOutBitmap->SetPixel(i+0.5, j+0.5, fPixelValue);
					}
				}
			}
			if (pProgress != nullptr)
				pProgress->Progress2(static_cast<int>(k+1));
		}
		if (pProgress != nullptr)
			pProgress->End2();
	}

	return pOutBitmap;
}
