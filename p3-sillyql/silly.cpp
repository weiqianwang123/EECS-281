// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D


#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>
#include <utility>
#include <map>
#include <unordered_map>
#include "getopt.h"
#include "TableEntry.h"

using namespace std;



class Table 
{
    friend class Base;
    vector<vector<TableEntry>> table;//every table is a 2d matrix
    vector<EntryType> colType; //every col may have different types
    unordered_map<string,int> colName;//every col has its own name and index,map is used for name to index
   
    
    map<TableEntry,vector<size_t>> bst;
    unordered_map<TableEntry,vector<size_t>> hash;

    string indexedColName = "";
    string indexedType = "";
    int indexedColIndex ;
private:
    TableEntry convert2TableEntry(string,EntryType);

};

TableEntry Table::convert2TableEntry(string value_in,EntryType type_in)
{
    //String
	if (type_in == EntryType::String)
    {
        return TableEntry(value_in);
    }
	//Double
	else if (type_in == EntryType::Double) 
    {
		return TableEntry(stod(value_in));
	}

	//Int
	else if (type_in == EntryType::Int) 
    {
		
		return TableEntry(stoi(value_in));
	}

	//Boolean
	else 
    {
		if (value_in == "true")
        {
            return TableEntry(true);
        }
		{
            return TableEntry(false);
        }
		
	}
}

class Compare
{

public:
    Compare(string type_in,TableEntry value_in,int col_index_in) :
        type{type_in},value_{value_in},col_index{col_index_in} {}
    
    bool operator()(vector<TableEntry>& row) {
        if(type=="<")
        {
            return row[col_index] < value_;
        }
        else if(type==">")
        {
            return row[col_index] > value_;
        }
        else
        {
             return row[col_index] == value_;
        }
		
	}
private:
    string type;
    TableEntry value_;
    int col_index;
    
};
class Base
{
    bool is_quiet{false};
    unordered_map<string ,Table*> base;//data base from name(string) to table
    
public:
    void getOptions(int, char**);
    void create();
    void quit();
    void remove();
    void insert();
    void print();
    void _delete();
    void generate();
    void join();
};

int main(int argc, char** argv) {

	ios_base::sync_with_stdio(false);  
	cin >> std::boolalpha;
	cout << std::boolalpha;

    string command = "";
	Base base;
	base.getOptions(argc, argv);


	while (true) {
		

		
		cin >> command;
        cout << "% ";
        try
        {
             if (command == "CREATE")
            {
                base.create();
            }
            else if (command == "REMOVE")
            {
                base.remove();
            }
            else if (command == "QUIT")
            {
                base.quit();
                break;
            }
            else if(command == "PRINT")
            {
                base.print();
            }
            else if(command[0]=='#')
            {
                string temp;
                getline(cin, temp); 
            }
            else if(command=="INSERT")
            {
                base.insert();
            }
            else if(command == "DELETE")
            {
                base._delete();
            }
            else if(command=="GENERATE")
            {
                base.generate();
            }
            else if(command =="JOIN")
            {
                base.join();
            }
            else
            {
                throw string{"Error: unrecognized command"};
            }
                
        }
        catch(string error)
        {
            string temp;
            getline(cin, temp); 
            cout<<error<<"\n";
        }
        
       
				
    }	
    return 0;

}

//get option
void Base::getOptions(int argc, char** argv)
{
    int option_index = 0, option = 0;
	opterr = false;
	struct option longOpts[] = {{ "quiet", no_argument, nullptr, 'q' },
								{ "help", no_argument, nullptr, 'h'},
								{ nullptr, 0, nullptr, '\0' } };

	while ((option = getopt_long(argc, argv, "qh", longOpts, &option_index)) != -1) {
		switch (option) {
		case 'q':
			is_quiet = true;
			break;

		case 'h':
			cout <<"nothing helpful"<<endl;
			exit(1);
		}
	}

}

