#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include<stdlib.h>
#define ADMIN "admin"
using namespace std;

string GetQuarantineInfo(string type,string id)
{
    MyDB db;
    Document jsonDoc;
    jsonDoc.SetObject();
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    StringBuffer ResBuffer;
    Writer<StringBuffer> Writer(ResBuffer);
    do{
        if(type==ADMIN){
            Value Key,Value;
            Key.SetString("result");
            string sql = "select buildingID from Admin where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                Value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            if(db.sqlResult.empty()){
                Value.SetInt(HAVENT_REGISTER);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            string BuildingID=db.sqlResult[0][0];
            unordered_map<int,string> keyNames;
            keyNames[0]="userId";
            keyNames[1]="name";
            keyNames[2]="position";
            keyNames[3]="phone";
            keyNames[4]="beginDate";
            keyNames[5]="endDate";
            int pos,begin=0;
            vector<vector<string>> FinalResult;
            while((pos=BuildingID.find("_"))!=BuildingID.npos){
                string id=BuildingID.substr(begin,pos);
                BuildingID=BuildingID.substr(pos+1,BuildingID.length()-pos-1);
                sql="select User.userID,userName,buildingID,familyID,phone,beginDate,endDate from User RIGHT JOIN Quarantine on User.userID=Quarantine.userID where buildingID=\""+id+"\"";
                if(!db.exeSQL(sql,RETRIEVE)) {      
                    Value.SetInt(HAVENT_REGISTER);
                    jsonDoc.AddMember(Key,Value,allocator);   
                    goto end;
                }
                for(unsigned int i=0;i<db.sqlResult.size();++i){
                    db.sqlResult[i][2]= db.sqlResult[i][2]+"|"+ db.sqlResult[i][3];
                    db.sqlResult[i].erase(db.sqlResult[i].begin()+3);
                    FinalResult.push_back(db.sqlResult[i]);
                }
            }
            sql="select User.userID,userName,buildingID,familyID,phone,beginDate,endDate from User RIGHT JOIN Quarantine on User.userID=Quarantine.userID where buildingID=\""+BuildingID+"\"";
            if(!db.exeSQL(sql,RETRIEVE)) {      
                Value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            for(unsigned int i=0;i<db.sqlResult.size();++i){
                db.sqlResult[i][2]= db.sqlResult[i][2]+"|"+ db.sqlResult[i][3];
                db.sqlResult[i].erase(db.sqlResult[i].begin()+3);
                FinalResult.push_back(db.sqlResult[i]);
            }
            Value.SetInt(SUCCESS);
            jsonDoc.AddMember(Key,Value,allocator);
            GenJsonArray("information",keyNames,FinalResult,jsonDoc);
        }
    }while(false);
end:    
    jsonDoc.Accept(Writer);
    cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}