#include "index_collection.h"

using namespace std;

#include <cassert>
#include <iostream>

void testInsFind() {
    IndexAddressTree tree;

    endNodeData entry = {"Some Address", "12345"};
    tree << entry;

    endNodeData* result = tree["12345"];
    assert(result != nullptr); 
    assert(result->address == "Some Address");
    assert(result->full_index == "12345"); 
    cout << "[InsFind] OK" << endl;
}

void testAutoc() {
    IndexAddressTree tree;

    tree << endNodeData{"Address A", "12345"};
    tree << endNodeData{"Address B", "12346"};
    tree << endNodeData{"Address C", "12347"};

    vector<string> result = tree.autocomplete("1234");

    assert(result.size() == 3);
    assert(result[0] == "12345");
    assert(result[1] == "12346");
    assert(result[2] == "12347");

    cout << "[Autoc] OK" << endl;
}

void testRm() {
    IndexAddressTree tree;

    tree << endNodeData{"Some Address", "12345"};
    tree << endNodeData{"Another Address", "12346"};

    endNodeData* entry = tree["12345"];
    tree.remove(entry, tree.root);
    assert(tree["12345"] == nullptr);

    entry = tree["12345"];
    assert(entry == nullptr);

    cout << "[Rm] OK" << endl;
}

void testEq() {
    IndexAddressTree tree1;
    IndexAddressTree tree2;

    tree1 << endNodeData{"Address A", "12345"};
    tree2 << endNodeData{"Address A", "12345"};

    assert(tree1 == tree2);

    tree1 << endNodeData{"Address B", "12346"};

    assert(!(tree1 == tree2));

    cout << "[Eq] OK" << endl;
}

void testSaveLoad() {
    IndexAddressTree tree;
    tree << endNodeData{"Some Address", "12345"};

    bool save_success = tree.save_to_file("test_tree.dat");
    assert(save_success);

    IndexAddressTree new_tree;
    bool load_success = new_tree.load_from_file("test_tree.dat");
    assert(load_success);

    endNodeData* result = new_tree["12345"];
    assert(result != nullptr);
    assert(result->address == "Some Address");
    assert(result->full_index == "12345");

    cout << "[SaveLoad] Ok" << endl;
}

void testDelVal() {
    IndexAddressTree tree;

    tree << endNodeData{"Address A", "12345"};
    tree << endNodeData{"Address B", "12346"};

    assert(tree["12345"] != nullptr);
    assert(tree["12346"] != nullptr);

    tree.delete_values(tree.root);

    assert(tree["12345"] == nullptr);
    assert(tree["12346"] == nullptr);
    cout << "[Delval] OK" << endl;
}

int main() {

    testInsFind();
    testAutoc();
    testRm();
    testEq();
    testSaveLoad();
    testDelVal();

    cout << "Tests passed!" << endl;
    return 0;
}