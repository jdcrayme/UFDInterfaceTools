#include "RadialTicks.h"
#include "Math/UnrealMathUtility.h"
 
void SRadialTicks::Construct(const FArguments& InArgs)
{
	StartAngleDeg = InArgs._StartAngleDeg;
	EndAngleDeg   = InArgs._EndAngleDeg;
	InnerRadius   = InArgs._InnerRadius;
	OuterRadius	  = InArgs._OuterRadius;
	Thickness     = InArgs._Thickness;
	Color         = InArgs._Color;
	Count   = InArgs._Count;
}

void SRadialTicks::SetStartAngle(float NewStartAngleDeg)
{
	if (!FMath::IsNearlyEqual(StartAngleDeg, NewStartAngleDeg))
	{
		StartAngleDeg = NewStartAngleDeg;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetEndAngle(float NewEndAngleDeg)
{
	if (!FMath::IsNearlyEqual(EndAngleDeg, NewEndAngleDeg))
	{
		EndAngleDeg = NewEndAngleDeg;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetInnerRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(InnerRadius, NewRadius))
	{
		InnerRadius = NewRadius;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetOuterRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(OuterRadius, NewRadius))
	{
		OuterRadius = NewRadius;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetThickness(float NewThickness)
{
	if (!FMath::IsNearlyEqual(Thickness, NewThickness))
	{
		Thickness = NewThickness;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetColor(FLinearColor NewColor)
{
	if (Color !=NewColor)
	{
		Color = NewColor;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialTicks::SetCount(int32 NewCount)
{
	if (Count != NewCount)
	{
		Count = NewCount;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}


FVector2D SRadialTicks::ComputeDesiredSize(float) const
{
	float radius = FMath::Max(InnerRadius, OuterRadius);
	
	// Size is diameter of the circle bounding the arc
	return FVector2D(radius * 2.f, radius * 2.f);
}
 
int32 SRadialTicks::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
 
	// Convert angles from degrees to radians
	float StartRad = FMath::DegreesToRadians(StartAngleDeg);
	float EndRad = FMath::DegreesToRadians(EndAngleDeg);
 
	int32 Segments = FMath::Max(Count, 1);
	TArray<FVector2D> Points;
	Points.Reserve(Segments + 1);
 
	// Generate points along the arc
	for (int32 i = 0; i <= Segments; ++i)
	{
		float Alpha = float(i) / Segments;
		float Angle = FMath::Lerp(StartRad, EndRad, Alpha);
		FVector2D Point1 = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * InnerRadius;
		FVector2D Point2 = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * OuterRadius;
 
		// Draw arc as a polyline with thickness and color
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			{Point1, Point2 },
			ESlateDrawEffect::None,
			Color,
			true, // Anti-alias
			Thickness
		);
	}
 
 
	return LayerId + 1;
}