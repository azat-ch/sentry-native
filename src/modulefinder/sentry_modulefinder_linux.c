/// This code was edited for ClickHouse by removing most of it.
/// Because ClickHouse does not use shared libraries, we don't care about this code.

#include "sentry_sync.h"
#include "sentry_value.h"

#include <sys/types.h>

static bool g_initialized = false;
static sentry_mutex_t g_mutex = SENTRY__MUTEX_INIT;
static sentry_value_t g_modules = { 0 };

sentry_value_t
sentry_get_modules_list(void)
{
    sentry__mutex_lock(&g_mutex);
    if (!g_initialized) {
        g_modules = sentry_value_new_list();
        sentry_value_freeze(g_modules);
        g_initialized = true;
    }
    sentry_value_t modules = g_modules;
    sentry_value_incref(modules);
    sentry__mutex_unlock(&g_mutex);
    return modules;
}

void
sentry_clear_modulecache(void)
{
    sentry__mutex_lock(&g_mutex);
    sentry_value_decref(g_modules);
    g_modules = sentry_value_new_null();
    g_initialized = false;
    sentry__mutex_unlock(&g_mutex);
}
