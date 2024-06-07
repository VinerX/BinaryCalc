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
    private:
   
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
        for (int i = 0; i < size - 1 - 1; i++) {

            if (bools[i] > second.bools[i]) {
                
                return true;
            }
            else if (bools[i] < second.bools[i]){

                return false;
            }
        }
        // Кейс наименьшего (Пример -128)
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
    bool isSmallest() {
        if (!negative) {
            return false;
        }
        for (int i = 1; i < size - 1; i++) {
            if (bools[i]) {
                return false;
            }
        }
        return true;
    }
    //Возвращает в виде дополнительного кода
    BNumber getAdditional() {
        BNumber subtrahend = getReversed();
        subtrahend = BNumber::sumSame(subtrahend, BNumber(1), subtrahend.negative, true);
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
            char lastn = number10[size - 1 - i];
            if (lastn == '0' || lastn == '1') {
                timedBools[size - 2 - i] = charToBool(lastn);

            }
            else {
                std::cout << lastn << " Error, incorrect digit replaced by 0" << std::endl;
                timedBools[size - 2 - i] = 0;
            }
        }
        // Проверка отрицательности
        if (number10[0] == '1' || number10[0] == '-') {
            negative = number10[0];
        }

        return timedBools;

    }
    public:

    //Блок переводов
    static BNumber decToBin(int number) {
        BNumber timedBnumer;
        if (number < -pow(2, size)/2 || number >= pow(2, size)/2 ) {
            throw std::invalid_argument("Number is not limited by size: " + number);
        }

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

    //Блок счета
    //Сложение при одинаковом знаке
    static BNumber sumSame(BNumber first, BNumber second, bool negative = false, bool ignoreLimit = false) {
        BNumber timedBNumber;
        //std::cout << "First: "; first.print();
        //std::cout << "Second: "; second.print();
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
        if (timedAddition > 0 && !ignoreLimit  && !negative) {
            throw std::invalid_argument("Limit was crossed during sum operation ");
        }
        return timedBNumber;
    }

    //Вычитание при одинаковом знаке
    static BNumber substractSame(BNumber first, BNumber second, bool negative = false) {
        BNumber subtrahend;
        if (first.isBigger(second)) {

            if (second.isSmallest()) {
                subtrahend = first.getAdditional();
                if (!subtrahend.isZero()) {
                    subtrahend.negative = true;
                }
            }
            else {
                subtrahend = second.getAdditional();
                subtrahend = BNumber::sumSame(first, subtrahend, first.negative, true);
                subtrahend.negative = first.negative;
            }


        }
        else {
            if (first.isSmallest()) {
                subtrahend = second.getAdditional();
                if (!subtrahend.isZero()) {
                    subtrahend.negative = true;
                }
            }
            else {
                subtrahend = first.getAdditional();
                subtrahend = BNumber::sumSame(second, subtrahend, second.negative, true);

                if (!subtrahend.isZero()) {
                    subtrahend.negative = true;
                }
            }
        }


        return subtrahend;
    }

    //Сложение
    static BNumber sum(BNumber first, BNumber second) {

        // Один знак
        if (first.negative == second.negative) {
            return sumSame(first, second, first.negative);
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
                return sumSame(first, second,true);
            }
            // второе число негативное a-(-b)=a+b
            else {
                return sumSame(first, second);
            }
        }
    }
    //Сдвиг влево или приписка 0 справа
    void addZeroRight() {
        //Проверка на лишний сдвиг
        if (bools[0] == true && !negative) {
            throw std::invalid_argument("Limit crossed during adding right zero");
        }
        for (int i = 0; i < size - 2; i++) {
            bools[i] = bools[i + 1];
        }
        bools[size - 2] = false;
    }
    //Несколько сдвигов
    void addZeroRight(int times) {
        
        for (int i = 0; i < times; i++) {
            
            //Проверка на лишний сдвиг
            if (bools[0] == true && !negative) {
                throw std::invalid_argument("Limit crossed during adding right zero");
            }
            for (int i = 0; i < size - 2; i++) {
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
                timedBnumber = BNumber::sumSame(timedBnumber, timedBnumber2);
                
            }
        }
        timedBnumber.negative = first.negative != second.negative;
        return timedBnumber;

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

// 2 Лабараторная работа //

// Узел списка
class Node {
public:
    BNumber data; // число
    Node* next; // ссылка на следующий элемент
    Node(const BNumber& data) : data(data), next(nullptr) {} // конструктор
};


// Односвязный список с ограниченным фукнционалом под задачу
class LinkedList {
private:
    Node* head; // первый элемент в списке

public:
    LinkedList() : head(nullptr) {} // конструктор

    ~LinkedList() { // деструктор
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // добавление элемента в начало списка
    void push_front(const BNumber& num) { 
        Node* newNode = new Node(num);
        newNode->next = head;
        head = newNode;
    }

    // удаление первого элемента списка
    void pop_front() { 
        if (head == nullptr) {
            throw std::underflow_error("LinkedList underflow: no elements to remove");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    // посмотреть первый элемент списка
    BNumber peek_front() const { 
        if (head == nullptr) {
            throw std::underflow_error("LinkedList underflow: no elements to get");
        }
        return head->data;
    }

    // проверка пустоты
    bool isEmpty() const { 
        return head == nullptr;
    }
};

// Стек с ограниченным функционалом под задачу
class Stack {
private:
    LinkedList list; // список для хранения элементов стека
public:
    Stack() {} // конструктор

    ~Stack() {} // деструктор

    // добавление элемента
    void push(const BNumber& num) { 
        list.push_front(num);
    }

    // удаление верхнего элемента
    void pop() { 
        list.pop_front();
    }

    // посмотреть верхний элемент
    BNumber peek() const { 
        return list.peek_front();
    }

    // проверка пустоты
    bool isEmpty() const { 
        return list.isEmpty();
    }
};

#include <sstream>
class Calculator {
private:
    // Выбор оператора
    BNumber performOperation(const BNumber& a, const BNumber& b, char op) {
        BNumber resultB;
        switch (op)
        {
        case '+':
            resultB = BNumber::sum(a, b);
            break;
        case '-':
            resultB = BNumber::substract(a, b);
            break;
        case '*':
            resultB = BNumber::multiply(a, b);
            break;
        default:
            throw std::invalid_argument("Invalid operation. Allowed operations: +, -, *.");
        }
        return resultB;
    }

public:
    BNumber evaluatePostfix(const std::string& expression) {
        std::istringstream iss(expression); // делим строку на токены
        std::string token; // токен
        Stack stack;

        while (iss >> token) { // пробегаемся по токенам
            if (token == "+" || token == "-" || token == "*") { // если встретили арифмитическое действие
                if (stack.isEmpty()) { // стек пустой?
                    throw std::runtime_error("Invalid postfix expression: insufficient operands for operation " + token);
                }

                // получаем верхний элемент из стека
                BNumber b = stack.peek(); 
                stack.pop(); // удаляем элемент

                if (stack.isEmpty()) { // стек пустой?
                    throw std::runtime_error("Invalid postfix expression: insufficient operands for operation " + token);
                }
                BNumber a = stack.peek();
                stack.pop();

                stack.push( performOperation(a, b, token[0]) ); // выполняем операцию
                
            }
            else { // если встретили число
                try {
                    int decimalValue = std::stoi(token); // преобразуем в число
                    stack.push(BNumber::decToBin(decimalValue)); // добавляем в стек
                }
                catch (...) {
                    throw std::invalid_argument("Invalid token in expression: " + token);
                }
            }
        }

        // обработка ошибки
        if (stack.isEmpty()) { 
            throw std::runtime_error("Invalid postfix expression: no result in the stack");
        }

        // очищение стека, получение результата
        BNumber result = stack.peek(); 
        stack.pop();

        // обработка ошибки
        if (!stack.isEmpty()) { 
            throw std::runtime_error("Invalid postfix expression: more than one result remains on the stack");
        }

        return result;
    }
};


//Вспомогательный класс
class Tester {
public:
    
    static void info() {
        std::cout <<"Size is "<< size << std::endl;
    }
    static void cleanConsole() {
        // Обнуление консоли
        system("cls");
        std::cin.clear(); // на случай, если предыдущий ввод завершился с ошибкой
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    //Промежуточные функции между вводом и счетом
    static void count(int first, int second, char operation) {
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
        std::cout << "Dec: (" << first << ") " << operation << " (" << second << ")" << std::endl;
        std::cout << "Bin:" << std::endl;
        firstB.print();
        secondB.print();
        std::cout << "Result bin:" << std::endl;
        resultB.print();
        std::cout << "Result dec:" << std::endl;
        std::cout << BNumber::binToDec(resultB) << std::endl << std::endl;
    }
    static void count(int first, int second, char operation, int expectedResult) {
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
            std::cout << "+++" << std::endl;
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
    static void countExp(std::string exp) {
        Calculator calc;
        BNumber result = calc.evaluatePostfix(exp);

        std::cout << "Exp: " << exp << std::endl;
        std::cout << "Bin: "; result.print();
        std::cout << "Dec: " << BNumber::binToDec(result) << std::endl;
    }
    static void countExp(std::string exp, int expectedResult) {
        Calculator calc;
        BNumber result = calc.evaluatePostfix(exp);


        int realResult = BNumber::binToDec(result);
        if (realResult == expectedResult) {
            std::cout << "+++" << std::endl;
        }
        else {
            std::cout << "Alert! " << std::endl;
            std::cout << "Exp: " << exp << std::endl;
            std::cout << "Bin: "; result.print();
            std::cout << "Dec: " << BNumber::binToDec(result);
        }
    }
    
    static void userTesting() {
        int first, second;
        char operation;
        while (true) {
            try
            {
                std::cout << "Enter two numbers (example: 1 3) being less than " << pow(2, size) / 2 - 1 << "And greater than " << -(pow(2, size) / 2) << std::endl;
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
            system("cls");
        }
        system("cls");
        std::cin.clear(); // на случай, если предыдущий ввод завершился с ошибкой
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    }
    static void userTestingCalc() {
        std::string exp;
        char operation;
        while (true) {
            try
            {
                std::cout << "Enter expression with numbers (and intermediate values) being less or equal than " << pow(2, size) / 2 - 1 << " and greater or equal than " << -(pow(2, size) / 2) << std::endl;
                std::getline(std::cin, exp);
                countExp(exp);
            }
            catch (std::exception e)
            {   
                std::cout << "Error, there was problem in input: "<< e.what()<<std::endl;                
                continue;
            }

            
            std::cout << "enter 0 to exit or any letter to continue" << std::endl;
            std::cin >> operation;
            if (operation == '0') {
                break;
            }
            cleanConsole();
            
        }

    }
    
    
    static void autoTesting() {
        count(-128, 100, '+', -28); 
        count(100, -128, '+', -28);
        count(-128, -100, '-', -28);



        // Граничные значения
        count(3, 4, '-', -1);
        count(63, 2, '*', 126);
        // Нижние
        count(-128, 1, '+', -127);
        count(-127, 1, '-', -128);
        count(-127, -1, '+', -128);

        // Верхние
        count(127, 1, '-', 126);
        count(126, 1, '+', 127);

        count(126, -127, '+', -1);
        count(126, 125, '-', 1);
        count(-128, 1, '+', -127);

        // Знаки умножения
        count(1, 1, '*', 1);
        count(-1, 1, '*', -1);
        count(1, -1, '*', -1);
        count(-1, -1, '*', 1);

        count(-128, 0, '*', 0);
        count(5, 5, '*', 25);
        count(-2, 64, '*', -128);

        count(75, 25, '-', 50);
        count(75, 25, '+', 100);

        count(-75, -25, '-', -50);
        count(-75, -25, '+', -100);
        count(127, 126, '-', 1);



    }
    static void autoTestingCalc() {

       
        //Базовые операции
        countExp("1 1 +", 2);
        countExp("1 1 -", 0);
        countExp("1 2 *", 2);

        // Отрицательные числа
        countExp("-1 -1 *", 1);
        countExp("10 -4 2 * -", 18); //10-(-4)*2
        // Несколько операций
        countExp("1 3 2 * +", 7); // (1 + (3*2))
        countExp("3 4 - 3 2 * *", -6); // ( (3-4) * (3*2) )
        countExp("2 5 * 3 4 * +", 22); // 2*5+3*4

        //countExp("-5 2 3 2 * 1 - -", -15); // -5 * 2 - (3*2-1)
        countExp("127 63 3 1 - * -", 1); // 127 - 63  * (3-1)

        countExp("-5 5 *", -25);
        countExp("1 1 *", 1);

    }


};

int main()
{
    Tester::info();
    // Авто тесты - +++ если тест пройден, иначе лог ошибки
    Tester::autoTesting(); // Тестирование отдельных операций
    Tester::autoTestingCalc(); // Тестирование выражений


    Tester::userTestingCalc();
}

// ToDo 
// Работа с вводом
// Приватность
// Лимиты дочинить