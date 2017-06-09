#include "qt_ni_fgen.h"

qt_ni_fgen::qt_ni_fgen()
{
    strcpy(ni_fgen_resource, "PXI2Slot2");
}

//Инициализация начальных параметров NI FGEN
ViUInt32 qt_ni_fgen::fgen_init() {
    ViUInt32 fgen_p2p_endpoint = 0;

    checkErr(niFgen_init(ni_fgen_resource,VI_TRUE, VI_TRUE, &fgen_vi)); //Инициализация ресурса <ni_fgen_resource>
    checkErr(niFgen_ConfigureChannels(fgen_vi, channels)); //Конфицгурация каналов <channels>
    checkErr(niFgen_ConfigureOutputEnabled(fgen_vi, channels, VI_TRUE)); //Инициализация каналов <channels>
    checkErr(niFgen_ConfigureOutputMode(fgen_vi, NIFGEN_VAL_OUTPUT_SCRIPT)); //Режим воспроизведения скрипта
    checkErr(niFgen_ConfigureSampleRate(fgen_vi, sample_rate)); //Установка частоты дискретизации ЦАП <sample rate>
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "0", NIFGEN_ATTR_TERMINAL_CONFIGURATION, NIFGEN_VAL_DIFFERENTIAL)); //Дифференциальный выход для 0 канала
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "1", NIFGEN_ATTR_TERMINAL_CONFIGURATION, NIFGEN_VAL_SINGLE_ENDED)); //Синфазный выход для 1 канала
    checkErr(niFgen_SetAttributeViReal64(fgen_vi, "0", NIFGEN_ATTR_LOAD_IMPEDANCE, load_imp0)); //Установка сопротивления нагрузки <load_imp0> для 0 канала
    checkErr(niFgen_SetAttributeViReal64(fgen_vi, "1", NIFGEN_ATTR_LOAD_IMPEDANCE, load_imp1)); //Установка сопротивления нагрузки <load_imp1> для 1 канала
    checkErr(niFgen_SetAttributeViReal64(fgen_vi, "0,1", NIFGEN_ATTR_ARB_GAIN, gain)); //Установка коэффициента усиления gain для всех каналов
    checkErr(niFgen_SetAttributeViBoolean(fgen_vi, p2p_endp_chan, NIFGEN_ATTR_P2P_ENABLED, VI_TRUE)); //Инициализация P2P
    checkErr(niFgen_SetAttributeViString(fgen_vi, p2p_endp_chan, NIFGEN_ATTR_P2P_DESTINATION_CHANNELS, channels)); //Приемные каналы P2P равны <channels>
    checkErr(niFgen_ConfigureP2PEndpointFullnessStartTrigger(fgen_vi, 0)); //Триггер начала воспроизведения при нулевом количестве элементов
    checkErr(niFgen_WriteScript(fgen_vi, channels, script)); //Записать скрипт <script>
    checkErr(niFgen_Commit(fgen_vi)); //Фиксирование параметров
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "1", NIFGEN_ATTR_IDLE_BEHAVIOR, NIFGEN_VAL_JUMP_TO_VALUE)); //По умолчанию выходное значение генератора равно IDLE_VALUE
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "1", NIFGEN_ATTR_IDLE_VALUE, -32767)); //Значение IDLE_VALUE равно -32767
    //Нововведение (настройка нуля для нулевого канала)
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "0", NIFGEN_ATTR_COMMON_MODE_OFFSET, CMO)); //Синфазное смещение
    checkErr(niFgen_SetAttributeViInt32(fgen_vi, "0", NIFGEN_ATTR_ARB_OFFSET, DO)); //Дифферециальное смещение
    //
    checkErr(niFgen_GetStreamEndpointHandle(fgen_vi, p2p_endp_chan, &fgen_p2p_endpoint)); //Получение адреса NI FGEN в транзакции P2P

    //Если одна из функций не сработала, ошибка выводится в MessageBox <msg>
    Error:
        if(error != VI_SUCCESS) {
           ViChar errMsg[256];
           niFgen_ErrorHandler(fgen_vi, error, errMsg);
           msg.setText(QString(errMsg));
           msg.exec();
           fgen_p2p_endpoint = 0;
        }

    return fgen_p2p_endpoint;
}
//Запуск генератора
void qt_ni_fgen::start_generation() {
    checkErr(niFgen_InitiateGeneration(fgen_vi));

    Error:
        if(error != VI_SUCCESS) {
           ViChar errMsg[256];
           niFgen_ErrorHandler(fgen_vi, error, errMsg);
           msg.setText(QString(errMsg));
           msg.exec();
        }
}
//Остановка генератора
void qt_ni_fgen::fgen_stop(){
    checkErr(niFgen_AbortGeneration(fgen_vi));
    checkErr(niFgen_close(fgen_vi));

    Error:
        if(error != VI_SUCCESS) {
           ViChar errMsg[256];
           niFgen_ErrorHandler(fgen_vi, error, errMsg);
           msg.setText(QString(errMsg));
           msg.exec();
        }
}
