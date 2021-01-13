#include "abstractmaton.h"

AbstractMaton::AbstractMaton()
{

}

void AbstractMaton::clear() {
    s_init_state = "";                   // 初态
    sl_state_set.clear();               // 状态集
    sl_input_list.clear();              // 输入符号表
    sl_final_state_set.clear();         // 终态集
    func_list.clear();
}

bool AbstractMaton::add_state_set(QString state) { // 添加状态
    for (const QString &s : sl_state_set)  // 不能重复
        if (s == state) return false;

    sl_state_set.append(state);
    return true;
}

bool AbstractMaton::set_init_state(QString state) { // 设置初态
    bool res = s_init_state.isEmpty();
    s_init_state = state;
    return res;
}

bool AbstractMaton::add_final_state(QString state) { // 添加终态
    for (const QString &s : sl_final_state_set)  // 不能重复
        if (s == state) return false;

    sl_final_state_set.append(state);
    return true;
}

bool AbstractMaton::add_func(Func f) {
    //sl_state_set.append(f);
    return true;
}
