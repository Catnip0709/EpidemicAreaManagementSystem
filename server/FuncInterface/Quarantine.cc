#include "../interface.h"
#include "../myDB.h"
#include "../common.h"
#include<stdlib.h>
#define ADMIN "admin"
#define USER "user"
#define ADMIN_ID 0
#define USER_ID 1
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
            Value Key,value;
            Key.SetString("result");
            string sql = "select buildingID from Admin where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            if(db.sqlResult.empty()){
                value.SetInt(HAVENT_REGISTER);
                jsonDoc.AddMember(Key,value,allocator);   
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
                    value.SetInt(HAVENT_REGISTER);
                    jsonDoc.AddMember(Key,value,allocator);   
                    goto end;
                }
                if(db.sqlResult.empty()){
                    continue;
                }
                for(unsigned int i=0;i<db.sqlResult.size();++i){
                    db.sqlResult[i][2]= db.sqlResult[i][2]+"|"+ db.sqlResult[i][3];
                    db.sqlResult[i].erase(db.sqlResult[i].begin()+3);
                    FinalResult.push_back(db.sqlResult[i]);
                }
            }
            sql="select User.userID,userName,buildingID,familyID,phone,beginDate,endDate from User RIGHT JOIN Quarantine on User.userID=Quarantine.userID where buildingID=\""+BuildingID+"\"";
            if(!db.exeSQL(sql,RETRIEVE)) {      
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            if(!db.sqlResult.empty()){
                for(unsigned int i=0;i<db.sqlResult.size();++i){
                    db.sqlResult[i][2]= db.sqlResult[i][2]+"|"+ db.sqlResult[i][3];
                    db.sqlResult[i].erase(db.sqlResult[i].begin()+3);
                    FinalResult.push_back(db.sqlResult[i]);
                }
            }            
            value.SetInt(SUCCESS);
            jsonDoc.AddMember(Key,value,allocator);
            Key.SetString("pageTotal");
            value.SetInt(FinalResult.size());
            jsonDoc.AddMember(Key,value,allocator);
            if(FinalResult.empty()){
                Value EmptyArray(kArrayType);
                jsonDoc.AddMember(Key,value,allocator);
                Key.SetString("information");
                jsonDoc.AddMember(Key,EmptyArray,allocator);
            }
            else{
                GenJsonObjectArray("information",keyNames,FinalResult,jsonDoc);
            }
        }
        else if(type==USER){
            Value Key,value;
            Key.SetString("result");
            string sql = "select * from User where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            if(db.sqlResult.empty()){
                value.SetInt(HAVENT_REGISTER);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            sql="select * from Quarantine where userID=\""+id+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
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
            if(!db.exeSQL(sql, RETRIEVE) || db.sqlResult.empty()) { 
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            KeyValues["unit"]=db.sqlResult[0][0];
            KeyValues["familyID"]=db.sqlResult[0][1];
            string BuildingId=db.sqlResult[0][0];
            string FamilyId=db.sqlResult[0][1];
            GenJsonObjectWithObjectValue("family",KeyValues,jsonDoc);
            sql="select userId,userName from User where BuildingId=\""+BuildingId+"\" and "+"familyID=\""+FamilyId+"\"";
            if(!db.exeSQL(sql, RETRIEVE)) { 
                value.SetInt(MYSQL_ERR);
                jsonDoc.AddMember(Key,value,allocator);   
                break;
            }
            value.SetInt(SUCCESS);
            jsonDoc.AddMember(Key,value,allocator);
            Key.SetString("pageTotal");
            value.SetInt(db.sqlResult.size());
            if(db.sqlResult.empty()){
                Value EmptyArray(kArrayType);
                Key.SetString("membersList");
                jsonDoc.AddMember(Key,EmptyArray,allocator);
            }
            else{
                unordered_map<int ,string> ArrayKeyValues;
                ArrayKeyValues[0]="userId";
                ArrayKeyValues[1]="name";
                GenJsonObjectArray("membersList",ArrayKeyValues,db.sqlResult,jsonDoc);
            }
        }
    }while(false);
end:    
    jsonDoc.Accept(Writer);
    //cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}
