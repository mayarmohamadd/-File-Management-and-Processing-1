#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include<bits/stdc++.h>
using namespace std;

fstream file_input_Employee;
fstream PrimaryIndex_Employee;
fstream SecondaryIndex_Employee;


void sort(map<string, int>& M)
{
    multimap<int, string> MM;

    // Insert every (key-value) pairs from
    // map M to multimap MM as (value-key)
    // pairs
    for (auto& it : M) {
        MM.insert({ it.second, it.first });
    }

    // Print the multimap
    for (auto& it : MM) {
        cout << it.second << ' '
             << it.first << endl;
    }
}
struct PEmp{
    string emp_id;
    int RRN_offset;
};
struct SEmp{
    vector<string> pri_key;
    string sec_key;
};

class Employee{
public:
    string Employee_ID;
    string Dept_ID;
    string Employee_Name;
    string Employee_position;
public:
    void writeSECIndex(SEmp T) {
        SecondaryIndex_Employee.close();
        SecondaryIndex_Employee.open("SecondaryIndex_Employee.txt", ios::out | ios::app);
        SecondaryIndex_Employee<< T.sec_key <<'|';
        for (string S:T.pri_key) {
            SecondaryIndex_Employee<< S << '|';
        }
        SecondaryIndex_Employee << '#';
        SecondaryIndex_Employee.close();
    }
    vector<SEmp> ReadSECIndex(){
        SecondaryIndex_Employee.open("SecondaryIndex_Employee.txt",ios::in);
        vector<SEmp> list;

        while (!SecondaryIndex_Employee.eof()){
            SEmp obj;
            getline(SecondaryIndex_Employee,obj.sec_key,'|');
            if(obj.sec_key=="") break;
            while (true){
                string id;
                getline(SecondaryIndex_Employee,id,'|');
                obj.pri_key.push_back(id);
                char c;
                SecondaryIndex_Employee.seekp(0,ios::cur);
                SecondaryIndex_Employee>>c;
                if(c=='#')break;
                else SecondaryIndex_Employee.seekp(-1,ios::cur);
            }
            list.push_back(obj);
        }
        SecondaryIndex_Employee.close();
        return list;
    }
    //check if sec_key exist or not
    SEmp checkInSecIndex(vector<SEmp>& l,string& s,string& p){
        int f=0;
        SEmp KK;
        for(SEmp& k:l){
            if(k.sec_key==s){
                f=1;
                k.pri_key.push_back(p);
                KK=k;
            }
        }
        if(f==1)return KK;
        else {
            KK.sec_key=s;
            KK.pri_key.push_back(p);
            l.push_back(KK);
            return KK;
        }
    }
    void WritePrimaryIndex(map<string,int>& PIE){
        PrimaryIndex_Employee.close();
        PrimaryIndex_Employee.open("PrimaryIndex_Employee.txt",ios::out|ios::trunc);
        PrimaryIndex_Employee.seekp(0,ios::beg);
        for(auto i=PIE.begin();i!=PIE.end();i++){
            string idd=i->first;
            int o=i->second;
            PrimaryIndex_Employee<<idd<<' ';
            PrimaryIndex_Employee<<o<<' ';
        }
        PrimaryIndex_Employee.close();
    }
    map<string,int> ReadPrimaryIndex(PEmp PE,int ch){
        PEmp PEfun;
        map<string,int> PIE;
        int flage_PE=0;
        string off,id;
        PrimaryIndex_Employee.close();
        PrimaryIndex_Employee.open("PrimaryIndex_Employee.txt",ios::in|ios::app);
        PrimaryIndex_Employee.seekg(0,ios::end);
        int i=PrimaryIndex_Employee.tellg();
        PrimaryIndex_Employee.seekg(0,ios::beg);
        while(!PrimaryIndex_Employee.eof()&&i!=0){
            flage_PE=1;
            string c;
            getline(PrimaryIndex_Employee,id,' ');
            getline(PrimaryIndex_Employee,off,' ');
            if(id==""||off=="") break;
            else{
                PEfun.emp_id=id;
                PEfun.RRN_offset= stoi(off);
                PIE.insert({PEfun.emp_id,PEfun.RRN_offset});
            }

        }
        if(ch==0){
        if(flage_PE==0){
            //PE.emp_id=id;
            PE.RRN_offset=0;
            PIE.insert({PE.emp_id,PE.RRN_offset});
        }
        else {
            //PE.emp_id = id;
            file_input_Employee.seekg(0, ios::end);
            PE.RRN_offset = file_input_Employee.tellg();
            PIE.insert({PE.emp_id,PE.RRN_offset});
        }
        }

        return PIE;
    }
    vector<SEmp> ReadSecondayIndex(string dept,string eid){
        SEmp SE;//struct_PIndex
        int flage_SE=0;
        vector<SEmp>List;
        SecondaryIndex_Employee.open("SecondaryIndex_Employee.txt",ios::in|ios::app);
        SecondaryIndex_Employee.seekg(0,ios::end);
        int j=SecondaryIndex_Employee.tellg();
        SecondaryIndex_Employee.seekg(0,ios::beg);
        if(!SecondaryIndex_Employee.eof()&&j!=0){
            flage_SE=1;
            SecondaryIndex_Employee.close();
            List=ReadSECIndex();
            SEmp T=checkInSecIndex(List,dept,eid);
        }
        if(flage_SE==0){
            SE.sec_key=dept;
            SE.pri_key.push_back(eid);
            writeSECIndex(SE);
        }
        if(flage_SE==1) {
            SecondaryIndex_Employee.close();
            SecondaryIndex_Employee.open("SecondaryIndex_Employee.txt",ios::out|ios::trunc);
            for (SEmp T:List) {
                writeSECIndex(T);
            }
        }
        return List;
    }
    void add_Employee(){
        map<string,int> PIE;
        vector<SEmp>List;
        PEmp PE;
        Employee Emp;
        int count_bits=4;
        cout<<"Enter Employee id"<<endl;string id;cin>>id;count_bits+= id.length();
        cout<<"Enter Employee name"<<endl;string name;cin>>name;count_bits+= name.length();
        cout<<"Enter Employee position"<<endl;string position;cin>>position;count_bits+= position.length();
        cout<<"Enter Dept Id"<<endl;string Dept_Id;cin>>Dept_Id;count_bits+= Dept_Id.length();
        PE.emp_id=id;
        PIE=ReadPrimaryIndex(PE,0);
        List=ReadSecondayIndex(Dept_Id,id);
        file_input_Employee.close();
        file_input_Employee.open("file_input_Employee.txt", ios::out | ios::app);
        file_input_Employee<<count_bits<<'|';
        file_input_Employee<<id<<'|';
        file_input_Employee<<name<<'|';
        file_input_Employee<<position<<'|';
        file_input_Employee<<Dept_Id<<'#';
        sort(PIE);
        WritePrimaryIndex(PIE);
    }

