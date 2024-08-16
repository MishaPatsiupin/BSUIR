#include <iostream>
#include <vector>
#include <string>

class ROT {
private:
    std::vector<std::string> Ai;                                //Итый остаток после операции умножения
    std::vector<std::string> L;                                 //Множество L
    std::vector<std::string> N;                                 //Множество N
    std::vector<std::pair<std::string,int>> Ci;                 //итое множество C
    std::vector<std::string> Z;                                 //Множество простых импликант
    std::vector<std::string> Bi;                                //Итое множество B
    std::vector<std::pair<std::string,std::string>> Lex;        //наборы которые могут быть L-екстремалями
    std::vector<std::string> E;                                 //L - экстремали
    std::vector<std::string> NotCoverZ;                         //наборы которые не покрыты множеством Z
    std::vector<std::vector<std::string>> Fmin;                 //минимальные функции

    void inpCi() {
        for (int i = 0; i < N.size(); ++i) {
            if (N[i] != "") { Ci.push_back(std::make_pair(N[i], 0)); }
        }
        for (int i = 0; i < L.size(); ++i) {
            Ci.push_back(std::make_pair(L[i], 0));
        }
    }

public:

    ROT() {
        L = { "" };
        N = { "" };
        Ai = { "" };
    }

    ROT(const std::vector<std::string>& L_, const std::vector<std::string>& N_) {
        L = L_;
        N = N_;
        Ai = { "" };
        inpCi();
    }

