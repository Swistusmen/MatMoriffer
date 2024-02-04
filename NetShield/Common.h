#ifndef COMMON_H
#define COMMON_H

#define LOCAL_DEBUG

#ifdef LOCAL_DEBUG
#define NETSHIELD_DATA_DIRECTORY "../configuration"
#define NETSHIELD_DATA_CONFIGURATION "../configuration/config.json"
#else
#define NETSHIELD_DATA_DIRECTORY "/etc/netshield"
#define NETSHIELD_DATA_CONFIGURATION "/etc/netshield/config.json"
#endif

//configuration file internals
#define MODULE_LOADED "module_loaded"

#endif // COMMON_H
