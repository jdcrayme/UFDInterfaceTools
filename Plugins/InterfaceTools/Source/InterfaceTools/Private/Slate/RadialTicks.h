#pragma once
 
#include "Widgets/SLeafWidget.h"
#include "Rendering/DrawElementTypes.h"
 
class SRadialTicks: public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SRadialTicks)
		: _StartAngleDeg(0.f)
		, _EndAngleDeg(90.f)
		, _InnerRadius(50.f)
		, _OuterRadius(70.f)
		, _Thickness(2.f)
		, _Count(4)
		, _Color(FLinearColor::White)
	{}
	SLATE_ARGUMENT(float, StartAngleDeg)    // Degrees, from positive X axis, CCW
	SLATE_ARGUMENT(float, EndAngleDeg)      // Degrees
	SLATE_ARGUMENT(float, InnerRadius)      // Radius in local space
	SLATE_ARGUMENT(float, OuterRadius)      // Radius in local space
	SLATE_ARGUMENT(float, Thickness)        // Line thickness
	SLATE_ARGUMENT(int32, Count)			// Number of ticks
	SLATE_ARGUMENT(FLinearColor, Color)     // Line color
SLATE_END_ARGS()
 
	void SetStartAngle(float NewStartAngleDeg);
	void SetEndAngle(float NewEndAngleDeg);
	void SetInnerRadius(float NewRadius);
	void SetOuterRadius(float NewRadius);
	void SetThickness(float NewThickness);
	void SetColor(FLinearColor NewColor);
	void SetCount(int32 NewCount);
	
	void Construct(const FArguments& InArgs);
 
protected:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
 
	virtual FVector2D ComputeDesiredSize(float) const override;
 
private:
	float StartAngleDeg;
	float EndAngleDeg;
	float InnerRadius;
	float OuterRadius;
	float Thickness;
	FLinearColor Color;
	int32 Count;
};