void Base::create()
{
    
    string table_name ;
    int table_num;
    cin>>table_name>>table_num;
    
    if(base.find(table_name)!=base.end())
    {
        throw string{"Error during CREATE: Cannot create already existing table "+table_name};
    }

    Table* new_table = new Table;
    new_table->colType.reserve(table_num); //reserve capacity for types
    string type; //type for each col
    for(int i=0;i<table_num;i++)
    {
        cin>>type;
        if (type[0] == 's')
			new_table->colType.emplace_back(EntryType::String);
        else if (type[0] == 'b')
			new_table->colType.emplace_back(EntryType::Bool);
		else if (type[0] == 'i')
			new_table->colType.emplace_back(EntryType::Int);
		else 
			new_table->colType.emplace_back(EntryType::Double);

        

		
    }
    cout << "New table " << table_name << " with column(s) ";
    vector<string> print(table_num);
    for(int i=0;i<table_num;i++)
    {
        cin>>type;
        new_table->colName[type] = i;
        print[i] = type;
    }
    

    this->base[table_name] = new_table;
    
	for (auto name : print)
		cout << name << " ";
	cout << "created\n";
	

}

void Base::quit()
{
    for (auto e : this->base)
    {
        delete e.second;
    }
    cout << "Thanks for being silly!\n";
		
}

void Base::remove()
{
    string table_name;
    cin>>table_name;
    //if error: table not exist
    if(base.find(table_name)==base.end())
    {
        throw string{"Error during REMOVE: "+table_name+" does not name a table in the database"};
    }
    auto it = base.find(table_name);
    if(it!=base.end())
    {
        delete it->second;
        base.erase(it);
        cout<<"Table "<<table_name<<" removed\n";
    }
    
}

void Base::insert()
{
    
    string temp;
    cin>>temp; //INTO
    string table_name;
    cin>>table_name;
   
   
    int num;
    cin>>num;
    cin>>temp;//ROWS
    //if error: table not exist
    if(base.find(table_name)==base.end())
    {
        for (int i = 0; i < num; i++) 
        {
			cin.clear();
			
		}
        throw string{"Error during INSERT: "+table_name+" does not name a table in the database"};
    }
    Table* current_table = this->base[table_name];
    int current_num = (int)(current_table->table).size();
    int total_num = num+current_num;
    current_table->table.resize(total_num);
    for(int i=current_num;i<total_num;i++)
    {
        current_table->table[i].reserve(current_table->colName.size());
    }
    
    if(current_table->indexedColName=="")
    {
        for(int i=current_num;i<total_num;i++)
        {
            for(int j=0;j<(int)current_table->colName.size();j++)
            {
                cin>>temp;
                current_table->table[i].emplace_back(current_table->convert2TableEntry(temp, current_table->colType[j]));
            }
        }
    }
    else
    {
        for(int i=current_num;i<total_num;i++)
        {
            for(int j=0;j<(int)current_table->colName.size();j++)
            {
                cin>>temp;
                current_table->table[i].emplace_back(current_table->convert2TableEntry(temp, current_table->colType[j]));
            }
            if(current_table->indexedType=="hash")
            {
                current_table->hash[current_table->table[i][current_table->indexedColIndex]].push_back(i);
            }
            else
            {
                current_table->bst[current_table->table[i][current_table->indexedColIndex]].push_back(i);
            }
        }
    }
    

    cout << "Added " << num << " rows to " << table_name << " from position "<< current_num << " to " <<total_num-1 << "\n";
}

