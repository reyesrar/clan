#ifndef CLANMEMBER_H
#define CLANMEMBER_H

#include <string>
#include <iostream>
using namespace std;

class ClanMember {
    public:
        int id;
        string name;
        string lastName;
        char gender;
        int age;
        int idFather;
        bool isDead;
        bool wasChief;
        bool isChief;

        ClanMember() 
            : id(0), name(""), lastName(""), gender(' '), age(0), idFather(0), isDead(false), wasChief(false), isChief(false) {}

        ClanMember(int id, string name, string lastName, char gender, int age, int idFather, bool isDead, bool wasChief, bool isChief)
            : id(id), name(name), lastName(lastName), gender(gender), age(age), 
              idFather(idFather ? 1 : 0), isDead(isDead ? 1 : 0), 
              wasChief(wasChief ? 1 : 0), isChief(isChief ? 1 : 0) {}

        bool operator<(const ClanMember& other) const {
            return id < other.id;
        }

        bool operator>(const ClanMember& other) const {
            return id > other.id;
        }

        bool operator==(const ClanMember& other) const {
            return id == other.id;
        }

        friend ostream& operator<<(ostream& os, const ClanMember& member) {
            os << member.id << ", " << member.name << ", " << member.lastName << ", " 
               << member.gender << ", " << member.age << ", " << member.idFather 
               << ", " << member.isDead << ", " << member.wasChief << ", " << member.isChief;
            return os;
        }
};

#endif