   void delete_Employee(string id_deleted){
       vector<SEmp> List;
        PEmp PE;
        string offsetdel;
       //delete from primary index
        map<string,int>mp;
        mp=ReadPrimaryIndex(PE,1);
        int offset=mp[id_deleted];
        mp.erase(id_deleted);
        WritePrimaryIndex(mp);
         sort(mp);
        //delete from file
        file_input_Employee.close();
        file_input_Employee.open("file_input_Employee.txt",ios::out|ios::app);
        file_input_Employee.seekp(offset,ios::beg);
        file_input_Employee.write("*",1);
        file_input_Employee.close();
    }

    Employee SearchPrimaryIndex(string S){
        map<string,int> mp;
        PEmp Pe;
        Pe.emp_id=S;
        mp= ReadPrimaryIndex(Pe,1);
        int offset=mp[S];
        string offsetdel;
        file_input_Employee.close();
        file_input_Employee.open("file_input_Employee.txt",ios::in|ios::app);
        file_input_Employee.seekg(offset,ios::beg);
        getline(file_input_Employee,offsetdel,'|');
        int len=offsetdel.length();
        file_input_Employee.close();
        file_input_Employee.open("file_input_Employee.txt",ios::in|ios::app);
        file_input_Employee.seekg(offset+len+1,ios::beg);
        Employee Emp;
        string s;
        int lengthind=stoi(offsetdel);

        getline(file_input_Employee,s,'|');
        Emp.Employee_ID=s;
        lengthind-=s.length();
        getline(file_input_Employee,s,'|');
        Emp.Employee_Name=s;
        lengthind-=s.length();
        getline(file_input_Employee,s,'|');
        Emp.Employee_position=s;
        lengthind-=s.length();
        lengthind-=4;
        s="";
        char f;
        while (lengthind!=0){
            file_input_Employee>>f;
            s+=f;
            lengthind--;
        }
        Emp.Dept_ID=s;
        return Emp;
    }
    vector<string> SearchSecIndex(string dept_id){
        vector<SEmp> List;
        vector<string> result;
        result.push_back("empty");
        List=ReadSECIndex();
        for(SEmp s:List){
            if(s.sec_key==dept_id){
                result.clear();
                result=s.pri_key;
                break;
            }
        }
        return result;
    }

};





