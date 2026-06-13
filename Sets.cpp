#include <iostream>
#include <unordered_set>

using namespace std;

// Set of unique integers optimized with Hash Table (unordered_set)
class Set {
private:
    unordered_set<int> elements; // Stores only integers, C++ automatically handles hashing O(1)

public:
    Set()  {}
    ~Set() {}

    // O(1) — Check if the set is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // O(1) — Check if a value belongs to the set
    bool isMember(int value) const {
        return elements.count(value) > 0;
    }

    // O(1) — Insert an element (duplicates are automatically ignored)
    void insert(int member) {
        elements.insert(member);
    }

    // O(1) — Remove an element from the set
    void removeMember(int value) {
        elements.erase(value);
    }

    // O(n) — Print the set, last element ends with a "."
    void print_set() const {
        size_t i = 0, total = elements.size();
        for (int val : elements) {
            cout << val;
            cout << (++i == total ? ".\n" : ", ");
        }
        if (total == 0) cout << "(empty)\n";
    }

    // O(n) — Union: Add all elements from both A and B
    Set setUnion(const Set& setB) const {
        Set newSet;
        for (int val : elements)      newSet.insert(val);
        for (int val : setB.elements) newSet.insert(val);
        return newSet;
    }

    // O(n) — Intersection: Keep only the common elements
    Set setIntersection(const Set& setB) const {
        Set newSet;
        for (int val : elements) {
            if (setB.isMember(val)) // O(1) lookup
                newSet.insert(val);
        }
        return newSet;
    }

    // O(n) — Difference: Keep elements present in A but not in B
    Set setDifference(const Set& setB) const {
        Set newSet;
        for (int val : elements) {
            if (!setB.isMember(val)) // O(1) lookup
                newSet.insert(val);
        }
        return newSet;
    }

    // O(n) — Check if it is a subset
    bool isSubset(const Set& setB) const {
        // FIX: Prevent unnecessary iterations. If A is larger than B, A cannot be a subset of B.
        if (elements.size() > setB.elements.size()) {
            return false;
        }

        for (int val : elements) {
            if (!setB.isMember(val)) // O(1) lookup
                return false;
        }
        return true;
    }

    // O(n) — Check if two sets are equal
    bool isEqual(const Set& setB) const {
        // FIX: No need to duplicate logic. C++ already provides the equality operator (==) 
        // for unordered_set, which automatically checks the size and compares elements optimally.
        return elements == setB.elements;
    }
};

int main() {
    // --- Set A: {5, 6, 7, 8} ---
    Set setA;
    cout << (setA.isEmpty() ? "SetA is empty.\n" : "SetA is not empty.\n");

    setA.insert(5); setA.insert(6);
    setA.insert(7); setA.insert(8);
    cout << "Set A: "; setA.print_set();

    // --- Set B: {7, 8, 9, 10} ---
    Set setB;
    setB.insert(7); setB.insert(8);
    setB.insert(9); setB.insert(10);
    cout << "Set B: "; setB.print_set();

    // --- Set operations ---
    Set setC = setA.setUnion(setB);
    cout << "Set C (A Union B): "; setC.print_set();

    Set setD = setA.setIntersection(setB);
    cout << "Set D (A Intersection B): "; setD.print_set();

    Set setE = setA.setDifference(setB);
    cout << "Set E (A - B): "; setE.print_set();

    Set setF = setB.setDifference(setA);
    cout << "Set F (B - A): "; setF.print_set();

    // --- Subset & equality checks ---
    cout << (setA.isSubset(setB) ? "A is a subset of B.\n"     : "A is not a subset of B.\n");
    cout << (setF.isSubset(setB) ? "F is a subset of B.\n"     : "F is not a subset of B.\n");
    cout << (setA.isEqual(setB)  ? "A and B are equal.\n"      : "A and B are not equal.\n");

    // --- Set G = {7, 8, 9, 10} (Equal to Set B) ---
    Set setG;
    setG.insert(7); setG.insert(8);
    setG.insert(9); setG.insert(10);
    cout << "Set G: "; setG.print_set();
    cout << (setG.isEqual(setB) ? "G and B are equal.\n" : "G and B are not equal.\n");

    // --- Search for an element ---
    cout << "Searching for 11 in set B...\n";
    cout << (setB.isMember(11) ? "Found 11 in B.\n" : "11 doesn't exist in B.\n");

    // --- Remove an element ---
    setA.removeMember(10); // Silently ignored because 10 is not in A
    cout << "Set A after removing 10: "; setA.print_set();

    return 0;
}
