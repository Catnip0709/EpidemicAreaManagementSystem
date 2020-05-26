#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include<stdlib.h>
#define ADMIN "admin"
#define USER "user"
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
            GenJsonObjectArray("information",keyNames,FinalResult,jsonDoc);
        }
        else if(type==USER){
            Value Key,Value;
            Key.SetString("result");
            string sql = "select * from User where userID=\""+id+"\"";
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
            sql="select * from Quarantine where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                Value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            unordered_map<string,string> KeyValues;
            if(db.sqlResult.empty()){
                KeyValues["Quarantine"]="0";
            }
            else{
                KeyValues["Quarantine"]="1";
                KeyValues["beginDate"]=db.sqlResult[0][1];
                KeyValues["endDate"]=db.sqlResult[0][2];
            }
            sql="select buildingID,familyID from User where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                Value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            KeyValues["unit"]=db.sqlResult[0][0];
            KeyValues["familyID"]=db.sqlResult[0][1];
            string BuildingId=db.sqlResult[0][0];
            string FamilyId=db.sqlResult[0][1];
            GenJsonObjectWithObjectValue("family",KeyValues,jsonDoc);
            sql="select userId,userName from User where BuildingId=\""+BuildingId+"\" and "+"familyID=\""+FamilyId+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                Value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,Value,allocator);   
                break;
            }
            Value.SetInt(SUCCESS);
            jsonDoc.AddMember(Key,Value,allocator);
            unordered_map<int ,string> ArrayKeyValues;
            ArrayKeyValues[0]="userId";
            ArrayKeyValues[1]="name";
            GenJsonObjectArray("membersList",ArrayKeyValues,db.sqlResult,jsonDoc);
        }
    }while(false);
end:    
    jsonDoc.Accept(Writer);
    cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}