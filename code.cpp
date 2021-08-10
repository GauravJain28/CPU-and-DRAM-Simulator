#include <bits/stdc++.h>
using namespace std;

vector<int> INS_COUNT;
int ROW_ACCESS_DELAY = 10;
int COL_ACCESS_DELAY = 2;
int CC = 0;
int ROW_BUFFER_UPDATE = 0;
int N;
int M;
int QSIZE = 32;
bool DETAILS;
int PREV = -1;
int CURR = -1;

bool CHANGED = false; //defined in assignment 4/5

vector<map<string, int>> labels;

map<string, int> Register_map {
    	{"$zero",0},{"$1",1},{"$2",2},{"$3",3},{"$4",4},{"$5",5},{"$6",6},{"$7",7},{"$8",8}, {"$9",9},{"$10",10},{"$11",11},{"$12",12},{"$13",13},{"$14",14},{"$15",15},{"$16",16},{"$17",17},{"$18",18}, {"$19",19},{"$20",20},{"$21",21},{"$22",22},{"$23",23},{"$24",24},{"$25",25},{"$26",26},{"$27",27},{"$28",28}, {"$29",29}, {"$30",30},{"$31",31},{"$r0",0},{"$at",1},{"$v0",2},{"$v1",3},{"$a0",4},{"$a1",5},{"$a2",6},{"$a3",7},{"$t0",8}, {"$t1",9},{"$t2",10},{"$t3",11},{"$t4",12},{"$t5",13},{"$t6",14},{"$t7",15},{"$s0",16},{"$s1",17},{"$s2",18}, {"$s3",19},{"$s4",20},{"$s5",21},{"$s6",22},{"$s7",23},{"$t8",24},{"$t9",25},{"$k0",26},{"$k1",27},{"$gp",28}, {"$sp",29}, {"$s8",30},{"$ra",31}
    }; 

map<int, string> map_int_register {
    {0, "$zero"},{1, "$at"},{2, "$v0"},{3, "$v1"},{4,"$a0"},{5,"$a1",},{6,"$a2"},{7,"$a3"},{8,"$t0"}, {9,"$t1"},{10,"$t2"},{11,"$t3"},{12,"$t4"},{13,"$t5"},{14,"$t6"},{15,"$t7"},{16,"$s0",},{17,"$s1"},{18,"$s2"}, {19,"$s3"},{20,"$s4"},{21,"$s5"},{22,"$s6"},{23,"$s7"},{24,"$t8"},{25,"$t9"},{26,"$k0"},{27,"$k0"},{28,"$gp"}, {29,"$sp"}, {30,"$s8"},{31,"$ra"}
}; //defined in assignment 4

class ins{
    public:
    string sen;
    string op;
    vector<string> params;
    int l_addr;
};

typedef unsigned char byte;

inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

string removeSpaces(string str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
}

int stringToInt(string str){
    char *end;
    long i = strtol( str.c_str(), &end, 10 );
    if (end == str.c_str())
    {
        cout << "Invalid integer entry\n";
        throw runtime_error("Invalid integer entry\n");
    }
    
    if ( *end == '\0' )
    {
        return (int) i;
    }
    else
    {
        cout << "Invalid integer entry\n";
        throw runtime_error("Invalid integer entry\n");
        
    }
}

int modifyReg(string str){
        if (str.substr(0,1) != "$"){
            cout << "Invalid register entry\n";
            throw "Invalid integer entry\n";
        }
        else{
            if (str.find("0")<str.size()-1){
                cout << "Invalid register entry\n";
                throw "Invalid integer entry\n";
            }
            if(str=="$zero") return 0;
            else if(str=="$sp") return 29;
            else if(str=="$at") return 1;
            else if(str=="$v0") return 2;
            else if(str=="$v1") return 3;
            else if(str=="$a0") return 4;
            else if(str=="$a1") return 5;
            else if(str=="$a2") return 6;
            else if(str=="$a3") return 7;
            else if(str=="$t8") return 24;
            else if(str=="$t9") return 25;
            else if (str.substr(1,1) == "t"){
                int i = stringToInt(&str[2]);
                if (i <= 7 && i >= 0){
                    return 8 + i;
                }
                else{
                    cout << "Invalid register entry\n";
                    throw "Invalid register entry\n";
                }
            }
            else if(str=="$s8") return 30;
            else if (str.substr(1,1) == "s"){
                
                int i = stringToInt(&str[2]);
                
                if (i <= 7 && i >= 0){
                    return 16 + i;
                }
                else{
                    cout << "Invalid register entry\n";
                    throw "Invalid register entry\n";
                }
                
            }
            else if(str=="$k0") return 26;
            else if(str=="$k1") return 27;
            else if(str=="$gp") return 28;
            // else if(str=="$r0") return 0;
            else if(str=="$ra") return 31;
            else{
                // if (str.substr(1,1) != "r"){
                //     cout << "Invalid register entry\n";
                //     throw "Invalid integer entry\n";
                    
                // }
                // else{
                    int i = stringToInt(&str[1]);
                    return i;
            }
                
        } 
    }
    

int byteToInt(byte* byte) {

    int n = 0;

    n = n + (byte[0] & 0x000000ff);
    n = n + ((byte[1] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[3] & 0x000000ff) << 24);


    return n;
}

void intToByte(int n, byte* result) {

     result[0] = n & 0x000000ff;
     result[1] = (n & 0x0000ff00) >> 8;
     result[2] = (n & 0x00ff0000) >> 16;
     result[3] = (n & 0xff000000) >> 24; 
}

class registers {
    public:
        int data;

        void set(int data1){
            data = data1;
        }

        int read(){
            return data;
        }
};

class arrayOfRegister {
    public:
        registers reg[32];
        int read(int num){
            return reg[num].read();
        }

        void set(int num, int data){
            reg[num].set(data);
        }

