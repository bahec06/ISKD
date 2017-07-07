#ifndef QT_NI_FGEN_H
#define QT_NI_FGEN_H

#include <QMessageBox>
#include "niFgen.h"

class qt_ni_fgen
{
private:
    ViChar ni_fgen_resource[256];
    ViConstString script = "script myScript\
                                repeat forever\
                                    stream 1280\
                                end repeat\
                            end script";
    ViConstString channels = "0,1";
    ViConstString p2p_endp_chan = "FIFOEndpoint0";
    const ViReal64 sample_rate = 1e8;
    const ViReal64 gain = 2.5;
    const ViReal64 load_imp0 = 50;
    const ViReal64 load_imp1 = 1e6;
public:
    qt_ni_fgen();

    ViSession fgen_vi = VI_NULL;
    ViStatus error = VI_SUCCESS;
    ViChar errMsg[256];
    double CMO, DO;

    ViUInt32 fgen_init();
    void start_generation();
    void fgen_stop();
};

#endif // QT_NI_FGEN_H
