#include "algrot.h"
#include <vector>
#include <iostream>
#include <string>


void algrot::inpCi() {
    for (int i = 0; i < N.size(); ++i) {
        if (N[i] != "") { Ci.push_back(std::make_pair(N[i], 0)); }
    }
    for (int i = 0; i < L.size(); ++i) {
        Ci.push_back(std::make_pair(L[i], 0));
    }
}

algrot::algrot()  {
    L = { "" };
    N = { "" };
    Ai = { "" };
}

algrot::algrot(const std::vector<std::string>& L_, const std::vector<std::string>& N_){
        L = L_;
        N = N_;
        Ai = { "" };
        inpCi();
    }

void algrot::AiOst()  {                                                                                           //функция для поиска кубов большей размерности после операции *

    Ai.clear();

    static int it = 0;
    std::string res;
    int counter = 0;
    for (int i = 0; i < Ci.size(); ++i) {
        for (int j = i + 1; j < Ci.size(); ++j){
            res = "";
            for (int k = 0; k < Ci[i].first.size();  ++k) {                                                //функция умножения и добавления в вектор отстатков
                if (Ci[i].first.at(k) == '0' && Ci[j].first.at(k) == '0') { res.push_back('0'); }
                else if (Ci[i].first.at(k) == '0' && Ci[j].first.at(k) == '1') { res.push_back('y'); ++counter; }
                else if (Ci[i].first.at(k) == '1' && Ci[j].first.at(k) == '0') { res.push_back('y'); ++counter; }
                else if (Ci[i].first.at(k) == '1' && Ci[j].first.at(k) == '1') { res.push_back('1'); }
                else if (Ci[j].first.at(k) == 'x') { res.push_back(Ci[i].first.at(k)); }
                else if (Ci[i].first.at(k) == 'x') { res.push_back(Ci[j].first.at(k)); }
                if (counter > 1) { res = "-"; }
            }
            counter = 0;
            if (res != "-") {
                for (int k = 0; k < res.size(); ++k) { if (res[k] == 'y') res[k] = 'x'; }                  //замена y на x

                int count_of_X = 0;
                for (int k = 0; k < res.size(); ++k) { if (res[k] == 'x') { ++count_of_X; } }
                if (count_of_X > it) {                                                                    //проверка на количество x
                    ++Ci[i].second;
                    ++Ci[j].second;
                    Ai.push_back(res);
                }
            }
        }
    }
    correctAi();
    ++it;
}

void algrot::correctAi()  {                         //корекция множества Ai
    for (int i = 0; i < Ai.size(); ++i) {
        for (int j = i + 1; j < Ai.size(); ++j) {
            if (Ai[i] == Ai[j]) {
                Ai.erase(Ai.begin() + j);
            }
        }
    }
}

void algrot::resBi()  {                                          //заполнения множества Bi
    Bi.clear();
    if (Z.empty()) {
        for (int i = 0; i < Ci.size(); i++) {
            Bi.push_back(Ci[i].first);
        }

        return;
    }
    bool fl = true;
    int counter = 0;
    for (int i = 0; i < Ci.size(); ++i) {
        for (int j = 0; j < Z.size(); ++j) {
            for (int k = 0; k < Ci[i].first.size(); ++k) {
                if (Ci[i].first.at(k) == Z[j].at(k)) {
                    continue;
                }
                else if (Z[j].at(k) == 'x') {
                    counter++;
                }
            }
        }
        if (!counter) {
            Bi.push_back(Ci[i].first);
        }
        counter = 0;
    }
}

void algrot::resZ()  {
    for (int i = 0; i < Ci.size(); ++i) {                //заполнение множества Z
        if (Ci[i].second == 0) {
            Z.push_back(Ci[i].first);
        }
    }
    correctZ();
}

void algrot::correctZ() {                                       //коррекция множествв Z
    for (int i = 0; i < Z.size(); ++i) {
        for (int j = i + 1; j < Z.size(); ++j) {
            if (Z[i] == Z[j]) { Z.erase(Z.begin() + (j--)); }
        }
    }
}

