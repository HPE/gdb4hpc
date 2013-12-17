/* configurations of topology, machines_file and filters */
#ifndef _GUARD_MRNET_CONFIGURE_H
#define _GUARD_MRNET_CONFIGURE_H

#define		GUARD_MRNET_TOP_FANOUT		"GUARD_MRNET_TOP_FANOUT"
#define		GUARD_MRNET_TOP_TYPE			"GUARD_MRNET_TOP_TYPE"
#define		GUARD_MRNET_TOP_GENERATOR	"GUARD_MRNET_TOP_GENERATOR"
#define		GUARD_MRNET_FILTER_TIMEOUT	"GUARD_MRNET_FILTER_TIMEOUT"

#define		ALPS_APRUNID_FILE				"apid.dat"
#define		MACHINES_FILE					"machines_file.dat"
#define		MRNET_TOPOLOGY 				"topology.dat"
#define		MRNET_ATTACH_CONNECTIONS		"be_attach.dat"

#define		AGGREGATION_FILTER 			"libdbgagg.so"
#define		AGGREGATION_FUNC 				"DbgMessageAggregation"

#define		GUARD_DEFAULT_TIMEOUT			60
#define		GUARD_MRNET_STARTUP_TIMEOUT	"CRAY_LGDB_STARTUP_TIMEOUT"
#define		GUARD_ADDITIONAL_SLEEP		"CRAY_LGDB_PMI_INIT_SLEEP"

#endif /* _GUARD_MRNET_CONFIGURE_H */
