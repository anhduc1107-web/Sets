#include <iostream>
#include <string>
#include "CustomSet.h"

using namespace std;

int main() {
    // TEST 1: WORKING WITH INTEGER (INT) TYPE
    cout << "TEST WITH INTEGER (INT) TYPE" << endl;
    
    CustomSet<int> setA;
    setA.insert(5);
    setA.insert(6);
    setA.insert(7);
    
    // Attempt to insert a duplicate element (Testing the Check->Insert mechanism)
    cout << "Inserting duplicate (5) into Set A: " 
         << (setA.insert(5) ? "Success" : "Rejected (Already exists)") << endl;
         
    cout << "Set A: "; 
    setA.display();

    CustomSet<int> setB;
    setB.insert(7);
    setB.insert(8);
    setB.insert(9);
    
    cout << "Set B: "; 
    setB.display();

    // Mathematical Operations Testing
    CustomSet<int> unionSet = setA.performUnion(setB);
    cout << "A Union B: "; 
    unionSet.display();

    CustomSet<int> intersectSet = setA.performIntersection(setB);
    cout << "A Intersect B: "; 
    intersectSet.display();

    CustomSet<int> diffSet = setA.performDifference(setB);
    cout << "A Difference B (A \\ B): "; 
    diffSet.display();

    // TEST 2: WORKING WITH STRING TYPE
    cout << "\nTEST WITH STRING TYPE" << endl;
    
    CustomSet<string> stringSet1;
    stringSet1.insert("Hanoi");
    stringSet1.insert("Danang");
    stringSet1.insert("Saigon");

    CustomSet<string> stringSet2;
    stringSet2.insert("Danang");
    stringSet2.insert("Hue");

    cout << "String Set 1: "; 
    stringSet1.display();
    
    cout << "String Set 2: "; 
    stringSet2.display();

    CustomSet<string> stringIntersect = stringSet1.performIntersection(stringSet2);
    cout << "Intersection of the two string sets: "; 
    stringIntersect.display();

    // Deletion Algorithm Testing (Swap-with-last strategy)
    cout << "\nRemoving 'Danang' from String Set 1" << endl;
    stringSet1.remove("Danang");
    
    cout << "String Set 1 after removal: "; 
    stringSet1.display(); 
    // Notice how "Saigon" (the last element) is swapped into the position of "Danang"

    return 0;
}