void Base::print()
{
    string temp;
    cin>>temp;//FROM
    string table_name;
    cin>>table_name;
    //if error: table not exist
    if(base.find(table_name)==base.end())
    {
        throw string{"Error during PRINT: "+table_name+" does not name a table in the database"};
    }
    Table* current_table = this->base[table_name];
    int num;
    cin>>num;


    vector<string> colName;
    vector<int> colIndex;
    colName.reserve(num);
    colIndex.reserve(num);
    for(int i=0;i<num;i++)
    {
        cin>>temp;
        auto it = current_table->colName.find(temp);
        if(it==current_table->colName.end())
        {
            throw string{"Error during PRINT: "+temp+ " does not name a column in "+table_name};
        }
        
        colName.emplace_back(temp);
        colIndex.emplace_back((*it).second);
        
        
    }

    cin>>temp;//condition
    if(temp=="ALL")
    {
        if(!is_quiet)
        {
            for(int i=0;i<num;i++)
            {
                cout<<colName[i]<<" ";
            }
            cout<<"\n";
            for(size_t i=0;i<current_table->table.size();i++)
            {

                for(int j=0;j<num;j++)
                {
                    cout<< current_table->table[i][colIndex[j]]<<" ";
                }
                cout<<"\n";
            }

        }
        
        cout << "Printed " << current_table->table.size() << " matching rows from " << table_name << "\n";
    }
    else if(temp == "WHERE")
    {
        string col_name;
        cin>>col_name;
        string op;
        cin>>op;
        string value;
        cin>>value;

        int print_count = 0;

        auto it_temp = current_table->colName.find(col_name);
        if(it_temp ==current_table->colName.end())
        {
            throw string{"Error during PRINT: "+col_name+ " does not name a column in "+table_name};
        }

        int col_index = (*it_temp).second;
        if(!is_quiet)
        {
            for(int i=0;i<num;i++)
            {
                cout<<colName[i]<<" ";
            }
            cout<<"\n";
        }
        

        if(op =="=")
        {
            if(col_name==current_table->indexedColName&&current_table->indexedType=="bst")
            {
                TableEntry val = current_table->convert2TableEntry(value,current_table->colType[col_index]);
				auto it_start = current_table->bst.find(val);
				if (it_start != current_table->bst.end()) {
					for (auto k : it_start->second) 
                    {
						if(!is_quiet)
                        {
                            for (auto index : colIndex) 
                            {
                                cout << current_table->table[k][index] << " ";
                                
                            }
                            cout << "\n";

                        }
						
						
						
						print_count++;
					}
				}

            }
            else if(col_name==current_table->indexedColName&&current_table->indexedType=="hash")
            {
                TableEntry val = current_table->convert2TableEntry(value,current_table->colType[col_index]);
				auto it_start = current_table->hash.find(val);
				if (it_start != current_table->hash.end()) {
					for (auto k : it_start->second) 
                    {
						if(!is_quiet)
                        {
                            for (auto index : colIndex) 
                            {
                                cout << current_table->table[k][index] << " ";
                                
                            }
                            cout << "\n";
                        }
						
						print_count++;
					}
				}
            }
            else
            {
                for(size_t i=0;i<current_table->table.size();i++)
                {
                    if(current_table->convert2TableEntry(value,current_table->colType[col_index]) == current_table->table[i][col_index])
                    {
                        if(!is_quiet)
                        {
                            for(int j=0;j<num;j++)
                            {
                                cout<< current_table->table[i][colIndex[j]]<<" ";
                            }
                            cout<<"\n";
                        }
                        print_count++;
                    }

                }

            }
           
            cout << "Printed " <<  print_count << " matching rows from " << table_name << "\n";
        }
        else if(op==">")
        {
            if(col_name==current_table->indexedColName&&current_table->indexedType=="bst")
            {
                TableEntry val = current_table->convert2TableEntry(value,current_table->colType[col_index]);
                auto it_start = current_table->bst.upper_bound(val);
                while (it_start != current_table->bst.end()) 
                {
					for (auto k : it_start->second) 
                    {
						if(!is_quiet)
                        {
                            for (auto index : colIndex) 
                            {
                                cout << current_table->table[k][index] << " ";
                                
                            }
                            cout << "\n";
                        }
						
						print_count++;
					}
                    it_start++;
				}

            }
            else
            {
                for(size_t i=0;i<current_table->table.size();i++)
                {
                    if(current_table->convert2TableEntry(value,current_table->colType[col_index]) < current_table->table[i][col_index])
                    {
                        if(!is_quiet)
                        {
                            for(int j=0;j<num;j++)
                            {
                                cout<< current_table->table[i][colIndex[j]]<<" ";
                            }
                            cout<<"\n";
                        }
                        print_count++;
                    }

                }
            }
            
            cout << "Printed " <<  print_count << " matching rows from " << table_name << "\n";
            
        }
        else if(op == "<")
        {
            if(col_name==current_table->indexedColName&&current_table->indexedType=="bst")
            {
                TableEntry val = current_table->convert2TableEntry(value,current_table->colType[col_index]);
                auto it_end = current_table->bst.lower_bound(val);
                auto it_start = current_table->bst.begin();
                while (it_start != it_end) 
                {
					for (auto k : it_start->second) 
                    {
						if(!is_quiet)
                        {
                            for (auto index : colIndex) 
                            {
                                cout << current_table->table[k][index] << " ";
                                
                            }
                            cout << "\n";
                        }
						
						print_count++;
					}
                    it_start++;
				}

            }
            else
            {
                for(size_t i=0;i<current_table->table.size();i++)
                {
                    if(current_table->convert2TableEntry(value,current_table->colType[col_index]) > current_table->table[i][col_index])
                    {
                        if(!is_quiet)
                        {
                            for(int j=0;j<num;j++)
                            {
                                cout<< current_table->table[i][colIndex[j]]<<" ";
                            }
                            cout<<"\n";
                        }
                        print_count++;
                    }

                }
            }
            
            cout << "Printed " <<  print_count << " matching rows from " << table_name << "\n";

        }
    }
}

