#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "CmTDC_CameraPawn.generated.h"

struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraPawnMoved, FVector&, InMovedPos);

UCLASS()
class CMTOPDOWNCAMERA_API ACmTDC_CameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACmTDC_CameraPawn(const FObjectInitializer& Initializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	void ActionMove(const FInputActionValue& InValue);
	UFUNCTION()
	void ActionZoom(const FInputActionValue& InValue);

	FOnCameraPawnMoved OnCameraPawnMoved;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Components")
	TObjectPtr<UCameraComponent> CmCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Components")
	TObjectPtr<USpringArmComponent> CmCameraBoom;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Components")
	TObjectPtr<UStaticMeshComponent> CmTestMesh;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Input Action")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Input Action")
	TObjectPtr<UInputAction> IA_Zoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting ")
	float MoveSpeed{5000.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting ")
	float EdgeMargin{30.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting | Zoom")
	float MinLen{1000.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting | Zoom")
	float MaxLen{4000.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting | Zoom")
	float ZoomSpeed{100000.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting | Zoom")
	float ZoomSmoothSpeed{5.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Setting | Zoom")
	float ZoomThreshold{20.f};

private:
	void Move(float DeltaTime);
	bool bHasMoveInput{false};
	bool bHasZoomInput{false};
	FVector CachedMoveDir;
	float CachedZoomDir;
	float CachedLen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CmTDC | Input", meta=(AllowPrivateAccess))
	bool bUseEdgeMove{false};
};