struct PDept{
    string DeptID;
    int offset;
};
struct SDept{
    string name;
    vector<string>pKey;
};

fstream PrimaryIndex_Department;
fstream secondryIndex_Department;
fstream file_input_Department;

class Department{
public:
    string Dept_Id;
    string Dept_Name;
    string Dept_Manager;

    void writePrimaryIndex(map<string,int>& PDept){
        PrimaryIndex_Department.close();
        PrimaryIndex_Department.open("PrimaryIndex_Department.txt",ios::out|ios::trunc);
        PrimaryIndex_Department.seekp(0,ios::beg);
        for(auto i=PDept.begin();i!=PDept.end();i++){
            string id=i->first;
            int off=i->second;
            PrimaryIndex_Department<<id<<' ';
            PrimaryIndex_Department<<off<<' ';
        }
        PrimaryIndex_Department.close();

    }

    map<string,int> readPrimaryIndex(PDept PD,int ch){
        PDept pD;
        map<string,int>Pdept;
        int flage=0;
        string off,id,c;
        PrimaryIndex_Department.close();
        PrimaryIndex_Department.open("PrimaryIndex_Department.txt",ios::in|ios::app);
        PrimaryIndex_Department.seekg(0,ios::end);
        //to know offset of last
        int i=PrimaryIndex_Department.tellg();
        PrimaryIndex_Department.seekg(0,ios::beg);
        while(!PrimaryIndex_Department.eof()&&i!=0){
            flage=1;
            getline(PrimaryIndex_Department,id,' ');
            getline(PrimaryIndex_Department,off,' ');
            if(id==""||off=="")break;
            else{
                pD.DeptID=id;
                pD.offset=stoi(off);
                Pdept.insert({pD.DeptID,pD.offset});
            }
        }
        //if file empty offset=0
        if(ch==0){
            if(flage==0){
                PD.offset=0;
                Pdept.insert({PD.DeptID,PD.offset});
            }

        else{
            file_input_Department.seekg(0,ios::end);
            PD.offset=file_input_Department.tellg();
            Pdept.insert({PD.DeptID,PD.offset});
        }
        }
        return Pdept;
    }

    vector<SDept> ReadSECIndex(){
        secondryIndex_Department.open("secondryIndex_Department.txt",ios::in);
        vector<SDept> list;

        while (!secondryIndex_Department.eof()){
            SDept obj;
            getline(secondryIndex_Department,obj.name,'|');
            if(obj.name=="") break;
            while (true){
                string name;
                getline(secondryIndex_Department,name,'|');
                obj.pKey.push_back(name);
                char c;
                secondryIndex_Department.seekp(0,ios::cur);
                secondryIndex_Department>>c;
                if(c=='#')break;
                else secondryIndex_Department.seekp(-1,ios::cur);
            }
            list.push_back(obj);
        }
        secondryIndex_Department.close();
        return list;
    }
    SDept checkInSecIndex(vector<SDept>& l,string& s,string& p){
        int f=0;
        SDept KK;
        for(SDept& k:l){
            if(k.name==s){
                f=1;
                k.pKey.push_back(p);
                KK=k;
            }
        }
        if(f==1)return KK;
        else {
            KK.name=s;
            KK.pKey.push_back(p);
            l.push_back(KK);
            return KK;
        }
    }