bool IsUserExit(const string& id,int type)
{
    string sql;
    MyDB db;
    bool iRet=false;
    do{
        if(type==USER_ID){
            sql = "select * from User where userID=\""+id+"\"";
        }
        else if(type==ADMIN_ID){
            sql = "select * from Admin where userID=\""+id+"\"";
        }
        if(!db.exeSQL(sql, RETRIEVE)) { 
            break;
        }
        if(db.sqlResult.empty()){  
            break;
        }
        iRet=true;
    }while(false);
   return iRet;
}
bool IsAdminOpValid(const string& userID,const string& adminId)
{
    string sql;
    MyDB db;
    bool iRet=false;
    do{
        sql="select buildingID from Admin where userID=\""+adminId+"\"";
        if(!db.exeSQL(sql, RETRIEVE)) { 
            break;
        }
        if(db.sqlResult.empty() || db.sqlResult[0].size()!=1 || db.sqlResult[0][0].empty()){  
            break;
        }
        string AdminBuildings=db.sqlResult[0][0];
        vector<string> EachBuilding=stringCut(AdminBuildings,"_");
        sql="select buildingID from User where userID=\""+userID+"\"";
        if(!db.exeSQL(sql, RETRIEVE)) { 
            break;
        }
        if(db.sqlResult.empty() || db.sqlResult[0].size()!=1 || db.sqlResult[0][0].empty()){  
            break;
        }
        string UserBuilding=db.sqlResult[0][0];
        if(find(begin(EachBuilding),end(EachBuilding),UserBuilding)!=end(EachBuilding)){
            iRet=true;
            break;
        }
    }while(false);
    return iRet;
}
string SetQuarantineInfo(string userID,string adminID,string endDate)
{
    if(userID.empty() || adminID.empty() || endDate.empty()){
        return "";
    }
    MyDB db;
    Document jsonDoc;
    jsonDoc.SetObject();
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    StringBuffer ResBuffer;
    Writer<StringBuffer> Writer(ResBuffer);
    do{
        Value Key,Value;
        Key.SetString("result");
        if(!IsUserExit(userID,USER_ID)){
            Value.SetInt(HAVENT_REGISTER);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        if(!IsUserExit(adminID,ADMIN_ID)){
            Value.SetInt(NOT_ADMINISTRATOR);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        if(!IsAdminOpValid(userID,adminID)){
            Value.SetInt(NOT_INCHARGE);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        string sql="update Quarantine set endDate=\""+endDate+"\" where userID=\""+userID+"\"";
        if(!db.exeSQL(sql,UPDATE)) {
            Value.SetInt(MYSQL_ERR);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        Value.SetInt(SUCCESS);
        jsonDoc.AddMember(Key,Value,allocator); 
    }while(false);
    jsonDoc.Accept(Writer);
    cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}

string SetUserHealthStat(string userID,string adminID,string status)
{
    if(userID.empty() || adminID.empty() || status.empty()){
        return "";
    }
    MyDB db;
    Document jsonDoc;
    jsonDoc.SetObject();
    Document::AllocatorType& allocator = jsonDoc.GetAllocator();
    StringBuffer ResBuffer;
    Writer<StringBuffer> Writer(ResBuffer);
    do{
        Value Key,Value;
        Key.SetString("result");
        if(!IsUserExit(userID,USER_ID)){
            Value.SetInt(HAVENT_REGISTER);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        if(!IsUserExit(adminID,ADMIN_ID)){
            Value.SetInt(NOT_ADMINISTRATOR);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        if(!IsAdminOpValid(userID,adminID)){
            Value.SetInt(NOT_INCHARGE);
            jsonDoc.AddMember(Key,Value,allocator); 
            break;
        }
        string sql="update User set state=\""+status+"\" where userID=\""+userID+"\"";
        if(!db.exeSQL(sql,UPDATE)) { 
            break;
        }
        Value.SetInt(SUCCESS);
        jsonDoc.AddMember(Key,Value,allocator); 
    }while(false);
    jsonDoc.Accept(Writer);
    cout<<ResBuffer.GetString()<<endl;
    return ResBuffer.GetString();
}