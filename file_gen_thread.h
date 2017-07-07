#ifndef FILE_GEN_THREAD_H
#define FILE_GEN_THREAD_H

#include "wr_file_loops.h"
#include "service_types.h"

class file_gen_thread : public QObject
{
    Q_OBJECT
public:
    explicit file_gen_thread(QObject *parent = nullptr);
    wr_file_loops *file_gen;
    int model_index, mode_index;
    double S, A, T, R0, R, TT, Kt, A_R;
    enrichment_enum Enrch;
    yield_enum En_Y;
    pu_u_enum Pu_U;
    r_models r_mod;

signals:
    void finished();
    void send_bar(int);
public slots:
    void process();
    void update_index(int);
};

#endif // FILE_GEN_THREAD_H
