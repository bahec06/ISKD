#include "bgif_generator.h"

bgif_generator::bgif_generator()
{
    stop_indicator = true;
    read_file.setFileName("D:/Qt_repo/time_file.bin");
    read_stream.setDevice(&read_file);
    data_to_fifo = new int16_t[fpga.host_fifo_depth];
    wfifo_length = fpga.host_fifo_depth;

    spectrum = (uint16_t *)malloc(2*SPEC_SIZE);
    pulse_form = (uint16_t *)malloc(2*P_FORM_SIZE);
}

//Инициализация начальных параметров FPGA
void bgif_generator::init_fpga_params() {
    fpga.TMode = mode; //Запись режима воспроизведения
    //0 - регулярные импульсы постоянной частоты
    //1 - случайные импульсы или регулярные импульсы переменной частоты
    if(mode == 0) {
        fpga.Time = (uint32_t)(1/(regular_freq*dt)); //Количестов интервало времени 1E-8 с для заданной частоты
    }
    else {
        fpga.Time = 10; //FIFO читается каждые 10 тактов 1E-8 с, что соотвтетствует 1E-7 с
    }
    fpga.SetWide = NiFpga_False; //Флаг воспроизведения высоких скоростей счёта
    fpga.RMS = 0; //Дисперсия шума в кодах ЦАП
    fpga.PulseCoeff = (uint32_t)(correction_gain*pow(2,32)); //Коэффициент преобразования амплитуды импульса (FXP : 34, 2)
    fpga.Noise = NiFpga_False; //Флаг включения/отключения шума
    fpga.C1 = (uint64_t)(mean_charge*pow(2,64)); //Средний заряд в импульсе (FXP : 64, 0)
    k0 = 0;
    k1 = 0;
    k2 = 0;
    k3 = 0;
    fpga.K0 = (int64_t)(k0*pow(2,45)); //Коэффициент K0 полинома БФШТ (FXP : 48, 3)
    fpga.K1 = (int64_t)(k1*pow(2,45)); //Коэффициент K1 полинома БФШТ (FXP : 48, 3)
    fpga.K2 = (int64_t)(k2*pow(2,45)); //Коэффициент K2 полинома БФШТ (FXP : 48, 3)
    fpga.K3 = (int64_t)(k3*pow(2,45)); //Коэффициент K3 полинома БФШТ (FXP : 48, 3)
}

//Запуск воспроизведения сигнала
void bgif_generator::start_generation()
{
    p2p = new qt_ni_p2p();
    init_fpga_params(); //Инициализация начальных параметров FPGA

    //Инициализация начальных параметров NI FGEN
    if(fgen.fgen_init() != VI_SUCCESS) {
        emit fgen_error();
        return;
    }
    else {
        fgen_p2p_endpoint = fgen.fgen_p2p_endpoint;
    }

    //Инициализация FPGA
    if(fpga.fpga_init(spectrum, pulse_form) != NiFpga_Status_Success) {
        emit fpga_error();
        return;
    }
    else {
        fpga_p2p_endpoint = fpga.fpga_p2p_endpoint;
    }

    p2p->CreateAndLinkStream(fpga_p2p_endpoint, fgen_p2p_endpoint); //Создать связь P2P между NI FPGA и NI FGEN
    //Запуск генератора
    if(fgen.start_generation() != VI_SUCCESS) {
        emit fgen_error();
        return;
    }

    stop_indicator = false;
    //read_file.open(QFile::ReadOnly);
    emit update_play(true);
    while(!stop_indicator) {
        //if(mode > 0) { //Для всех режимов, кроме регулярных импульсов постоянной частоты
        //    read_stream.readRawData((char *) data_to_fifo, wfifo_length); //Читаем данные из файла
        //    fpga.write_fifo(data_to_fifo, wfifo_length, &empty_elements); //Записываем прочитанные данные в FIFO и читаем количество пустых элементов в empty_elements
        //    wfifo_length = empty_elements; //Количество элементов для чтения равно количество пустых элементов в FIFO
        //}

        //Добавить const_fifo !!!!!!!!!!!!!

        //Читать по таймеру!!!
        fpga.read_freq(); //Читаем текущее значение скорости счёта
        emit send_frequency(fpga.Frequency); //Отправить сигнал обновления текущего значения частоты
    }
    stop_generation();
    emit generation_finished(); //Отправить сигнал окончания воспроизведения
}
//Остановка воспроизведения сигнала
void bgif_generator::stop_generation()
{
    fpga.fpga_stop(); //Остановка FPGA
    fpga.fpga_close();
    fgen.fgen_stop(); //Остановка генератора
    p2p->FlushAndDisableStream(); //Очистить и отключить поток P2P
    p2p->DestroyStream();//Уничтожить поток P2P
    emit update_play(false);
}
