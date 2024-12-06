//g++ -o index_collection index_collection.cpp main.cpp -lboost_serialization -lboost_system


#include "index_collection.h"
using namespace std;

IndexAddressTree::IndexAddressTree(){
	root = new nodeData();
}

IndexAddressTree::~IndexAddressTree(){
	return;
}

nodeData::nodeData(){
	is_end = false;
	address = "\x0";
	index_char = '\x0';
	is_end = false;
}

nodeData::nodeData(string add, string fullindex){
	address = add;
	index_char = fullindex.back();
	is_end=true;
}

nodeData::nodeData(char i){
	index_char = i;
	is_end = false;
}

nodeData::nodeData(const nodeData& other)
    : index_char(other.index_char),
      address(other.address),
      is_end(other.is_end) {
    for (const auto& child : other.children) {
        children.push_back(new nodeData(*child));
    }
}


bool operator==(endNodeData t1, endNodeData t2){
	//cout << "Comparing endNodes..." << endl;
	return (t1.address == t2.address && t1.full_index == t2.full_index);
}

bool operator << (const IndexAddressTree& t, endNodeData entry){
	string index = "";
	nodeData* current_node = t.root;
	int pos = 0;
	nodeData* next;
	string inserting_index = entry.full_index;
	vector<nodeData*>::iterator i;
	while(pos!= inserting_index.size() - 1){
		vector<nodeData*>::iterator i;
		for(i = current_node->children.begin(); (i < current_node->children.end() && \
		 							(*i)->index_char != inserting_index[pos]); i++);

		if(i == current_node->children.end()){
			 //не нашелся этот символ, добавим его
			//cout << "inserting: " << inserting_index[pos] << endl;
			next = new nodeData(inserting_index[pos]);
			current_node->children.push_back(next);
		}
		else{
			next = *i;
		}

		current_node = next;
		index+=inserting_index[pos];
		pos+=1;
	}
	//теперь мы нашли куда вставить наш адрес. надо проверить что такого еще нет
	//cout << current_node->index_char;

	for(i = current_node->children.begin(); (i < current_node->children.end() && \
		 							(*i)->index_char != inserting_index[pos]); i++);
	if(i!=current_node->children.end()){
		cout << "[ERR] this index is taken";
		return false;
	}
	nodeData *insert = new nodeData(entry.address, entry.full_index);
	current_node->children.push_back(insert);
	return true;


};


int IndexAddressTree::length(nodeData* b) const{
	
	if(b==nullptr) b=root;

	if (b->is_end) {

		//cout << "end: "; //<<// ((struct endNodeData *)(b))->address << end;
		return 1;
	}
	else{
		int s = 0;
		for(auto i=b->children.begin(); i < b->children.end(); i++) s+=this->length(*i);
		return s;
	}
}

int IndexAddressTree::remove(endNodeData* entry, nodeData* cur, int ind){
	if(ind == entry->full_index.size()){
		delete cur; return 1;
	} //cur - end
	else{
		//cout << "looking for " << entry->full_index[ind] << "in" << entry << endl;
		vector<nodeData*>::iterator i;
		for(i = cur->children.begin(); (i< cur->children.end() && (*i)->index_char != entry->full_index[ind]); i++){
			cout << (*i)->index_char << endl;
		}
		if(i == cur->children.end()){
		cout << "[ERR] index not found";
		return -1;}
		else{
			int removed = this->remove(entry, (*i), ++ind);
			if (removed == -1) return -1;
			if (removed == 0) return 0;
			cur->children.erase(i);
			if(cur->children.empty()){delete cur;return 1;}
			return 0;
		}	
	}
}

endNodeData* IndexAddressTree::operator [] (string ind) const{
	nodeData* cur = this->root;
	for(char& c : ind) {
		vector<nodeData*>::iterator i;
    	for(i = cur->children.begin(); i<cur->children.end() && (*i)->index_char != c; i++);
    	if (i == cur->children.end()){
    		cout << "[ERR] index not found" << endl;
    		return nullptr;
    	}
    	cur = (*i);
	}

	if(cur->is_end) {
		endNodeData* ret = new endNodeData;
		ret->address = cur->address;
		ret->full_index = ind;
		return ret;
	}
	cout << "[ERR] index not found" << endl;
	return nullptr;
}