    void AiOst() {                                                                                           //функция для поиска кубов большей размерности после операции *

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

     void correctAi() {                         //корекция множества Ai
         for (int i = 0; i < Ai.size(); ++i) {
             for (int j = i + 1; j < Ai.size(); ++j) {
                 if (Ai[i] == Ai[j]) {
                     Ai.erase(Ai.begin() + j);
                 }
             }
         }
     }

     void resBi() {                                          //заполнения множества Bi
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

     void resZ() {
         for (int i = 0; i < Ci.size(); ++i) {                //заполнение множества Z
             if (Ci[i].second == 0) {
                 Z.push_back(Ci[i].first);
             }
         }
         correctZ();
     }


     void correctZ() {                                       //коррекция множествв Z
         for (int i = 0; i < Z.size(); ++i) {
             for (int j = i + 1; j < Z.size(); ++j) {
                 if (Z[i] == Z[j]) { Z.erase(Z.begin() + (j--)); }
             }
         }
     }

     void resCi() {                          //метод для поиска множества Ci

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

     void canBeLext() {                       //метод для поиска возможных L-екстремалей

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

     void findLExtr() {                              //поиск L-екстремалей
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

     void CorrectZ_forVetvl() {                   //алгоритм ветвления
         Z[Z.size() - 1] = Z[Z.size() - 2];
     }

     void NotcoverZ() {                            //метод для поиска кубов которые не покрывают
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

     void resFmin(bool alg) {                   //метод для нахождения минимального покрытия множества L множеством Z

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

    void getE() {                        //метод для вывод на экран L-экстремалей
        if (E.empty()) { std::cout << "Множество L-екстремалей пустое\n"; return; }
        for (int i = 0; i < E.size(); ++i) { std::cout << E[i] << " "; }
        std::cout << std::endl;
    }
     
    void getNoCoverZ() {                            //метод для вывода на экрна
        if (NotCoverZ.empty()) { std::cout << "множество имликант которые не покрыты пустое\n"; return; }
        for (int i = 0; i < NotCoverZ.size(); ++i) { std::cout << NotCoverZ[i] << " "; }
        std::cout << std::endl;
    }


     void getCi() {                        //метод для вывода на экран множества Ci
         if (Ci.empty()) { std::cout << "множество Ci - пустое\n"; }
         for (int i = 0; i < Ci.size(); ++i) { std::cout << Ci[i].first << " " ; }

         std::cout << std::endl;
     }



     void getZ() {                         //метод для вывода на экран множества Z
         if (!Z.empty()) { for (int i = 0; i < Z.size(); ++i) std::cout << Z[i] << ' '; std::cout << std::endl; }
         if (Z.empty()) { std::cout << "множество Z - пустое\n"; return; }
     }


    void getAi() {                          //метод для вывода
        if (Ai.empty()) { std::cout << "множество Ai - пустое\n"; return; }
        for (int i = 0; i < Ai.size(); ++i) { std::cout << Ai[i] << " "; }

        std::cout << std::endl;
    }


    void getBi() {                        //метод для вывода на экран множества Bi
        if (Bi.empty()) { std::cout << "множество Bi - пустое\n"; return; }
        for (int i = 0; i < Bi.size(); ++i) { std::cout << Bi[i] << ' '; }
        std::cout << std::endl;
    }

    void getN() {                         //метод для вывада на экран не обязательного множества N - множество безразличных наборов
        for (int i = 0; i < N.size(); ++i) { std::cout << N[i] << "  "; }

        std::cout << std::endl;
    }


    void getL() {                          //метод для вывода на экран множесва L - еденичные наборы
        for (int i = 0; i < L.size(); ++i) { std::cout << L[i] << "   "; }
        std::cout << std::endl;
    }

    void getSetsMbLex() {                     //метод для вывода на экран простых имликант которые могут быть L-эестремалями
        std::cout << "Возможные L-экстремали: ";
        if (Lex.empty()) { std::cout << "множество возможных простых имликант пустое\n"; return; }

        for (int i = 0; i < Lex.size(); ++i) {

            std::cout << Lex[i].first << ' ';
        }
        std::cout << std::endl;
    }

    void getFmins() {               //функция для вывода минимальных функций на экран
        for (int i = 0; i < Fmin.size(); ++i) {
            for (int j = 0; j < Fmin[i].size(); ++j) {
                std::cout << Fmin[i][j] << " ";
            }

            std::cout << std::endl;
        }
    }



    void AlgRot() {                    //Алгоритм рота по шагам
        int i = 0;
        bool alg = false;                //перменная показывает использовался ли алгорти
        while (!Ai.empty()) {
            AiOst();
            std::cout << "A" << i << ": ";
            getAi();

            resBi();
            std::cout << "B" << i <<": ";
            getBi();

            resZ();
            std::cout << "Z is: ";
            getZ();


            resCi();
            std::cout << "C" << i + 1 <<": ";
            getCi();
            ++i;
        }
        canBeLext();
        getSetsMbLex();
        findLExtr();
        std::cout << "L-экстремали: ";
        getE();
        if (Lex.empty()) {                         //алгоритм ветвления
            alg = true;
            while (Lex.empty()) {
                CorrectZ_forVetvl();
                canBeLext();
                getSetsMbLex();
                findLExtr();
                std::cout << "L-экстремали: ";
                getE();
            }
        }
        NotcoverZ();
        std::cout << "Множество не покрытое множеством Z: ";
        getNoCoverZ();
        resFmin(alg);
        std::cout << "Fmin: ";
        getFmins();
    }

    void Test1() {        //превый тест
        N = { "01000", "10011","11001","11010"};
        L = {"01100","01110","10001","11000","11011"};
        Ai = { "" };
        inpCi();
        AlgRot();
    }

    void Test2() {         //второй тест
        L = { "1000","1001","1011"};
        Ai = { "" };
        inpCi();
        AlgRot();
    }

    void Test3() {         //третий тест
        L = { "00000","11001","00010","10010","10110","01101","00100" };
        N = { "00011","00110","00111","01111","11101" };
        Ai = { "" };
        inpCi();
        AlgRot();
    }

    void Test4() {         //четвертый тест
        L = {"0000","0001","0101","1100","1101","1000"};
        N = {};
        Ai = {""};
        inpCi();
        AlgRot();
    }
    
    void Test5() {         //пятый тест
        L = {"0x1010","0x1101","x01100","x10100","0x10x1"};
        N = {"1xx101","xxx11x","1xx0xx"};
        Ai = {""};
        inpCi();
        AlgRot();
    }



    ~ROT() = default;

};

void intpurMn(std::vector<std::string>& b) {

    int size = 0;
    std::cout << "Введите количество наборов: ";
    std::cin >> size;
    if (size == 0) { return; }

    std::string buf;
    for (int i = 0; i < size; i++) {

        std::cout << "Введите " << i + 1 << " набор: ";
        std::cin >> buf;
        b.push_back(buf);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    
//    std::vector<std::string> N;                   //ввод с клавиатуры
//    std::vector<std::string> L;
//    intpurMn(L);
//    intpurMn(N);
//    ROT alg(L,N);
//    alg.AlgRot();
    
    
    ROT alg;                                        //использование готовых тестов
    alg.Test1();
//    alg.Test2();
//    alg.Test3();
//    alg.Test4();
//    alg.Test5();

    return 0;
}