void Base::generate()
{
    string temp;
    cin>>temp;//FOR
    string table_name;
    cin>>table_name;
    string index_type;
    cin>>index_type;
    cin>>temp;//INDEX
    cin>>temp;//ON
    string col_name;
    cin>>col_name;


    if(base.find(table_name)==base.end())
    {
        throw string{"Error during GENERATE: "+table_name+" does not name a table in the database"};
    }
    Table* current_table = this->base[table_name];
    auto it_col = current_table->colName.find(col_name);
    if(it_col==current_table->colName.end())
    {
        throw string{"Error during GENERATE: "+col_name+ " does not name a column in "+table_name};
    }
    int col_index = (*current_table->colName.find(col_name)).second;

    if(!current_table->bst.empty())
    {
        current_table->bst.clear();
    }
    else if(!current_table->hash.empty())
    {
        current_table->hash.clear();
    }

    if(index_type == "bst")
    {
        for(size_t i=0;i<current_table->table.size();i++)
        {
            current_table->bst[current_table->table[i][col_index]].push_back(i);
        }
         cout << "Created " << index_type << " index for table " << table_name << " on column " <<col_name << ", with "<<current_table->bst.size()<<" distinct keys\n";
    }
    else if(index_type=="hash")
    {
        for(size_t i=0;i<current_table->table.size();i++)
        {
            current_table->hash[current_table->table[i][col_index]].push_back(i);
        }
         cout << "Created " << index_type << " index for table " << table_name << " on column " <<col_name << ", with "<<current_table->hash.size()<<" distinct keys\n";
    }
    current_table->indexedColName = col_name;
    current_table->indexedColIndex = col_index;
    current_table->indexedType = index_type;

   

}

void Base::_delete()
{
    string temp;
    string table_name;
    cin>>temp;//FROM
    cin>>table_name;
    cin>>temp;//WHERE
    string col_name;
    cin>>col_name;
    string op;
    cin>>op;
    string value;
    cin>>value;

   
    //if error: table not exist
    if(base.find(table_name)==base.end())
    {
        throw string{"Error during DELETE: "+table_name+" does not name a table in the database"};
    }
    Table* current_table = base[table_name]; 
    auto it_col = current_table->colName.find(col_name);
    if(it_col==current_table->colName.end())
    {
        throw string{"Error during DELETE: "+col_name+ " does not name a column in "+table_name};
    }
    int col_index = (*it_col).second;
    int current_num = (int)current_table->table.size();

    auto it = current_table->table.end();
    
    it = remove_if(current_table->table.begin(), current_table->table.end(), 
    Compare(op,current_table->convert2TableEntry(value,current_table->colType[col_index]),col_index));

    current_table->table.erase(it,current_table->table.end());

    cout << "Deleted " << current_num-current_table->table.size() << " rows from " << table_name << "\n";

    //update the index in bst or hash
    col_index = current_table->indexedColIndex;
    string index_type =  current_table->indexedType;
    if(!current_table->bst.empty())
    {
        current_table->bst.clear();
    }
    else if(!current_table->hash.empty())
    {
        current_table->hash.clear();
    }

    if(index_type == "bst")
    {
        for(size_t i=0;i<current_table->table.size();i++)
        {
            current_table->bst[current_table->table[i][col_index]].push_back(i);
        }

    }
    else if(index_type=="hash")
    {
        for(size_t i=0;i<current_table->table.size();i++)
        {
            current_table->hash[current_table->table[i][col_index]].push_back(i);
        }
    }

}



