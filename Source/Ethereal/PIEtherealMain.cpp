// TriglavPlugIn SDK - Copyright (c) CELSYS Inc.
// Modified by kame404 (2025)

#include "TriglavPlugInSDK/TriglavPlugInSDK.h"
#include "PlugInCommon/PIEtherealFilter.h"
#include <vector>

typedef	unsigned char	BYTE;

static	const int kItemKeyExposure			= 1;
static	const int kItemKeyWhiteLift			= 2;
static	const int kItemKeySatCompress		= 3;
static	const int kItemKeyRedSuppress		= 4;
static	const int kItemKeyMidContrast		= 5;
static	const int kItemKeyCoolBias			= 6;
static	const int kItemKeyHighlightAdjust	= 7;
static	const int kItemKeyColorTemp			= 8;
static	const int kItemKeyVividBoost		= 9;

static	const int kStringIDFilterCategoryName		= 101;
static	const int kStringIDFilterName				= 102;
static	const int kStringIDItemCaptionExposure		= 103;
static	const int kStringIDItemCaptionWhiteLift		= 104;
static	const int kStringIDItemCaptionSatCompress	= 105;
static	const int kStringIDItemCaptionRedSuppress	= 106;
static	const int kStringIDItemCaptionMidContrast	= 107;
static	const int kStringIDItemCaptionCoolBias		= 108;
static	const int kStringIDItemCaptionHighlightAdjust	= 109;
static	const int kStringIDItemCaptionColorTemp		= 110;
static	const int kStringIDItemCaptionVividBoost	= 111;

struct	LookDevFilterInfo
{
	int		exposure;
	int		whiteLift;
	int		satCompress;
	int		redSuppress;
	int		midContrast;
	int		coolBias;
	int		highlightAdjust;
	int		colorTemp;
	int		vividBoost;

	TriglavPlugInPropertyService*	pPropertyService;
};