    void writeSecondryIndex(SDept t){
        secondryIndex_Department.close();
        secondryIndex_Department.open("secondryIndex_Department.txt",ios::out|ios::app);
        secondryIndex_Department<<t.name<<'|';
        for(string S:t.pKey){
            secondryIndex_Department<<S<<'|';
        }
        secondryIndex_Department<<'#';
        secondryIndex_Department.close();

    }

    vector<SDept>ReadSecondryIndex(string name,string pKey){
        SDept SD;
        int flag=0;
        vector<SDept>list;
        secondryIndex_Department.close();
        secondryIndex_Department.open("secondryIndex_Department.txt",ios::in|ios::app);
        secondryIndex_Department.seekg(0,ios::end);
        int j=secondryIndex_Department.tellg();
        secondryIndex_Department.seekg(0,ios::beg);
        if(!secondryIndex_Department.eof()&&j!=0){
            flag=1;
            secondryIndex_Department.close();
            list=ReadSECIndex();
            SDept t=checkInSecIndex(list,name,pKey);
        }
        if(flag==0){
            SD.name=name;
            SD.pKey.push_back(pKey);
            writeSecondryIndex(SD);
        }
        if(flag==1) {
            secondryIndex_Department.close();
            secondryIndex_Department.open("secondryIndex_Department.txt",ios::out|ios::trunc);
            for (SDept T:list) {
                writeSecondryIndex(T);
            }
        }
        return list;

    }
    void add_Dept(){
        map<string,int>Pdept;
        vector<SDept>list;
        PDept pd;
        Department dept;
        int bitCount=3;
        string id,name, manager;
        cout<<"Enter Department ID"<<endl;
        cin>>id;
        bitCount+=id.length();
        cout<<"Enter Department Name"<<endl;
        cin>>name;
        bitCount+=name.length();
        cout<<"Enter Manager Name"<<endl;
        cin>>manager;
        bitCount+=manager.length();
        pd.DeptID=id;
        Pdept=readPrimaryIndex(pd,0);
        list=ReadSecondryIndex(name,id);
        file_input_Department.close();
        file_input_Department.open("file_input_Department.txt",ios::out|ios::app);
        file_input_Department<<bitCount<<'|';
        file_input_Department<<id<<'|';
        file_input_Department<<name<<'|';
        file_input_Department<<manager<<'#';

        sort(Pdept);
        writePrimaryIndex(Pdept);


    }
    Department SearchPrimaryIndex(string S){
        map<string,int> mp;
        PDept Pd;
        Pd.DeptID=S;
        mp= readPrimaryIndex(Pd,1);
        int offset=mp[S];
        string offsetdel;
        file_input_Department.close();
        file_input_Department.open("file_input_Department.txt",ios::in|ios::app);
        file_input_Department.seekg(offset,ios::beg);
        getline(file_input_Department,offsetdel,'|');
        int len=offsetdel.length();
        file_input_Department.close();
        file_input_Department.open("file_input_Department.txt",ios::in|ios::app);
        file_input_Department.seekg(offset+len+1,ios::beg);
        Department dep;
        string s;
        int lengthind=stoi(offsetdel);

        getline(file_input_Department,s,'|');
        dep.Dept_Id=s;
        lengthind-=s.length();
        getline(file_input_Department,s,'|');
        dep.Dept_Name=s;
        lengthind-=s.length();
        getline(file_input_Department,s,'#');
        dep.Dept_Manager=s;
        lengthind-=s.length();
        lengthind-=3;
        s="";
        char f;
        while (lengthind!=0){
            file_input_Department>>f;
            s+=f;
            lengthind--;
        }
        return dep;
    }
    vector<string> SearchSecIndex(string nam){
        vector <SDept>List;
        vector<string>result;
        result.push_back("empty");
        List=ReadSECIndex();
        for(SDept s:List){
            if(s.name==nam){
                result.clear();
                result=s.pKey;
                break;
            }
        }
        return result;
    }
    void delete_Department(string id_deleted){
        map<string,int>mp;
        PDept PE;
        mp=readPrimaryIndex(PE,1);
        int offset=mp[id_deleted];
        mp.erase(id_deleted);
        writePrimaryIndex(mp);
         sort(mp);

    }


};



