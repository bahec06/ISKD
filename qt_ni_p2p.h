#ifndef QT_NI_P2P_H
#define QT_NI_P2P_H

#include "nip2p.h"
#include "visatype.h"

class qt_ni_p2p
{
private:
    int32_t p2p_stat = 0;
    nip2p_tStreamHandle p2p_handle = 0;
    uint8_t flush_tout;
public:
    qt_ni_p2p();
    int32_t CreateAndLinkStream(nip2p_tEndpointHandle fpga_endpoint, nip2p_tEndpointHandle fgen_endpoint);
    int32_t FlushAndDisableStream();
    int32_t DestroyStream();
};

#endif // QT_NI_P2P_H
