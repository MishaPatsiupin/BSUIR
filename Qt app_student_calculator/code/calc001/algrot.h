#ifndef TEST_ROT_ALGROT_H
#define TEST_ROT_ALGROT_H

#include <vector>
#include <iostream>
#include <string>
#include "log.h"

class algrot : public log {
private:
    std::vector<std::string> Ai;                                //Итый остаток после операции умножения
    std::vector<std::pair<std::string, int>> Ci;                 //итое множество C
    std::vector<std::string> Z;                                 //Множество простых импликант
    std::vector<std::string> Bi;                                //Итое множество B
    std::vector<std::pair<std::string, std::string>> Lex;        //наборы которые могут быть L-екстремалями
    std::vector<std::string> E;                                 //L - экстремали
    std::vector<std::string> NotCoverZ;                         //наборы которые не покрыты множеством Z



public:
    log log;
    std::vector<std::string> L;                                 //Множество L
    std::vector<std::string> N;                                 //Множество N
    std::vector<std::vector<std::string>> Fmin;   //минимальные функции
    algrot();
    algrot(const std::vector<std::string> &L_, const std::vector<std::string> &N_);
    void AiOst();
    void inpCi();
    void correctAi();
    void resBi();
    void resZ();
    void correctZ();
    void resCi();
    void canBeLext();
    void findLExtr();
    void CorrectZ_forVetvl();
    void NotcoverZ();
    void resFmin(bool alg);
    void getE();
    void getNoCoverZ();
    void getCi();
    void getZ();
    void getAi();
    void getBi();
    void getN();
    void getL();
    void getSetsMbLex();
    void getFmins();
    void AlgRot();
    void clrar();
    ~algrot() = default;
};


#endif //TEST_ROT_ALGROT_H