        void printall(){
            cout << "Register\t" << "Data\n";
            // cout << "zero" << "\t\t" << hex << read(0) <<endl<< dec;
            for (int i=0; i < 32; i++){
                cout << map_int_register[i]<< "\t\t" << hex << read(i) << endl << dec;
            }
            // cout << "sp" << "\t\t" << hex << read(31) << endl << dec;
        }
};

class Memory {
    public:
        byte mem[1024][1024];
        byte rowbuffer[1024];
        int rownum = 0;
        int opr;
        int count;
        int regd;
        int addr;
        bool inprocess = false;
        bool nonblock = true;
        int delayrow;
        int delaycol;
        int rowbufferupd;

        int read(int num){
            int row = num/1024;
            int col = num%1024;
            if (row == rownum){
                return byteToInt(&rowbuffer[col]);
            }
            else{
                copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
                copy(begin(mem[row]), end(mem[row]), begin(rowbuffer));
                rownum = row;
                return byteToInt(&rowbuffer[col]);
            }
        }

        void set(int data, int num){
            int row = num/1024;
            int col = num%1024;
            if (row == rownum){
                intToByte(data, &rowbuffer[col]);
            }
            else{
                copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
                copy(begin(mem[row]), end(mem[row]), begin(rowbuffer));
                rownum = row;
                intToByte(data, &rowbuffer[col]);
            }
        }

        void printall(){
            for(int c = 0; c < N; c++){
                for (int i = (INS_COUNT[c]/256); i<1024/N; i++){
                    int j=0;
                    if(i == (INS_COUNT[c]/256)) j = (4*INS_COUNT[c])%1024;
                    for ( ; j < 1024; j=j+4){
                        int offset = (1024*c)/N;
                        int data = byteToInt(&mem[i+offset][j]);
                        if (data != 0){
                            int num = 1024*(i+offset) + j;
                            cout << num <<"-"<< num + 3<<"\t:\t" <<data<<endl;
                        }
                    }
                }
            }
            
        }

        void flush(){
            copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
        }
};

bool add(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr, string sens){
    bool err = false;
    
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) + reg->read(x3);
        reg->set(x1, x);
        
        if (DETAILS) cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction:\t" << sens<<endl;
    }
    else {
        cout<<"Incorrect register values in add instruction"<<endl;
        return true;
    }
    return err;
}

bool addi(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr, string sens){
    
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        int x = reg->read(x2) + x3;
        reg->set(x1, x);
        if (DETAILS) cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction:\t" << sens<<endl;
        return false;
    }
    else {
        cout<<"Incorrect register values in addi instruction"<<endl;
        return true;
    }

}

bool sub(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr, string sens){
    bool err = false;
    
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) - reg->read(x3);
        reg->set(x1, x);
        if (DETAILS) cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction:\t" << sens<<endl;
        
    }
    else {
        cout<<"Incorrect register values in sub instruction"<<endl;
        return true;
    }
    return err;
}

bool mul(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr, string sens){
   
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) * reg->read(x3);
        reg->set(x1, x);
        if (DETAILS) cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction:\t" << sens<<endl;
    }
    else {
        cout<<"Incorrect register values in mul instruction"<<endl;
        return true;
    }
    return err;
}

int beq(arrayOfRegister* reg, int x1, int x2, string x3, int l_addr, string sens){
    
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) == reg->read(x2)){
            if (DETAILS) cout<< "cycle " << CC << ":\t" << "jump to " << x3 << "; " << "Instruction:\t" << sens<<endl;
            return 1;
        } 
        else {
            if (DETAILS) cout<< "cycle " << CC << ":\t" << "not jumped to " << x3 << "; " << "Instruction:\t" << sens<<endl;
            return 0;
        }
    }
    else {
        cout<<"Incorrect register values in beq instruction"<<endl;
        return -1;
    }
    return err;
}

int bne(arrayOfRegister* reg, int x1, int x2, string x3, int l_addr, string sens){
    
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) != reg->read(x2)){
            if (DETAILS) cout<< "cycle " << CC << ":\t" << "jump to " << x3 << "; " << "Instruction:\t" << sens<<endl;
            return 1;
        } 
        else {
            if (DETAILS) cout<< "cycle " << CC << ":\t" << "not jumped to " << x3 << "; " << "Instruction:\t" << sens<<endl;
            return 0;
        }
    }
    else {
        cout<<"Incorrect register values in bne instruction"<<endl;
        return -1;
    }
    return err;
}

bool slt(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr, string sens){
    
    bool err = false;
    int x;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        if (reg->read(x2) < reg->read(x3)){
        reg->set(x1, 1);
        x = 1;
        }
    else{
        reg->set(x1, 0);
        x=0;
        }
    }
    else {
        cout<<"Incorrect register values in slt instruction"<<endl;
        return true;
    }
    if (DETAILS) cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x <<"; Instruction:\t" << sens<<endl;
    return err;
}

class dramdata
{
    public:
    bool lw;
    int reg;
    int add;
    int l_addr;
    int data;
    int I;
    string l_sens;

    bool dramequal(dramdata d1, dramdata d2){
        if(d1.add == d2.add && d1.data == d2.data && d1.I == d2.I && d1.l_addr == d2.l_addr && d1.lw == d2.lw && d1.reg == d2.reg && d1.l_sens == d2.l_sens){
            return true;
        }
        return false;
    }
};

class DRAM
{
    public:
        dramdata toexecute;
        int delay=0;
        int estimate;
        bool active = false;
        int pre;
        string yy;
        bool pri;
        
