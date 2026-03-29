	#pragma once
 
#include "Widgets/SWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "Math/UnrealMathUtility.h"
#include "Rendering/DrawElementTypes.h"
 
class SRadialLabels : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SRadialLabels)
		: _Texts()
		, _AngleDegrees()
		, _Radius(0.f)
		, _Font(FSlateFontInfo())
		, _Color(FLinearColor::White)
		, _bRotateText(false)
	{}
		SLATE_ATTRIBUTE(TArray<FText>, Texts)
		SLATE_ATTRIBUTE(TArray<float>, AngleDegrees)
		SLATE_ATTRIBUTE(float, Radius)
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)
		SLATE_ATTRIBUTE(FLinearColor, Color)
		SLATE_ATTRIBUTE(bool, bRotateText)
SLATE_END_ARGS()
 
void Construct(const FArguments& InArgs);
 
	// Runtime setters
	void SetTexts(const TArray<FText>& InTexts);
	void SetAngleDegrees(const TArray<float>& InAngles);
	void SetRadius(float InRadius);
	void SetFont(const FSlateFontInfo& InFont);
	void SetColor(const FLinearColor& InColor);
	
	virtual int32 OnPaint(const FPaintArgs& Args,
						  const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect,
						  FSlateWindowElementList& OutDrawElements,
						  int32 LayerId,
						  const FWidgetStyle& InWidgetStyle,
						  bool bParentEnabled) const override;
 
	virtual FVector2D ComputeDesiredSize(float) const override;
 
private:
	TAttribute<TArray<FText>> Texts;
	TAttribute<TArray<float>> AngleDegrees;
	TAttribute<float> Radius;
	TAttribute<FSlateFontInfo> Font;
	TAttribute<FLinearColor> Color;
	TAttribute<bool>bRotateText;
};