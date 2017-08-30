#ifndef QT_NI_FGEN_H
#define QT_NI_FGEN_H

#include <QMessageBox>
#include "niFgen.h"

class qt_ni_fgen
{
private:
    QString script = "script myScript\nrepeat forever\nstream 65536\nend repeat\nend script";
    const ViChar * channels = "0,1";
    const ViChar * p2p_endp_chan = "FIFOEndpoint0";
    const ViReal64 sample_rate = 1e8;
    const ViReal64 gain = 2.5;
    const ViReal64 load_imp0 = 50;
    const ViReal64 load_imp1 = 1e6;
public:
    qt_ni_fgen();
    ViChar ni_fgen_resource[256];
    ViSession fgen_vi = VI_NULL;
    ViStatus error = VI_SUCCESS;
    ViChar errMsg[256];
    ViUInt32 fgen_p2p_endpoint;
    double CMO, DO;

    ViStatus fgen_init();
    ViStatus start_generation();
    ViStatus fgen_stop();
};

#endif // QT_NI_FGEN_H