        void push(dramdata d, bool x){
            pre=CC+1;
            pri = x;
            toexecute = d;
            active = true;
            int loc = d.add;
            CURR= loc/1024;
            if (PREV == -1){
                delay = ROW_ACCESS_DELAY + COL_ACCESS_DELAY + CC;
                yy = "Row Buffer is initialized";
            }
            else{
                if(PREV == CURR){ 
                    delay= CC + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                }
                else{
                    if (CHANGED){
                        delay = CC+ 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                        yy = "Row Buffer is updated";
                    }
                    else{
                        delay = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                        yy = "Row Buffer is updated";
                    }
                }
                
            }
        }

        bool exe_lw(int r, int add, arrayOfRegister* reg,Memory* m, int l_addr, int core, string sens){
            if(r>0 && r<32) {
                int loc = add;
                if ((loc % 4) != 0){
                    cout << "Incorrect address: address not multiple of 4!\n";
                    return false;
                }
                else if( loc <= 4*INS_COUNT[core]-4 + ((1024*core)/N)*1024){
                    cout << "Incorrect address: cannot access Instruction Memory address\n";
                    return false;
                }
                else{  
                    CURR = loc/1024;
                    PREV=CURR;
                    reg->set(r, m->read(loc));
                    //cout<<"PREV is " << PREV << endl;
                    if (DETAILS) cout << "Core " << core  << ":\tcycle " << pre << "-" << CC << ":\t"  << map_int_register[r] << " = " <<  m->read(loc) <<"; " << yy << "; Instruction:\t" << sens<<endl;
                    return true;
                }
            }
            else {
                cout<<"Incorrect register values in lw instruction\n";
                return false;
            }
        }
    
        bool exe_sw(int data, int add, arrayOfRegister* reg,Memory* m, int l_addr, int core, string sens){
        // if(r>=0 && r<32) {
            int loc = data;
            int x2 = add;
            if ((x2 % 4) != 0){
                cout<<"Incorrect address: address not multiple of 4!\n";
                return false;
            }
            
            else if( x2 <= 4*INS_COUNT[core]-4 + ((1024*core)/N)*1024){
                //cout << x2 << endl;
                //cout<<INS_COUNT[core]<<endl;
                //cout<<core<<endl;
                cout<<"Incorrect address: cannot access Instruction Memory address\n";
                return false;
            }
            else{
                CURR = x2/1024; 
                CHANGED = true;
                ROW_BUFFER_UPDATE++;
                PREV=CURR;
                m->set(loc, x2);
                if (DETAILS) cout<< "Core " << core << ":\tcycle " << pre << "-" << CC << ":\t" << "Memory address " << x2 << "-" << x2+3 << " = " << loc << "; " << yy << "; Instruction:\t" << sens<<endl;
                return true;
            }
        }

        void execute1(Memory* m , vector<arrayOfRegister*>regs, vector<map<int,int>>* dep_reg, vector<bool>* idle, vector<bool>* wrport, vector<int>* instr_count, vector<bool>* done){
            // cout << "dram debug " << CC << endl;
            if(active){
                if (delay == CC){
                    // cout << "final cycle of dram\n";
                    active = false;
                    //call lw/sw
                    if(toexecute.lw){
                        int c = toexecute.I;
                        if(!exe_lw(toexecute.reg,toexecute.add,regs[c],m, toexecute.l_addr,c, toexecute.l_sens)){
                            //operation is not successful
                            //throw error for that core
                            (*done)[c]=true;
                            cout << "Core "<<c<<": cycle: "<<CC<<": \tlw returned error; Instruction: "<<toexecute.l_sens<<endl;
                        }
                        (*instr_count)[c]++;
                        (*dep_reg)[c].erase(toexecute.reg);
                        if (pri){
                            (*idle)[c] = false;
                        }
                        (*wrport)[toexecute.I] = false;
                    } 
                    else {
                        int c = toexecute.I;
                        //exe_sw(toexecute.data,toexecute.add,regs[c],m, toexecute.l_addr);
                        if(!exe_sw(toexecute.data,toexecute.add,regs[c],m, toexecute.l_addr,c,toexecute.l_sens)){
                            //not sucessful
                            //stop the core
                            (*done)[c]=true;
                            cout << "Core "<<c<<": "<<CC<<": \tsw returned error; Instruction: "<<toexecute.l_sens<<endl;
                        }
                        (*instr_count)[c]++;
                        if (pri){
                            (*idle)[c] = false;
                        }   
                    }

                }
                else if(delay - COL_ACCESS_DELAY == CC){
                    if (toexecute.lw){
                        (*wrport)[toexecute.I] = true;
                    }
                    ROW_BUFFER_UPDATE++;// do nothing 
                    CHANGED = false;   
                }
                else if(delay - COL_ACCESS_DELAY - ROW_ACCESS_DELAY == CC && CHANGED){
                    //ROW_BUFFER_UPDATE++;
                    CHANGED = false;
                }
                else {
                }
            }
            else{
                // do nothing
            }
        }
        
};

