#include "RadialLabel.h"
 
#include "Fonts/FontMeasure.h"

void SRadialLabel::Construct(const FArguments& InArgs)
{
	Text = InArgs._Text;
	AngleDegrees = InArgs._AngleDegrees;
	Radius = InArgs._Radius;
	Font = InArgs._Font;
	Color = InArgs._Color;
}
 
void SRadialLabel::SetText(const FText& InText)
{
	Text = InText;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabel::SetAngleDegrees(float InAngleDegrees)
{
	AngleDegrees = InAngleDegrees;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabel::SetRadius(float InRadius)
{
	Radius = InRadius;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabel::SetFont(const FSlateFontInfo& InFont)
{
	Font = InFont;
	Invalidate(EInvalidateWidgetReason::Paint);
}
 
void SRadialLabel::SetColor(const FLinearColor& InColor)
{
	Color = InColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

FVector2D SRadialLabel::ComputeDesiredSize(float) const
{
	
	// Get the Slate font measure service
	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
 	
	// Measure the text size
	FVector2D TextSize = FontMeasureService->Measure(Text.Get(), Font.Get());
	
	// Convert radial (polar) to Cartesian coordinates
	float AngleRad = FMath::DegreesToRadians(AngleDegrees.Get());
	float X, Y;
	FMath::PolarToCartesian(Radius.Get(), AngleRad, X, Y);
	
	
	return FVector2D(TextSize.X/2 +X, TextSize.Y/2+Y); // Default widget size; adjust as needed
}
 
int32 SRadialLabel::OnPaint(const FPaintArgs& Args,
							const FGeometry& AllottedGeometry,
							const FSlateRect& MyCullingRect,
							FSlateWindowElementList& OutDrawElements,
							int32 LayerId,
							const FWidgetStyle& InWidgetStyle,
							bool bParentEnabled) const
{
	FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
	FVector2D Center = WidgetSize * 0.5f;
 
	// Convert radial (polar) to Cartesian coordinates
	float AngleRad = FMath::DegreesToRadians(AngleDegrees.Get());
	float X, Y;
	FMath::PolarToCartesian(Radius.Get(), AngleRad, X, Y);
 
	FVector2D LabelPosition = Center + FVector2D(X, Y);
 
	// Draw the text label at computed position
	FSlateDrawElement::MakeText(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(LabelPosition, FVector2D(1.f, 1.f)),
		Text.Get().ToString(),
		Font.Get(),
		ESlateDrawEffect::None,
		Color.Get()
	);
 
	return LayerId + 1;
}