int main(){
   cout<<"1-Add new Employee"<<endl;
    cout<<"2-x Add new Department"<<endl;
    cout<<"3-x Delete Employee(ID)"<<endl;
    cout<<"4-x Delete Department(ID)"<<endl;
    cout<<"5-Print Employee(ID)"<<endl;
    cout<<"6-Print Employee(Dept_id)"<<endl;
    cout<<"7-x print Department(ID)"<<endl;
    cout<<"8-x print Department(name)"<<endl;
    cout<<"9-x write Query"<<endl;
    cout<<"10-Exit"<<endl;
    Employee e;

    Department d;

    while(true){

    int num;
    cout<<"Enter num: ";
    cin>>num;
    if(num==1){
        e.add_Employee();

    }
    else if(num==2){
        d.add_Dept();

    }
    else if(num==3){
        cout<<"enter emp id"<<endl;
        string id;
        cin>>id;
        e.delete_Employee(id);
    }
    else if(num==4){
        cout<<"enter Department id"<<endl;
        string id;
        cin>>id;
        d.delete_Department(id);
    }
    else if(num==5){
        Employee ee;
        cout<<"enter emp id"<<endl;
        string id;
        cin>>id;

        ee=e.SearchPrimaryIndex(id);
        cout<<"Department ID :" ;
        cout<<ee.Dept_ID<<endl;;
        cout<<"Employee name :" ;
        cout<<ee.Employee_Name<<endl;
        cout<<"Employee position : ";
        cout<<ee.Employee_position<<endl;

    }else if(num==6){
        vector<string>result;
      cout<<"enter department id "<<endl;
      string depid;
      cin>>depid;
      result=e.SearchSecIndex(depid);
      cout<<"employees in this department are : "<< endl;
      for(string s : result){
        Employee er;
        er=e.SearchPrimaryIndex(s);

        cout<<er.Employee_Name<<endl;

      }



    }else if(num==7){
        Department dd;
        cout<<"enter department id"<<endl;
        string id;
        cin>>id;

        dd=d.SearchPrimaryIndex(id);
        cout<<"Department Name :" ;
        cout<<dd.Dept_Name<<endl;;
        cout<<"Department Id :" ;
        cout<<dd.Dept_Id<<endl;
        cout<<"Manager Name: ";
        cout<<dd.Dept_Manager<<endl;

    }else if(num==8){
        vector<string>result;
        cout<<"enter department name "<<endl;
        string dep_name;
        cin>>dep_name;
        result=d.SearchSecIndex(dep_name);
        cout<<"Manager in this department are : "<< endl;
        for(string s : result){
            Department dep;
            dep=d.SearchPrimaryIndex(s);

            cout<<dep.Dept_Manager<<endl;
        }
    }else if(num==9){
        string arr[8];
        arr[0]="  select  ";
        arr[2]="  from  ";
        arr[4]=" where  ";
        arr[6]="  =  ";
        int j=0;
        for(int i=1;i<8;i=i+2){

            cout<<arr[j];
            cin>>arr[i];
            j=j+2;
        }
        if(arr[3]=="Employee"){
            Employee eee;
            if(arr[5]=="Emp_id"){
                eee=e.SearchPrimaryIndex(arr[7]);
                if(arr[1]=="All"){
                    cout<<"dep num :" ;
                    cout<<eee.Dept_ID<<endl;
                    cout<<"emp name :" ;
                    cout<<eee.Employee_Name<<endl;
                    cout<<"emp postion : ";
                    cout<<eee.Employee_position<<endl;

                } else if(arr[1]=="Dep_id"){
                    cout<<"dep num :" ;
                    cout<<eee.Dept_ID<<endl;

                } else if(arr[1]=="Emp_name"){
                    cout<<"emp name :" ;
                    cout<<eee.Employee_Name<<endl;

                } else if(arr[1]=="Emp_po"){
                    cout<<"emp postion : ";
                    cout<<eee.Employee_position<<endl;

                } else{
                    break;
                }

            }
            else if(arr[5]=="Dep_id"){
                    vector<string>result;
                    //cout<<"enter dep id "<<endl;
                    //string depid;
                    //cin>>depid;
                    result=e.SearchSecIndex(arr[7]);
                    if(arr[1]=="All"){
                    //cout<<"employees in this dep are : "<< endl;
                    for(string s : result){
                        Employee er;
                        er=e.SearchPrimaryIndex(s);
                        cout<<"emp name : ";
                        cout<<er.Employee_Name<<endl;
                        cout<<"emp id : ";
                        cout<<er.Employee_ID<<endl;
                        cout<<"emp postion : ";
                        cout<<er.Employee_position<<endl;
                        }
                    } else if( arr[1]=="Emp_name"){
                        for(string s : result){
                        Employee er;
                        er=e.SearchPrimaryIndex(s);
                        cout<<"emp name : ";
                        cout<<er.Employee_Name<<endl;}
                    } else if(arr[1]=="Emp_id"){
                        for(string s : result){
                        Employee er;
                        er=e.SearchPrimaryIndex(s);
                        cout<<"emp id : ";
                        cout<<er.Employee_ID<<endl;}

                    } else if(arr[1]=="Emp_po"){
                        for(string s : result){
                        Employee er;
                        er=e.SearchPrimaryIndex(s);
                        cout<<"emp postion : ";
                        cout<<er.Employee_position<<endl;}

                    } else{
                        break;
                    }

            }
            else{
                break;
            }


        }
        else if(arr[3]=="Department"){
            Department ddd;
            if(arr[5]=="Dep_id"){
                ddd=d.SearchPrimaryIndex(arr[7]);
                if(arr[1]=="All"){
                    cout<<"Department Name :" ;
                    cout<<ddd.Dept_Name<<endl;
                   // cout<<"Department Id :" ;
                   // cout<<dd.Dept_Id<<endl;
                    cout<<"Manager Name: ";
                    cout<<ddd.Dept_Manager<<endl;

                } else if(arr[1]=="Dep_name"){
                    cout<<"Department Name :" ;
                    cout<<ddd.Dept_Name<<endl;

                } else if (arr[1]=="Dep_man"){
                    cout<<"Manager Name: ";
                    cout<<ddd.Dept_Manager<<endl;

                }

            }
            else if(arr[5]=="Dep_name"){
                vector<string>result;
                result=d.SearchSecIndex(arr[7]);
                if(arr[1]=="All"){
                    for(string s : result){
                    Department dep;
                    dep=d.SearchPrimaryIndex(s);
                    cout<<"manger name : "<<endl;
                    cout<<dep.Dept_Manager<<endl;
                    cout<<"dep id : ";
                    cout<<dep.Dept_Id<<endl;
                        }
                } else if(arr[1]=="Dep_id"){
                    for(string s : result){
                     Department dep;
                     dep=d.SearchPrimaryIndex(s);
                     cout<<"dep id : ";
                     cout<<dep.Dept_Id<<endl;
                        }

                } else if(arr[1]=="Dep_man"){
                    for(string s : result){
                     Department dep;
                     dep=d.SearchPrimaryIndex(s);
                     cout<<"dep Manger : ";
                     cout<<dep.Dept_Manager<<endl;
                        }
                } else{
                    break;
                }

            }
            else{
                break;
            }
        }
        else{
            cout<<"wrong input"<<endl;
        }


    }
    else if(num==10){
        file_input_Department.close();
        break;
    }

    }
    return 0;
}