class MRM
{
    public:
    int SIZE = N;
    bool priority = false;
    int cycle1 = 0;
    int estimate = 2;
    dramdata toexecute;
    vector<pair<int,dramdata*>> priorityqueue;
    vector<pair<int,dramdata*>> max;
    int it;
    bool active = false;
    int y;
    
    
        bool priorityexe(dramdata* d, int row){
            if(priorityqueue.size()<2*N){
                priorityqueue.push_back(make_pair(row, d));
                return true;
            }
            return false;
        }
        void normal_execute(DRAM* dram, vector<vector<pair<int,dramdata*>>>* que, vector<map<int,int>>* dep_reg ){
            active = true;
            //cout << cycle1 << "\t" << CC << endl;
            if (cycle1 == CC){
                // push toexecute to dram
                //Dram-.toexecute = toexecute
                //DRAM->cycle = 
                //if bool = true then cycle1++ return
                //else push
                //if delete is from priorityqueue then make priority false
                if((dram)->active){
                    cycle1++;
                    return;
                }
                if (priority){
                    if (DETAILS) cout << "Core "<<toexecute.I<<": "<<"cycle "<< CC<<": \tMRM gives priority instruction to DRAM; Instruction: "<<toexecute.l_sens<<endl;
                    (*dram).push(toexecute,true);
                    priorityqueue.erase(priorityqueue.begin()+it);
                    priority = false;
                }
                else{
                    if (DETAILS) cout << "Core "<<toexecute.I<<": "<<"cycle "<<CC<<": \tMRM gives instruction to DRAM; Instruction: "<<toexecute.l_sens<<endl;
                    //cout << CC << " MRM pushed" << endl;
                    (*dram).push(toexecute,false);
                    ((* que)[y]).erase((*que)[y].begin() + it);
                    //(* que)[y] = {};

                }
                
            }
            if(CC < cycle1){
                // do nothing
                if (priority){
                    //do nothing
                }
                else{
                    if (priorityqueue.size()==0){
                        // do nothing
                    }
                    else{
                        bool samerow = false;
                        bool change = false;
                        int rr = it;
                        
                        int core = CURR/(1024/N);
                        it = 0;
                        for( auto r : priorityqueue){
                            if(r.first == CURR){
                                cycle1 = CC+estimate;
                                toexecute = *r.second;
                                samerow = true;
                                priority = true;
                                change = true;
                                break;
                            }
                            it++;
                        }
                        if(!samerow && !(toexecute.add/1024 == CURR)){
                            it =0;
                            toexecute = *priorityqueue[0].second;
                            cycle1 = CC+1;
                            priority = true;
                            change = true;
                        }
                        
                        if(!change) it = rr;
                        
                        
                        // it =0;
                        // toexecute = *priorityqueue[0].second;
                        // cycle1 = CC+1;
                        // priority = true;
                    }
                }
            }
            else{
                if (priority){
                    //do nothing
                }
                else{
                    if(dram->active){
                        if (dram->delay - estimate <= CC){
                            if (priorityqueue.size()==0){

                            bool samerow = false;
                            if (CURR != -1){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : (* que)[core]){
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        y = core;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            
                            if(!samerow || CURR == -1){
                                int size = 0;
                                y = -1;
                                for(int i=0; i < N; i++){
                                    if((* que)[i].size() > size){
                                        size = (* que)[i].size();
                                        y = i;
                                    }
                                }
                                if (y!=-1){
                                    cycle1 = CC+estimate;
                                    max = (*que)[y];
                                    toexecute = *max[0].second;
                                    it = 0;
                                }
                                else{
                                    active = false;
                                }
                            }
                        }
                        else{
                            //cout << "ll" << endl;
                            bool samerow = false;
                            if (CURR != -1){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : priorityqueue){
                                    //cout << r.first << " " << ((*r.second).l_sens) << CURR << endl;
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        priority = true;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            if (CURR != -1 && !samerow){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : (* que)[core]){
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        y = core;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            if(!samerow){
                                it = 0;
                                toexecute = *priorityqueue[0].second;
                                cycle1 = CC+estimate;
                                priority = true;
                            }
                        }
                        }
                        else{
                            //do nothing
                        }
                    }
                    else{
                        if (priorityqueue.size()==0){

                            bool samerow = false;
                            if (CURR != -1){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : (* que)[core]){
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        y = core;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            
                            if(!samerow || CURR ==-1){
                                int size = 0;
                                y = -1;
                                for(int i=0; i < N; i++){
                                    if((* que)[i].size() > size){
                                        size = (* que)[i].size();
                                        y = i;
                                    }
                                }
                                if (y!=-1){
                                    cycle1 = CC+estimate;
                                    max = (*que)[y];
                                    toexecute = *max[0].second;
                                    it = 0;
                                }
                                else{
                                    active = false;
                                }
                            }
                        }
                        else{

                            bool samerow = false;
                            if (CURR != -1){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : priorityqueue){
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        priority = true;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            if (CURR != -1 && !samerow){
                                int core = CURR/(1024/N);
                            
                                //it = (* que)[core].begin();
                                it = 0;
                                for( auto r : (* que)[core]){
                                    if(r.first == CURR){
                                        cycle1 = CC+estimate;
                                        toexecute = *r.second;
                                        samerow = true;
                                        y = core;
                                        break;
                                    }
                                    it++;
                                }
                            }
                            if(!samerow){
                                it = 0;
                                toexecute = *priorityqueue[0].second;
                                cycle1 = CC+estimate;
                                priority = true;
                            }
                        }
                    }
                    
                }
                
                
            }
            
            
            
        }
};



void callpriority(MRM* mrm, vector<pair<int,dramdata*>>* que1, map<int,int>* dep_reg1, int I, int r, vector<bool>* idle){
    int x = (*dep_reg1)[r];
    for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
        if((* it).first == x && (* it).second -> reg == r && (* it).second->lw){
            (*idle)[I] = true;
            if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                (* que1).erase(it);
                (mrm)->priorityexe((* it).second,(* it).first);
                mrm->priority = true;
                mrm->it = mrm->priorityqueue.size() - 1;
                break;
            }
            else{
            (mrm)->priorityexe((* it).second,(* it).first);
            (* que1).erase(it);
            break;
            }
        }
    }
    

}

void fullcall(MRM* mrm, vector<pair<int,dramdata*>>* que1, int I, vector<bool>* idle){
    (*idle)[I] = true;
    if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((*que1)[0].second))){
        mrm->priorityexe((*que1)[0].second,(*que1)[0].first);
        (* que1).erase((*que1).begin());
        mrm->priority = true;
        mrm->it = mrm->priorityqueue.size()-1;
        return;
    }
    mrm->priorityexe((*que1)[0].second,(*que1)[0].first);
    (* que1).erase((*que1).begin());
    
}

int execute_mu(MRM* mrm, arrayOfRegister* regs, Memory* m, map<int,ins*> instructions, vector<pair<int,dramdata*>>* que1, map<int,int>* dep_reg1, int I, int i, int* instr_count, vector<bool>* idle, vector<bool>* wrport, vector<bool>* done){
    //cout <<"before idle return " << CC << endl;
    if ((*idle)[I]){
        return i;
    }
    vector<int> pcs;

    int pc;
    vector<pair<int,dramdata*>> que = *que1;
    map<int,int>dep_reg = *dep_reg1;

    for(auto x : instructions){
        pcs.push_back(x.first);
    }
    
        
    string opr = instructions.find(pcs[i])->second->op;
    vector<string> args = instructions.find(pcs[i])->second->params;
    int l_addr = instructions.find(pcs[i])->second->l_addr;
    string l_sens = instructions.find(pcs[i])->second->sen;

    string x1,x2,x3;
    pc = pcs[i] + 4;

    bool mem_ins = false;
    //cout << opr << CC << endl;
    if(opr=="lw"){

        mem_ins = true;
        
        auto it = args.begin();
        x1 = *it; it++;
        x2 = *it; it++;
        x3 = *it;

        string x4;
        if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
            //cout << "Invalid memory location\n";
            cout << "Core " << I  << ": ";
            cout<<"invalid memory location; Instruction: "<<l_sens<<endl;
            (*done)[I]=true;
            return -1;
        }
        else{
            x4 = trim(x3.substr(1,x3.size()-2));
        }

        int a = modifyReg(x1);
        int b = stringToInt(x2);
        int c = modifyReg(x4);
        if(c<0 || c>31) {
            cout << "Core " << I  << ": ";
            cout<<"Incorrect register values in lw instruction; Instruction: "<<l_sens<<endl;
            (*done)[I]=true;
            return -1;
        }
        
        
        //cout << "DRAM request issued for lw" << endl;
        bool analyze_count = false;
        if (dep_reg.count(c)){
            
            callpriority(mrm, que1, dep_reg1, I, c, idle);
            //call mrm to execute the previous lw/sw
            return i;
        }
        if (DETAILS){cout << "Core " << I  << ": ";
        cout << "cycle " << CC << ":\t" << "DRAM request issued for lw instruction: "<< l_sens << endl;}
        if(dep_reg.count(a)){
            
            int x = dep_reg[a];
            //terminate the instruction corresponding to x
            for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
                if((* it).first == x && (* it).second -> reg == a && (* it).second->lw){
                    if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                        mrm->cycle1 = CC - 1;
                    }
                    (* que1).erase(it);
                    (* dep_reg1).erase(a);
                    if (DETAILS) cout<< "\t\t\t" << "Reduntant lw instructions in queue terminated" << endl;
                    (*instr_count)++;
                    break;
                }
            }
        }
        int addr_no = (regs->read(c) + b)/1024;
        if( addr_no>= 1024/N || addr_no <0){
            cout << "Memory address out of range\n";
            (*done)[I]=true;
            return -1;
        }
        int addr = (regs->read(c)) + b+ ((1024*I)/N)*1024;
        //cout << addr<<endl;
        for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
            if((* it).second -> add == addr && !((* it).second->lw)){
                int r = a;
                if(r>0 && r<32) {
                    int loc = addr;
                    if ((loc % 4) != 0){
                        cout << "Incorrect address: address not multiple of 4!\n";
                        return -1;
                    }
                    else if( loc <= 4*INS_COUNT[I]-4 + ((1024*I)/N)*1024 ){
                        cout << "Incorrect address: cannot access Instruction Memory address\n";
                        return -1;
                    }
                    else{  
                        regs->set(r, (* it).second->data);
                        (*instr_count)++;
                        //cout<<"PREV is " << PREV << endl;
                        if (DETAILS) cout << "\t\t\t"  << map_int_register[r] << " = " <<  (* it).second->data <<"; " << "Data forwarded from sw instruction in queue" << "; Instruction:\t" << l_sens<<endl;
                        return i+1;
                    }
                }
                else {
                    cout<<"Incorrect register values in lw instruction\n";
                    return -1;
                }
                break;
            }
        }
        if (DETAILS) cout<< "\t\t\tDRAM request added to queue" << endl;
        (* dep_reg1)[a] = addr/1024;
        dramdata* d = new dramdata();
        d->lw=true;
        d->I=I;
        d->reg = a;
        d->add = addr;
        d->l_addr = l_addr;
        d->l_sens = l_sens;
        if((*que1).size()<QSIZE){
            (* que1).push_back(make_pair(addr/1024,d));
        }
        else{
            fullcall(mrm, que1, I, idle);
            (* que1).push_back(make_pair(addr/1024,d));
        }
    }
          

    else if(opr=="sw"){
        
        mem_ins = true;

        auto it = args.begin();
        x1 = *it; it++;
        x2 = *it; it++;
        x3 = *it;

        string x4;
        if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
            //cout << "Invalid memory location\n";
            cout << "Core " << I  << ": ";
            cout <<"invalid memory location\n";
            (*done)[I]=true;
            return -1;
        }
        else{
            x4 = trim(x3.substr(1,x3.size()-2));
        }

        int a = modifyReg(x1);
        int b = stringToInt(x2);
        int c = modifyReg(x4);
        if(c<0 || c>31) {
            cout << "Core " << I  << ": ";
            cout<<"Incorrect register values in sw instruction\n";
            (*done)[I]=true;
            return -1;            
        }
        
        
        //cout << "DRAM request issued for lw" << endl;
        bool analyze_count = false;
        if (dep_reg.count(c)||dep_reg.count(a)){
            if(dep_reg.count(c)){
                callpriority(mrm, que1, dep_reg1, I, c, idle);
            }
            if(dep_reg.count(a)){
                callpriority(mrm, que1, dep_reg1, I, a, idle);
            }
            //call mrm to execute the previous lw/sw (c)
            return i;
        }
        if (DETAILS){cout << "Core " << I  << ": ";
        cout << "cycle " << CC << ":\t" << "DRAM request issued for sw instruction: "<< l_sens << endl;}
        int addr_no = (regs->read(c) + b)/1024;
        if( addr_no>= 1024/N || addr_no <0){
            cout << "Memory address out of range\n";
            (*done)[I]=true;
            return -1;
        }
    
        
        int addr = (regs->read(c)) + b + ((1024*I)/N)*1024;

        for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
            if((* it).second -> add == addr && !((* it).second->lw)){
                if (DETAILS) cout<< "\t\t\t" << "Reduntant sw instructions in queue terminated" << endl;
                if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                    mrm->cycle1 = CC - 1;
                }
                (* que1).erase(it);
                (*instr_count)++;
                break;
            }
        }
        
    
        if (DETAILS) cout<< "\t\t\tDRAM request added to queue" << endl;

        dramdata* d = new dramdata();
        d->reg = a;
        if(a>0 && a<32) {
             d->data = regs->read(a);
        }
        else{
            cout << "Incorrect register values in sw instruction\n";
            (*done)[I]=true;
            return -1;
        }
                
        d->add = addr;
        d->I=I;
        d->lw = false;
        d->l_addr = l_addr;
        d->l_sens = l_sens;
        if((*que1).size()<QSIZE){
            (* que1).push_back(make_pair(addr/1024,d));
        }
        else{
            fullcall(mrm, que1, I, idle);
            (* que1).push_back(make_pair(addr/1024,d));
        }

    }   

    else if(opr == "add"){
        if ((*wrport)[I]){
        return i;
        }
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<<"Incorrect arguments in add instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            if (dep_reg.count(c)||dep_reg.count(b)){
                if(dep_reg.count(c)){
                    //cout << CC << " callp c"<<endl;
                    callpriority(mrm, que1, dep_reg1, I, c, idle);
                    
                }
                if(dep_reg.count(b)){
                    //cout << CC << " callp b"<<endl;
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                    
                }
                //cout << "calling return i" << CC << endl;
                return i;
                
            //call mrm to execute the previous lw/sw (c)
                
            }
            if (DETAILS) cout << "Core " << I  << ": ";
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                
                
                int ff = 0;
                //cout << "xbb\n" ;
                for(auto t : que){
                    
                    if(t.first == x && t.second -> reg == a && t.second->lw){
                        if (DETAILS) cout<< "cycle " << CC << ":\t" << "Reduntant lw instruction in queue terminated" << endl;
                        (* que1).erase((*que1).begin()+ff);
                        if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((t).second)) ){
                            mrm->cycle1 = CC - 1;
                        }
                        (* dep_reg1).erase(a);
                        (*instr_count)++;
                        break;

                    }

                    ff++;
                }
                
                //return i;
            }
            
            if(add(regs,a,b,c,l_addr, l_sens)) return -1;
        }
    }

    else if(opr=="sub"){
        if ((*wrport)[I]){
        return i;
        }
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<< "Incorrect arguments in sub instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                if(dep_reg.count(c)){
                    callpriority(mrm, que1, dep_reg1, I, c, idle);
                }
                if(dep_reg.count(b)){
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                }
            //call mrm to execute the previous lw/sw (c)
                return i;
            }
        
            if (DETAILS) cout << "Core " << I  << ": ";
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                
                //terminate the instruction corresponding to x
                for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
                    if((* it).first == x && (* it).second -> reg == a && (* it).second->lw){
                        if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                            mrm->cycle1 = CC - 1;
                        }
                        (* que1).erase(it);
                        (*dep_reg1).erase(a);
                        if (DETAILS) cout<< "\t\t" << "Reduntant lw instructions in queue terminated" << endl;
                        (*instr_count)++;
                        break;
                    }
                }
            }
            
            if(sub(regs,a,b,c, l_addr, l_sens)) return -1;
            

        }
    }
    else if(opr == "mul"){
        if ((*wrport)[I]){
        return i;
        }
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<< "Incorrect arguments in mul instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            if (dep_reg.count(c)||dep_reg.count(b)){
                if(dep_reg.count(c)){
                    callpriority(mrm, que1, dep_reg1, I, c, idle);
                }
                if(dep_reg.count(b)){
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                }
            //call mrm to execute the previous lw/sw (c)
                return i;
            }
            if (DETAILS) cout << "Core " << I  << ": ";
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                
                
                for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
                    if((* it).first == x && (* it).second -> reg == a && (* it).second->lw){
                        if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                            mrm->cycle1 = CC - 1;
                        }
                        (* que1).erase(it);
                        (* dep_reg1).erase(a);
                        if (DETAILS) cout<< "\t\t" << "Reduntant lw instructions in queue terminated" << endl;
                        (*instr_count)++;
                        break;
                    }
                }
            }
            
            if(mul(regs,a,b,c,l_addr, l_sens)) return -1;
        }
    }
    else if(opr == "addi"){
        //cout << "in addi" << CC << endl;
        if ((*wrport)[I]){
        return i;
        }
        string x1,x2,x3;
        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout << "Incorrect arguments in addi instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = stringToInt(x3);

            if (dep_reg.count(b)){
                callpriority(mrm, que1, dep_reg1, I, b, idle);
                //cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
                //cout << "b return"<< CC << endl;
                return i;
            }
            if (DETAILS) cout << "Core " << I  << ": ";
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                
                
                for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
                    if((* it).first == x && (* it).second -> reg == a && (* it).second->lw){
                        if (DETAILS) cout<< "\t\t" << "Reduntant lw instructions in queue terminated" << endl;
                        (* que1).erase(it);
                        (* dep_reg1).erase(a);
                        if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                            mrm->cycle1 = CC - 1;
                        }
                        (*instr_count)++;
                        break;
                    }
                }
            }
            
            if(addi(regs,a,b,c,l_addr,l_sens)){ 
                //cout << "return -1" << CC << endl;
                return -1;}
        }
    }
    else if(opr=="bne"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<< "Incorrect arguments in bne instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 



            int a = modifyReg(x1);
            int b = modifyReg(x2);

            bool analyze_count;
            if (dep_reg.count(a)||dep_reg.count(b)){
                if(dep_reg.count(a)){
                    callpriority(mrm, que1, dep_reg1, I, a, idle);
                }
                if(dep_reg.count(b)){
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                }
            //call mrm to execute the previous lw/sw (c)
                return i;
            }

            if (DETAILS) cout << "Core " << I  << ": ";

            int k = bne(regs,a,b, x3, l_addr, l_sens);

            // if(k) return 0;
            if(k==0) pc = pc;
            else if(k==-1){
                (*done)[I]=true;
                return -1;
            } 
            else{
                if (labels[I].count(x3)!=0){
                    (*instr_count)++;
                    return labels[I][x3]/4;
                }
                else{
                    cout << "label doesn't exist\n";
                    (*done)[I]=true;
                    return -1;
                }
            }

        }
    }
    else if(opr=="beq"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<< "Incorrect arguments in bne instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 



            int a = modifyReg(x1);
            int b = modifyReg(x2);

            bool analyze_count;
            if (dep_reg.count(a)||dep_reg.count(b)){
                if(dep_reg.count(a)){
                    callpriority(mrm, que1, dep_reg1, I, a, idle);
                }
                if(dep_reg.count(b)){
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                }
            //call mrm to execute the previous lw/sw (c)
                return i;
            }

            if (DETAILS) cout << "Core " << I  << ": ";

            bool k = beq(regs,a,b, x3, l_addr, l_sens);

            // if(k) return 0;
            if(k==0) pc = pc;
            else if(k==-1){
                (*done)[I]=true;
                return -1;
            }
            else{
                if (labels[I].count(x3)!=0){
                    (*instr_count)++;
                    return labels[I][x3]/4;
                }
                else{
                    cout<<"label doesn't exist\n";
                    (*done)[I]=true;
                    return -1;
                }
            }

        }
    }
    else if(opr=="slt"){
        if ((*wrport)[I]){
        return i;
        }
        string x1,x2,x3;

        if(args.size()!= 3){
            cout << "Core " << I  << ": ";
            cout<< "Incorrect arguments in slt instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;



            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                if(dep_reg.count(c)){
                    callpriority(mrm, que1, dep_reg1, I, c, idle);
                }
                if(dep_reg.count(b)){
                    callpriority(mrm, que1, dep_reg1, I, b, idle);
                }
            //call mrm to execute the previous lw/sw (c)
                return i;
            }
            
            if (DETAILS) cout << "Core " << I  << ": ";
            
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                
                //terminate the instruction corresponding to x
                for(auto it = (*que1).begin() ; it < (*que1).end() ; it++){
                    if((* it).first == x && (* it).second -> reg == a && (* it).second->lw){
                        if (DETAILS) cout<< "\t\t" << "Reduntant lw instructions in queue terminated" << endl;
                        (*que1).erase(it);
                        (*dep_reg1).erase(a);
                        if(mrm->cycle1 >= CC && mrm->toexecute.dramequal(mrm->toexecute, *((* it).second)) ){
                            mrm->cycle1 = CC - 1;
                        }
                        (*instr_count)++;
                        break;
                    }
                }
            }

            if(slt(regs,a,b,c, l_addr, l_sens)) return -1;
        }
    }

    else if(opr=="j"){
        string x;
        if (DETAILS) cout << "Core " << I  << ": ";
        if(args.size()!= 1){
            
            cout<<"Incorrect arguments in jump instruction\n";
            (*done)[I]=true;
            return -1;
        }
       
        else{
            auto it = args.begin();
            x = *it; 
            if (labels[I].count(x)!=0){
                (*instr_count)++;
                if (DETAILS) cout<< "cycle " << CC << ":\t" << "jump to " << x << "; " << "Instruction:\t" << l_sens<<endl;
                return labels[I][x]/4;
                
            }
            else{
                cout<<"label doesn't exist\n";
                (*done)[I]=true;
                return -1;
            }
            
        }
        
        }
        
    else{
        cout << "Core " << I  << ": ";
        cout<<"Wrong Instruction\n";
        (*done)[I]=true;
        return -1;
    }
    if(!mem_ins) (*instr_count)++;
    return i+1;

        

}