void algrot::resCi()  {                          //метод для поиска множества Ci

    Ci.clear();

    for (int i = 0; i < Ai.size(); ++i) {
        Ci.push_back(std::make_pair(Ai[i], 0));
    }

    for (int i = 0; i < Bi.size(); ++i) {
        for (int j = 0; j < Ci.size(); ++j) {
            for (int k = 0; k < Ci[j].first.size(); ++k) {

                if (Ci[j].first.at(k) == Bi[i].at(k)) { continue; }
                if (Ci[j].first.at(k) == 'x') { continue; }
                else {
                    Ci[j].second += 1;
                    break;
                }

            }
        }

        int count = 0;
        for (int j = 0; j < Ci.size(); ++j) {
            if (Ci[j].second != 0) { ++count; }
        }
        for (int j = 0; j < Ci.size(); ++j) {
            Ci[j].second = 0;
        }
        if (count == Ci.size()) { Ci.push_back(std::make_pair(Bi[i], 0)); }
    }
}

void algrot::canBeLext()  {                       //метод для поиска возможных L-екстремалей

    std::vector<std::string> Ost;
    std::vector<std::string> NewOst;
    std::string tempOst;
    int counter = 0;

    for (int i = 0; i < Z.size(); ++i) {
        Ost.push_back(Z[i]);


        for (int j = 0; j < Z.size(); ++j) {
            if (j == i) continue;

            for (int l = 0; l < Ost.size(); ++l) {

                for (int k = 0; k < Z[j].size(); ++k) {


                    if (Ost[l].at(k) == Z[j].at(k)) { tempOst.push_back('z'); ++counter; continue; }
                    else if (Z[j].at(k) == 'x') { tempOst.push_back('z'); ++counter; continue; }
                    else if (Ost[l].at(k) == '0' && Z[j].at(k) == '1' || Ost[l].at(k) == '1' && Z[j].at(k) == '0') { tempOst.push_back('y'); continue; }
                    else if (Ost[l].at(k) == 'x' && Z[j].at(k) == '0') { tempOst.push_back('1'); continue; }
                    else if (Ost[l].at(k) == 'x' && Z[j].at(k) == '1') { tempOst.push_back('0'); continue; }

                }

                if (counter == tempOst.size()) { break; }

                std::string OstChanged;
                bool fl = false;
                for (int k = 0; k < tempOst.size(); ++k) {

                    if (tempOst[k] == 'y') { fl = true; NewOst.push_back(Ost[l]); break; }

                }

                if (fl) { break; }

                for (int k = 0; k < tempOst.size(); ++k) {

                    if (tempOst[k] == '1' || tempOst[k] == '0') {

                        OstChanged = Ost[l];
                        OstChanged[k] = tempOst[k];
                        NewOst.push_back(OstChanged);
                        OstChanged.clear();

                    }
                }
            }
            counter = 0;
            Ost = NewOst;
            NewOst.clear();
            tempOst.clear();
        }

        if (!Ost.empty()) {
            Lex.push_back(std::make_pair(Z[i], Ost[0]));
            Ost.clear();
        }
    }
}

void algrot::findLExtr()  {                              //поиск L-екстремалей
    bool fl;
    for (int i = 0; i < Lex.size(); ++i) {
        for (int j = 0; j < L.size(); ++j) {
            fl = true;
            if (Lex[i].second == L[j]) {
                E.push_back(Lex[i].first);
                break;
            }
            for (int k = 0; k < Lex[i].second.size(); ++k) {
                if (Lex[i].second.at(k) == L[j].at(k)) { continue; }
                if (Lex[i].second.at(k) == 'x') { continue; }
                else if (Lex[i].second.at(k) == '1' && L[j].at(k) == '0' || L[j].at(k) == '1' && Lex[i].second.at(k) == '0') { fl = false; break; }
            }
            if (fl) {
                E.push_back(Lex[i].first);
                break;
            }

        }
    }
}

void algrot::CorrectZ_forVetvl() {                   //алгоритм ветвления
    Z[Z.size() - 1] = Z[Z.size() - 2];
}

void algrot::NotcoverZ() {                            //метод для поиска кубов которые не покрывают
    std::vector<std::pair<std::string, int>> l_;
    int counter;
    for (int i = 0; i < L.size(); ++i) { l_.push_back(std::make_pair(L[i], 0)); }
    for (int i = 0; i < l_.size(); ++i) {
        for (int j = 0; j < E.size(); ++j) {
            counter = 0;
            for (int k = 0; k < E[j].size(); ++k) {

                if (l_[i].first.at(k) == E[j].at(k)) { ++counter; }
                else if (l_[i].first.at(k) != 'x' && E[j].at(k) == 'x') { ++counter; }

            }
            if (counter == E[j].size()) { break; }
            else { ++l_[i].second; }
        }
    }

    for (int i = 0; i < l_.size(); ++i) {
        if (l_[i].second >= E.size()) { NotCoverZ.push_back(l_[i].first); }
    }
}

