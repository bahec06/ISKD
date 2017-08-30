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
    uint16_t *spectrum;
    uint16_t *pulse_form;

    double regular_freq;
    bool mode;
    uint16_t pulse_threshold;
    bool big_frequency;
    double correction_gain;
    double mean_charge;
    double k0;
    double k1;
    double k2;
    double k3;

    const double dt = 1e-8;
    qt_ni_fpga fpga;
    qt_ni_fgen fgen;

    uint64_t current_freq;
    size_t wfifo_length;
    size_t cfifo_length;
    bool stop_indicator;
public slots:
    void start_generation();
    void stop_generation();
signals:
    void start_read();
    void generation_finished();
    void fgen_error();
    void fpga_error();
private:
    qt_ni_p2p *p2p;

    QFile read_file;
    QFile low_file;
    QDataStream read_stream;
    QDataStream low_stream;
    size_t empty_elements;
    size_t const_empty_elements;
    int16_t *data_to_fifo;
    int16_t *const_data;

    ViUInt32 fgen_p2p_endpoint;
    uint32_t fpga_p2p_endpoint;

    void init_fpga_params();
    void preinit_generation();
};

#endif // BGIF_GENERATOR_H