void multi_execute(Memory* m , vector<map<int,ins*>> instructions , vector<arrayOfRegister*>regs ){

    vector<vector<pair<int,dramdata*>>> que(N);
    vector<map<int,int>> dep_reg(N);
    vector<int> pc(N,0);
    vector<int> instr_count(N,0);
    vector<bool> done(N,false);
    vector<bool> idle(N,false);
    vector<bool> wrport(N,false);
    MRM* mrm = new MRM();
    DRAM* dram = new DRAM();

    for(int i = 0; i < M  ; i++){
        CC++;
        // cout << "debug"<<endl;
        for(int j = 0; j < N ; j++){
            //call function
            // for(auto c : que[j]){
            //     cout << c.second->reg << " " << c.second->l_addr << endl;
            // }
            // cout << "___\n\n" << endl;
            // if (idle[0]){
            //     cout << "processor is idle " << CC<<endl;
            // }
            // if (wrport[0]){
            //     cout << "writeport active" << CC << endl;
            // } 
            if( pc[j] < instructions[j].size() && pc[j] > -1 && !done[j] ) {
                if(!idle[j]) ;//cout<< "Core " << j<<":\t";
                int l = execute_mu(mrm, regs[j], m, instructions[j], &que[j], &dep_reg[j], j, pc[j], &instr_count[j], &idle, &wrport, &done);
                pc[j] = l;
                if (pc[j] >= instructions[j].size() || pc[j] <= -1){
                    done[j] = true;
                }
                //cout << l << " " << j << endl; 

            }
            else{
                done[j] = true;
            }
        }
        dram->execute1(m,regs,&dep_reg,&idle,&wrport, &instr_count, &done);
        mrm->normal_execute(dram,&que,&dep_reg);
        

        int r = 0;
        for(auto x : done) {
            //cout << x << "done" << endl;
            if (x==false)  r++;
        }
        if(r>0) continue;
        // cout<<dram->active<<" "<<mrm->active<<endl;
        if(dram->active) continue;
        if(mrm->active) continue;
        //cout << "break" << endl;
        break;

    }
    m->flush();
    cout<<"---------------------------------------------------------------\n";
    for(int j=0; j < N; j++){
        cout<<"Final Register File content of Core " << j << " after execution:\n";
        regs[j]->printall();
        cout<<"---------------------------------------------------------------\n";
        
    }
    cout<<"Final Memory content after execution:\n";
    m->printall();
    cout<<"---------------------------------------------------------------\n";
    cout<<"Relevant Statistics:"<<endl;
    cout<<"No of clock cycles: "<< CC<<endl;
    int total = 0;
    for(int j=0;j<N;j++){
        cout<<"No. of instructions executed of Core "<< j<<":\t" << instr_count[j] << endl;
        total += instr_count[j];
    }
    cout<<"Total no of instructions executed: " << total << endl;
    cout<<"No of times Row Buffer is updated: " << ROW_BUFFER_UPDATE <<endl;

}


