#ifndef BGIF_GENERATOR_H
#define BGIF_GENERATOR_H

#include "qt_ni_fgen.h"
#include "qt_ni_fpga.h"
#include "qt_ni_p2p.h"
#include "service_types.h"
#include <QFile>
#include <QDataStream>

class bgif_generator : public QObject
{
Q_OBJECT

public:
    bgif_generator();

    p_form pulse_form;
    uint16_t *spectrum;

    double regular_freq;
    uint16_t mode;
    uint16_t pulse_threshold;
    bool big_frequency;
    double noise_rms;
    double correction_gain;
    bool noise_on;
    double mean_charge;
    double k0;
    double k1;
    double k2;
    double k3;

    const double dt = 1e-8;

    uint64_t current_freq;
    bool stop_indicator;
public slots:
    void start_generation();
    void stop_generation();
signals:
    void send_frequency(uint64_t);
    void generation_finished();
private:
    qt_ni_fpga fpga;
    qt_ni_fgen fgen;
    qt_ni_p2p p2p;

    QFile read_file;
    QDataStream read_stream;
    size_t empty_elements;
    size_t wfifo_length;
    int16_t *data_to_fifo;

    ViUInt32 fgen_p2p_endpoint;
    uint32_t fpga_p2p_endpoint;

    void init_fpga_params();
};

#endif // BGIF_GENERATOR_H
