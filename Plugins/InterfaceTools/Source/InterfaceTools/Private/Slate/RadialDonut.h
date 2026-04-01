#pragma once

#include "Widgets/SLeafWidget.h"
#include "Rendering/DrawElementTypes.h"
#include "Math/UnrealMathUtility.h"

class SRadialDonut : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SRadialDonut)
		: _StartAngleDeg(0.f)
		, _EndAngleDeg(90.f)
		, _InnerRadius(50.f)
		, _OuterRadius(10.f)
		, _Color(FLinearColor::White)
		, _NumSegments(32)
	{}
	SLATE_ARGUMENT(float, StartAngleDeg)   // Degrees, from positive X axis CCW
	SLATE_ARGUMENT(float, EndAngleDeg)     // Degrees
	SLATE_ARGUMENT(float, InnerRadius)          // Outer radius in local space
	SLATE_ARGUMENT(float, OuterRadius)          // Outer radius in local space
	SLATE_ARGUMENT(FLinearColor, Color)    // Fill color
	SLATE_ARGUMENT(int32, NumSegments)     // Number of segments for smoothness
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

	void SetStartAngle(float NewStartAngleDeg);
	void SetEndAngle(float NewEndAngleDeg);
	void SetInnerRadius(float NewRadius);
	void SetOuterRadius(float NewRadius);
	void SetColor(FLinearColor NewColor);
	void SetNumSegments(int32 NewNumSegments);

protected:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float) const override;

private:
	float StartAngleDeg = 0;
	float EndAngleDeg = 90;
	float InnerRadius = 50;
	float OuterRadius = 10;
	FLinearColor Color = FLinearColor::White;
	int32 NumSegments = 32;
};