inline bool file_notexist (const std::string& name) {  // a function used to check a file with a given name exists in the working directory or not
  if (FILE *file = fopen(name.c_str(), "r")) {        // returns true if file doesn't exist
      fclose(file);                                   // returns false if a file with the given name exists
      return false;
    } else {
        return true;
    }   
}



int main(int argc, char const *argv[]){

    if (argc!=6){       // unexpected no. of parameters
        cout << "Please run the program using this command format \"./out NO_OF_CORES NO_OF_CLOCK_CYCLES ROW_ACCESS_DELAY COL_ACCESS_DELAY DISPLAY_EXECUTION\"\n";
    return 1;
    }

    // if (file_notexist(argv[1])){   // checks if file doesn't exist, prints error message if true
    //     cout << "Please put the input file in the directory"<<endl;
    // return 1;
    // }
    
    
    if (stringToInt(argv[5])==1){
        DETAILS = true;
    }
    else if (stringToInt(argv[5])==0){
        DETAILS = false;
    }
    else{
        cout << "Wrong parameter for output"<<endl;
        return 1;
    }
    ROW_ACCESS_DELAY = stringToInt(argv[3]);
    COL_ACCESS_DELAY = stringToInt(argv[4]);
    N = stringToInt(argv[1]);
    M = stringToInt(argv[2]);
    vector<int> temp(N,0);
    INS_COUNT = temp;
    Memory* m = new Memory();
    vector<map<int,ins*>> instructions(N);
    vector<arrayOfRegister*>regs(N);
    vector<map<string,int>> temp5(N);
    labels = temp5;
    
    for(int i = 0; i < N ;i++){
        regs[i] = new arrayOfRegister();
        regs[i]->set(29,1048576*(i+1)/N);
        int COUNT = 1;
        string text;
        string inp1;
        ifstream infile;
        cout << "Enter the text file for core " << i << endl;
        cin >> inp1;
        if (file_notexist(inp1)){   // checks if file doesn't exist, prints error message if true
            cout << "Please put the input file in the directory"<<endl;
            return 1;
        }
        infile.open(inp1);
    
    

        while (getline (infile, text)) {
            ins* in = new ins();
            
            text = trim(text);
            in->sen = text;
            string temp3 = text;
            if ((temp3.find("#")) != string::npos){
                stringstream s1(temp3);
                string temp4;
                getline(s1, temp4, '#');
                text = trim(temp4);
            }
            string word; 
            stringstream iss(text); 
            iss >> word;

            if(word==""){
                continue;
            }
            // cout << &word[word.size()-1] << endl;
            word = trim(word);
            if (word.substr(word.size()-1)==":"){
                labels[i][word.substr(0,word.size()-1)] = COUNT - 1;
                // cout << "here" << endl;
                // cout << word << endl;
                if(word==text){
                    continue;
                }
                iss >> word;
            }
            in->op = word;
            in->l_addr = COUNT - 1;
            string w = iss.str();
            string substr;
            while (getline(iss, substr, ',')){
                substr = trim(substr);
                stringstream ss(substr);
                // cout << substr.find("(") << endl;
                if ((substr.find("(")) != string::npos){
                    string temp;
                    getline(ss, temp, '(');
                    int l = temp.size();
                    temp = trim(temp);
                    // cout << temp << "test" << endl;
                    if (temp==""){
                        temp = "0";
                    }
                    
                    in->params.push_back(temp);
                    string temp1 = ss.str().substr(l);
                    if (temp1.substr(temp1.size()-1) != ")"){
                        cout << "non-closed bracket\n";
                        throw runtime_error("non-closed bracket\n");
                    }
                    else{
                        in->params.push_back(temp1);
                    }
                    
                    
                }
                else{
                    in->params.push_back(substr);
                }
                
                
                
            }

        instructions[i].insert(make_pair(COUNT,in));  
        m->set(COUNT,(COUNT-1)+((1024*i/N)*1024));  
        COUNT = COUNT+4;

        if(COUNT>1048572){
            cout<<"Out of Memory!"<<endl;
            break;
        }
        INS_COUNT[i] = COUNT/4;
        
    }
    infile.close();
    

    

    m->flush();
    }
    multi_execute(m, instructions, regs);

    //execute(regs,m,instructions);
    // call function

    return 0;
}