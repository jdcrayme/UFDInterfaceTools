#include "RadialLabels.h"
 
#include "Fonts/FontMeasure.h"

void SRadialLabels::Construct(const FArguments& InArgs)
{
	Texts = InArgs._Texts;
	AngleDegrees = InArgs._AngleDegrees;
	Radius = InArgs._Radius;
	Font = InArgs._Font;
	Color = InArgs._Color;
}
 
void SRadialLabels::SetTexts(const TArray<FText>& InTexts)
{
	Texts = InTexts;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabels::SetAngleDegrees(const TArray<float>& InAngles)
{
	AngleDegrees = InAngles;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabels::SetRadius(float InRadius)
{
	Radius = InRadius;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabels::SetFont(const FSlateFontInfo& InFont)
{
	Font = InFont;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabels::SetColor(const FLinearColor& InColor)
{
	Color = InColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

FVector2D SRadialLabels::ComputeDesiredSize(float) const
{
	const TArray<FText>& Labels = Texts.Get();
	const TArray<float>& Angles = AngleDegrees.Get();
 
	if (Labels.Num() == 0 || Angles.Num() == 0)
	{
		// Nothing to draw, minimal size
		return FVector2D::ZeroVector;
	}
 
	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
 
	float MaxX = 0.f;
	float MaxY = 0.f;
	float MinX = 0.f;
	float MinY = 0.f;
 
	float CommonRadius = Radius.Get();
 
	int32 CommonCount = FMath::Min(Labels.Num(), Angles.Num());
 
	// Compute bounding box covering all labels
	for (int32 i = 0; i < CommonCount; ++i)
	{
		FVector2D TextSize = FontMeasureService->Measure(Labels[i], Font.Get());
 
		float AngleRad = FMath::DegreesToRadians(Angles[i]);
		float X, Y;
		FMath::PolarToCartesian(CommonRadius, AngleRad, X, Y);
 
		// Account for half text extents
		float LabelMinX = X - TextSize.X * 0.5f;
		float LabelMaxX = X + TextSize.X * 0.5f;
		float LabelMinY = Y - TextSize.Y * 0.5f;
		float LabelMaxY = Y + TextSize.Y * 0.5f;
 
		MinX = FMath::Min(MinX, LabelMinX);
		MaxX = FMath::Max(MaxX, LabelMaxX);
		MinY = FMath::Min(MinY, LabelMinY);
		MaxY = FMath::Max(MaxY, LabelMaxY);
	}
 
	return FVector2D(MaxX - MinX, MaxY - MinY);
}

int32 SRadialLabels::OnPaint(const FPaintArgs& Args,
					   const FGeometry& AllottedGeometry,
					   const FSlateRect& MyCullingRect,
					   FSlateWindowElementList& OutDrawElements,
					   int32 LayerId,
					   const FWidgetStyle& InWidgetStyle,
					   bool bParentEnabled) const
{
	FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
	FVector2D Center = WidgetSize * 0.5f;
 
	const TArray<FText>& Labels = Texts.Get();
	const TArray<float>& Angles = AngleDegrees.Get();
 
	if (Labels.Num() == 0 || Angles.Num() == 0)
	{
		return LayerId;
	}
 
	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	
	int32 CommonCount = FMath::Min(Labels.Num(), Angles.Num());
	float CommonRadius = Radius.Get();
 
	for (int32 i = 0; i < CommonCount; ++i)
	{
		float AngleRad = FMath::DegreesToRadians(Angles[i]);
 
		float X, Y;
		FMath::PolarToCartesian(CommonRadius, AngleRad, X, Y);
 
		FVector2D TextSize = FontMeasureService->Measure(Labels[i], Font.Get());
		
		FVector2D LabelPosition = Center + FVector2D(X, Y)-TextSize/2;
 
		FSlateDrawElement::MakeText(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(LabelPosition, FVector2D(1.f, 1.f)),
			Labels[i].ToString(),
			Font.Get(),
			ESlateDrawEffect::None,
			Color.Get()
		);
		LayerId++;
	}
 
	return LayerId;
}
