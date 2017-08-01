#include "qt_ni_fpga.h"

qt_ni_fpga::qt_ni_fpga()
{
    config_fifo_elements = 0;
}

//Инициализация начальных параметров NI FPGA
NiFpga_Status qt_ni_fpga::fpga_init(uint16_t *spectrum, uint16_t *pulse_form) {
    fpga_stat = NiFpga_Initialize(); //Инициализация FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_Open(bitfile, NiFpga_fpga_main_Signature, ni_fpga_resource, 0, &fpga_sess); //Открыть прошивку FPGA по адресу <bitfile> с сигнатурой NiFpga_fpga_main_Signature
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_Reset(fpga_sess); //Сброс всех регистров и FIFO
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_Run(fpga_sess,0); //Запуск FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, spectrum, SPEC_SIZE, 5000, &config_fifo_elements); //Записать спектр в FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_spectrum, 1); //Старт чтения спектра
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_ReadU16(fpga_sess, NiFpga_fpga_main_IndicatorU16_state, &state);
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    while(state != 0) {
    fpga_stat = NiFpga_ReadU16(fpga_sess, NiFpga_fpga_main_IndicatorU16_state, &state);
        if(fpga_stat != NiFpga_Status_Success) {
            fpga_p2p_endpoint = 0;
            return fpga_stat;
        }
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, pulse_form, P_FORM_SIZE, 5000, &config_fifo_elements); //Записать форму импульса в FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_configure_form, 1); //Старт чтения формы импульса
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_ReadU16(fpga_sess, NiFpga_fpga_main_IndicatorU16_state, &state);
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    while(state != 0) {
    fpga_stat = NiFpga_ReadU16(fpga_sess, NiFpga_fpga_main_IndicatorU16_state, &state);
        if(fpga_stat != NiFpga_Status_Success) {
            fpga_p2p_endpoint = 0;
            return fpga_stat;
        }
    }

    fpga_stat = NiFpga_WriteFifoU16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU16_config_fifo, pulse_form, 0, 5000, &config_fifo_elements); //Записать форму импульса в FPGA
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_random, Random); //Записать коэффициент преобразования амплитуды импульса
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteU64(fpga_sess, NiFpga_fpga_main_ControlU64_pulse_coeff, PulseCoeff); //Записать коэффициент преобразования амплитуды импульса
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_rms_on, Noise); //Вкл/выкл шум
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteU16(fpga_sess, NiFpga_fpga_main_ControlU16_rms, RMS); //RMS шума в кодах ЦАП
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_ConfigureFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoI16_host_pnum_fifo, host_fifo_depth); //Установить глубину буфера "host_fifo" равную <host_fifo_depth>
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Play, 1); //Перейти в цикл воспроизведения сигнала
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    fpga_stat = NiFpga_GetPeerToPeerFifoEndpoint(fpga_sess, NiFpga_fpga_main_PeerToPeerWriterFifoU32_p2p_fifo, &fpga_p2p_endpoint); //Получение адреса NI FPGA в транзакции P2P
    if(fpga_stat != NiFpga_Status_Success) {
        fpga_p2p_endpoint = 0;
        return fpga_stat;
    }

    return fpga_stat;
}

void qt_ni_fpga::fpga_close() {
    fpga_stat = NiFpga_StopFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoI16_host_pnum_fifo);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

//    fpga_stat = NiFpga_StopFifo(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU64_const_fifo);
//    if(fpga_stat != NiFpga_Status_Success) {
//        return;
//    }

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

//Остановка FPGA
void qt_ni_fpga::fpga_stop() {
    fpga_stat = NiFpga_WriteBool(fpga_sess, NiFpga_fpga_main_ControlBool_Play, 0); //Выйти из цикла воспроизведения сигнала и перейти в режим ожидания
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::read_freq() {
    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_host_valid, &host_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_pnum_valid, &pnum_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_w_valid, &w_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_fir_valid, &fir_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_raw_signal_valid, &raw_signal_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadBool(fpga_sess, NiFpga_fpga_main_IndicatorBool_ch0_valid, &ch0_valid);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }

    fpga_stat = NiFpga_ReadU16(fpga_sess, NiFpga_fpga_main_IndicatorU16_state, &state);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::write_fifo(int16_t *buffer, size_t length, size_t *empty_elements){
    fpga_stat = NiFpga_WriteFifoI16(fpga_sess, NiFpga_fpga_main_HostToTargetFifoI16_host_pnum_fifo, buffer, length, 5000, empty_elements);
    if(fpga_stat != NiFpga_Status_Success) {
        return;
    }
}

void qt_ni_fpga::write_const_fifo(uint64_t *buffer, size_t length, size_t *empty_elements){
//    fpga_stat = NiFpga_WriteFifoU64(fpga_sess, NiFpga_fpga_main_HostToTargetFifoU64_const_fifo, buffer, length, 5000, empty_elements);
//    if(fpga_stat != NiFpga_Status_Success) {
//        return;
//    }
}

