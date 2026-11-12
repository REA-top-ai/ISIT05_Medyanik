module;

#include <iostream>

module list;

List::List() : head_(nullptr), size_(0) {
}

List::~List() {
    clear();
}

void List::clear() {
    while (head_ != nullptr) {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    size_ = 0;
}

void List::push_front(int value) {
    Node* node = new Node{value, head_};
    head_ = node;
    ++size_;
}

void List::push_back(int value) {
    Node* node = new Node{value, nullptr};

    if (head_ == nullptr) {
        head_ = node;
    } else {
        Node* current = head_;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = node;
    }
    ++size_;
}

bool List::insert(int index, int value) {
    if (index < 0 || index > size_) {
        return false;
    }

    if (index == 0) {
        push_front(value);
        return true;
    }

    Node* previous = head_;
    for (int i = 0; i < index - 1; ++i) {
        previous = previous->next;
    }

    Node* node = new Node{value, previous->next};
    previous->next = node;
    ++size_;
    return true;
}

bool List::remove_at(int index) {
    if (index < 0 || index >= size_) {
        return false;
    }

    Node* to_delete = nullptr;

    if (index == 0) {
        to_delete = head_;
        head_ = head_->next;
    } else {
        Node* previous = head_;
        for (int i = 0; i < index - 1; ++i) {
            previous = previous->next;
        }
        to_delete = previous->next;
        previous->next = to_delete->next;
    }

    delete to_delete;
    --size_;
    return true;
}

bool List::remove_value(int value) {
    int index = find(value);
    if (index == -1) {
        return false;
    }
    return remove_at(index);
}

bool List::get(int index, int& value) const {
    if (index < 0 || index >= size_) {
        return false;
    }

    Node* current = head_;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    value = current->value;
    return true;
}

int List::find(int value) const {
    Node* current = head_;
    int index = 0;

    while (current != nullptr) {
        if (current->value == value) {
            return index;
        }
        current = current->next;
        ++index;
    }

    return -1;
}

int List::size() const {
    return size_;
}

bool List::empty() const {
    return size_ == 0;
}

void List::print() const {
    std::cout << "[";

    Node* current = head_;
    while (current != nullptr) {
        std::cout << current->value;
        if (current->next != nullptr) {
            std::cout << ", ";
        }
        current = current->next;
    }

    std::cout << "]\n";
}
