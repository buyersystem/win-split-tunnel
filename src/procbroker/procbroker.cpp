#include "procbroker.h"
#include "context.h"
#include "../defs/types.h"

#include "../trace.h"
#include "procbroker.tmh"

namespace procbroker
{

NTSTATUS
Initialize
(
	CONTEXT **Context
)
{
    auto context = (CONTEXT*)ExAllocatePoolWithTag(PagedPool, sizeof(CONTEXT), ST_POOL_TAG);

    if (NULL == context)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(context, sizeof(*context));

    auto status = WdfWaitLockCreate(WDF_NO_OBJECT_ATTRIBUTES, &context->SubscriptionsLock);

    if (!NT_SUCCESS(status))
    {
        DbgPrint("WdfWaitLockCreate() failed 0x%X\n", status);

        ExFreePoolWithTag(context, ST_POOL_TAG);

        return status;
    }

	InitializeListHead(&context->Subscriptions);

    *Context = context;
   
    return STATUS_SUCCESS;
}

void
TearDown
(
	CONTEXT **Context
)
{
    auto context = *Context;

    LIST_ENTRY *record;

    while ((record = RemoveHeadList(&context->Subscriptions)) != &context->Subscriptions)
    {
        ExFreePoolWithTag(record, ST_POOL_TAG);
    }

    WdfObjectDelete(context->SubscriptionsLock);

    ExFreePoolWithTag(context, ST_POOL_TAG);

    *Context = NULL;
}

NTSTATUS
Subscribe
(
    CONTEXT *Context,
    ST_PB_CALLBACK Callback,
    void *ClientContext
)
{
    auto sub = (SUBSCRIPTION*)ExAllocatePoolWithTag(PagedPool, sizeof(SUBSCRIPTION), ST_POOL_TAG);

    if (NULL == sub)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(sub, sizeof(SUBSCRIPTION));

    sub->Callback = Callback;
    sub->ClientContext = ClientContext;

    WdfWaitLockAcquire(Context->SubscriptionsLock, NULL);

    InsertTailList(&Context->Subscriptions, &sub->ListEntry);

    WdfWaitLockRelease(Context->SubscriptionsLock);

    return STATUS_SUCCESS;
}

void
CancelSubscription
(
    CONTEXT *Context,
    ST_PB_CALLBACK Callback
)
{
    WdfWaitLockAcquire(Context->SubscriptionsLock, NULL);

	for (auto entry = Context->Subscriptions.Flink;
		entry != &Context->Subscriptions;
		entry = entry->Flink)
	{
        if (((SUBSCRIPTION*)entry)->Callback == Callback)
        {
            RemoveEntryList(entry);

            break;
        }
    }

    WdfWaitLockRelease(Context->SubscriptionsLock);
}

void
Publish
(
	CONTEXT *Context,
    HANDLE ProcessId,
    bool Arriving
)
{
    WdfWaitLockAcquire(Context->SubscriptionsLock, NULL);

	for (auto entry = Context->Subscriptions.Flink;
		entry != &Context->Subscriptions;
		entry = entry->Flink)
	{
        auto sub = (SUBSCRIPTION*)entry;

        sub->Callback(ProcessId, Arriving, sub->ClientContext);
    }

    WdfWaitLockRelease(Context->SubscriptionsLock);
}

} // namespace procbroker
