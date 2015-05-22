#include <iostream>
#include "client/dbclient.h"
#include <string>
#include <memory>
using namespace std;
using namespace mongo;
class mongohelper
{
  public:
    DBClientConnection m_mongodb_handler;
};
int main()
{
//   mongohelper m_mongo;
//   try{
//   m_mongo.m_mongodb_handler.connect("127.0.0.1:10001");
/*  }
  catch(DBException &e)
  {
    cout<<"connect failure"<<endl;
}*/
 mongo::DBClientConnection c;
 c.connect("127.0.0.1:27017");
 cout<<c.count("tests.test")<<endl;
//BSONObj p = BSONObjBuilder().append("name", "Joe").append("age",30).obj();
//BSONObj p = BSONObjBuilder().append("age",30).append("name","joe").obj();
//BSONObj p = BSONObjBuilder().append("name", "Joe").obj();
BSONObj p=BSON( "device_id"<<50<<"motiontime" << GTE << "2014-12-12 12:12:11"<<LTE<<"2014-12-12 12:12:12" );
int age;
 auto_ptr<DBClientCursor> cursor=c.query("tests.test",p,1,0);
// auto_ptr<DBClientCursor> cursor=c.query("test.test",QUERY("name"<<"Joe");
if(cursor->more())
{
  BSONObj testcur=cursor->next();
  cout<<"already have one"<<endl;
  cout<<testcur["motiontime"].String()<<endl;
//  cout<<testcur["name"].String()<<testcur["age"].Int()<<endl;
//  age=testcur["age"].Int();
//  c.update("test.test",BSON("name"<<"joe"), BSON("$inc"<<BSON("age"<<3)),true,false);
//  string err=c.getLastError();
//  cout<<err;
//  cout<<age<<endl;
//  c.update("test.test", QUERY("name"<<"joe"), BSON("$set"<<BSON("age"<<2)),true,true);  
//  c.update("test.test", QUERY("name"<<"joe"<<"age"<<30), BSON("$inc"<<BSON("age"<<age)),true,true);  
//  c.update("test.test", QUERY("name"<<"joe"), BSON("$inc"<<BSON("age"<<age)),true,true);  
//  string err=c.getLastError();
//  cout<<err;
//  c.update("test.test", QUERY("name"<<"joe"), BSON("$set"<<BSON("age"<<2)));  
//  cursor=c.query("test.test",p);
//  testcur=cursor->next();
//  cout<<testcur["name"].String()<<testcur["age"].Int()<<endl;
//  c.remove("test.test",QUERY("name"<<"joe"),true);
}
else
c.insert("tests.test", p);
//c.insert("test.test", QUERY("name"<<"Joe");
/*
  string db = "test";
string collection = "test";
Query condition = QUERY("age"<<20);
auto_ptr<DBClientConnection> cursor;
BSONObj p = BSONObjBuilder().append("name", "Joe").append("age", 33).obj();
cursor = m_mongo.m_mongodb_handler.query(db+"."+collection, QUERY("age"<<20));
*/
   return 1;
}
