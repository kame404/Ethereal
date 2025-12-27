// Copyright (c) 2025 kame404

#ifndef __PIEtherealFilter__
#define __PIEtherealFilter__

#include <cmath>
#include <algorithm>

namespace PIEtherealFilter
{
	inline float saturate(float x)
	{
		return std::max(0.0f, std::min(1.0f, x));
	}

	inline float luminance(float r, float g, float b)
	{
		return 0.2126f * r + 0.7152f * g + 0.0722f * b;
	}

	inline void ApplyLookDev(
		unsigned char& r,
		unsigned char& g,
		unsigned char& b,
		float exposure,
		float whiteLift,
		float satCompress,
		float redSuppress,
		float midContrast,
		float coolBias,
		float highlightAdjust,
		float colorTemp,
		float vividBoost
	)
	{
		float rf = r / 255.0f;
		float gf = g / 255.0f;
		float bf = b / 255.0f;

		float exposureMult = std::pow(2.0f, exposure);
		rf *= exposureMult;
		gf *= exposureMult;
		bf *= exposureMult;

		float lum = luminance(rf, gf, bf);

		float whiteMask = saturate((lum - 0.6f) / 0.4f);
		float whiteAmount = whiteLift * whiteMask;
		rf = rf * (1.0f - whiteAmount) + whiteAmount;
		gf = gf * (1.0f - whiteAmount) + whiteAmount;
		bf = bf * (1.0f - whiteAmount) + whiteAmount;

		lum = luminance(rf, gf, bf);

		float satR = rf - lum;
		float satG = gf - lum;
		float satB = bf - lum;

		float satFactor = 1.0f - satCompress * lum;
		rf = lum + satR * satFactor;
		gf = lum + satG * satFactor;
		bf = lum + satB * satFactor;

		if (vividBoost > 0.0f) {
			satR = rf - lum;
			satG = gf - lum;
			satB = bf - lum;

			float currentSat = std::sqrt(satR * satR + satG * satG + satB * satB);
			float satMask = saturate(currentSat / 0.6f);
			float boostAmount = 1.0f + vividBoost * satMask * 0.8f;
			rf = lum + satR * boostAmount;
			gf = lum + satG * boostAmount;
			bf = lum + satB * boostAmount;
		}

		rf *= 1.0f - redSuppress * whiteMask * 0.1f;

		float mid = (lum - 0.5f) * midContrast * 0.3f;
		rf += mid;
		gf += mid;
		bf += mid;

		float highlightBoost = highlightAdjust * whiteMask * 0.3f;
		rf += highlightBoost;
		gf += highlightBoost;
		bf += highlightBoost;

		bf *= 1.0f + coolBias * 0.05f;
		rf *= 1.0f - coolBias * 0.05f;

		if (colorTemp > 0.0f) {
			bf *= 1.0f + colorTemp * 0.15f;
			rf *= 1.0f - colorTemp * 0.1f;
		} else if (colorTemp < 0.0f) {
			rf *= 1.0f - colorTemp * 0.15f;
			bf *= 1.0f + colorTemp * 0.1f;
		}

		r = static_cast<unsigned char>(saturate(rf) * 255.0f);
		g = static_cast<unsigned char>(saturate(gf) * 255.0f);
		b = static_cast<unsigned char>(saturate(bf) * 255.0f);
	}
}

#endif
