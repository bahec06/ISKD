#include "qt_ni_p2p.h"

qt_ni_p2p::qt_ni_p2p()
{
    p2p_handle = 0;
}

int32_t qt_ni_p2p::CreateAndLinkStream(nip2p_tEndpointHandle fpga_endpoint, nip2p_tEndpointHandle fgen_endpoint) {
    return nip2pCreateAndLinkStream(fpga_endpoint, fgen_endpoint, 1, &p2p_handle);
}

int32_t qt_ni_p2p::FlushAndDisableStream() {
    return nip2pFlushAndDisableStream(p2p_handle, -1, &flush_tout);
}

int32_t qt_ni_p2p::DestroyStream() {
    return nip2pDestroyStream(p2p_handle);
}
