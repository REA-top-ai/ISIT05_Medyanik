export module list;

export class List {
public:
    List();
    ~List();

    List(const List& other) = delete;
    List& operator=(const List& other) = delete;

    void push_front(int value);
    void push_back(int value);
    bool insert(int index, int value);
    bool remove_at(int index);
    bool remove_value(int value);
    bool get(int index, int& value) const;
    int find(int value) const;
    int size() const;
    bool empty() const;
    void clear();
    void print() const;

private:
    struct Node {
        int value;
        Node* next;
    };

    Node* head_;
    int size_;
};
