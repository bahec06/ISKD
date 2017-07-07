#include "qt_ni_fpga.h"

qt_ni_fpga::qt_ni_fpga()
{
    config_fifo_elements = 0;
}

//Инициализация начальных параметров NI FPGA
uint32_t qt_ni_fpga::fpga_init(const uint16_t *spectrum, p_form pulse_form) {
    uint32_t fpga_p2p_endpoint = 0;

    fpga_stat = NiFpga_Initialize(); //Инициализация FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }
    fpga_stat = NiFpga_Open(bitfile, NiFpga_fpga_main_Signature, ni_fpga_resource, 0, &fpga_sess); //Открыть прошивку FPGA по адресу <bitfile> с сигнатурой NiFpga_fpga_main_Signature
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_Reset(fpga_sess); //Сброс всех регистров и FIFO
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_Run(fpga_sess,0); //Запуск FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, spectrum, SPEC_SIZE, 5000, &config_fifo_elements); //Записать спектр в FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_spectrum, 1); //Старт чтения спектра
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, pulse_form.pulse_form, P_FORM_SIZE, 5000, &config_fifo_elements); //Записать форму импульса в FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_form, 1); //Старт чтения формы импульса
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteU16(fpga_sess, NiFpga_fpga_main_ControlU16_TMode, TMode); //Записать режим воспроизведения в FPGA (0 - регулярные импульсы постоянной частоты, 1 - случайные импульсы, 2 - регулярные импульсы переменной частоты)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteU32(fpga_sess, NiFpga_fpga_main_ControlU64_Pulse_Coeff, PulseCoeff); //Записать коэффициент преобразования амплитуды импульса
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Noise, Noise); //Вкл/выкл шум
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteU16(fpga_sess, NiFpga_fpga_main_ControlU16_RMS, RMS); //RMS шума в кодах ЦАП
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteU64(fpga_sess, NiFpga_fpga_main_ControlU64_C1, C1); //Средний заряд в импульсе (FXP : 64, 0)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteU32(fpga_sess, NiFpga_fpga_main_ControlU32_Time, Time); //Количество тактов 1E-8 c, между чтениями буфера (обычно равно 10 для шага 1E-7 с), либо количество тактов между соседними импульсами для регулярных импульсов постоянной частоты
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Set_Wide, SetWide); //Флаг включения больших скоростей счёта
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteI64(fpga_sess, NiFpga_fpga_main_ControlI64_K0, K0); //Коэффициент K0 полинома БФШТ (FXP : 48, 3)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteI64(fpga_sess, NiFpga_fpga_main_ControlI64_K1, K1); //Коэффициент K1 полинома БФШТ (FXP : 48, 3)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteI64(fpga_sess, NiFpga_fpga_main_ControlI64_K2, K2); //Коэффициент K2 полинома БФШТ (FXP : 48, 3)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_WriteI64(fpga_sess, NiFpga_fpga_main_ControlI64_K3, K3); //Коэффициент K3 полинома БФШТ (FXP : 48, 3)
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_ConfigureFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoI16_host_pnum_fifo, host_fifo_depth); //Установить глубину буфера "host_fifo" равную <host_fifo_depth>
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_ConfigureFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU64_const_fifo, const_fifo_depth); //Установить глубину буфера "const_fifo" равную <const_fifo_depth>
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    fpga_stat = NiFpga_GetPeerToPeerFifoEndpoint(fpga_sess, NiFpga_fpga_main_PeerToPeerWriterFifoU32_p2p_fifo, &fpga_p2p_endpoint); //Получение адреса NI FPGA в транзакции P2P
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return 0;
    }

    return fpga_p2p_endpoint;
}

void qt_ni_fpga::fpga_reconfigure(const uint16_t *spectrum, p_form pulse_form) {
    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, spectrum, SPEC_SIZE, 5000, &config_fifo_elements);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_spectrum, 1);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, pulse_form.pulse_form, P_FORM_SIZE, 5000, &config_fifo_elements);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_form, 1);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_WriteU32(fpga_sess, NiFpga_fpga_main_ControlU64_Pulse_Coeff, PulseCoeff);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::fpga_close() {
    fpga_stat = NiFpga_StopFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_Reset(fpga_sess);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_Close(fpga_sess, 0);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}
//Запуск FPGA
void qt_ni_fpga::fpga_play() {
    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Play, 1); //Перейти в цикл воспроизведения сигнала
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}
//Остановка FPGA
void qt_ni_fpga::fpga_stop() {
    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Play, 0); //Выйти из цикла воспроизведения сигнала и перейти в режим ожидания
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::read_freq() {
    fpga_stat = NiFpga_ReadU64(fpga_sess, NiFpga_fpga_main_IndicatorU64_frequency, &Frequency);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::write_fifo(int16_t *buffer, size_t length, size_t *empty_elements){
    fpga_stat = NiFpga_WriteFifoI16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, buffer, length, 5000, empty_elements);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}