void Base::join() {
    string temp;
    string table1, table2, compCol1, compCol2;
    cin >>table1>>temp>>table2>>temp>>compCol1>>temp>>compCol2;


    //if error: table not exist
    if(base.find(table1)==base.end())
    {
        throw string{"Error during JOIN: "+table1+" does not name a table in the database"};
    }
    if(base.find(table2)==base.end())
    {
        throw string{"Error during JOIN: "+table2+" does not name a table in the database"};
    }

    // Accessing tables from the base map
    Table* tablePtr1 = this->base.find(table1)->second;
    Table* tablePtr2 = this->base.find(table2)->second;


    auto it_col_1 = tablePtr1->colName.find(compCol1);
    if(it_col_1==tablePtr1->colName.end())
    {
        throw string{"Error during JOIN: "+compCol1+ " does not name a column in "+table1};
    }
    auto it_col_2 = tablePtr2->colName.find(compCol2);
    if(it_col_2==tablePtr2->colName.end())
    {
        throw string{"Error during JOIN: "+compCol2+ " does not name a column in "+table2};
    }
    // Finding column indexes in the tables
    size_t numCol1 = it_col_1->second;
    size_t numCol2 = it_col_2->second;

    // Processing join conditions and selected columns for output
    int numCols;
    cin >>temp>>temp>>numCols;
    vector<pair<int, string>> colNames; // First: table number, Second: column name
    for (int i = 0; i < numCols; ++i) {
        int tableNum = 0;
        string colName;
        cin >> colName >> tableNum;
        if(tableNum==1)
        {
            it_col_1 = tablePtr1->colName.find(colName);
            if(it_col_1==tablePtr1->colName.end())
            {
                throw string{"Error during JOIN: "+colName+ " does not name a column in "+table1};
            }
        }
        else
        {
            it_col_2 = tablePtr2->colName.find(colName);
            if(it_col_2==tablePtr2->colName.end())
            {
                throw string{"Error during JOIN: "+colName+ " does not name a column in "+table2};
            }
        }
        colNames.push_back({tableNum, colName});
    }

    // Printing column names
    if(!is_quiet)
    {
        for (const auto& col : colNames) 
        {
            cout << col.second << " ";
        }
        cout << "\n";
    }
    

    int printed = 0;
    unordered_map<TableEntry, vector<size_t>> hash(tablePtr2->table.size()*2);
    if(tablePtr2->indexedColName != compCol2 || tablePtr2->indexedColName =="")
    {
        
        for (size_t i = 0; i < tablePtr2->table.size(); ++i) 
        {
            hash[tablePtr2->table[i][numCol2]].push_back(i);
        }
    }
   
    // Iterating through rows of the first table
    for (const auto& row1 : tablePtr1->table) {
        // Find matching rows in the second table based on index type
        vector<size_t> matchingRows;
        if (tablePtr2->indexedColName == compCol2 && tablePtr2->indexedType == "bst") {
            auto it = tablePtr2->bst.find(row1[numCol1]);
            if (it != tablePtr2->bst.end()) {
                matchingRows = it->second;
            }
        } else if (tablePtr2->indexedColName == compCol2 && tablePtr2->indexedType == "hash") {
            auto it = tablePtr2->hash.find(row1[numCol1]);
            if (it != tablePtr2->hash.end()) {
                matchingRows = it->second;
            }
        } else {
            // No index or different column indexed, full table scan required
           
            auto it = hash.find(row1[numCol1]);
            if (it != hash.end()) {
                matchingRows = it->second;
            }
        }

        // Printing matching rows
        for (size_t idx : matchingRows) {
            
            if(!is_quiet)
            {
                for (const auto& col : colNames) 
                {
                    if (col.first == 1) 
                    {
                        cout << row1[tablePtr1->colName[col.second]] << " ";
                    } else 
                    {
                        cout << tablePtr2->table[idx][tablePtr2->colName[col.second]] << " ";
                    }
                }
                cout<<"\n";
            }
            
            printed++;
        }
    }

    cout << "Printed " << printed << " rows from joining " << table1 << " to " << table2 << endl;
}
