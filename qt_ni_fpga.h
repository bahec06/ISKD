#ifndef QT_NI_FPGA_H
#define QT_NI_FPGA_H

#include "NiFpga.h"
#include "qmath.h"
#include "ISKD.h"
#include "service_types.h"

class qt_ni_fpga
{
private:
    NiFpga_Status fpga_stat;
    NiFpga_Session fpga_sess;
    size_t config_fifo_elements = 0;
    const char* bitfile = "C:/lvbit/ISKD.lvbitx";
    const char* ni_fpga_resource = "PXI1Slot4";
public:
    const size_t host_fifo_depth = 6553600; //Глубина импульсного FIFO буфера
    const size_t const_fifo_depth = 10240; //Глубина FIFO буфера постоянной составляющей
    //Кнопки
    NiFpga_Bool Noise;
    NiFpga_Bool Play;
    NiFpga_Bool SetWide;
    NiFpga_Bool Two_Channels;
    //ControlU16
    uint16_t CountVal1;
    uint16_t CountVal2;
    uint16_t RMS;
    uint16_t TMode; //Режим воспроизведения сигнала
    uint16_t WideRMS;
    //ControlU32
    uint32_t Time;
    //ControlI64
    int64_t K0;
    int64_t K1;
    int64_t K2;
    int64_t K3;
    //ControlU64
    uint64_t C1;
    uint64_t PulseCoeff;
    uint64_t Offset;
    uint64_t Frequency;

    qt_ni_fpga();
    uint32_t fpga_init(const uint16_t *spectrum, p_form pulse_form);
    void fpga_play();
    void fpga_stop();
    void fpga_reconfigure(const uint16_t *spectrum, p_form pulse_form);
    void fpga_close();
    void read_freq();
    void write_fifo(int16_t *buffer, size_t length, size_t *empty_elements);
};

#endif // QT_NI_FPGA_H