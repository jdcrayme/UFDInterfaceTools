// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialGaugeWidget.h"

#include "SlateBasics.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "Fonts/SlateFontInfo.h"
#include "SlateOptMacros.h"
#include "Widgets/SOverlay.h"
#include "Slate/RadialArc.h"
#include "Slate/RadialTicks.h"
#include "Slate/RadialLabels.h"
#include "Slate/RadialDonut.h"

#include "InterfaceToolsLibrary.h"

URadialGaugeWidget::URadialGaugeWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
  if (!IsRunningDedicatedServer())
  {
    static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
    Font = FSlateFontInfo(RobotoFontObj.Object, 16, FName("Bold"));
  }
}

void URadialGaugeWidget::SetCurrentValue(float NewValue)
{
  if (FMath::IsNearlyEqual(CurrentValue, NewValue))
    return;

  CurrentValue = NewValue;
}

float URadialGaugeWidget::AngleFromValue(float value) const
{
  return FMath::GetMappedRangeValueClamped(
    TRange<float>(NiceDataStart, NiceDataEnd),
    TRange<float>(StartAngle, EndAngle),
    value);
}

void URadialGaugeWidget::CalculateLayout()
{
  UInterfaceToolsLibrary::GenerateNiceAxisSteps(DataRangeStart, DataRangeEnd, 5, NiceDataStart, NiceDataEnd, NiceDataStep);
  NiceTickCount= (NiceDataEnd-NiceDataStart)/NiceDataStep;
  
  TickLength = 10;
    
}

TSharedRef<SWidget> URadialGaugeWidget::RebuildWidget()
{
  if (!SlateRoot.IsValid())
  {
    CalculateLayout();
    
    SlateRoot = SNew(SOverlay);

    SlateFill = SNew(SRadialDonut)
      .StartAngleDeg(StartAngle)
      .EndAngleDeg(EndAngle)
      .InnerRadius(10)
      .OuterRadius(Radius - 10)
      .Color(FLinearColor::Blue);

    SlateRoot->AddSlot()[SlateFill.ToSharedRef()];
    	       
    
    // Background Arc
    SlateBaseArc = SNew(SRadialArc)
      .StartAngleDeg(StartAngle)
      .EndAngleDeg(EndAngle)
      .Radius(Radius)
      .Color(Color)
      .Thickness(Thickness);

    SlateRoot->AddSlot()[SlateBaseArc.ToSharedRef()];
    	 
    SlateTicks = SNew(SRadialTicks)
      .StartAngleDeg(AngleFromValue(NiceDataStart))
      .EndAngleDeg(AngleFromValue(NiceDataEnd))
      .InnerRadius(Radius-Thickness/2)
      .OuterRadius(Radius+10)
      .Count(NiceTickCount)
      .Color(Color)
      .Thickness(Thickness);

    SlateRoot->AddSlot()[SlateTicks.ToSharedRef()];
        
    SlateLabels = SNew(SRadialLabels)
      .Radius(Radius+20)
      .Color(Color)
      .Font(Font);

    SlateRoot->AddSlot()[SlateLabels.ToSharedRef()];
  }

  SynchronizeProperties();

  return SlateRoot.ToSharedRef();
}

void URadialGaugeWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();
  
  CalculateLayout();
  
  if (SlateBaseArc.IsValid())
  {
    // Update arc properties - these require you to expose setters on SArc if mutable
    SlateBaseArc->SetStartAngle(StartAngle);
    SlateBaseArc->SetEndAngle(EndAngle);
    SlateBaseArc->SetColor(Color);
    SlateBaseArc->SetRadius(Radius);
    SlateBaseArc->SetThickness(Thickness);
    SlateBaseArc->Invalidate(EInvalidateWidget::LayoutAndVolatility);
  }

  if (SlateFill.IsValid())
  {
    SlateFill->SetStartAngle(StartAngle);
    SlateFill->SetEndAngle(AngleFromValue(CurrentValue));
    SlateFill->SetInnerRadius(10);
    SlateFill->SetOuterRadius(Radius-20);
    SlateFill->SetColor(FLinearColor::Blue);
  }
  
  if (SlateTicks.IsValid())
  {
    // Update arc properties - these require you to expose setters on SArc if mutable
    SlateTicks->SetStartAngle(StartAngle);
    SlateTicks->SetEndAngle(EndAngle);
    SlateTicks->SetInnerRadius(Radius-Thickness/2);
    SlateTicks->SetOuterRadius(Radius+TickLength);
    SlateTicks->SetColor(Color);
    SlateTicks->SetThickness(Thickness);
    SlateTicks->Invalidate(EInvalidateWidget::LayoutAndVolatility);
  }
  
  if (SlateLabels.IsValid())
  {
    
    TArray<float> TickAngles;
    TArray<FText> TickLabels;
    for (int i=0; i<=NiceTickCount; i++)
    {
      float num = i*NiceDataStep + NiceDataStart;
      
      TickAngles.Push(AngleFromValue(num));
      
      FNumberFormattingOptions FormatOptions;
      FormatOptions.SetMaximumFractionalDigits(2);
      FormatOptions.SetMinimumFractionalDigits(0);      
      TickLabels.Push(FText::AsNumber(num, &FormatOptions));
    }
    
    // Update arc properties - these require you to expose setters on SArc if mutable
    SlateLabels->SetAngleDegrees(TickAngles);
    SlateLabels->SetRadius(Radius+TickLength*2);
    SlateLabels->SetTexts(TickLabels);
    SlateLabels->SetColor(Color);
    SlateLabels->SetFont(Font);
    SlateLabels->Invalidate(EInvalidateWidget::LayoutAndVolatility);
  }
  
  if (SlateRoot.IsValid())
  {
    SlateRoot->Invalidate(EInvalidateWidget::LayoutAndVolatility);
  }
}

void URadialGaugeWidget::ReleaseSlateResources(bool bReleaseChildren)
{
  Super::ReleaseSlateResources(bReleaseChildren);

  SlateBaseArc.Reset();
  SlateRoot.Reset();
}

