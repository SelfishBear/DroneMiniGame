// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickable.generated.h"

UINTERFACE(MinimalAPI)
class UIPickable : public UInterface
{
	GENERATED_BODY()
};

class DRONETASK_API IIPickable
{
	GENERATED_BODY()

public:
	virtual void PickUp(AActor* Picker) = 0;
};
