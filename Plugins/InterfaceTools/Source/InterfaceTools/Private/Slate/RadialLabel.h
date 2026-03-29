	#pragma once
 
#include "Widgets/SWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "Math/UnrealMathUtility.h"
#include "Rendering/DrawElementTypes.h"
 
class SRadialLabel : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SRadialLabel)
		: _Text(FText::GetEmpty())
		, _AngleDegrees(0.f)
		, _Radius(0.f)
		, _Font(FSlateFontInfo())
		, _Color(FLinearColor::White)
	{}
	SLATE_ATTRIBUTE(FText, Text)
	SLATE_ATTRIBUTE(float, AngleDegrees)
	SLATE_ATTRIBUTE(float, Radius)
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)
	SLATE_ATTRIBUTE(FLinearColor, Color)
SLATE_END_ARGS()
 
void Construct(const FArguments& InArgs);
 
	// Runtime setters
	void SetText(const FText& InText);
	void SetAngleDegrees(float InAngleDegrees);
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
	TAttribute<FText> Text;
	TAttribute<float> AngleDegrees;
	TAttribute<float> Radius;
	TAttribute<FSlateFontInfo> Font;
	TAttribute<FLinearColor> Color;
};