vector<string> IndexAddressTree::find_indexes(nodeData* cur, vector<string> found, string ind) const{
	if(cur == nullptr) cur = root;
	if(cur->is_end) {found.push_back(ind);
		//cout << ind;
	}
	for(auto i = cur->children.begin(); i < cur->children.end(); i++){
		found = this->find_indexes((*i), found, ind+((*i)->index_char));
	}

	return found;
}

IndexAddressTree* IndexAddressTree::operator && (const IndexAddressTree* t){
	vector<string> ind = this->find_indexes();

	IndexAddressTree* newtree = new IndexAddressTree();
	for (auto i = ind.begin(); i<ind.end(); i++){
		if (*(*this)[*i] == *(*t)[*i]){
			endNodeData* n = new endNodeData{(*this)[*i]->address, (*i)};
			*newtree << *n;
		}
	}

	return newtree;
}


bool operator == (const IndexAddressTree& t1, const IndexAddressTree& t2){
	//cout << "Comparing trees..." << endl;
	vector<string> i1 = t1.find_indexes();
	vector<string> i2 = t2.find_indexes();

	for(auto i = i1.begin(); i<i1.end(); i++){
		if(t2[*i]==nullptr || !(*(t1[*i]) == *(t2[*i]))){
		//cout << "NOT found " << *i << endl;
		return false;
		} 
	}

	for(auto i = i2.begin(); i<i2.end(); i++){
		if(t1[*i] == nullptr  || !(*(t1[*i]) == *(t2[*i]))){ 
			//cout << "NOT found " << *i << endl;
			return false;}
	}

	return true;
};

bool IndexAddressTree::delete_values(nodeData* cur){
	if(cur==nullptr) cur = root;
	if(cur->children.empty() && cur!=root){
		//cout << "deleted child";
		delete cur;
		return true;
	}

	for(auto i = cur->children.begin(); i<cur->children.end(); i++){
		//cout << (*i)->index_char;
		this->delete_values(*i);
		(*i)->children.clear();
	}
	if(cur!=root){
	delete cur;
	}
	return true;
}


bool IndexAddressTree::has_index(string i){
	if(((*this)[i]) != nullptr) return true;
	return false;
};//

bool IndexAddressTree::save_to_file(string f) const{
	ofstream ofs(f);
	if(ofs.is_open()){
		cout << f << endl;
		boost::archive::text_oarchive oa(ofs);
		oa & *this;
		return true;
	}
	cout << "[ERR] write";
	return false;

};//
bool IndexAddressTree::load_from_file(string f){
	ifstream ifs(f);
	if(ifs.is_open()){
		boost::archive::text_iarchive ia(ifs);
		ia & *this;
		//cout << "LOADED" << endl;
		return true;
	}
	cout << "[ERR] load" << endl;
	return false;
};


void IndexAddressTree::debug_print(nodeData* cur, string prefix) const {
    if (!cur) cur = root;
    cout << "cur = " << cur << endl;
    if (cur->is_end) {
        cout << "Index: " << prefix << " -> Address: " << cur->address << endl;
    }

    for (auto* child : cur->children) {
        debug_print(child, prefix + child->index_char);
    }
}


vector<string> IndexAddressTree::autocomplete(string prefix, nodeData* cur, vector<string> r) const{
	if (cur == nullptr){
		cur = root;
		int i;
		vector<nodeData*>::iterator ii;
		for(i = 0; i<prefix.size(); i++){
			for(ii = cur->children.begin(); ii<cur->children.end() && \
				(*ii)->index_char != prefix[i]; ii++){
			}
			if(ii == cur->children.end()){
				//cout << "No autocomplete. " << endl;
				return {};
			}
			cur = *ii;
		}
	}

	if (cur->is_end){
		r.push_back(prefix);
		return r;
	}

	for(auto i: cur->children){
		r = this->autocomplete(prefix+(i->index_char), i, r);
	}
	return r;

};