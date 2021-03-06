// Copyright 2015-2021 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "PsMRGSProxy.generated.h"

UENUM(BlueprintType)
enum class EPsMRGSEventsTypes : uint8
{
	MRGS_INIT_COMPLETE = 0,
	MRGS_PRODUCTS_LOADED,
	MRGS_PURCHASE_COMPLETE,
	MRGS_PURCHASE_PENDING,
	MRGS_PURCHASE_FAILED,
	MRGS_PURCHASE_CANCELED,
	MRGS_SUPPORT_CLOSED,
	MRGS_SUPPORT_ERROR,
	MRGS_SUPPORT_TICKETS_NEW,
	MRGS_SUPPORT_TICKETS_ERROR,
	MRGS_FULLSCREEN_CLOSED,
	MRGS_INTERSTITIAL_SLIDER_CLOSED,
	MRGS_INTERSTITIAL_DATA_ERROR,
	MRGS_FULLSCREEN_DATA_ERROR,
	MRGS_SHOWCASE_DATA_ERROR,
	MRGS_SHOWCASE_DATA_EMPTY,
	MRGS_USERINIT_COMPLETE,
	MRGS_USERINIT_ERROR,
	MRGS_GDPR_ACCEPTED_WITH_ADS,
	MRGS_GDPR_ACCEPTED_WITHOUT_ADS,
	MRGS_GDPR_ERROR,
	MRGS_NOT_IMPLEMENTED,
	MRGS_ATT_NOT_DETERMINED,
	MRGS_ATT_RESTRICTED,
	MRGS_ATT_DENIED,
	MRGS_ATT_AUTHORIZED,
	MRGS_ATT_ERROR,
	MRGS_NOTIFICATION_CLICKED,
};

/** CCPA setting */
UENUM(BlueprintType)
enum class EPsMRGSCCPASetting : uint8
{
	Share = 0,
	DontShare = 1
};

/** App Tracking status */
UENUM(BlueprintType)
enum class EPsMRGSATTStatus : uint8
{
	NotDetermined = 0,
	Restricted = 1,
	Denied = 2,
	Authorized = 3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPsMRGSDelegate, EPsMRGSEventsTypes, MyEventName);
typedef TMulticastDelegate<void(EPsMRGSEventsTypes MyEventName, FString ProductId, FString TransactionId, FString Payload)> FPsMRGSIAPDelegate;

USTRUCT(BlueprintType)
struct FPsMRGSPurchaseInfo
{
	GENERATED_USTRUCT_BODY()

	/** Product bundle id */
	UPROPERTY(BlueprintReadOnly)
	FString Sku;

	/** Product price from store */
	UPROPERTY(BlueprintReadOnly)
	FString Price;

	/** Product micros from store (Android) */
	UPROPERTY(BlueprintReadOnly)
	FString PriceMicros;

	/** Product currency from store */
	UPROPERTY(BlueprintReadOnly)
	FString Currency;

	/** Price formatted */
	UPROPERTY(BlueprintReadOnly)
	FString FormattedPrice;

	/** Product title from store */
	UPROPERTY(BlueprintReadOnly)
	FString Title;

	/** Product type from store */
	UPROPERTY(BlueprintReadOnly)
	FString Type;

	/** Product description from store */
	UPROPERTY(BlueprintReadOnly)
	FString Description;

	/** Whether this is a debug (fake) product generated on client for testing purposes */
	UPROPERTY(BlueprintReadOnly)
	bool bDebug;
};

UCLASS()
class PSMRGS_API UPsMRGSProxy : public UObject
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////
	// GDPR

public:
	/** Show MRGS built-in GDPR agreement */
	UFUNCTION(BlueprintCallable, Category = "MRGS|GDPR")
	virtual void ShowDefaultGDPRAgreement(bool bOnlyEU, bool bWithAdvertising);

	/** Show specified GDPR agreement */
	UFUNCTION(BlueprintCallable, Category = "MRGS|GDPR")
	virtual void ShowGDPRAgreement(bool bOnlyEU, bool bWithAdvertising);

	/** Get accepted version of the agreement */
	UFUNCTION(BlueprintCallable, Category = "MRGS|GDPR")
	virtual int32 GetGDPRAcceptedVersion();

	//////////////////////////////////////////////////////////////////////////
	// CCPA

public:
	/** Get whether CCPA is applicable */
	UFUNCTION(BlueprintCallable, Category = "MRGS|CCPA")
	virtual bool ShouldShowCCPA();

	/** Get CPPA setting value */
	UFUNCTION(BlueprintCallable, Category = "MRGS|CCPA")
	virtual EPsMRGSCCPASetting GetCCPASettingValue();

	/** Set CPPA setting value */
	UFUNCTION(BlueprintCallable, Category = "MRGS|CCPA")
	virtual void SetCCPASettingValue(EPsMRGSCCPASetting Value);

	//////////////////////////////////////////////////////////////////////////
	// App tracking