void algrot::resFmin(bool alg = 0) {                   //метод для нахождения минимального покрытия множества L множеством Z

    std::vector<std::string> fmin1;
    std::vector<std::pair<std::string, std::vector<std::string>>> Z_;                   //множество Z без L-екстремалей

    std::vector<std::string> k;
    for (int i = 0; i < Z.size(); ++i) { Z_.push_back(std::make_pair(Z[i], k)); }

    for (int i = 0; i < E.size(); ++i) {
        for (int j = 0; j < Z_.size(); ++j) {
            if (E.at(i) == Z_.at(j).first) {
                Z_.erase(Z_.begin() + j);
            }
        }
    }

    std::vector<std::string> FminI;
    bool fl = true;
    int maxSize = 0;


    for (int i = 0; i < Z_.size(); ++i) {
        for (int j = 0; j < NotCoverZ.size(); ++j) {

            for (int k = 0; k < NotCoverZ[j].size(); ++k) {

                if (Z_[i].first.at(k) != NotCoverZ[j].at(k) && Z_[i].first.at(k) != 'x') {
                    fl = false;
                }
            }


            if (fl) {            //покрытый набор Zi-тым из множества непокрытых наборов
                Z_[i].second.push_back(NotCoverZ[j]);
            }

            fl = true;
        }
    }

    for (int i = 0; i < Z_.size(); ++i) {                   //корекция множества Z_
        for (int j = i + 1; j < Z_.size(); ++j) {
            if (Z_[i].first == Z_[j].first) {
                Z_.erase(Z_.begin() + j);
            }
        }
    }

    for (int i = 0; i < Z_.size(); ++i) {                   //поиск максимального числа покрываемых импликант из множества L1
        if (Z_[i].second.size() > maxSize) {
            maxSize = (int)Z_[i].second.size();
        }
    }
    std::vector<std::string> NotCov = NotCoverZ;

    std::vector<std::string> buf;
    for (maxSize; maxSize > 0; --maxSize) {                          //поиск импликант которые полностью покрывают множество                                                                       непокрытых имликант
        for (int i = 0; i < Z_.size(); ++i) {
            for (int j = 0; j < Z_[i].second.size(); ++j) {
                if (Z_[i].second.size() != maxSize) { continue; }
                for (int k = 0; k < NotCov.size(); ++k) {

                    if (NotCov[k] == Z_[i].second[j]) {

                        buf.push_back(Z_[i].first);
                        NotCov.erase(NotCov.begin() + k);
                        break;

                    }
                }
            }
        }
        if (NotCov.size() == 0) { break; }
    }

    for (int i = 0; i < buf.size(); ++i) {
        for (int j = i + 1; j < buf.size(); ++j) {
            if (buf[i] == buf[j]) {
                buf.erase(buf.begin() + j);
            }
        }
    }

    if (alg) {
        fl = true;
        std::string bigCube;
        for (int i = 0; i < buf.size(); ++i) {
            for (int j = 0; j < buf.size(); ++j) {
                if (i == j) { continue; }
                if (bigCube.empty()) { bigCube = buf[j]; }
                for (int k = 0; k < bigCube.size(); ++k) {
                    if (buf[j].at(k) == 'x' || bigCube.at(k) == 'x') { bigCube[k] = 'x'; }
                    if (buf[j].at(k) == '0' && bigCube.at(k) == '1' || buf[j].at(k) == '1' && bigCube.at(k) == '0') { bigCube[k] = 'x'; }
                }
            }

            for (int j = 0; j < bigCube.size(); ++j) {
                if (bigCube[j] != 'x' && buf[i].at(j) == 'x') { fl = false; break; }
                if (bigCube[j] != buf[i].at(j) && bigCube[j] != 'x') { fl = false; break; }
            }

            if (fl) { buf.erase(buf.begin() + i); }


            fl = true;
            bigCube.clear();
        }

    }

    for (int i = 0; i < E.size(); ++i) { buf.push_back(E[i]); }

    Fmin.push_back(buf);
}

