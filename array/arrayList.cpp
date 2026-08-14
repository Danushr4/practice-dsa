#include <iostream>
#include <stdexcept>

class ArrayList {
private:
    int* data;
    int count;
    int cap;

    static int nextPowerOfTwo(int n) {
        if (n <= 16) {
            return 16;
        }
        int cap = 16;
        while (cap < n) {
            cap *= 2;
        }
        return cap;
    }

    void resize(int new_capacity) {
        if (new_capacity < 1) {
            new_capacity = 1;
        }
        int* newData = new int[new_capacity];
        int copyCount = count < new_capacity ? count : new_capacity;
        for (int i = 0; i < copyCount; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = new_capacity;
        if (count > cap) {
            count = cap;
        }
    }

    void growIfNeeded() {
        if (count >= cap) {
            resize(cap == 0 ? 16 : cap * 2);
        }
    }

    void shrinkIfNeeded() {
        if (cap > 16 && count <= cap / 4) {
            resize(cap / 2);
        }
    }

public:
    ArrayList(int initial_capacity = 16) {
        cap = nextPowerOfTwo(initial_capacity);
        count = 0;
        data = new int[cap];
    }

    ~ArrayList() {
        delete[] data;
    }

    int size() const { return count; }
    int capacity() const { return cap; }

    bool is_empty() const {
        return count == 0;
    }

    int at(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    void push(int item) {
        growIfNeeded();
        data[count++] = item;
    }

    void insert(int index, int item) {
        if (index < 0 || index > count) {
            throw std::out_of_range("Index out of bounds");
        }
        growIfNeeded();
        for (int i = count; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = item;
        count++;
    }

    void prepend(int item) {
        insert(0, item);
    }

    int pop() {
        if (count == 0) {
            throw std::out_of_range("Cannot pop from empty list");
        }
        int value = data[--count];
        shrinkIfNeeded();
        return value;
    }

    void delete_at(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
        shrinkIfNeeded();
    }

    void remove(int item) {
        int index = find(item);
        if (index != -1) {
            delete_at(index);
        }
    }

    int find(int item) const {
        for (int i = 0; i < count; i++) {
            if (data[i] == item) {
                return i;
            }
        }
        return -1;
    }

    void print() const {
        for (int i = 0; i < count; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    ArrayList list;

    list.push(10);
    list.push(20);
    list.push(30);
    list.prepend(5);
    list.insert(2, 15);

    std::cout << "size: " << list.size()
              << " capacity: " << list.capacity() << std::endl;
    list.print();

    std::cout << "at(2): " << list.at(2) << std::endl;
    std::cout << "find(20): " << list.find(20) << std::endl;

    std::cout << "pop: " << list.pop() << std::endl;
    list.delete_at(1);
    list.remove(15);

    list.print();
    std::cout << "is_empty: " << list.is_empty() << std::endl;

    return 0;
}