public:
	/** Get whether should show tracking auth dialog */
	UFUNCTION(BlueprintCallable, Category = "MRGS|App Tracking")
	virtual bool ShouldShowTrackingAuthorizationDialog();

	/** Request app tracking authorization */
	UFUNCTION(BlueprintCallable, Category = "MRGS|App Tracking")
	virtual void RequestTrackingAuthorization();

	/** Get app tracking authorization status */
	UFUNCTION(BlueprintCallable, Category = "MRGS|App Tracking")
	virtual EPsMRGSATTStatus GetTrackingAuthorizationStatus();

	//////////////////////////////////////////////////////////////////////////
	// Setup

	/** Check library integration */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Setup")
	virtual void CheckIntegration();

	/** Start mrgs initialization */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Setup")
	virtual void InitModule();

	/** Login or register user in mrgs */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Setup")
	virtual void InitUser(const FString& UserId);

	/** MRGS initialize complete */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Setup")
	virtual bool IsReady() const;

	/** MRGS user auth complete */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Setup")
	virtual bool UserLoggedIn() const;

	//////////////////////////////////////////////////////////////////////////
	// Events

	/** Send applsflyer event */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Events")
	virtual void SendAFEvent(const FString& InEventName, const FString& InValue);

	/** Log metric on mrgs with numeric id */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Events")
	virtual void AddMetricWithId(int32 MetricId);

	/** Log metric on mrgs with string code */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Events")
	virtual void AddMetricWithCode(const FString& MetricCode, int32 Value, int32 Level, int32 ObjectId);

	//////////////////////////////////////////////////////////////////////////
	// Store

	/** Whether to restore transactions after loading products. Must be called after InitModule but before LoadStoreProducts */
	virtual void SetAutoRestoreTransactions(bool bRestore);

	/** Load products from store */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Store")
	virtual void LoadStoreProducts(const TArray<FString>& ProductsList);

	/** Buy product in store */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Store")
	virtual void BuyProduct(const FString& ProductId, const FString& Payload);

	/** Get loaded products */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Store")
	virtual const TArray<FPsMRGSPurchaseInfo>& GetProducts() const;

	/** Override loaded products list (debug purposes only */
	void DebugSetProducts(const TArray<FPsMRGSPurchaseInfo>& InProducts);

	//////////////////////////////////////////////////////////////////////////
	// Support

	/** Show support screen */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Support")
	virtual void ShowSupport();

	/** Check if there are new support tickets */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Support")
	virtual void CheckSupportTickets();

	//////////////////////////////////////////////////////////////////////////
	// Tools

	/** Get platform */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Tools")
	virtual FString GetDevicePlatform() const;

	/** Get OpenUDID */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Tools")
	virtual FString GetOpenUDID() const;

	/** Open application page in system settings */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Tools")
	virtual void OpenApplicationPageInSystemSettings();

	//////////////////////////////////////////////////////////////////////////
	// Notifications

	/** Call EnableMRGSNotifications */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Notifications")
	virtual void EnableNotifications();

	/** Call DisableMRGSNotifications */
	UFUNCTION(BlueprintCallable, Category = "MRGS|Notifications")
	virtual void DisableNotifications();

	//////////////////////////////////////////////////////////////////////////
	// Callbacks

public:
	/** GDPR agreement accepted */
	virtual void OnGDPRAccepted(bool bWithAdvertising);

	/** GDPR error */
	virtual void OnGDPRError();

	/** Mrgs initialization complete  */
	virtual void OnInitComplete();

	/** Fullscreen closed */
	virtual void OnFullscreenClosed();

	/** Interstitial slider closed */
	virtual void OnInterstitialSliderClosed();

	/** Interstitial data receive error */
	virtual void OnInterstitialDataRecieveError(const FString& Error);

	/** Fullscreen data receive error */
	virtual void OnFullscreenDataRecieveError(const FString& Error);

	/** Showcase data receive error */
	virtual void OnShowcaseDataRecieveError(const FString& Error);

	/** Showcase data is empty */
	virtual void OnShowCaseDataHasNoAds();

	/** Notification clicked handler */
	virtual void OnClickOnNotification(int32 NotificationId, const FString& Title, const FString& Message, const FString& DeveloperPayload, bool bIsLocal);

	/** Get notification developer payload */
	UFUNCTION(BlueprintCallable)
	virtual FString GetNotificationDeveloperPayload() const;

	/** Support received error */
	virtual void OnSupportReceivedError(const FString& Error);

	/** Support has updates on tickets */
	virtual void OnSupportTicketsReceived();

	/** Support tickets update fail with error */
	virtual void OnSupportTicketsFailWithError(const FString& Error);

	/** Support closed */
	virtual void OnSupportClosed();

	/** Productrs successfully loaded from store */
	virtual void OnStoreProductsLoaded(TArray<FPsMRGSPurchaseInfo> InLoadedProducts);

	/** Purchase successfully complete on store */
	virtual void OnPurchaseComplete(const FString& PaymentId, const FString& TransactionId, const FString& Payload);

	/** Purchase is pending */
	virtual void OnPurchasePending(const FString& ProductId);

	/** Error while processing purchase on store */
	virtual void OnPurchaseFailed(const FString& ProductId, const FString& Answer);

	/** Canceled while processing purchase on store */
	virtual void OnPurchaseCanceled(const FString& ProductId, const FString& Answer);

	/** Dispatch success user auth */
	virtual void OnUserAuthSuccess();

	/** Dispatch failed user auth */
	virtual void OnUserAuthError();

protected:
	/** Loaded products from store */
	TArray<FPsMRGSPurchaseInfo> LoadedProducts;

	/** MRGS initialize complete */
	bool bInitComplete;

	/** MRGS auth complete */
	bool bUserLoggedin;

public:
	UPROPERTY(BlueprintAssignable, Category = "MRGS|Events")
	FPsMRGSDelegate MRGSDelegate;

	/** Delegate that would be trigget on purchase related events */
	FPsMRGSIAPDelegate MRGSIAPDelegate;

private:
	/** Debug CPPA setting value*/
	EPsMRGSCCPASetting DebugCCPASetting;
};
