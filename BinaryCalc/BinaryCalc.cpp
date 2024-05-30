#include <iostream>
#include <array>
#include <string>
const int size = 8; // Настройка кол-ва битов (здесь булов)
class BNumber {

public:
    // Данные каждого объекта
    std::array<bool, size - 1> bools;
    bool negative = false;

    //Блок конструкторов
    // Задание строкой
    BNumber(std::string number10) {
        bools = convert(number10);
    }
    //Задание интом
    BNumber(int number10) {
        std::string s = std::to_string(number10);
        if (number10 < 0) {
            negative = true;
            s[0] = '0';
        }

        if (s.size() != size) {
            while (s.size() < size) {
                s.insert(0, "0");
            }
        }
        bools = convert(s);
    }
    // Нулевая иницилизация
    BNumber() {
        for (int i = 0; i < size - 1; i++) {
            bools[i] = false;
        }
    }

    bool charToBool(char a) {
        if (a == '1') {
            return true;
        }
        else {
            return 0;
        }
    }

    //Вспомогательные функции
    BNumber getReversed() {
        BNumber newBNumber;
        for (int i = 0; i < size - 1; i++) {
            newBNumber.bools[i] = !bools[i];
        }
        return newBNumber;
    }
    void removeLeft() {
        bools[0] = false;
    }
    //без учета знака
    bool isBigger(BNumber second) {
        //std::cout << "Bigger check";
        //print();
        //second.print();
        for (int i = 0; i < size - 1 - 1; i++) {
            if (bools[i] > second.bools[i]) {
                return true;
            }
        }

        // Кейс наименьшего
        if (second.bools[size - 2] == false) {
            if (second.negative) {
                return false;
            }
            else {
                return true;
            }
        }
        return false;
        
    }
        
        
    
    bool isZero() {
        if (negative) {
            return false;
        }
        for (int i = 0; i < size - 1; i++) {
            if (bools[i]){
                return false;
            }
        }
        return true;
    }
    //Возвращает в виде дополнительного кода
    BNumber getAdditional() {
        BNumber subtrahend = getReversed();
        subtrahend = BNumber::sumSame(subtrahend, BNumber(1), subtrahend.negative);
        subtrahend.negative = negative;
        return subtrahend;
    }

    // Конверт ввода в виде строки
    std::array<bool, size - 1> convert(std::string number10) {
        if (number10.size() != size) {
            std::cout << "Error, entered string has different size";
        }

        std::array<bool, size - 1> timedBools;
        for (int i = 0; i < size - 1; i++) {
            //std::cout << number10[size - 1 - i] <<" " << std::endl;
            char lastn = number10[size - 1 - i];
            if (lastn == '0' || lastn == '1') {
                timedBools[size - 2 - i] = charToBool(lastn);

            }
            else {
                std::cout << lastn << "Error, incorrect digit replaced by 0" << std::endl;
                timedBools[size - 2 - i] = 0;
            }
        }
        // Проверка отрицательности
        if (number10[0] == '1' || number10[0] == '-') {
            negative = number10[0];
        }

        return timedBools;

    }

    //Блок счета
    //Сложение при одинаковом знаке
    static BNumber sumSame(BNumber first, BNumber second, bool ignoreLimit = false, bool negative = false) {
        BNumber timedBNumber;
        timedBNumber.negative = negative;
        long timedAddition = 0;
        for (int i = size - 2; i >= 0; i--) {
            //Копия данного для нового контейнера
            timedBNumber.bools[i] = first.bools[i];
            //Добавление остатка с предыдущего 
            if (timedAddition > 0) {
                if (timedBNumber.bools[i] == false) {
                    timedBNumber.bools[i] = true;
                    timedAddition--;
                }
                else {
                    timedBNumber.bools[i] = false;
                }

            }
            long timedSum = timedBNumber.bools[i] + second.bools[i];
            timedAddition += timedSum / 2;
            timedSum %= 2;
            timedBNumber.bools[i] = timedSum;
        }

        if (timedAddition > 0 && !ignoreLimit) {
            std::cout << "Error, number is too large" << std::endl;
        }
        return timedBNumber;
    }

    //Вычитание при одинаковом знаке
    static BNumber substractSame(BNumber first, BNumber second, bool negative = false) {
        if (first.isBigger(second)) {
            BNumber subtrahend = second.getAdditional();
            subtrahend = BNumber::sumSame(first, subtrahend, true);
            subtrahend.negative = first.negative;
            return subtrahend;
        }
        else {
            BNumber subtrahend = first.getAdditional();
            subtrahend = BNumber::sumSame(second, subtrahend, true);
            subtrahend.negative = second.negative;
            return subtrahend;
        }



    }

