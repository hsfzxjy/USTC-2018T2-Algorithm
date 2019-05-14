#include <iostream>
#include <cstdlib>
#include "RBTree.h"
#include "OSTree.h"

using namespace std;

inline void printSplitter () {
    cout << "-----------------------------------------------------------------------" << endl;
}

inline void testRBTree () {
    // Generate a Red-Black Tree with provided data

    auto rbtree = new RBTree<int>();

    std::vector<int> numbers{3, 1, 4, 15, 9, 2, 6, 5, 35, 8, 97, 93, 23, 84, 62};

    printSplitter();

    cout << "Calling `Insert` to set up RBTree with the following numbers:"<< endl;
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        cout << *it << ' ';
        rbtree->Insert(*it);
    }
    cout << endl << "Successfully inserted " << numbers.size() << " numbers." << endl;

    printSplitter();

    RBTree<int>::vector_t placeholder;
    rbtree->InOrder(placeholder);

    cout << "Calling `InOrder` to traverse the tree in order:" << endl;
    for (auto it = placeholder.begin(); it != placeholder.end(); ++it)
        cout << (*it)->data << ' ';
    cout << endl;

    printSplitter();

    cout << "Calling `Maximum` and `Minimum`:" << endl;
    cout << "Maximum: " << rbtree->Maximum()->data << endl;
    cout << "Minimum: " << rbtree->Minimum()->data << endl;

    printSplitter();

    cout << "Calling `Search` to get node with data `35`:" << endl;
    auto node = rbtree->Search(35);
    if (node != rbtree->nil) {
        cout << "Found." << endl;

        cout << "Predecessor of this node:\t" << rbtree->Pred(node)->data << endl;
        cout << "Successor of this node:\t\t" << rbtree->Succ(node)->data << endl;
    } else {
        cout << "Not Found." << endl;
    }

    printSplitter();

    cout << "Calling `Search` to get node with data `36`:" << endl;
    node = rbtree->Search(36);
    if (node != rbtree->nil) {
        cout << "Found." << endl;

        cout << "Predecessor of this node:\t" << rbtree->Pred(node)->data << endl;
        cout << "Successor of this node:\t\t" << rbtree->Succ(node)->data << endl;
    } else {
        cout << "Not Found." << endl;
    }

    printSplitter();


#if FALSE
    cout << "Now calling `Delete` to remove the node we found above." << endl;
    rbtree->Delete(node);
    cout << "Removal done." << endl;
    cout << "And we traverse the tree again:" << endl;

    placeholder.clear();
    rbtree->InOrder(placeholder);

    for (auto it = placeholder.begin(); it != placeholder.end(); ++it)
        cout << (*it)->data << ' ';
    cout << endl;
#endif

    rbtree->Persist("trees/rbtree.dot");
}

inline void testOSTree () {
    // Generate a OSTree with provided data

    auto tree = new OSTree<int>;

    std::vector<int> numbers{3, 1, 4, 15, 9, 2, 6, 5, 35, 8, 97, 93, 23, 84, 62};

    printSplitter();

    cout << "Calling `Insert` to set up RBTree with the following numbers:"<< endl;
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        cout << *it << ' ';
        tree->Insert(*it);
    }
    cout << endl << "Successfully inserted " << numbers.size() << " numbers." << endl;

    printSplitter();

    OSTree<int>::vector_t placeholder;
    tree->InOrder(placeholder);

    cout << "Calling `InOrder` to traverse the tree in order:" << endl;
    for (auto it = placeholder.begin(); it != placeholder.end(); ++it)
        cout << (*it)->data.value << ' ';
    cout << endl;

    printSplitter();

    cout << "Now we call `Select` to select elements by rank:" << endl;
    for (size_t i = 1; i <= tree->root->data.size; ++i) {
        auto node = tree->Select(i);
        cout << "Rank:\t" << i << "\t" << "Value:\t" << node->data.value << endl;
    }

    printSplitter();

    cout << "And then we call `Rank` to evaluate the rank of `35`:" << endl;
    auto node = tree->Search(35);
    cout << "The rank of `35` is " << tree->Rank(node) << endl;

    printSplitter();

    cout << "And then we call `Rank` to evaluate the rank of `36`:" << endl;
    node = tree->Search(36);
    cout << "The rank of `36` is " << tree->Rank(node) << endl;
    cout << "`0` represents that the element is not in the tree." << endl;



    tree->Persist("trees/ostree.dot");
}

inline void testBigTree () {
    // Generate a big tree with 100 nodes
    auto tree = new RBTree<int>;
    srand(42);
    for (int i = 0; i < 100; i++) {
        tree->Insert(rand() % 10000);
    }
    tree->Persist("trees/big.dot");
}

int main () {

    cout << "########################### Testing RBTree ###########################" << endl;
    testRBTree();
    cout << "########################### Testing OSTree ###########################" << endl;
    testOSTree();
    testBigTree();
    return 0;
}
