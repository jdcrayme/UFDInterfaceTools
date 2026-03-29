// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialGaugeWidget.generated.h"

UCLASS()
class INTERFACETOOLS_API URadialGaugeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	URadialGaugeWidget(const FObjectInitializer& ObjectInitializer);
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (SyncHorizontalModification))
	float StartAngle=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (SyncHorizontalModification))
	float EndAngle=90;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (SyncHorizontalModification))
	float Radius=10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (SyncHorizontalModification))
	float Thickness=15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (SyncHorizontalModification))
	float DataRangeStart =0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (SyncHorizontalModification))
	float DataRangeEnd =100;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (SyncHorizontalModification))
	float CurrentValue;

	UFUNCTION(BlueprintCallable, Category="Needle")
	void SetCurrentValue(float NewValue);
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	FSlateFontInfo Font;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	FLinearColor Color = FLinearColor::Red;
  
protected:
	float AngleFromValue(float value) const;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
  
	void CalculateLayout();
	
	float GetCurrentAngle() const { return AngleFromValue(CurrentValue); }
	float NiceDataStart, NiceDataEnd, NiceDataStep, NiceTickCount;
	float TickLength;
	
	TSharedPtr<SOverlay> SlateRoot;
	TSharedPtr<class SRadialArc> SlateBaseArc;
	TSharedPtr<class SRadialTicks> SlateTicks;
	TSharedPtr<class SRadialLabels> SlateLabels;
	TSharedPtr<class SRadialDonut> SlateFill;
};