void algrot::getE() {                        //метод для вывод на экран L-экстремалей
    if (E.empty()) { log.f_log_text("множество L-екстремалей пустое\n") ; return; }
    log.f_log_vec("", E);
}

void algrot::getNoCoverZ() {                            //метод для вывода на экрна
    if (NotCoverZ.empty()) { log.f_log_text("множество имликант которые не покрыты пустое\n"); return; }
    log.f_log_vec("", NotCoverZ);
}

void algrot::getCi()  {                        //метод для вывода на экран множества Ci
    if (Ci.empty()) { log.f_log_text("множество Ci - пустое\n"); }
    for (int i = 0; i < Ci.size(); ++i) { f_log_text("{");f_log_text(Ci[i].first);f_log_text("}, "); }

    f_log_text("\n");
}

void algrot::getZ()  {                         //метод для вывода на экран множества Z
    if (!Z.empty()) { f_log_vec("", Z); }
    if (Z.empty()) { f_log_text("множество Z - пустое\n"); return; }
}

void algrot::getAi() {                          //метод для вывода
    if (Ai.empty()) { f_log_text("множество Ai - пустое\n"); return; }
    f_log_vec("", Ai);
}

void algrot::getBi(){                        //метод для вывода на экран множества Bi
    if (Bi.empty()) { f_log_text("множество Bi - пустое\n"); return; }
    f_log_vec("",Bi);
}

void algrot::getN() {                         //метод для вывада на экран не обязательного множества N - множество безразличных наборов
    f_log_vec("",N);
}

void algrot::getL() {                          //метод для вывода на экран множесва L - еденичные наборы
    f_log_vec("",L);
}

void algrot::getSetsMbLex() {                     //метод для вывода на экран простых имликант которые могут быть L-эестремалями
    f_log_text("Возможные L-экстремали: ");
    if (Lex.empty()) { f_log_text("множество возможных простых имликант пустое\n"); return; }

    for (int i = 0; i < Lex.size(); ++i) {
        f_log_text("{");
        f_log_text(Lex[i].first);
        f_log_text("}, ");
    }
    f_log_text("\n");
}

void algrot::getFmins() {               //функция для вывода минимальных функций на экран
    std::ofstream fout;
    fout.open("rot_temp.txt", std::ios_base::out | std::ios_base::trunc);
            f_log_text("{");
            fout << "{";

            for (int i = 0; i < Fmin.size(); ++i) {
            for (int j = 0; j < Fmin[i].size(); ++j) {

            f_log_text(Fmin[i][j]);
            fout << Fmin[i][j];

            if(j == Fmin[i].size() - 1){
                f_log_text("}");
                fout << "}";
            }else{
                f_log_text(", ");
                fout << ", ";
            }

        }

        f_log_text("\n");
    }
    fout.close();
}

void algrot::AlgRot() {                    //Алгоритм рота по шагам
    int i = 0;
 //   bool alg = false;                //перменная показывает использовался ли алгорти
    while (!Ai.empty()) {
        AiOst();
        f_log_text("A");
        f_log_int(i);
        f_log_text(": ");
        getAi();

        resBi();
        f_log_text("B");
        f_log_int(i);
        f_log_text(": ");
        getBi();

        resZ();
        f_log_text("Z is: ");
        getZ();


        resCi();
        f_log_text("C");
        f_log_int(i + 1);
        f_log_text(": ");
        getCi();
        ++i;
    }
    canBeLext();
    getSetsMbLex();
    findLExtr();
    f_log_text("L-экстремали: ");
    getE();
    if (Lex.empty()) {                         //алгоритм ветвления
   //     alg = true;
        while (Lex.empty()) {
            CorrectZ_forVetvl();
            canBeLext();
            getSetsMbLex();
            findLExtr();
            f_log_text("L-экстремали: ");
            getE();
 //   alg = true;
        }
    }
    NotcoverZ();
    f_log_text("Множество не покрытое множеством Z: ");
    getNoCoverZ();
    resFmin();
    f_log_text("Fmin: ");
    getFmins();
    i = 0;
 //   alg = false;
    clrar();

}

void algrot::clrar()
{
        Ai.clear();
        Ci.clear();
        Z.clear();
        Bi.clear();
        Lex.clear();
        E.clear();
        NotCoverZ.clear();
        L.clear();
        N.clear();
        Fmin.clear();

}
