// Property callback
static void TRIGLAV_PLUGIN_CALLBACK TriglavPlugInFilterPropertyCallBack( TriglavPlugInInt* result, TriglavPlugInPropertyObject propertyObject, const TriglavPlugInInt itemKey, const TriglavPlugInInt notify, TriglavPlugInPtr data )
{
	(*result)	= kTriglavPlugInPropertyCallBackResultNoModify;

	LookDevFilterInfo*	pFilterInfo	= static_cast<LookDevFilterInfo*>(data);
	if( pFilterInfo != NULL )
	{
		TriglavPlugInPropertyService*	pPropertyService	= (*pFilterInfo).pPropertyService;
		if( pPropertyService != NULL )
		{
			if( notify == kTriglavPlugInPropertyCallBackNotifyValueChanged )
			{
				TriglavPlugInInt	value;
				(*(*pFilterInfo).pPropertyService).getIntegerValueProc(&value,propertyObject,itemKey);

				bool changed = false;

				if( itemKey == kItemKeyExposure )
				{
					if( (*pFilterInfo).exposure != value )
					{
						(*pFilterInfo).exposure	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyWhiteLift )
				{
					if( (*pFilterInfo).whiteLift != value )
					{
						(*pFilterInfo).whiteLift	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeySatCompress )
				{
					if( (*pFilterInfo).satCompress != value )
					{
						(*pFilterInfo).satCompress	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyRedSuppress )
				{
					if( (*pFilterInfo).redSuppress != value )
					{
						(*pFilterInfo).redSuppress	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyMidContrast )
				{
					if( (*pFilterInfo).midContrast != value )
					{
						(*pFilterInfo).midContrast	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyCoolBias )
				{
					if( (*pFilterInfo).coolBias != value )
					{
						(*pFilterInfo).coolBias	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyHighlightAdjust )
				{
					if( (*pFilterInfo).highlightAdjust != value )
					{
						(*pFilterInfo).highlightAdjust	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyColorTemp )
				{
					if( (*pFilterInfo).colorTemp != value )
					{
						(*pFilterInfo).colorTemp	= value;
						changed = true;
					}
				}
				else if( itemKey == kItemKeyVividBoost )
				{
					if( (*pFilterInfo).vividBoost != value )
					{
						(*pFilterInfo).vividBoost	= value;
						changed = true;
					}
				}

				if (changed)
				{
					(*result)	= kTriglavPlugInPropertyCallBackResultModify;
				}
			}
		}
	}
}

// Plugin main entry point
void TRIGLAV_PLUGIN_API TriglavPluginCall( TriglavPlugInInt* result, TriglavPlugInPtr* data, TriglavPlugInInt selector, TriglavPlugInServer* pluginServer, TriglavPlugInPtr reserved )
{
	*result	= kTriglavPlugInCallResultFailed;
	if( pluginServer != NULL )
	{
		if( selector == kTriglavPlugInSelectorModuleInitialize )
		{
			// Module initialization
			TriglavPlugInModuleInitializeRecord*	pModuleInitializeRecord	= (*pluginServer).recordSuite.moduleInitializeRecord;
			TriglavPlugInStringService*				pStringService			= (*pluginServer).serviceSuite.stringService;
			if( pModuleInitializeRecord != NULL && pStringService != NULL )
			{
				TriglavPlugInInt	hostVersion;
				(*pModuleInitializeRecord).getHostVersionProc(&hostVersion,(*pluginServer).hostObject);
				if( hostVersion >= kTriglavPlugInNeedHostVersion )
				{
					TriglavPlugInStringObject	moduleID	= NULL;
					const char*					moduleIDString	= "B7FF1903-95F8-4C59-A8E9-0FBBD027FFD5";
					(*pStringService).createWithAsciiStringProc(&moduleID,moduleIDString,static_cast<TriglavPlugInInt>(::strlen(moduleIDString)));
					(*pModuleInitializeRecord).setModuleIDProc((*pluginServer).hostObject,moduleID);
					(*pModuleInitializeRecord).setModuleKindProc((*pluginServer).hostObject,kTriglavPlugInModuleSwitchKindFilter);
					(*pStringService).releaseProc(moduleID);

					LookDevFilterInfo*	pFilterInfo	= new LookDevFilterInfo;
					*data	= pFilterInfo;
					*result	= kTriglavPlugInCallResultSuccess;
				}
			}
		}
		else if( selector == kTriglavPlugInSelectorModuleTerminate )
		{
			// Module termination
			LookDevFilterInfo*	pFilterInfo	= static_cast<LookDevFilterInfo*>(*data);
			delete pFilterInfo;
			*data	= NULL;
			*result	= kTriglavPlugInCallResultSuccess;
		}
		else if( selector == kTriglavPlugInSelectorFilterInitialize )
		{
			// Filter initialization
			TriglavPlugInRecordSuite*				pRecordSuite			= &(*pluginServer).recordSuite;
			TriglavPlugInHostObject					hostObject				= (*pluginServer).hostObject;
			TriglavPlugInStringService*				pStringService			= (*pluginServer).serviceSuite.stringService;
			TriglavPlugInPropertyService*			pPropertyService		= (*pluginServer).serviceSuite.propertyService;
			if( TriglavPlugInGetFilterInitializeRecord(pRecordSuite) != NULL && pStringService != NULL && pPropertyService != NULL )
			{
				// Set filter category and name
				TriglavPlugInStringObject	filterCategoryName	= NULL;
				TriglavPlugInStringObject	filterName			= NULL;
				(*pStringService).createWithStringIDProc(&filterCategoryName,kStringIDFilterCategoryName,(*pluginServer).hostObject);
				(*pStringService).createWithStringIDProc(&filterName,kStringIDFilterName,(*pluginServer).hostObject);

				TriglavPlugInFilterInitializeSetFilterCategoryName(pRecordSuite,hostObject,filterCategoryName,'k');
				TriglavPlugInFilterInitializeSetFilterName(pRecordSuite,hostObject,filterName,'s');
				(*pStringService).releaseProc(filterCategoryName);
				(*pStringService).releaseProc(filterName);

				// Preview
				TriglavPlugInFilterInitializeSetCanPreview(pRecordSuite,hostObject,true);

				// Target
				TriglavPlugInInt	target[]={kTriglavPlugInFilterTargetKindRasterLayerRGBAlpha};
				TriglavPlugInFilterInitializeSetTargetKinds(pRecordSuite,hostObject,target,1);

				// Create property
				TriglavPlugInPropertyObject	propertyObject;
				(*pPropertyService).createProc(&propertyObject);

				TriglavPlugInStringObject	satCompressCaption	= NULL;
				(*pStringService).createWithStringIDProc(&satCompressCaption,kStringIDItemCaptionSatCompress,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeySatCompress,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,satCompressCaption,'t');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeySatCompress,80);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeySatCompress,80);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeySatCompress,0);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeySatCompress,300);
				(*pStringService).releaseProc(satCompressCaption);

				TriglavPlugInStringObject	highlightAdjustCaption	= NULL;
				(*pStringService).createWithStringIDProc(&highlightAdjustCaption,kStringIDItemCaptionHighlightAdjust,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyHighlightAdjust,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,highlightAdjustCaption,'a');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyHighlightAdjust,-20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyHighlightAdjust,-20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyHighlightAdjust,-100);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyHighlightAdjust,100);
				(*pStringService).releaseProc(highlightAdjustCaption);

				TriglavPlugInStringObject	exposureCaption	= NULL;
				(*pStringService).createWithStringIDProc(&exposureCaption,kStringIDItemCaptionExposure,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyExposure,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,exposureCaption,'r');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyExposure,30);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyExposure,30);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyExposure,-100);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyExposure,100);
				(*pStringService).releaseProc(exposureCaption);

				TriglavPlugInStringObject	coolBiasCaption	= NULL;
				(*pStringService).createWithStringIDProc(&coolBiasCaption,kStringIDItemCaptionCoolBias,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyCoolBias,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,coolBiasCaption,'k');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyCoolBias,20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyCoolBias,20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyCoolBias,0);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyCoolBias,100);
				(*pStringService).releaseProc(coolBiasCaption);

				TriglavPlugInStringObject	colorTempCaption	= NULL;
				(*pStringService).createWithStringIDProc(&colorTempCaption,kStringIDItemCaptionColorTemp,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyColorTemp,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,colorTempCaption,'i');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyColorTemp,0);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyColorTemp,0);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyColorTemp,-100);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyColorTemp,100);
				(*pStringService).releaseProc(colorTempCaption);

				TriglavPlugInStringObject	whiteLiftCaption	= NULL;
				(*pStringService).createWithStringIDProc(&whiteLiftCaption,kStringIDItemCaptionWhiteLift,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyWhiteLift,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,whiteLiftCaption,'w');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyWhiteLift,20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyWhiteLift,20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyWhiteLift,0);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyWhiteLift,100);
				(*pStringService).releaseProc(whiteLiftCaption);

				TriglavPlugInStringObject	vividBoostCaption	= NULL;
				(*pStringService).createWithStringIDProc(&vividBoostCaption,kStringIDItemCaptionVividBoost,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyVividBoost,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,vividBoostCaption,'v');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyVividBoost,20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyVividBoost,20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyVividBoost,0);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyVividBoost,100);
				(*pStringService).releaseProc(vividBoostCaption);

				TriglavPlugInStringObject	midContrastCaption	= NULL;
				(*pStringService).createWithStringIDProc(&midContrastCaption,kStringIDItemCaptionMidContrast,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyMidContrast,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,midContrastCaption,'c');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyMidContrast,-20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyMidContrast,-20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyMidContrast,-100);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyMidContrast,100);
				(*pStringService).releaseProc(midContrastCaption);

				TriglavPlugInStringObject	redSuppressCaption	= NULL;
				(*pStringService).createWithStringIDProc(&redSuppressCaption,kStringIDItemCaptionRedSuppress,(*pluginServer).hostObject);
				(*pPropertyService).addItemProc(propertyObject,kItemKeyRedSuppress,kTriglavPlugInPropertyValueTypeInteger,kTriglavPlugInPropertyValueKindDefault,kTriglavPlugInPropertyInputKindDefault,redSuppressCaption,'y');
				(*pPropertyService).setIntegerValueProc(propertyObject,kItemKeyRedSuppress,20);
				(*pPropertyService).setIntegerDefaultValueProc(propertyObject,kItemKeyRedSuppress,20);
				(*pPropertyService).setIntegerMinValueProc(propertyObject,kItemKeyRedSuppress,0);
				(*pPropertyService).setIntegerMaxValueProc(propertyObject,kItemKeyRedSuppress,100);
				(*pStringService).releaseProc(redSuppressCaption);

				// Set property
				TriglavPlugInFilterInitializeSetProperty(pRecordSuite,hostObject,propertyObject);
				TriglavPlugInFilterInitializeSetPropertyCallBack(pRecordSuite,hostObject,TriglavPlugInFilterPropertyCallBack,*data);

				// Release property
				(*pPropertyService).releaseProc(propertyObject);

				*result	= kTriglavPlugInCallResultSuccess;
			}
		}
		else if( selector == kTriglavPlugInSelectorFilterTerminate )
		{
			// Filter termination
			*result	= kTriglavPlugInCallResultSuccess;
		}
		else if( selector == kTriglavPlugInSelectorFilterRun )
		{
			// Filter execution
			TriglavPlugInRecordSuite*		pRecordSuite		= &(*pluginServer).recordSuite;
			TriglavPlugInOffscreenService*	pOffscreenService	= (*pluginServer).serviceSuite.offscreenService;
			TriglavPlugInPropertyService*	pPropertyService	= (*pluginServer).serviceSuite.propertyService;
			if( TriglavPlugInGetFilterRunRecord(pRecordSuite) != NULL && pOffscreenService != NULL && pPropertyService != NULL )
			{
				TriglavPlugInPropertyObject		propertyObject;
				TriglavPlugInFilterRunGetProperty(pRecordSuite,&propertyObject,(*pluginServer).hostObject);

				TriglavPlugInOffscreenObject	sourceOffscreenObject;
				TriglavPlugInFilterRunGetSourceOffscreen(pRecordSuite,&sourceOffscreenObject,(*pluginServer).hostObject);

				TriglavPlugInOffscreenObject	destinationOffscreenObject;
				TriglavPlugInFilterRunGetDestinationOffscreen(pRecordSuite,&destinationOffscreenObject,(*pluginServer).hostObject);

				TriglavPlugInRect				selectAreaRect;
				TriglavPlugInFilterRunGetSelectAreaRect(pRecordSuite,&selectAreaRect,(*pluginServer).hostObject);

				TriglavPlugInOffscreenObject	selectAreaOffscreenObject;
				TriglavPlugInFilterRunGetSelectAreaOffscreen(pRecordSuite,&selectAreaOffscreenObject,(*pluginServer).hostObject);

				TriglavPlugInInt	r,g,b;
				(*pOffscreenService).getRGBChannelIndexProc(&r,&g,&b,destinationOffscreenObject);

				TriglavPlugInInt	blockRectCount;
				(*pOffscreenService).getBlockRectCountProc(&blockRectCount,destinationOffscreenObject,&selectAreaRect);

				std::vector<TriglavPlugInRect>	blockRects;
				blockRects.resize(blockRectCount);
				for( TriglavPlugInInt i=0; i<blockRectCount; ++i )
				{
					(*pOffscreenService).getBlockRectProc(&blockRects[i],i,destinationOffscreenObject,&selectAreaRect);
				}

				TriglavPlugInFilterRunSetProgressTotal(pRecordSuite,(*pluginServer).hostObject,blockRectCount);

				LookDevFilterInfo*	pFilterInfo			= static_cast<LookDevFilterInfo*>(*data);
				(*pFilterInfo).pPropertyService		= pPropertyService;
				(*pFilterInfo).exposure				= 0;
				(*pFilterInfo).whiteLift			= 0;
				(*pFilterInfo).satCompress			= 0;
				(*pFilterInfo).redSuppress			= 0;
				(*pFilterInfo).midContrast			= 0;
				(*pFilterInfo).coolBias				= 0;
				(*pFilterInfo).highlightAdjust		= 0;
				(*pFilterInfo).colorTemp			= 0;
				(*pFilterInfo).vividBoost			= 0;

				bool	restart		= true;

				float	exposure		= 0.0f;
				float	whiteLift		= 0.0f;
				float	satCompress		= 0.0f;
				float	redSuppress		= 0.0f;
				float	midContrast		= 0.0f;
				float	coolBias		= 0.0f;
				float	highlightAdjust	= 0.0f;
				float	colorTemp		= 0.0f;
				float	vividBoost		= 0.0f;

				TriglavPlugInInt	blockIndex	= 0;
				while( true )
				{
					if( restart )
					{
						restart	= false;

						TriglavPlugInInt	processResult;
						TriglavPlugInFilterRunProcess(pRecordSuite,&processResult,(*pluginServer).hostObject,kTriglavPlugInFilterRunProcessStateStart);
						if( processResult == kTriglavPlugInFilterRunProcessResultExit ){ break; }

						// Check if any parameter is non-zero
						bool hasEffect = ((*pFilterInfo).exposure != 0 || (*pFilterInfo).whiteLift != 0 ||
						                  (*pFilterInfo).satCompress != 0 || (*pFilterInfo).redSuppress != 0 ||
						                  (*pFilterInfo).midContrast != 0 || (*pFilterInfo).coolBias != 0 ||
						                  (*pFilterInfo).highlightAdjust != 0 || (*pFilterInfo).colorTemp != 0 ||
						                  (*pFilterInfo).vividBoost != 0);

						if( hasEffect )
						{
							blockIndex			= 0;

							// Convert integer parameters to float (0-100 -> 0.0-1.0, etc.)
							exposure	= (*pFilterInfo).exposure / 100.0f;
							whiteLift	= (*pFilterInfo).whiteLift / 100.0f;
							satCompress	= (*pFilterInfo).satCompress / 100.0f;
							redSuppress	= (*pFilterInfo).redSuppress / 100.0f;
							midContrast	= (*pFilterInfo).midContrast / 100.0f;
							coolBias	= (*pFilterInfo).coolBias / 100.0f;
							highlightAdjust	= (*pFilterInfo).highlightAdjust / 100.0f;
							colorTemp	= (*pFilterInfo).colorTemp / 100.0f;
							vividBoost	= (*pFilterInfo).vividBoost / 100.0f;
						}
						else
						{
							// No effect - just update
							blockIndex	= blockRectCount;
							TriglavPlugInFilterRunUpdateDestinationOffscreenRect(pRecordSuite,(*pluginServer).hostObject,&selectAreaRect);
						}
					}

					if( blockIndex < blockRectCount )
					{
						TriglavPlugInFilterRunSetProgressDone(pRecordSuite,(*pluginServer).hostObject,blockIndex);

						TriglavPlugInRect	blockRect	= blockRects[blockIndex];
						TriglavPlugInPoint	pos;
						pos.x	= blockRect.left;
						pos.y	= blockRect.top;
						TriglavPlugInRect	tempRect;

						TriglavPlugInPtr	dstImageAddress;
						TriglavPlugInInt	dstImageRowBytes;
						TriglavPlugInInt	dstImagePixelBytes;
						(*pOffscreenService).getBlockImageProc(&dstImageAddress,&dstImageRowBytes,&dstImagePixelBytes,&tempRect,destinationOffscreenObject,&pos);

						TriglavPlugInPtr	dstAlphaAddress;
						TriglavPlugInInt	dstAlphaRowBytes;
						TriglavPlugInInt	dstAlphaPixelBytes;
						(*pOffscreenService).getBlockAlphaProc(&dstAlphaAddress,&dstAlphaRowBytes,&dstAlphaPixelBytes,&tempRect,destinationOffscreenObject,&pos);

						if( dstImageAddress != NULL && dstAlphaAddress != NULL )
						{
							if( selectAreaOffscreenObject == NULL )
							{
								BYTE*		pDstImageAddressY	= static_cast<BYTE*>(dstImageAddress);
								BYTE*		pDstAlphaAddressY	= static_cast<BYTE*>(dstAlphaAddress);
								for( int y=blockRect.top; y<blockRect.bottom; ++y )
								{
									BYTE*		pDstImageAddressX	= pDstImageAddressY;
									BYTE*		pDstAlphaAddressX	= pDstAlphaAddressY;
									for( int x=blockRect.left; x<blockRect.right; ++x )
									{
										if( *pDstAlphaAddressX > 0 )
										{
											PIEtherealFilter::ApplyLookDev(
												pDstImageAddressX[r],
												pDstImageAddressX[g],
												pDstImageAddressX[b],
												exposure,
												whiteLift,
												satCompress,
												redSuppress,
												midContrast,
												coolBias,
												highlightAdjust,
												colorTemp,
												vividBoost
											);
										}
										pDstImageAddressX	+= dstImagePixelBytes;
										pDstAlphaAddressX	+= dstAlphaPixelBytes;
									}
									pDstImageAddressY	+= dstImageRowBytes;
									pDstAlphaAddressY	+= dstAlphaRowBytes;
								}
							}
							else
							{
								TriglavPlugInPtr	selectAddress;
								TriglavPlugInInt	selectRowBytes;
								TriglavPlugInInt	selectPixelBytes;
								(*pOffscreenService).getBlockSelectAreaProc(&selectAddress,&selectRowBytes,&selectPixelBytes,&tempRect,selectAreaOffscreenObject,&pos);

								if( selectAddress != NULL )
								{
									BYTE*		pDstImageAddressY	= static_cast<BYTE*>(dstImageAddress);
									BYTE*		pDstAlphaAddressY	= static_cast<BYTE*>(dstAlphaAddress);
									const BYTE*	pSelectAddressY		= static_cast<const BYTE*>(selectAddress);
									for( int y=blockRect.top; y<blockRect.bottom; ++y )
									{
										BYTE*		pDstImageAddressX	= pDstImageAddressY;
										BYTE*		pDstAlphaAddressX	= pDstAlphaAddressY;
										const BYTE*	pSelectAddressX		= pSelectAddressY;
										for( int x=blockRect.left; x<blockRect.right; ++x )
										{
											if( *pDstAlphaAddressX > 0 && *pSelectAddressX > 0 )
											{
												PIEtherealFilter::ApplyLookDev(
													pDstImageAddressX[r],
													pDstImageAddressX[g],
													pDstImageAddressX[b],
													exposure,
													whiteLift,
													satCompress,
													redSuppress,
													midContrast,
													coolBias,
													highlightAdjust,
													colorTemp,
													vividBoost
												);
											}

											pDstImageAddressX	+= dstImagePixelBytes;
											pDstAlphaAddressX	+= dstAlphaPixelBytes;
											pSelectAddressX		+= selectPixelBytes;
										}
										pDstImageAddressY	+= dstImageRowBytes;
										pDstAlphaAddressY	+= dstAlphaRowBytes;
										pSelectAddressY		+= selectRowBytes;
									}
								}
							}
						}
						TriglavPlugInFilterRunUpdateDestinationOffscreenRect(pRecordSuite,(*pluginServer).hostObject,&blockRect);
						++blockIndex;
					}

					TriglavPlugInInt	processResult;
					if( blockIndex < blockRectCount )
					{
						TriglavPlugInFilterRunProcess(pRecordSuite,&processResult,(*pluginServer).hostObject,kTriglavPlugInFilterRunProcessStateContinue);
					}
					else
					{
						TriglavPlugInFilterRunSetProgressDone(pRecordSuite,(*pluginServer).hostObject,blockIndex);
						TriglavPlugInFilterRunProcess(pRecordSuite,&processResult,(*pluginServer).hostObject,kTriglavPlugInFilterRunProcessStateEnd);
					}
					if( processResult == kTriglavPlugInFilterRunProcessResultRestart )
					{
						restart	= true;
					}
					else if( processResult == kTriglavPlugInFilterRunProcessResultExit )
					{
						break;
					}
				}
				*result	= kTriglavPlugInCallResultSuccess;
			}
		}
	}
	return;
}
