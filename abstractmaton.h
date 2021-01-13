#ifndef ABSTRACTMATON_H
#define ABSTRACTMATON_H

#include <QString>
#include <QStringList>
#include <QVector>

struct Func {
    QString from;
    QString to;
    QString input;
    Func(QString from, QString to, QString input):from(from),to(to),input(input){}
};

struct AbstractMaton
{
    AbstractMaton();
    QString s_init_state;                   // 初态
    QStringList sl_state_set;               // 状态集
    QStringList sl_input_list;              // 输入符号表
    QStringList sl_final_state_set;         // 终态集
    QVector<Func> func_list;                // 转换函数
    void clear();
    bool add_state_set(QString state);
    bool set_init_state(QString state);
    bool add_final_state(QString state);
    bool add_func(Func f);
};

#endif // ABSTRACTMATON_H
