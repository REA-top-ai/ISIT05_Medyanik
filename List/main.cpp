#include <iostream>

import list;

int main() {
    List list;

    // добавление в конец и в начало
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_front(5);
    std::cout << "После добавления: ";
    list.print();                       // [5, 10, 20, 30]

    // вставка по индексу
    list.insert(2, 15);
    std::cout << "После insert(2, 15): ";
    list.print();                       // [5, 10, 15, 20, 30]

    // поиск и получение значения
    std::cout << "Индекс числа 20: " << list.find(20) << '\n';
    int value = 0;
    if (list.get(3, value)) {
        std::cout << "Элемент с индексом 3: " << value << '\n';
    }

    // удаление
    list.remove_at(0);
    list.remove_value(30);
    std::cout << "После удаления: ";
    list.print();                       // [10, 15, 20]

    // проверка ошибок: индекс вне списка
    if (!list.remove_at(100)) {
        std::cout << "remove_at(100): такого индекса нет\n";
    }

    std::cout << "Размер: " << list.size() << '\n';

    list.clear();
    std::cout << "После clear, список пуст: " << (list.empty() ? "да" : "нет") << '\n';

    return 0;
}
