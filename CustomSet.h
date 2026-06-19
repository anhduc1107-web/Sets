#ifndef CUSTOM_SET_H
#define CUSTOM_SET_H

#include <vector>
#include <iostream>

// CUSTOMSET TEMPLATE CLASS DEFINITION & IMPLEMENTATION
template <typename T>
class CustomSet {
private:
    std::vector<T> members; // Underlying dynamic contiguous array

public:
    CustomSet() {}
    ~CustomSet() {}

    // Utility operations
    bool isEmpty() const { return members.empty(); }
    size_t size() const { return members.size(); }

    // Basic Operations
    bool search(const T& value) const;
    bool insert(const T& element);
    bool remove(const T& value);
    void display() const;

    // Mathematical Operations
    CustomSet<T> performUnion(const CustomSet<T>& other) const;
    CustomSet<T> performIntersection(const CustomSet<T>& other) const;
    CustomSet<T> performDifference(const CustomSet<T>& other) const;
    bool isSubset(const CustomSet<T>& other) const;
    bool isEqual(const CustomSet<T>& other) const;
};

// Algorithm 1: Membership Test (O(n))
template <typename T>
bool CustomSet<T>::search(const T& value) const {
    if (this->members.empty()) return false;
    
    // Sequential read-only traversal
    for (const auto& item : this->members) {
        if (item == value) return true;
    }
    return false;
}

// Algorithm 2: Insertion via Check->Insert Pipeline (O(n))
template <typename T>
bool CustomSet<T>::insert(const T& element) {
    // Fail-fast: Automatically reject duplicate elements to enforce uniqueness
    if (this->search(element)) return false; 
    
    this->members.push_back(element);
    return true;
}

// Algorithm 3: Deletion via Swap-with-last strategy (O(n))
template <typename T>
bool CustomSet<T>::remove(const T& value) {
    if (this->members.empty()) return false;
    
    for (size_t i = 0; i < this->members.size(); ++i) {
        if (this->members[i] == value) {
            // Swap the target element with the last element to avoid O(n) shifts
            this->members[i] = this->members.back(); 
            this->members.pop_back(); // Remove the last element in O(1) auxiliary time
            return true;
        }
    }
    return false;
}

// Algorithm 4: Display standard mathematical format (O(n))
template <typename T>
void CustomSet<T>::display() const {
    if (this->members.empty()) {
        std::cout << "{ }." << std::endl;
        return;
    }
    std::cout << "{ ";
    for (size_t i = 0; i < this->members.size(); ++i) {
        std::cout << this->members[i];
        if (i == this->members.size() - 1) {
            std::cout << " }." << std::endl; // Terminal boundary
        } else {
            std::cout << ", "; // Continuous element delimiter
        }
    }
}

// Mathematical Operations (O(n x m) and O((n+m)^2))
template <typename T>
CustomSet<T> CustomSet<T>::performUnion(const CustomSet<T>& other) const {
    CustomSet<T> result;
    for (const auto& item : this->members) result.insert(item);
    for (const auto& item : other.members) result.insert(item);
    return result;
}

template <typename T>
CustomSet<T> CustomSet<T>::performIntersection(const CustomSet<T>& other) const {
    CustomSet<T> result;
    for (const auto& item : this->members) {
        if (other.search(item)) result.insert(item);
    }
    return result;
}

template <typename T>
CustomSet<T> CustomSet<T>::performDifference(const CustomSet<T>& other) const {
    CustomSet<T> result;
    for (const auto& item : this->members) {
        if (!other.search(item)) result.insert(item);
    }
    return result;
}

template <typename T>
bool CustomSet<T>::isSubset(const CustomSet<T>& other) const {
    if (this->members.empty()) return true;
    if (this->members.size() > other.members.size()) return false;
    
    for (const auto& item : this->members) {
        if (!other.search(item)) return false;
    }
    return true;
}

template <typename T>
bool CustomSet<T>::isEqual(const CustomSet<T>& other) const {
    if (this->members.size() != other.members.size()) return false;
    return this->isSubset(other);
}

#endif // CUSTOM_SET_H