    //Сложение
    static BNumber sum(BNumber first, BNumber second) {

        // Один знак
        if (first.negative == second.negative) {
            return sumSame(first, second, false, first.negative);
        }
        else {
            // первое число отрицательное -a+b=b-a
            if (first.negative) {
                //first.print();
                return substractSame(second, first);
            }
            // второе число отрицательное a+(-b)=a-b
            else {
                return substractSame(first, second);
            }
        }
    }
    //Вычитание
    static BNumber substract(BNumber first, BNumber second) {

        // Один знак
        if (first.negative == second.negative) {
            return substractSame(first, second, first.negative);
        }
        else {
            // первое число отрицательное -a-(+b) = -(a + b);
            if (first.negative) {
                return sumSame(first, second, false, true);
            }
            // второе число негативное a-(-b)=a+b
            else {
                return sumSame(first, second);
            }
        }
    }
    //Сдвиг влево или приписка 0 справа
    void addZeroRight() {
        for (int i = 0; i < size - 2; i++) {
            bools[i] = bools[i + 1];
        }
        bools[size - 2] = false;
    }
    //Несколько сдвигов
    void addZeroRight(int times) {
        for (int i = 0; i < times; i++) {
            for (int i = 1; i < size - 2; i++) {
                bools[i] = bools[i + 1];
            }
            bools[size - 2] = false;
        }
    }

    // Умножение
    static BNumber multiply(BNumber first, BNumber second) {
        BNumber timedBnumber;
        BNumber timedBnumber2;

        //Кейс 0
        if (first.isZero() || second.isZero() ){
            return BNumber(0);
        }

        for (int i = size - 2; i >= 0; i--) {
            //Это 1
            if (second.bools[i] == true) {
                timedBnumber2 = first;
                timedBnumber2.addZeroRight(size - 2 - i);
                //    std::cout << "@";
                //    timedBnumber.print();
                  //  timedBnumber2.print();
                timedBnumber = BNumber::sum(timedBnumber, timedBnumber2);
            }
        }
        timedBnumber.negative = first.negative != second.negative;
        return timedBnumber;

    }




    //Блок переводов
    static BNumber decToBin(int number) {
        BNumber timedBnumer;
        if (number < 0) {
            timedBnumer.negative = true;
        }

        for (int i = size - 2; i >= 0; i--) {
            timedBnumer.bools[i] = number % 2;
            number /= 2;
            //timedBnumer.print();
        }
        return timedBnumer;

    }

    static int binToDec(BNumber timedBnumer) {
        int number = 0;
        for (int i = size - 2; i >= 0; i--) {
            number += timedBnumer.bools[i] * pow(2, size - 2 - i);
        }
        if (timedBnumer.negative == true) {
            number *= -1;
            //Кейс наименьшего
            if (number == 0) {
                number = pow(2, size) / -2;
            }
        }
        return number;
    }




    // Для мониторинга
    void print() {
        if (negative) {
            std::cout << "-";
        }
        for (bool b : bools) {
            b == true ? std::cout << 1 : std::cout << 0;
        }
        std::cout << std::endl;
    }
};

class Tester {
public:
    static void count(int first, int second, char operation, int expectedResult=0) {
        BNumber firstB = BNumber::decToBin(first);
        BNumber secondB = BNumber::decToBin(second);
        BNumber resultB;
        switch (operation)
        {
        case '+':
            //firstB.print();
            resultB = BNumber::sum(firstB, secondB);
            break;
        case '-':
            resultB = BNumber::substract(firstB, secondB);
            break;
        case '*':
            resultB = BNumber::multiply(firstB, secondB);
            break;
        }
        

        
        if (BNumber::binToDec(resultB) == expectedResult)
        {
            std::cout << "+++"<<std::endl;
        }
        else {
            std::cout << "alert!" << std::endl;
            std::cout << "Dec: (" << first << ") " << operation << " (" << second << ")" << std::endl;
            std::cout << "Bin:" << std::endl;
            firstB.print();
            secondB.print();
            std::cout << "Result bin:" << std::endl;
            resultB.print();
            std::cout << "Result dec:" << std::endl;
            std::cout << BNumber::binToDec(resultB) << std::endl << std::endl;
        }
    }
    static void startTesting() {
        int first, second;
        char operation;
        while (true) {
            try
            {
                std::cout << "Enter two numbers (example: 1 3) less than " << pow(2, size) / 2 - 1 << "And greater than " << -(pow(2, size) / 2) << std::endl;
                std::cin >> first >> second;
                std::cout << "Enter operation +,-,* (example: +)" << std::endl;
                std::cin >> operation;
            }
            catch (const std::exception&)
            {
                std::cout << "Error, incorrect imput";
                continue;
            }

            count(first, second, operation);
            std::cout << "enter 0 to exit or any letter to continue" << std::endl;
            std::cin >> operation;
            if (operation == '0') {
                break;
            }
        }

    }
    static void autoTesting() {
        count(-128, 1, '+',-127);

        count(126, 1, '+',127);
        count(127, 1, '-',126);
        count(126, -127, '+',-1);
        count(126, 125, '-',1);
        count(-128, 1, '+',-127);
        count(-127, 1, '-',-128);

        count(1, 1, '*',1);
        count(-1, 1, '*',-1);
        count(1, -1, '*',-1);
        count(-1, -1, '*',1);

        count(-128, 0, '*',0);
        count(5, 5, '*', 25);
        count(-2,64, '*', -128);

    }


};

int main()
{
    Tester::autoTesting();
}