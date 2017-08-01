#include "bgif_generator.h"

bgif_generator::bgif_generator()
{
    stop_indicator = true;
    read_file.setFileName("D:/Qt_repo/time_file.bin");
    low_file.setFileName("D:/Qt_repo/ls_file.bin");
    read_stream.setDevice(&read_file);
    low_stream.setDevice(&low_file);
    data_to_fifo = new int16_t[fpga.host_fifo_depth];
    const_data = new uint64_t[fpga.const_fifo_depth];
    wfifo_length = fpga.host_fifo_depth;
    cfifo_length = fpga.const_fifo_depth;

    spectrum = (uint16_t *)malloc(2*SPEC_SIZE);
    pulse_form = (uint16_t *)malloc(2*P_FORM_SIZE);
}

//Инициализация начальных параметров FPGA
void bgif_generator::init_fpga_params() {
    fpga.TMode = mode; //Запись режима воспроизведения
    //0 - регулярные импульсы постоянной частоты
    //1 - случайные импульсы
    //2 - регулярные импульсы переменной частоты
    fpga.Two_Channels = (NiFpga_Bool)(mode == 1);
    if(mode == 1) {
        //fpga.Time = (uint32_t)(1/(regular_freq*dt)); //Количестов интервалов времени 1E-8 с для заданной частоты
        fpga.Random = NiFpga_True;
    }
    else {
        //fpga.Time = 10; //FIFO читается каждые 10 тактов 1E-8 с, что соотвтетствует 1E-7 с
        fpga.Random = NiFpga_False;
    }
    fpga.SetWide = NiFpga_False; //Флаг воспроизведения высоких скоростей счёта
    fpga.RMS = 0; //Дисперсия шума в кодах ЦАП
    fpga.PulseCoeff = (uint32_t)(correction_gain/4*pow(2,32)); //Коэффициент преобразования амплитуды импульса (FXP : 34, 2)
    fpga.Noise = NiFpga_False; //Флаг включения/отключения шума
    fpga.C1 = (uint64_t)(mean_charge*pow(2,64)); //Средний заряд в импульсе (FXP : 64, 0)
    k0 = 1.5;
    k1 = 0.52249;
    k2 = -0.035581;
    k3 = 0.02625;
    fpga.K0 = (int64_t)(k0*pow(2,45)); //Коэффициент K0 полинома БФШТ (FXP : 48, 3)
    fpga.K1 = (int64_t)(k1*pow(2,45)); //Коэффициент K1 полинома БФШТ (FXP : 48, 3)
    fpga.K2 = (int64_t)(k2*pow(2,45)); //Коэффициент K2 полинома БФШТ (FXP : 48, 3)
    fpga.K3 = (int64_t)(k3*pow(2,45)); //Коэффициент K3 полинома БФШТ (FXP : 48, 3)

    fpga.CountVal1 = 1000;
    fpga.CountVal2 = 800;
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

    emit start_read();
    p2p->CreateAndLinkStream(fpga_p2p_endpoint, fgen_p2p_endpoint); //Создать связь P2P между NI FPGA и NI FGEN

    stop_indicator = false;
    read_file.open(QFile::ReadOnly);
    //low_file.open(QFile::ReadOnly);

    //cfifo_length = low_stream.readRawData((char*) const_data, cfifo_length*8);
    //fpga.write_const_fifo(const_data, cfifo_length/8, &const_empty_elements);
    //cfifo_length = const_empty_elements;
    wfifo_length = read_stream.readRawData((char *) data_to_fifo, wfifo_length*2); //Читаем данные из файла
    fpga.write_fifo(data_to_fifo, wfifo_length/2, &empty_elements); //Записываем прочитанные данные в FIFO и читаем количество пустых элементов в empty_elements
    wfifo_length = empty_elements; //Количество элементов для чтения равно количество пустых элементов в FIFO

    //Запуск генератора
    if(fgen.start_generation() != VI_SUCCESS) {
        emit fgen_error();
        return;
    }

    emit update_play(true);
    emit start_read();
    while(!stop_indicator) {
        wfifo_length = read_stream.readRawData((char *) data_to_fifo, wfifo_length*2); //Читаем данные из файла
        fpga.write_fifo(data_to_fifo, wfifo_length/2, &empty_elements); //Записываем прочитанные данные в FIFO и читаем количество пустых элементов в empty_elements
        wfifo_length = empty_elements; //Количество элементов для чтения равно количество пустых элементов в FIFO

        //cfifo_length = low_stream.readRawData((char*) const_data, cfifo_length*8);
        //fpga.write_const_fifo(const_data, cfifo_length/8, &const_empty_elements);
        //cfifo_length = const_empty_elements;
        niFgen_GetError(fgen.fgen_vi, &fgen.error, 256, fgen.errMsg);
        //niFgen_ErrorHandler(fgen.fgen_vi, fgen.error, fgen.errMsg);
        if(fgen.error != VI_SUCCESS) {
            int c = 0;
        }
        //Циклическое воспроизведение
        if(read_file.atEnd()) read_file.seek(0);
        //if(low_file.atEnd()) low_file.seek(0);
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
