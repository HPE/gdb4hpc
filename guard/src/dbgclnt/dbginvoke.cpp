#include "dbginvoke.h"

dbginvoke::dbginvoke(DebugClientProtocol * const proto)
    : d_handle{nullptr}
    , d_appid{0}
    , d_type{DBG_ATTACHED} // 0
    , d_started{0}
    , d_isShutdown{0}
    , d_nproc{0}
    , d_cmd{nullptr}
    , d_stat{0}
    , d_evfds{nullptr}
    , d_bitset_full{nullptr}
    , d_bitset_launched{nullptr}
    , d_proto_id{0}
    , d_proto_net{nullptr}
    , d_proto_outp{nullptr}
    , d_proto{proto}
    , d_proto_ev_callback{nullptr}
    , d_clnt_ev_callback{nullptr}
    , d_proto_outp_callback{nullptr}
    , d_frontend{nullptr}
    , d_eventset_list{nullptr}
    , d_bpset_list{nullptr}
    , d_global_bp_set_id{0}
{};
