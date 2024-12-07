/*С появлением почтовой службы в Квадратном королевстве,
 наконец, появилась возможность обмена сообщениями.
Уйма различных почтовых индексов обескуражила почтальонов,
поэтому они решили составить справочник, в котором каждому индексу,
состоящему из нескольких цифр и букв, соответствовал некоторый адрес
почтового отделения. В помощь почтальонам, напишите программу,
эффективно реализующую такой справочник, а также предлагающую
функцию автодополнения индексов при неполном вводе.

Коллекция: префиксное дерево <индекс, адрес> с методом,
возвращающим список хранящихся индексов, начинающихся с заданной
последовательности.
*/
#pragma once
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <boost/serialization/vector.hpp> 
#include <boost/serialization/string.hpp> 
using namespace std;

class nodeData
{
public:
	nodeData();
	nodeData(string, string);
	nodeData(char);
	nodeData(const nodeData&);
	~nodeData();
	char index_char;
	string address;
	vector<nodeData*> children{};
	bool is_end=false;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
        ar & index_char;
        ar & address;
        ar & is_end;
        ar & children;
    }

};

struct endNodeData
{
	string address; 
	string full_index;

};


class IndexAddressTree{
public:
	IndexAddressTree();
	IndexAddressTree(const IndexAddressTree&);
	~IndexAddressTree();

	bool delete_values();
	int length(nodeData* n = nullptr) const;
	bool save_to_file(string) const;
	bool load_from_file(string);
	
	endNodeData* operator [] (string) const;
	IndexAddressTree* operator && (const IndexAddressTree*);

	int remove(endNodeData* entry, nodeData* cur = nullptr, int ind = 0);
	bool has_index(string);

	vector<string> find_indexes(nodeData* n = nullptr, vector<string> r = {}, string s = "") const;
	void debug_print(nodeData* cur, string prefix) const;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
    {
        ar & root;
    }

	vector<string> autocomplete(string prefix, nodeData* cur = nullptr, vector<string> r = {}) const;
	bool operator << (endNodeData);

private:
		nodeData* root;
};


bool operator == (const IndexAddressTree&, const IndexAddressTree&);
bool operator == (const endNodeData&, const endNodeData&);