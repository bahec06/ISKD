#include "qt_ni_fgen.h"

qt_ni_fgen::qt_ni_fgen()
{
}

//Инициализация начальных параметров NI FGEN
ViStatus qt_ni_fgen::fgen_init() {
    //Инициализация ресурса <ni_fgen_resource>
    error = niFgen_init(ni_fgen_resource, VI_TRUE, VI_TRUE, &fgen_vi);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }

    //Конфицгурация каналов <channels>
    error = niFgen_ConfigureChannels(fgen_vi, channels);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Инициализация каналов <channels>
    error = niFgen_ConfigureOutputEnabled(fgen_vi, channels, VI_TRUE);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Установка частоты дискретизации ЦАП <sample rate>
    error = niFgen_ConfigureSampleRate(fgen_vi, sample_rate);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Режим воспроизведения скрипта
    error = niFgen_ConfigureOutputMode(fgen_vi, NIFGEN_VAL_OUTPUT_SCRIPT);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Дифференциальный выход для 0 канала
    error = niFgen_SetAttributeViInt32(fgen_vi, "0", NIFGEN_ATTR_TERMINAL_CONFIGURATION, NIFGEN_VAL_DIFFERENTIAL);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Установка сопротивления нагрузки <load_imp0> для 0 канала
    error = niFgen_SetAttributeViReal64(fgen_vi, "0", NIFGEN_ATTR_LOAD_IMPEDANCE, load_imp0);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Синфазный выход для 1 канала
    error = niFgen_SetAttributeViInt32(fgen_vi, "1", NIFGEN_ATTR_TERMINAL_CONFIGURATION, NIFGEN_VAL_SINGLE_ENDED);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Установка сопротивления нагрузки <load_imp1> для 1 канала
    error = niFgen_SetAttributeViReal64(fgen_vi, "1", NIFGEN_ATTR_LOAD_IMPEDANCE, load_imp1);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Установка коэффициента усиления gain для всех каналов
    error = niFgen_SetAttributeViReal64(fgen_vi, channels, NIFGEN_ATTR_ARB_GAIN, gain);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Инициализация P2P
    error = niFgen_SetAttributeViBoolean(fgen_vi, p2p_endp_chan, NIFGEN_ATTR_P2P_ENABLED, VI_TRUE);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Приемные каналы P2P равны <channels>
    error = niFgen_SetAttributeViString(fgen_vi, p2p_endp_chan, NIFGEN_ATTR_P2P_DESTINATION_CHANNELS, channels);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Триггер начала воспроизведения при нулевом количестве элементов
    error = niFgen_ConfigureP2PEndpointFullnessStartTrigger(fgen_vi, -1);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Записать скрипт <script>
    error = niFgen_WriteScript(fgen_vi, channels, script.toLocal8Bit().data());
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Фиксирование параметров
    error = niFgen_Commit(fgen_vi);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }
    //Получение адреса NI FGEN в транзакции P2P
    error = niFgen_GetStreamEndpointHandle(fgen_vi, p2p_endp_chan, &fgen_p2p_endpoint);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        fgen_p2p_endpoint = 0;
        return error;
    }

    return error;
}
//Запуск генератора
ViStatus qt_ni_fgen::start_generation() {
    error = niFgen_InitiateGeneration(fgen_vi);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        return error;
    }

    return error;
}
//Остановка генератора
ViStatus qt_ni_fgen::fgen_stop(){
    error = niFgen_ConfigureOutputEnabled(fgen_vi, channels, VI_FALSE);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        return error;
    }

    error = niFgen_AbortGeneration(fgen_vi);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        return error;
    }

    error = niFgen_close(fgen_vi);
    if(error != VI_SUCCESS) {
        niFgen_ErrorHandler(fgen_vi, error, errMsg);
        return error;
    }

    return error;
}
